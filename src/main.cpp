/* main.cpp — flowery : interactive spirograph renderer.
 *
 * SDL3 port of the classic gnuplot "wheels on wheels on wheels"
 * (Farris) curve generator that lives in legacy/.
 *
 * Renders, in real time:
 *
 *     z(t) = a1*exp(i*2*pi*(n1*t+s1)) + a2*exp(i*2*pi*(n2*t+s2))
 *          + a3*exp(i*2*pi*(n3*t+s3)),  t in [0,1]
 *
 * Keys:
 *   1 2 3        select wheel to edit
 *   arrows       adjust wheel n (Up/Down) or phase s (Left/Right)
 *   [ ]          adjust wheel radius a
 *   + -          halve / double the number of samples
 *   space        toggle phase animation (rotating wheels)
 *   c            toggle rainbow colouring
 *   r            randomise the wheels
 *   f            toggle fullscreen
 *   s            save an SVG file (like the legacy generator)
 *   b            save a BMP screenshot
 *   h            toggle help overlay
 *   Esc / q      quit
 *
 * Holding one of the adjusting keys (arrows, [ ]) repeats it: after a short
 * delay the key fires again a few dozen times a second, so n, s and a can be
 * swept without hammering the keyboard. The keys that toggle or save
 * something do not repeat.
 *
 * The curve is recomputed only when something about it actually changed, and
 * the polyline handed to the renderer is thinned to the resolution of the
 * window - see update_geometry() and build_display_points(). NOTES.md has the
 * measurements behind both.
 *
 * Everything is drawn in physical pixels: the display scale is applied to the
 * layout in build_screen_points() and the renderer is left at scale 1, which
 * is what keeps SDL_RenderLines on its driver line path. The HUD and the saved
 * SVG are the two exceptions, both for the same reason - they are in points.
 */
#include <SDL3/SDL.h>

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#if defined(__AVX2__)
#  include <immintrin.h>
#endif

#include "flowery.h"

#define WHEELS 3

/* Held-key repeat: how long the key has to be down before it starts
 * repeating, and how often it fires afterwards. */
#define REPEAT_DELAY_MS 300
#define REPEAT_RATE_MS  40
#define MAX_HELD        8

/* How far apart, in window points, two drawn vertices have to be to be worth
 * drawing separately. Segments shorter than this land on the same pixel (or
 * the next one), so at a million samples the curve is thinned by an order of
 * magnitude for free. The saved SVG is not thinned. */
#define MIN_SEGMENT_PTS 0.5

/* Distinct hues the rainbow mode uses. One per segment would mean a million
 * SDL_RenderLine calls; 256 round the wheel is past what the eye resolves. */
#define RAINBOW_STEPS 256

typedef struct {
    SDL_Keycode key;
    SDL_Keymod  mod;
    Uint64      next;   /* when this key should fire again */
} HeldKey;

typedef struct {
    SDL_Window   *window;
    SDL_Renderer *renderer;
    int           winW, winH;   /* window size in points (logical) */
    int           outW, outH;   /* render output size in pixels */
    float         scale;        /* pixels per point (2.0 on a Retina screen) */
    bool          fullscreen;
    bool          vsync;
    FloweryParams p;
    int           sel;          /* selected wheel 0..2 */
    bool          rainbow;
    bool          animate;
    bool          help;
    HeldKey       held[MAX_HELD];
    int           nheld;
    Uint64        lastTicks;

    /* Scratch, kept between frames instead of allocated per frame. `xs`/`ys`
     * are the curve in world coordinates, `pts` the same laid out for the
     * window, and `disp` the thinned subset that is actually drawn. */
    double     *xs, *ys;
    SDL_FPoint *pts;
    SDL_FPoint *disp;
    int        *didx;           /* source sample of each disp[] entry */
    int         cap;            /* samples the buffers can hold */
    int         ndisp;
    bool        dirtyCurve;     /* xs/ys need recomputing */
    bool        dirtyLayout;    /* pts/disp need recomputing */
} App;

/* ---------------------------------------------------------------- colours */

