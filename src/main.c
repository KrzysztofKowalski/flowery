/* main.c — flowery : interactive spirograph renderer.
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
 */
#include <SDL3/SDL.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "flowery.h"

#define WHEELS 3

/* Held-key repeat: how long the key has to be down before it starts
 * repeating, and how often it fires afterwards. */
#define REPEAT_DELAY_MS 300
#define REPEAT_RATE_MS  40
#define MAX_HELD        8

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
    FloweryParams p;
    int           sel;          /* selected wheel 0..2 */
    bool          rainbow;
    bool          animate;
    bool          help;
    HeldKey       held[MAX_HELD];
    int           nheld;
    Uint64        lastTicks;
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
 * window: the window is measured in points, the back buffer in pixels. All
 * drawing is done in points with the renderer scaled up, so the curve is
 * rasterised at the full native resolution instead of being stretched, and
 * looks the same size on every display. */
static void
update_sizes(App *a)
{
    float scale = SDL_GetWindowDisplayScale(a->window);

    if (!(scale > 0.0f)) scale = 1.0f;
    a->scale = scale;
    SDL_GetWindowSize(a->window, &a->winW, &a->winH);
    SDL_SetRenderScale(a->renderer, scale, scale);
    SDL_GetCurrentRenderOutputSize(a->renderer, &a->outW, &a->outH);
}

/* Map world coordinates to the drawing surface, preserving aspect
 * ratio (like gnuplot `set size ratio -1`) and centering. */
static void
build_screen_points(const App *a, const double *xs, const double *ys,
                    SDL_FPoint *pts, int n)
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

    scale = fmin(availW / spanX, availH / spanY);
    mx = (double)a->winW / 2.0;
    my = (double)a->winH / 2.0;

    cx = (minx + maxx) / 2.0;
    cy = (miny + maxy) / 2.0;

    for (int i = 0; i < n; ++i) {
        pts[i].x = (float)(mx + (xs[i] - cx) * scale);
        pts[i].y = (float)(my - (ys[i] - cy) * scale); /* flip y for screen */
    }
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
 * resolution of its own and can be scaled to anything. */
static void
save_svg(const App *a, const char *fname)
{
    double *xs, *ys;
    SDL_FPoint *pts;
    FILE *fp;

    int n = a->p.samples;
    xs = malloc(sizeof(double) * n);
    ys = malloc(sizeof(double) * n);
    pts = malloc(sizeof(SDL_FPoint) * n);
    if (!xs || !ys || !pts) { free(xs); free(ys); free(pts); return; }

    flowery_points(&a->p, xs, ys);
    build_screen_points(a, xs, ys, pts, n);

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
render_frame(App *a)
{
    const int n = a->p.samples;
    double *xs = malloc(sizeof(double) * n);
    double *ys = malloc(sizeof(double) * n);
    SDL_FPoint *pts = malloc(sizeof(SDL_FPoint) * n);
    if (!xs || !ys || !pts) return;

    flowery_points(&a->p, xs, ys);
    build_screen_points(a, xs, ys, pts, n);

    SDL_SetRenderDrawColor(a->renderer, 8, 8, 12, 255);
    SDL_RenderClear(a->renderer);

    if (a->rainbow) {
        /* draw each segment with its own hue */
        for (int i = 1; i < n; ++i) {
            float hue = 360.0f * (float)i / (float)n;
            Uint8 r, g, b;
            hsv_to_rgb(hue, &r, &g, &b);
            SDL_SetRenderDrawColor(a->renderer, r, g, b, 255);
            SDL_RenderLine(a->renderer, pts[i-1].x, pts[i-1].y,
                           pts[i].x, pts[i].y);
        }
    } else {
        SDL_SetRenderDrawColor(a->renderer, 148, 0, 211, 255);
        SDL_RenderLines(a->renderer, pts, n);
    }

    free(xs); free(ys); free(pts);
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
             "s=(%.3f %.3f %.3f)   samples=%d%s   %dx%d@%.0fx",
             a->sel,
             a->p.n[0], a->p.n[1], a->p.n[2],
             a->p.a[0], a->p.a[1], a->p.a[2],
             a->p.s[0], a->p.s[1], a->p.s[2],
             a->p.samples,
             a->rainbow ? "  rainbow" : "",
             a->winW, a->winH, (double)a->scale);
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
        break;
    case SDLK_DOWN:
        a->p.n[k] -= shifted ? 10.0 : 1.0;
        break;
    case SDLK_LEFT:
        a->p.s[k] -= shifted ? 0.05 : 0.01;
        a->p.s[k] = fmod(a->p.s[k] + 1.0, 1.0);
        break;
    case SDLK_RIGHT:
        a->p.s[k] = fmod(a->p.s[k] + (shifted ? 0.05 : 0.01), 1.0);
        break;

    case SDLK_LEFTBRACKET:
        a->p.a[k] -= shifted ? 0.5 : 0.1;
        if (a->p.a[k] < 0.0) a->p.a[k] = 0.0;
        break;
    case SDLK_RIGHTBRACKET:
        a->p.a[k] += shifted ? 0.5 : 0.1;
        break;

    case SDLK_PLUS:
    case SDLK_EQUALS:
        a->p.samples = (int)fmin((double)a->p.samples * 2.0, 1048576.0);
        break;
    case SDLK_MINUS:
        a->p.samples = (int)fmax((double)a->p.samples / 2.0, 32.0);
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
    App app = {0};

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

    char status[256];

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
        }
        app.lastTicks = now;

        render_frame(&app);
        format_status(&app, status, sizeof status);
        draw_debug_text(&app, status);
        if (app.help)
            draw_help(&app);

        SDL_RenderPresent(app.renderer);
        SDL_Delay(8);
    }
}