static void
hsv_to_rgb(float h, Uint8 *r, Uint8 *g, Uint8 *b)
{
    float c, x, m, rr, gg, bb;
    int sext;
    h = fmodf(h, 360.0f); if (h < 0) h += 360.0f;
    c = 1.0f;
    x = c * (1.0f - fabsf(fmodf(h / 60.0f, 2.0f) - 1.0f));
    m = 0.0f;
    sext = (int)(h / 60.0f);
    switch (sext) {
    case 0: rr=c; gg=x; bb=0.0f; break;
    case 1: rr=x; gg=c; bb=0.0f; break;
    case 2: rr=0.0f; gg=c; bb=x; break;
    case 3: rr=0.0f; gg=x; bb=c; break;
    case 4: rr=x; gg=0.0f; bb=c; break;
    default: rr=c; gg=0.0f; bb=x; break;
    }
    *r = (Uint8)((rr + m) * 255.0f);
    *g = (Uint8)((gg + m) * 255.0f);
    *b = (Uint8)((bb + m) * 255.0f);
}

/* --------------------------------------------------------------- sizing */

/* A Retina (or any scaled) display gives us a back buffer larger than the
 * window: the window is measured in points, the back buffer in pixels. The
 * curve is laid out in those pixels - build_screen_points() applies the
 * display scale - and the renderer is left at scale 1. That last part is what
 * matters for speed: with the renderer scaled up, SDL_RenderLines cannot use
 * the driver's line API and falls back to one quad per line *pixel*, which at
 * 2880x1800 costs an order of magnitude more than a GL_LINE_STRIP.
 *
 * The HUD is drawn in points instead (draw_overlays()), because SDL's debug
 * font is a fixed 8x8 *pixels*: at scale 1 it would come out half size on a 2x
 * display. */
static void
update_sizes(App *a)
{
    float scale = SDL_GetWindowDisplayScale(a->window);

    if (!(scale > 0.0f)) scale = 1.0f;
    a->scale = scale;
    SDL_GetWindowSize(a->window, &a->winW, &a->winH);
    SDL_SetRenderScale(a->renderer, 1.0f, 1.0f);
    SDL_GetCurrentRenderOutputSize(a->renderer, &a->outW, &a->outH);
    a->dirtyLayout = true;
}

/* The buffers follow the sample count, which the user can double and halve,
 * so they are grown on demand rather than once up front. */
static bool
ensure_buffers(App *a, int n)
{
    if (n <= a->cap) return true;

    double     *xs   = (double *)realloc(a->xs, sizeof(double) * n);
    double     *ys   = (double *)realloc(a->ys, sizeof(double) * n);
    SDL_FPoint *pts  = (SDL_FPoint *)realloc(a->pts, sizeof(SDL_FPoint) * n);
    SDL_FPoint *disp = (SDL_FPoint *)realloc(a->disp, sizeof(SDL_FPoint) * n);
    int        *didx = (int *)realloc(a->didx, sizeof(int) * n);

    /* If any of them failed, keep what we had and report; the caller leaves
     * the frame alone rather than drawing from a half-updated set. */
    if (xs) a->xs = xs;
    if (ys) a->ys = ys;
    if (pts) a->pts = pts;
    if (disp) a->disp = disp;
    if (didx) a->didx = didx;
    if (!xs || !ys || !pts || !disp || !didx) return false;

    a->cap = n;
    return true;
}

/* Map world coordinates to the drawing surface, preserving aspect
 * ratio (like gnuplot `set size ratio -1`) and centering.
 *
 * The layout is worked out in window points and then multiplied by `zoom`:
 * the screen wants physical pixels (zoom = the display scale), the saved SVG
 * wants points (zoom = 1), which is what keeps the file independent of the
 * display it happened to be written on. */
static void
build_screen_points(const App *a, const double *xs, const double *ys,
                    SDL_FPoint *pts, int n, double zoom)
{
    double minx, miny, maxx, maxy;
    double scale, mx, my;
    double margin, availW, availH, spanX, spanY, cx, cy;

    flowery_bbox(xs, ys, n, &minx, &miny, &maxx, &maxy);

    if (a->winW <= 0 || a->winH <= 0) return;

    /* the margin shrinks with the window so a small window is not all margin */
    margin = fmin(60.0, fmin((double)a->winW, (double)a->winH) * 0.08);
    availW = (double)a->winW - 2.0 * margin;
    availH = (double)a->winH - 2.0 * margin;
    if (availW < 1.0) availW = 1.0;
    if (availH < 1.0) availH = 1.0;

    spanX = (maxx - minx) > 1e-12 ? (maxx - minx) : 1.0;
    spanY = (maxy - miny) > 1e-12 ? (maxy - miny) : 1.0;

    scale = fmin(availW / spanX, availH / spanY) * zoom;
    mx = (double)a->winW / 2.0 * zoom;
    my = (double)a->winH / 2.0 * zoom;

    cx = (minx + maxx) / 2.0;
    cy = (miny + maxy) / 2.0;

    int i = 0;
#if defined(__AVX2__)
    /* Four points at a time. The operations are the same ones, in the same
     * order, as the scalar loop below, so this is bit-for-bit identical. */
    {
        const __m256d vscale = _mm256_set1_pd(scale);
        const __m256d vmx = _mm256_set1_pd(mx), vmy = _mm256_set1_pd(my);
        const __m256d vcx = _mm256_set1_pd(cx), vcy = _mm256_set1_pd(cy);

        for (; i + 4 <= n; i += 4) {
            const __m256d x = _mm256_loadu_pd(xs + i);
            const __m256d y = _mm256_loadu_pd(ys + i);
            const __m256d sx = _mm256_add_pd(
                vmx, _mm256_mul_pd(_mm256_sub_pd(x, vcx), vscale));
            const __m256d sy = _mm256_sub_pd(
                vmy, _mm256_mul_pd(_mm256_sub_pd(y, vcy), vscale));
            const __m128 fx = _mm256_cvtpd_ps(sx);
            const __m128 fy = _mm256_cvtpd_ps(sy);
            /* interleave into SDL_FPoint {x,y} pairs */
            _mm_storeu_ps((float *)(pts + i),     _mm_unpacklo_ps(fx, fy));
            _mm_storeu_ps((float *)(pts + i + 2), _mm_unpackhi_ps(fx, fy));
        }
    }
#endif
    for (; i < n; ++i) {
        pts[i].x = (float)(mx + (xs[i] - cx) * scale);
        pts[i].y = (float)(my - (ys[i] - cy) * scale); /* flip y for screen */
    }
}

/* Thin the laid-out polyline down to what the window can actually show.
 *
 * A point is kept only once it is `minsep` away from the last kept one, so a
 * segment is never longer than that for the curve's shape to bend inside it,
 * and never shorter than a pixel for the rasteriser to care. The caller
 * passes one device pixel (MIN_SEGMENT_PTS in window points), because the
 * points handed in are device pixels. The last sample is always kept so the
 * loop still closes. At the default sample count this keeps everything; it is
 * only at six figures that it matters. `didx` records where each kept point
 * came from, so the rainbow gradient stays anchored to the curve's parameter
 * rather than to the thinned list. */
static int
build_display_points(const SDL_FPoint *pts, int n, SDL_FPoint *out, int *didx,
                     float minsep)
{
    int m = 0;

    if (n <= 0) return 0;
    out[0] = pts[0];
    didx[0] = 0;
    m = 1;
    if (n == 1) return m;

    {
        const float minsep2 = minsep * minsep;
        for (int i = 1; i < n - 1; ++i) {
            const float dx = pts[i].x - out[m - 1].x;
            const float dy = pts[i].y - out[m - 1].y;
            if (dx * dx + dy * dy >= minsep2) {
                out[m] = pts[i];
                didx[m] = i;
                ++m;
            }
        }
    }

    out[m] = pts[n - 1];
    didx[m] = n - 1;
    ++m;
    return m;
}

/* Rebuild exactly as much as is out of date. Cheap when nothing changed,
 * which is the common case: an idle frame does no curve maths at all. */
static bool
update_geometry(App *a)
{
    const int n = a->p.samples > 0 ? a->p.samples : 1;

    if (!ensure_buffers(a, n)) return false;

    if (a->dirtyCurve) {
        flowery_points(&a->p, a->xs, a->ys);
        a->dirtyCurve = false;
        a->dirtyLayout = true;
    }
    if (a->dirtyLayout) {
        build_screen_points(a, a->xs, a->ys, a->pts, n, (double)a->scale);
        a->ndisp = build_display_points(a->pts, n, a->disp, a->didx,
                                        (float)(MIN_SEGMENT_PTS * a->scale));
        a->dirtyLayout = false;
    }
    return true;
}

/* ------------------------------------------------------------ randomise */

static void
randomize_wheels(FloweryParams *p)
{
    for (int k = 0; k < WHEELS; ++k) {
        p->n[k] = (double)(rand() % 121) - 60;          /* -60..60  */
        p->s[k] = (double)(rand() % 1000) / 1000.0;     /* 0..1     */
        p->a[k] = 0.75 + (double)(rand() % 2001) / 2000.0 * 1.25; /* .75..2 */
    }
}

static void
set_default_wheels(FloweryParams *p)
{
    p->n[0] = 480; p->n[1] = 648; p->n[2] = 816;
    p->a[0] = p->a[1] = p->a[2] = 1.0;
    p->s[0] = p->s[1] = p->s[2] = 0.0;
}

/* --------------------------------------------------------------- saving */

/* The SVG is vector, so it is written in window points: it carries no
 * resolution of its own and can be scaled to anything. It gets every sample,
 * not the thinned display list. */
static void
save_svg(const App *a, const char *fname)
{
    const int n = a->p.samples;
    double *xs = (double *)malloc(sizeof(double) * n);
    double *ys = (double *)malloc(sizeof(double) * n);
    SDL_FPoint *pts = (SDL_FPoint *)malloc(sizeof(SDL_FPoint) * n);
    FILE *fp;

    if (!xs || !ys || !pts) { free(xs); free(ys); free(pts); return; }

    flowery_points(&a->p, xs, ys);
    build_screen_points(a, xs, ys, pts, n, 1.0);   /* points, not pixels */

    fp = fopen(fname, "w");
    if (!fp) { free(xs); free(ys); free(pts); return; }

    fprintf(fp,
            "<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"no\"?>\n"
            "<svg width=\"%d\" height=\"%d\" viewBox=\"0 0 %d %d\"\n"
            "     xmlns=\"http://www.w3.org/2000/svg\">\n"
            "<polyline points=\"",
            a->winW, a->winH, a->winW, a->winH);
    for (int i = 0; i < n; ++i)
        fprintf(fp, "%s%.1f,%.1f", i ? " " : "", pts[i].x, pts[i].y);
    fprintf(fp, "\" fill=\"none\" stroke=\"rgb(148, 0, 211)\" stroke-width=\"1\"/>\n</svg>\n");
    fclose(fp);

    free(xs); free(ys); free(pts);
}

static void
save_bmp(App *a, const char *fname)
{
    SDL_Surface *surf = SDL_RenderReadPixels(a->renderer, NULL);
    if (!surf) {
        SDL_Log("could not read pixels: %s", SDL_GetError());
        return;
    }
    if (!SDL_SaveBMP(surf, fname))
        SDL_Log("could not save %s: %s", fname, SDL_GetError());
    else
        SDL_Log("saved %s (%dx%d)", fname, surf->w, surf->h); /* pixels */
    SDL_DestroySurface(surf);
}

/* ---------------------------------------------------------------- render */

static void
draw_curve(App *a)
{
    const int m = a->ndisp;
    const SDL_FPoint *pts = a->disp;

    if (m < 2) return;

    if (!a->rainbow) {
        SDL_SetRenderDrawColor(a->renderer, 148, 0, 211, 255);
        SDL_RenderLines(a->renderer, pts, m);
        return;
    }

    /* A hue per segment would be one draw call per segment. The hue rises
     * monotonically with the sample index, so equal steps come out as
     * contiguous runs and each run is a single polyline; the gradient is the
     * same, the call count drops by three orders of magnitude. */
    const int n = a->p.samples;
    int s = 0;
    while (s < m - 1) {
        const int q = (int)((long long)a->didx[s + 1] * RAINBOW_STEPS / n);
        int e = s;
        while (e < m - 2 && (int)((long long)a->didx[e + 2] * RAINBOW_STEPS / n) == q)
            ++e;

        Uint8 r, g, b;
        hsv_to_rgb(360.0f * ((float)q + 0.5f) / (float)RAINBOW_STEPS, &r, &g, &b);
        SDL_SetRenderDrawColor(a->renderer, r, g, b, 255);
        SDL_RenderLines(a->renderer, pts + s, e - s + 2);
        s = e + 1;
    }
}

static void
render_frame(App *a)
{
    SDL_SetRenderDrawColor(a->renderer, 8, 8, 12, 255);
    SDL_RenderClear(a->renderer);
    draw_curve(a);
}

/* ------------------------------------------------------------- text/HUD */

static void
draw_debug_text(const App *a, const char *text)
{
    SDL_SetRenderDrawColor(a->renderer, 255, 255, 255, 255);
    SDL_RenderDebugTextFormat(a->renderer, 12.0f, 12.0f, "%s", text);
}

static void
draw_help(const App *a)
{
    SDL_SetRenderDrawColor(a->renderer, 255, 255, 255, 255);
    const char *lines[] = {
        "1 2 3        select wheel",
        "Up/Down      wheel teeth n",
        "Left/Right   phase offset s",
        "[  ]         wheel radius a",
        "+  -         samples (halve/double)",
        "space        rotate wheels (animate)",
        "c            rainbow colour",
        "r            randomise",
        "f            fullscreen",
        "s            save SVG",
        "b            save BMP",
        "h            hide help",
        "Esc / q      quit",
    };
    float y = 40.0f;
    for (size_t i = 0; i < sizeof(lines) / sizeof(lines[0]); ++i) {
        SDL_RenderDebugText(a->renderer, 12.0f, y, lines[i]);
        y += SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE + 4.0f;
    }
}

/* ----------------------------------------------------------- status line */

static void
format_status(const App *a, char *buf, size_t bufsz)
{
    snprintf(buf, bufsz,
             "wheel[%d] n=%3.0f n=%3.0f n=%3.0f   a=(%.2f %.2f %.2f)  "
             "s=(%.3f %.3f %.3f)   samples=%d (drawing %d)%s   %dx%d@%.0fx",
             a->sel,
             a->p.n[0], a->p.n[1], a->p.n[2],
             a->p.a[0], a->p.a[1], a->p.a[2],
             a->p.s[0], a->p.s[1], a->p.s[2],
             a->p.samples, a->ndisp,
             a->rainbow ? "  rainbow" : "",
             a->winW, a->winH, (double)a->scale);
}

/* The status line and the help overlay stay in window points, so the renderer
 * goes back to the display scale for them and back to 1 for the curve: one
 * device pixel per point is what the curve wants, and a point per pixel is
 * what the 8x8-pixel debug font needs to keep its size on any display. */
static void
draw_overlays(App *a)
{
    char status[320];

    SDL_SetRenderScale(a->renderer, a->scale, a->scale);
    format_status(a, status, sizeof status);
    draw_debug_text(a, status);
    if (a->help)
        draw_help(a);
    SDL_SetRenderScale(a->renderer, 1.0f, 1.0f);
}

/* ----------------------------------------------------------------- input */

/* Only the keys that sweep a value are worth repeating; a held `s` writing
 * an SVG sixty times a second is not what anybody wants. */
static bool
key_repeats(SDL_Keycode key)
{
    switch (key) {
    case SDLK_UP:
    case SDLK_DOWN:
    case SDLK_LEFT:
    case SDLK_RIGHT:
    case SDLK_LEFTBRACKET:
    case SDLK_RIGHTBRACKET:
        return true;
    default:
        return false;
    }
}

static void
hold_key(App *a, const SDL_KeyboardEvent *ke)
{
    if (!key_repeats(ke->key) || a->nheld >= MAX_HELD) return;
    a->held[a->nheld].key = ke->key;
    a->held[a->nheld].mod = ke->mod;
    a->held[a->nheld].next = SDL_GetTicks() + REPEAT_DELAY_MS;
    ++a->nheld;
}

static void
release_key(App *a, SDL_Keycode key)
{
    for (int i = 0; i < a->nheld; ++i) {
        if (a->held[i].key == key) {
            a->held[i] = a->held[a->nheld - 1];
            --a->nheld;
            return;
        }
    }
}

static void
handle_key(App *a, const SDL_KeyboardEvent *ke)
{
    bool shifted = (ke->mod & SDL_KMOD_SHIFT) != 0;
    int k = a->sel;

    switch (ke->key) {
    case SDLK_ESCAPE:
    case SDLK_Q:
        exit(0);
        break;
    case SDLK_1: a->sel = 0; break;
    case SDLK_2: a->sel = 1; break;
    case SDLK_3: a->sel = 2; break;

    case SDLK_UP:
        a->p.n[k] += shifted ? 10.0 : 1.0;
        a->dirtyCurve = true;
        break;
    case SDLK_DOWN:
        a->p.n[k] -= shifted ? 10.0 : 1.0;
        a->dirtyCurve = true;
        break;
    case SDLK_LEFT:
        a->p.s[k] -= shifted ? 0.05 : 0.01;
        a->p.s[k] = fmod(a->p.s[k] + 1.0, 1.0);
        a->dirtyCurve = true;
        break;
    case SDLK_RIGHT:
        a->p.s[k] = fmod(a->p.s[k] + (shifted ? 0.05 : 0.01), 1.0);
        a->dirtyCurve = true;
        break;

    case SDLK_LEFTBRACKET:
        a->p.a[k] -= shifted ? 0.5 : 0.1;
        if (a->p.a[k] < 0.0) a->p.a[k] = 0.0;
        a->dirtyCurve = true;
        break;
    case SDLK_RIGHTBRACKET:
        a->p.a[k] += shifted ? 0.5 : 0.1;
        a->dirtyCurve = true;
        break;

    case SDLK_PLUS:
    case SDLK_EQUALS:
        a->p.samples = (int)fmin((double)a->p.samples * 2.0, 1048576.0);
        a->dirtyCurve = true;
        break;
    case SDLK_MINUS:
        a->p.samples = (int)fmax((double)a->p.samples / 2.0, 32.0);
        a->dirtyCurve = true;
        break;

    case SDLK_F:
        a->fullscreen = !a->fullscreen;
        SDL_SetWindowFullscreen(a->window, a->fullscreen);
        break;
    case SDLK_SPACE:
        a->animate = !a->animate;
        break;
    case SDLK_C:
        a->rainbow = !a->rainbow;
        break;
    case SDLK_R:
        randomize_wheels(&a->p);
        a->dirtyCurve = true;
        break;
    case SDLK_S:
        {
            char fname[128];
            snprintf(fname, sizeof fname,
                     "spiro-%+d-%+d-%+d-%d.svg",
                     (int)a->p.n[0], (int)a->p.n[1], (int)a->p.n[2],
                     a->p.samples);
            save_svg(a, fname);
            SDL_Log("saved %s", fname);
        }
        break;
    case SDLK_B:
        {
            char fname[128];
            snprintf(fname, sizeof fname,
                     "spiro-%+d-%+d-%+d-%d.bmp",
                     (int)a->p.n[0], (int)a->p.n[1], (int)a->p.n[2],
                     a->p.samples);
            save_bmp(a, fname);
            SDL_Log("saved %s", fname);
        }
        break;
    case SDLK_H:
        a->help = !a->help;
        break;
    default:
        break;
    }
}

/* ------------------------------------------------------------------- main */

int
main(int argc, char *argv[])
{
    App app = {};
    app.xs = app.ys = NULL;
    app.pts = app.disp = NULL;
    app.didx = NULL;
    app.cap = 0;
    app.dirtyCurve = true;
    app.dirtyLayout = true;

    if (argc > 4) {
        /* ./flowery n1 n2 n3 [samples] */
        app.p.n[0] = atof(argv[1]);
        app.p.n[1] = atof(argv[2]);
        app.p.n[2] = atof(argv[3]);
        app.p.a[0] = app.p.a[1] = app.p.a[2] = 1.0;
        app.p.s[0] = app.p.s[1] = app.p.s[2] = 0.0;
        app.p.samples = argc > 4 ? atoi(argv[4]) : 2001;
    } else {
        set_default_wheels(&app.p);
        app.p.samples = 2001;
    }
    app.sel = 0;
    app.rainbow = false;
    app.animate = false;
    app.help = true;

    srand((unsigned)time(NULL));

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }
    atexit(SDL_Quit);

    app.window = SDL_CreateWindow(
        "flowery — spirograph (SDL3)",
        900, 900,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);
    if (!app.window) {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        return 1;
    }
    /* Left alone, SDL draws a line as one quad per line *pixel*; at 2880x1800
     * that is 20x the cost of the driver's own line API, which is only used
     * at render scale 1. An explicit SDL_RENDER_LINE_METHOD wins, so the
     * other methods stay reachable for comparison. Must precede the renderer. */
    if (!SDL_GetHint(SDL_HINT_RENDER_LINE_METHOD))
        SDL_SetHint(SDL_HINT_RENDER_LINE_METHOD, "2");

    app.renderer = SDL_CreateRenderer(app.window, NULL);
    if (!app.renderer) {
        SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
        return 1;
    }
    /* The window's pixel size is only known once the renderer exists, and
     * the first pixel-size event may well arrive after the first frame. */
    update_sizes(&app);
    SDL_Log("window %dx%d points, renderer %dx%d pixels (scale %.2f)",
            app.winW, app.winH, app.outW, app.outH, (double)app.scale);

    /* Pace the loop off the display when we can (it also stops the tearing
     * an 8 ms sleep never did); only fall back to sleeping if vsync is not
     * available, as it is not for the offscreen driver. */
    app.vsync = SDL_SetRenderVSync(app.renderer, 1);
    SDL_Log("vsync: %s", app.vsync ? "on" : "unavailable");

    while (1) {
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            switch (ev.type) {
            case SDL_EVENT_QUIT:
                return 0;
            case SDL_EVENT_KEY_DOWN:
                /* OS key repeats are ignored: the held-key table below does
                 * the repeating, so it can be limited to the few keys where
                 * it makes sense and timed however we like. */
                if (ev.key.repeat) break;
                handle_key(&app, &ev.key);
                hold_key(&app, &ev.key);
                break;
            case SDL_EVENT_KEY_UP:
                release_key(&app, ev.key.key);
                break;
            case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
            case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED:
                update_sizes(&app);
                break;
            case SDL_EVENT_WINDOW_FOCUS_LOST:
                app.nheld = 0;      /* no KEY_UP arrives once we are unfocused */
                break;
            default:
                break;
            }
        }

        Uint64 now = SDL_GetTicks();

        /* fire the keys that have been held long enough */
        for (int i = 0; i < app.nheld; ++i) {
            if ((Sint64)(now - app.held[i].next) >= 0) {
                SDL_KeyboardEvent ke;
                SDL_zero(ke);
                ke.type = SDL_EVENT_KEY_DOWN;
                ke.key = app.held[i].key;
                ke.mod = app.held[i].mod;
                ke.down = true;
                handle_key(&app, &ke);
                app.held[i].next = now + REPEAT_RATE_MS;
            }
        }

        if (app.animate) {
            double dt = (double)(now - app.lastTicks) / 1000.0;
            for (int k = 0; k < WHEELS; ++k)
                app.p.s[k] = fmod(app.p.s[k] + 0.008 * (k + 1) * dt, 1.0);
            app.dirtyCurve = true;
        }
        app.lastTicks = now;

        if (!update_geometry(&app)) continue;

        render_frame(&app);
        draw_overlays(&app);

        SDL_RenderPresent(app.renderer);
        if (!app.vsync)
            SDL_Delay(8);
    }
}
