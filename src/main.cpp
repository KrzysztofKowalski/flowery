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
 *   , .          stroke width, thinner / thicker
 *   < >          waves along the curve, fewer / more (wave mode)
 *   v            cycle the stroke variation: flat / taper / speed / wave
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

#include <bit>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <ctime>

#if defined(__AVX2__)
#  include <immintrin.h>
#endif

#include "flowery.h"

/* -------------------------------------------------------------- raw GL */

/* The curve is drawn as a triangle strip with a shader of our own rather than
 * by SDL, because on this machine SDL's GL line drawing costs ~72ns a point
 * and the same geometry handed over as triangles costs ~3ns (NOTES.md has the
 * table; the harness that produced it is in tmp/linebench/). Past six figures
 * that difference is the whole frame.
 *
 * SDL's text cannot come along for the ride: once our GL state has been bound,
 * SDL_RenderDebugText puts 0 pixels on the screen - it returns true and sets
 * no error - and SDL_FlushRenderer, the documented "call this between SDL's
 * render API and the low-level API", does not repair it from either side. So
 * the HUD is drawn with the same raw GL, from an atlas of SDL's own debug font
 * captured through a software renderer before any of this starts. Same 8x8
 * glyphs, so the HUD looks the way it always did.
 *
 * GL is declared by hand here, so that no GL header can fight SDL's. */
typedef unsigned int GLenum_t;
typedef unsigned int GLuint_t;
typedef int          GLint_t;
typedef int          GLsizei_t;
typedef char         GLchar_t;
typedef unsigned int GLbitfield_t;
typedef ptrdiff_t    GLsizeiptr_t;

#define GL_TRIANGLES        0x0004
#define GL_TRIANGLE_STRIP   0x0005
#define GL_DEPTH_TEST       0x0B71
#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_FLOAT            0x1406
#define GL_UNSIGNED_BYTE    0x1401
#define GL_ARRAY_BUFFER     0x8892
#define GL_DYNAMIC_DRAW     0x88E8
#define GL_VERTEX_SHADER    0x8B31
#define GL_FRAGMENT_SHADER  0x8B30
#define GL_COMPILE_STATUS   0x8B81
#define GL_LINK_STATUS      0x8B82
#define GL_RGBA             0x1908
#define GL_BGRA             0x80E1
#define GL_TEXTURE_2D       0x0DE1
#define GL_TEXTURE0         0x84C0
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_WRAP_S   0x2802
#define GL_TEXTURE_WRAP_T   0x2803
#define GL_NEAREST          0x2600
#define GL_CLAMP_TO_EDGE    0x812F
#define GL_BLEND            0x0BE2
#define GL_SRC_ALPHA        0x0302
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#define GL_PACK_ALIGNMENT   0x0D05
#define GL_UNPACK_ALIGNMENT 0x0CF5
#define GL_NO_ERROR         0

#define GLFUNCS \
    X(void,     glClear,        (GLbitfield_t)) \
    X(void,     glClearColor,   (float, float, float, float)) \
    X(void,     glViewport,     (int, int, int, int)) \
    X(void,     glEnable,       (GLenum_t)) \
    X(void,     glDisable,      (GLenum_t)) \
    X(void,     glBlendFunc,    (GLenum_t, GLenum_t)) \
    X(void,     glPixelStorei,  (GLenum_t, int)) \
    X(void,     glReadPixels,   (int, int, int, int, GLenum_t, GLenum_t, void *)) \
    X(GLenum_t, glGetError,     (void)) \
    X(GLuint_t, glCreateShader, (GLenum_t)) \
    X(void,     glShaderSource, (GLuint_t, int, const GLchar_t *const *, const int *)) \
    X(void,     glCompileShader,(GLuint_t)) \
    X(void,     glGetShaderiv,  (GLuint_t, GLenum_t, int *)) \
    X(void,     glGetShaderInfoLog, (GLuint_t, int, int *, GLchar_t *)) \
    X(void,     glDeleteShader, (GLuint_t)) \
    X(GLuint_t, glCreateProgram, (void)) \
    X(void,     glAttachShader, (GLuint_t, GLuint_t)) \
    X(void,     glLinkProgram,  (GLuint_t)) \
    X(void,     glGetProgramiv, (GLuint_t, GLenum_t, int *)) \
    X(void,     glGetProgramInfoLog, (GLuint_t, int, int *, GLchar_t *)) \
    X(void,     glUseProgram,   (GLuint_t)) \
    X(GLint_t,  glGetUniformLocation, (GLuint_t, const GLchar_t *)) \
    X(void,     glUniform1i,    (GLint_t, int)) \
    X(void,     glUniform1f,    (GLint_t, float)) \
    X(void,     glUniform2f,    (GLint_t, float, float)) \
    X(void,     glUniform4f,    (GLint_t, float, float, float, float)) \
    X(void,     glGenVertexArrays, (int, GLuint_t *)) \
    X(void,     glBindVertexArray, (GLuint_t)) \
    X(void,     glGenBuffers,   (int, GLuint_t *)) \
    X(void,     glBindBuffer,   (GLenum_t, GLuint_t)) \
    X(void,     glBufferData,   (GLenum_t, GLsizeiptr_t, const void *, GLenum_t)) \
    X(void,     glVertexAttribPointer, (GLuint_t, GLint_t, GLenum_t, unsigned char, GLsizei_t, const void *)) \
    X(void,     glEnableVertexAttribArray, (GLuint_t)) \
    X(void,     glDrawArrays,   (GLenum_t, int, GLsizei_t)) \
    X(void,     glGenTextures,  (int, GLuint_t *)) \
    X(void,     glBindTexture,  (GLenum_t, GLuint_t)) \
    X(void,     glActiveTexture,(GLenum_t)) \
    X(void,     glTexImage2D,   (GLenum_t, int, int, int, int, int, GLenum_t, GLenum_t, const void *)) \
    X(void,     glTexParameteri,(GLenum_t, GLenum_t, int))

#define X(ret, name, args) typedef ret (*PFN_##name) args; static PFN_##name p_##name;
GLFUNCS
#undef X

static bool
load_gl(void)
{
    bool ok = true;
#define X(ret, name, args) \
    p_##name = (PFN_##name)SDL_GL_GetProcAddress(#name); \
    if (!p_##name) { SDL_Log("missing GL entry point %s", #name); ok = false; }
    GLFUNCS
#undef X
    return ok;
}

/* One vertex of the curve strip: where the point is, the half-width normal to
 * push it along, which side to push it to, and the colour. The width is folded
 * into the normal on the CPU, so the shader is one multiply-add and the width
 * is free to vary per point - which is what every stroke mode needs. */
#define CURVE_FLOATS 9

/* How far the strip is pushed either side of the curve, as a multiple of half
 * the stroke. Exactly half is not enough: a pixel only lights up when its
 * centre falls inside the triangle, so a one-pixel band centred on the curve
 * leaves out the diagonal pixels a Bresenham line would have lit. sqrt(2) is
 * where the far corner of the neighbouring pixel comes within reach at 45
 * degrees, and it is the value that stays just under the line path rather than
 * just over. Measured at N=2001, against what SDL_RenderLines draws:
 *
 *     1.0    71.3% of its ink, 3.8% of its pixels with nothing within 1px
 *     1.2    83.6%                2.1%
 *     1.414  96.6%                1.1%   <- this
 *     1.5   101.7%                0.9%
 */
#define STROKE_HALF_BIAS 1.41421356f

/* One vertex of the HUD: a position in window points, a texture coordinate and
 * a colour. */
#define HUD_FLOATS 8

/* SDL's debug font is 8x8. The atlas is cut from the font itself rather than
 * hand-written out, so the HUD keeps the exact glyphs it had. */
#define FONT_W 8
#define FONT_H 8
#define FONT_FIRST 32
#define FONT_LAST  126
#define FONT_N     (FONT_LAST - FONT_FIRST + 1)

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
 * SDL_RenderLine calls, so the hue walks the wheel in whole steps and each
 * step is one polyline.
 *
 * 1530 is the whole wheel, not a taste call: at full saturation the colour
 * has one channel at 255, one at 0 and one walking between them, so eight-bit
 * RGB can express 6*255 of them and no more. Measured, the number of distinct
 * colours actually produced is 256 at 256 steps, 1023 at 1024, and 1527 at
 * 1530 - and still 1527 at 1536, 4096 and 65536, i.e. everything past the
 * wheel is a repeat.
 *
 * The cost is per draw call, not per colour: the run count is bounded by both
 * this and the number of drawn points, so the ceiling is 1530 calls, about
 * 0.36 ms, whatever the sample count. */
#define RAINBOW_STEPS 1530

/* Ceiling on the number of polylines the rainbow may cut the curve into.
 *
 * The hue follows the point's direction, and a spirograph's direction sweeps
 * the whole circle once per turn, so left alone the colour changes every few
 * points: 166k runs at N=262144, which at ~150ns a draw call is 25ms of the
 * frame. A run is therefore not cut for a colour change until it is
 * m/MAX_RAINBOW_RUNS points long - no cut at all at the default 2001 sam-
 * ples, 32 points at 262144, 128 at 1048576 - so the ceiling is ~1.2ms of
 * drawing whatever the sample count, and the colour is per point exactly
 * where the curve is coarse enough for that to be what the eye sees. */
#define MAX_RAINBOW_RUNS 8192

/* Widest stroke, in device pixels. SDL 3.4.16 has no line width at all - no
 * SDL_SetRenderLineWidth - and the geometry API that could draw a genuinely
 * thick line measured about 15x the cost of the driver line path this uses,
 * so a stroke is laid down as several offset copies of the thin line instead.
 * That is why the ceiling is a modest one: the cost is linear in this. */
#define MAX_WIDTH 16

/* How the stroke width varies along the curve. Widths are in device pixels
 * and never below 1, so the flat, one-pixel case is what the drawing path is
 * tuned for and stays a single polyline per colour run. */
enum {
    WIDTH_FLAT = 0,     /* every point the same width */
    WIDTH_TAPER,        /* thin at t=0, full width at t=1 */
    WIDTH_SPEED,        /* thin where the curve is travelling fast */
    WIDTH_WAVE,         /* a sine between 1 pixel and the full width */
    WIDTH_MODE_COUNT
};

static const char *const width_mode_names[WIDTH_MODE_COUNT] = {
    "flat", "taper", "speed", "wave"
};

/* Widest the stroke gets where the wave peaks, and how many waves fit along
 * the curve. The wave is the one mode whose shape has a parameter of its own,
 * so it gets two: how wide it swings, and how often. */
#define MAX_WAVES 64

/* What `v` sets the width to when it finds a one-pixel line - a variation of
 * a one-pixel line is still a one-pixel line, so the mode has to bring its
 * own room to move. Seven is as wide as the wave is meant to go by default;
 * `,` and `.` still take it anywhere up to MAX_WIDTH. */
#define DEFAULT_VARIED_WIDTH 7

/* Waves along the curve when the wave mode is first reached. */
#define DEFAULT_WAVE_COUNT 7

/* Entries in the wave's cosine table. One cosine per drawn point would be a
 * libm call per point - at a million points that is real money for a number
 * that only ever lands on one of `maxw` integers, so the wave is a table
 * lookup on a phase that is accumulated rather than recomputed. A power of
 * two, because the phase wraps with a mask. */
#define WAVE_TABLE 1024

/* Two pi, for that table. flowery.cpp keeps its own copy for the sampling
 * kernel; this is a separate translation unit. */
#define WAVE_TAU 6.283185307179586

/* Points either side of a drawn point that the speed is averaged over, when
 * the width comes from the speed. The raw speed of a spirograph swings over
 * orders of magnitude - it is a sum of three rotating vectors - and smoothing
 * it both makes the result look like a gradient and keeps the runs the
 * drawing walks long enough that the call count stays sane. */
#define SPEED_SMOOTH 8

/* Bins the speed is ranked in. The ranking is what turns "fast is thin" into a
 * stroke that uses the whole width range instead of piling up at one end. */
#define SPEED_BINS 128

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
    int           width;        /* stroke width in device pixels, 1 = thin */
    int           widthMode;    /* WIDTH_FLAT / WIDTH_TAPER / WIDTH_SPEED / ... */
    int           waveCount;    /* waves along the curve, WIDTH_WAVE only */
    HeldKey       held[MAX_HELD];
    int           nheld;
    Uint64        lastTicks;

    /* Frame timing, in milliseconds. `seen` counts every frame the loop has
     * run, `frames` only those past the warm-up that went into the sums. */
    bool          profile;
    int           benchFrames;  /* FLOWERY_BENCH: quit after this many; 0 = run */
    const char   *benchBmp;     /* leave one frame behind for the eye */
    int           seen, frames;
    double        tKernel, tLayout, tThin, tDraw, tHud, tPresent, tTotal;

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
    /* The bounding box of xs/ys as {minx, miny, maxx, maxy}. The sampling
     * kernel fills this in on its way through the points, so the layout does
     * not have to walk 16 MB of them again to find out where they are; it
     * stays valid for as long as xs/ys do, which is what dirtyLayout without
     * dirtyCurve (a resize) relies on. */
    double      bbox[4];

    /* Stroke geometry, only built when the stroke is more than one pixel of
     * flat: `wid` is the width at each drawn point and `nrm` the unit normal
     * there, which is the direction the offset copies are pushed along.
     * `thk` is the scratch one copy is written into and `spd` the speed the
     * speed mode reads. None of it is touched by the plain 1-pixel path. */
    int        *wid;
    SDL_FPoint *nrm;
    SDL_FPoint *thk;
    float      *spd;
    bool        strokeDirty;    /* wid/nrm need rebuilding */

    /* The rainbow's hue bucket at each drawn point, when the hue comes from
     * where the point is rather than from which sample it was. Position-
     * dependent, so it is rebuilt with the layout, not with the stroke. */
    int        *rbq;
    bool        rbqDirty;
    bool        rainbowParam;   /* hue from the sample index, the old way */

    /* The raw GL path, which is the default. `rawgl` is false only when
     * FLOWERY_DRAW=lines was asked for: that keeps the SDL renderer and every
     * line of the code that draws through it, so the two can still be
     * compared. They never draw into the same frame - SDL's text stops
     * working the moment our GL state is bound, so the choice is made once at
     * startup and not revisited. */
    bool          rawgl;
    SDL_GLContext glctx;
    GLuint_t      curveProg, curveVao, curveVbo;
    GLint_t       curveScale, curveOffset;
    GLuint_t      hudProg, hudVao, hudVbo, hudTex;
    GLint_t       hudScale, hudOffset, hudPointScale, hudTexUni;
    float        *vbuf;         /* curve vertices, grown on demand */
    int           vcap;         /* vertices the buffer can hold */
    bool          vertsDirty;   /* the strip in the VBO is out of date */
    float        *hbuf;         /* HUD vertices, same */
    int           hcap;
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

/* --------------------------------------------------------------- timing */

/* Frame timing. A handful of counter reads is nothing next to a frame, so
 * they are always taken and the numbers are always accumulated; only the
 * reporting is behind a flag. FLOWERY_PROFILE=1 prints a running average
 * every PROFILE_REPORT_FRAMES frames, and FLOWERY_BENCH=<n> additionally
 * holds the curve still and forces it dirty every frame - the worst case,
 * and the only way to compare two builds, because an idle frame does no
 * curve maths at all and there is nothing to compare. */
#define PROFILE_REPORT_FRAMES 60

/* The first frames after the window opens are not representative: the
 * compositor is still settling the size and the first draw compiles shaders.
 * A benchmark throws them away. */
#define BENCH_WARMUP_FRAMES 30

static double
perf_ms(Uint64 t0, Uint64 t1)
{
    static double freq = 0.0;

    if (freq == 0.0) freq = (double)SDL_GetPerformanceFrequency();
    return (double)(t1 - t0) * 1000.0 / freq;
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
    if (a->rawgl) {
        /* The drawable, which for a scaled display is larger than the window.
         * There is no renderer to ask, and no render scale to keep at 1 -
         * raw GL draws in whatever units the shader is told to. */
        SDL_GetWindowSizeInPixels(a->window, &a->outW, &a->outH);
    } else {
        SDL_SetRenderScale(a->renderer, 1.0f, 1.0f);
        SDL_GetCurrentRenderOutputSize(a->renderer, &a->outW, &a->outH);
    }
    a->dirtyLayout = true;
    /* Only worth saying out loud when something is being measured: a drag
     * resize would otherwise print a line per frame. */
    if (a->profile)
        SDL_Log("sizes: %dx%d points -> %dx%d pixels, scale %.2f",
                a->winW, a->winH, a->outW, a->outH, (double)a->scale);
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
    int        *wid  = (int *)realloc(a->wid, sizeof(int) * n);
    SDL_FPoint *nrm  = (SDL_FPoint *)realloc(a->nrm, sizeof(SDL_FPoint) * n);
    SDL_FPoint *thk  = (SDL_FPoint *)realloc(a->thk, sizeof(SDL_FPoint) * n);
    float      *spd  = (float *)realloc(a->spd, sizeof(float) * n);
    int        *rbq  = (int *)realloc(a->rbq, sizeof(int) * n);

    /* If any of them failed, keep what we had and report; the caller leaves
     * the frame alone rather than drawing from a half-updated set. */
    if (xs) a->xs = xs;
    if (ys) a->ys = ys;
    if (pts) a->pts = pts;
    if (disp) a->disp = disp;
    if (didx) a->didx = didx;
    if (wid) a->wid = wid;
    if (nrm) a->nrm = nrm;
    if (thk) a->thk = thk;
    if (spd) a->spd = spd;
    if (rbq) a->rbq = rbq;
    if (!xs || !ys || !pts || !disp || !didx
        || !wid || !nrm || !thk || !spd || !rbq) return false;

    a->cap = n;
    return true;
}

/* Map world coordinates to the drawing surface, preserving aspect
 * ratio (like gnuplot `set size ratio -1`) and centering.
 *
 * `bbox` is the box of xs/ys, which the caller already has: the screen takes
 * the one flowery_points() filled in beside them, and save_svg() the one it
 * asked for when it sampled. Nothing here walks the points to find it.
 *
 * The layout is worked out in window points and then multiplied by `zoom`:
 * the screen wants physical pixels (zoom = the display scale), the saved SVG
 * wants points (zoom = 1), which is what keeps the file independent of the
 * display it happened to be written on. */
static void
build_screen_points(const App *a, const double *xs, const double *ys,
                    SDL_FPoint *pts, int n, double zoom, const double *bbox)
{
    double minx, miny, maxx, maxy;
    double scale, mx, my;
    double margin, availW, availH, spanX, spanY, cx, cy;

    minx = bbox[0]; miny = bbox[1]; maxx = bbox[2]; maxy = bbox[3];

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

/* ---------------------------------------------------------------- stroke */

/* One pass over the drawn points, filling in two things that come out of the
 * same difference between neighbours:
 *
 *   - the unit normal at each point, which is the direction the offset copies
 *     of a stroke are pushed along;
 *   - the speed there, in device pixels per sample, when the width is going to
 *     come from it.
 *
 * They are one pass rather than two because the square root that turns the
 * difference into a normal is the same square root the speed needs, and doing
 * them apart cost a second walk over the arrays and a second root for every
 * point - several milliseconds at a million points, which is more than the
 * drawing it was in aid of.
 *
 * The last point looks backwards so that every point ends up with the same
 * sense, and a repeated point has no tangent at all and borrows its
 * neighbour's. */
static void
build_normals(App *a, bool want_speed)
{
    const int m = a->ndisp;
    const SDL_FPoint *p = a->disp;

    for (int i = 0; i < m; ++i) {
        const int j = (i + 1 < m) ? i + 1 : i - 1;
        float dx = 1.0f, dy = 0.0f, len;

        if (j >= 0) {
            dx = p[j].x - p[i].x;
            dy = p[j].y - p[i].y;
        }

        len = sqrtf(dx * dx + dy * dy);

        if (want_speed) {
            int step = j >= 0 ? a->didx[i] - a->didx[j] : 0;
            if (step < 0) step = -step;
            a->spd[i] = len / (step > 0 ? (float)step : 1.0f);
        }

        if (len > 1e-6f) {
            a->nrm[i].x = -dy / len;
            a->nrm[i].y =  dx / len;
        } else if (i > 0) {
            a->nrm[i] = a->nrm[i - 1];      /* a repeated point has no tangent */
        } else {
            a->nrm[i].x = 0.0f;
            a->nrm[i].y = 0.0f;
        }
    }
}

/* log2 of a positive float, to about a tenth of a bin.
 *
 * The exponent is already sitting in the top bits of the float and the
 * mantissa is close enough to linear over [1,2) to stand in for the fraction,
 * so this is a shift, an and and an add. It exists because ranking the speeds
 * below wants a log at every point, and the honest one cost about 50 cycles -
 * at a million points that was 20 ms, four times the drawing it was in aid
 * of. Ordering into 128 bins is all it has to get right, and it is monotone. */
static float
fast_log2(float x)
{
    const std::uint32_t bits = std::bit_cast<std::uint32_t>(x);
    const float e = (float)((int)((bits >> 23) & 0xFFu) - 127);
    const float m = (float)(bits & 0x7FFFFFu) * (1.0f / 8388608.0f);
    return e + m;
}

/* Width from how fast the curve travels, in device pixels per sample: fast is
 * thin. What the width follows is the point's *rank* in the speed distribution
 * rather than the speed itself, because a spirograph's speed spans orders of
 * magnitude and any direct mapping puts almost the whole curve at one end of
 * the range. Ranking fixes that without needing to know what the distribution
 * is, on this curve or the next one. */
static void
build_widths_speed(App *a)
{
    const int m = a->ndisp;
    const int maxw = a->width > MAX_WIDTH ? MAX_WIDTH : a->width;
    float *spd = a->spd;
    float cdf[SPEED_BINS];
    int hist[SPEED_BINS];
    float lo = 0.0f, hi = 0.0f, lr;
    int sum = 0, cnt = 0;
    double run = 0.0;

    for (int i = 0; i < SPEED_BINS; ++i) hist[i] = 0;

    /* build_normals() has already filled in the speed - it had the segment
     * difference in hand anyway. All that is wanted here is its log2, which
     * is the scale the ranking is done on: the distribution is heavy-tailed,
     * and on a linear scale the top bin would hold almost everything. */
    for (int i = 0; i < m; ++i) {
        const float lg = fast_log2(spd[i] > 0.0f ? spd[i] : 1e-9f);
        spd[i] = lg;
        if (i == 0 || lg < lo) lo = lg;
        if (i == 0 || lg > hi) hi = lg;
    }

    lr = hi - lo;
    if (!(lr > 0.0f)) lr = 1.0f;
    for (int i = 0; i < m; ++i) {
        int k = (int)((spd[i] - lo) / lr * SPEED_BINS);
        if (k < 0) k = 0;
        if (k >= SPEED_BINS) k = SPEED_BINS - 1;
        spd[i] = (float)k;              /* the bin, so the next pass is a lookup */
        ++hist[k];
    }
    for (int i = 0; i < SPEED_BINS; ++i) {
        sum += hist[i];
        cdf[i] = m > 0 ? (float)((double)sum / (double)m) : 0.0f;
    }

    /* The percentile, averaged over a short window, as a width: fast is thin.
     * Because it is a percentile the widths spread over the whole range
     * whatever the curve's speed happens to look like, which is what makes
     * this a stroke rather than a special case per wheel set. The average is
     * a running sum, and it is what keeps the runs - which the drawing walks
     * by looking for equal widths - long enough that the call count stays
     * sane. */
    const int half = SPEED_SMOOTH;
    for (int i = 0; i < m && i <= half; ++i) { run += cdf[(int)spd[i]]; ++cnt; }
    for (int i = 0; i < m; ++i) {
        const double u = cnt > 0 ? run / cnt : 0.0;
        int w = maxw - (int)((maxw - 1) * u + 0.5);

        if (w < 1) w = 1;
        if (w > maxw) w = maxw;
        a->wid[i] = w;

        if (i - half >= 0)     { run -= cdf[(int)spd[i - half]]; --cnt; }
        if (i + half + 1 < m)  { run += cdf[(int)spd[i + half + 1]]; ++cnt; }
    }
}

/* The wave: a sine that starts at one pixel and swells to the full width and
 * back, `waveCount` times along the curve.
 *
 *     w(t) = 1 + (maxw - 1) * (0.5 - 0.5*cos(2*pi*waves*t))
 *
 * so t=0 is exactly the thinnest line the app can draw rather than an
 * arbitrary point on the sine, and the peaks are exactly `maxw`. Phase is
 * accumulated across the drawn points instead of recomputed from the sample
 * index, because a division and a cosine per point is more than the drawing
 * this is in aid of: what is left per point is a multiply-add and a lookup.
 * The table holds the finished widths, so the rounding to whole pixels
 * happens once instead of a million times. */
static void
build_widths_wave(App *a, int maxw)
{
    const int m = a->ndisp;
    const int n = a->p.samples;
    const int waves = a->waveCount < 1 ? 1
                    : (a->waveCount > MAX_WAVES ? MAX_WAVES : a->waveCount);
    int tab[WAVE_TABLE];

    for (int j = 0; j < WAVE_TABLE; ++j) {
        const double ph = WAVE_TAU * (double)j / (double)WAVE_TABLE;
        int w = 1 + (int)((double)(maxw - 1) * (0.5 - 0.5 * cos(ph)));
        if (w < 1) w = 1;
        if (w > maxw) w = maxw;
        tab[j] = w;
    }

    /* Table entries per sample index: `waves` whole turns over n-1 of them. */
    const double step = n > 1 ? (double)waves * (double)WAVE_TABLE
                                / (double)(n - 1) : 0.0;
    double ph = 0.0;
    int prev = 0;

    for (int i = 0; i < m; ++i) {
        const int d = a->didx[i];
        ph += (double)(d - prev) * step;
        prev = d;
        a->wid[i] = tab[(int)ph & (WAVE_TABLE - 1)];
    }
}

/* The stroke width at each drawn point, in device pixels, always at least 1.
 * `wid` is indexed like `disp`, and the drawing walks it looking for runs of
 * equal width, so a mode that changes width often also costs more draw
 * calls - which is why the speed one is smoothed before it gets here. */
static void
build_widths(App *a)
{
    const int m = a->ndisp;
    const int maxw = a->width > MAX_WIDTH ? MAX_WIDTH : (a->width > 0 ? a->width : 1);
    const int n = a->p.samples > 1 ? a->p.samples : 1;

    if (a->widthMode == WIDTH_SPEED) {
        build_widths_speed(a);
        return;
    }
    if (a->widthMode == WIDTH_TAPER) {
        for (int i = 0; i < m; ++i)
            a->wid[i] = 1 + (maxw - 1) * a->didx[i] / (n - 1);
        return;
    }
    if (a->widthMode == WIDTH_WAVE) {
        build_widths_wave(a, maxw);
        return;
    }
    for (int i = 0; i < m; ++i) a->wid[i] = maxw;
}

/* Built with the layout, but defined down in the render section with the rest
 * of the colour. */
static void build_rainbow_buckets(App *a);

/* Rebuild exactly as much as is out of date. Cheap when nothing changed,
 * which is the common case: an idle frame does no curve maths at all. */
static bool
update_geometry(App *a)
{
    const int n = a->p.samples > 0 ? a->p.samples : 1;
    /* Warm-up frames are drawn but not timed. */
    const bool count = !a->benchFrames || a->seen > BENCH_WARMUP_FRAMES;

    if (!ensure_buffers(a, n)) return false;

    if (a->dirtyCurve) {
        const Uint64 t0 = SDL_GetPerformanceCounter();
        flowery_points(&a->p, a->xs, a->ys, a->bbox);
        if (count) a->tKernel += perf_ms(t0, SDL_GetPerformanceCounter());
        a->dirtyCurve = false;
        a->dirtyLayout = true;
    }
    if (a->dirtyLayout) {
        Uint64 t0 = SDL_GetPerformanceCounter();
        build_screen_points(a, a->xs, a->ys, a->pts, n, (double)a->scale,
                            a->bbox);
        if (count) a->tLayout += perf_ms(t0, SDL_GetPerformanceCounter());

        t0 = SDL_GetPerformanceCounter();
        a->ndisp = build_display_points(a->pts, n, a->disp, a->didx,
                                        (float)(MIN_SEGMENT_PTS * a->scale));
        if (count) a->tThin += perf_ms(t0, SDL_GetPerformanceCounter());
        a->dirtyLayout = false;
        a->strokeDirty = true;
        a->rbqDirty = true;
        a->vertsDirty = true;
    }

    /* The hue buckets are positions on the screen, so they follow the layout
     * rather than the stroke - and they are only needed when the hue is
     * coming from them. Turning the rainbow on later finds the flag still
     * set, which is what it is for. */
    if (a->rbqDirty && a->rainbow && !a->rainbowParam) {
        const Uint64 t0 = SDL_GetPerformanceCounter();
        build_rainbow_buckets(a);
        if (count) a->tThin += perf_ms(t0, SDL_GetPerformanceCounter());
        a->rbqDirty = false;
    }

    /* Only when the stroke is more than one flat pixel. The plain path never
     * builds any of this, which is the point: it is the case that has to stay
     * a single polyline per colour run.
     *
     * The raw GL path is the exception: its triangle strip needs the unit
     * normal at every point to push the pair of vertices apart, even for a
     * one-pixel stroke, so it asks for this whenever it is drawing. */
    if (a->strokeDirty && (a->width > 1 || a->rawgl)) {
        const Uint64 t0 = SDL_GetPerformanceCounter();
        build_normals(a, a->widthMode == WIDTH_SPEED);
        build_widths(a);
        if (count) a->tThin += perf_ms(t0, SDL_GetPerformanceCounter());
        a->strokeDirty = false;
        a->vertsDirty = true;
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
    double bbox[4];
    FILE *fp;

    if (!xs || !ys || !pts) { free(xs); free(ys); free(pts); return; }

    flowery_points(&a->p, xs, ys, bbox);
    build_screen_points(a, xs, ys, pts, n, 1.0, bbox); /* points, not pixels */

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
    if (a->rawgl) {
        /* glReadPixels hands the rows over bottom-up. BMP wants them that way
         * too, but SDL_SaveBMP writes the surface's first row *last*, so the
         * surface has to hold them top-down - which is also how
         * SDL_RenderReadPixels hands them over on the other path. Flip, so
         * the two paths produce the same file and a saved frame is not upside
         * down. */
        SDL_Surface *surf = SDL_CreateSurface(a->outW, a->outH,
                                              SDL_PIXELFORMAT_XRGB8888);
        if (!surf) {
            SDL_Log("could not make a surface to read into: %s", SDL_GetError());
            return;
        }
        p_glPixelStorei(GL_PACK_ALIGNMENT, 4);
        p_glReadPixels(0, 0, a->outW, a->outH, GL_BGRA, GL_UNSIGNED_BYTE,
                       surf->pixels);
        {
            unsigned char *base = (unsigned char *)surf->pixels;
            unsigned char *tmp = (unsigned char *)SDL_malloc((size_t)surf->pitch);

            if (tmp) {
                for (int y = 0; y < a->outH / 2; ++y) {
                    unsigned char *lo = base + (size_t)y * surf->pitch;
                    unsigned char *hi = base + (size_t)(a->outH - 1 - y) * surf->pitch;

                    SDL_memcpy(tmp, lo, (size_t)surf->pitch);
                    SDL_memcpy(lo, hi, (size_t)surf->pitch);
                    SDL_memcpy(hi, tmp, (size_t)surf->pitch);
                }
                SDL_free(tmp);
            }
        }
        if (!SDL_SaveBMP(surf, fname))
            SDL_Log("could not save %s: %s", fname, SDL_GetError());
        else
            SDL_Log("saved %s (%dx%d)", fname, surf->w, surf->h);
        SDL_DestroySurface(surf);
        return;
    }

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

/* --------------------------------------------------------------- raw GL */

/* Vertex positions arrive in device pixels, which is what the layout is
 * computed in, and the two uniforms turn that into clip space: one unit is one
 * pixel and y runs down. */
static const char *kCurveVS =
    "#version 330 core\n"
    "layout(location=0) in vec2 a_pos;\n"
    "layout(location=1) in vec2 a_half;\n"
    "layout(location=2) in float a_side;\n"
    "layout(location=3) in vec4 a_col;\n"
    "uniform vec2 u_scale; uniform vec2 u_offset;\n"
    "out vec4 v_col;\n"
    "void main() {\n"
    "  vec2 p = a_pos + a_half * a_side;\n"
    "  gl_Position = vec4(p * u_scale + u_offset, 0.0, 1.0);\n"
    "  v_col = a_col;\n"
    "}\n";

static const char *kCurveFS =
    "#version 330 core\n"
    "in vec4 v_col; out vec4 o_col;\n"
    "void main() { o_col = v_col; }\n";

/* The HUD arrives in window points and is scaled by the display scale, so the
 * 8x8 font stays 8x8 points on any display - 16x16 device pixels at 2x, which
 * is what SDL's debug text did. Only the atlas's alpha is sampled; the colour
 * comes from the vertex. */
static const char *kHudVS =
    "#version 330 core\n"
    "layout(location=0) in vec2 a_pos;\n"
    "layout(location=1) in vec2 a_uv;\n"
    "layout(location=2) in vec4 a_col;\n"
    "uniform vec2 u_scale; uniform vec2 u_offset; uniform float u_pscale;\n"
    "out vec2 v_uv; out vec4 v_col;\n"
    "void main() {\n"
    "  vec2 p = a_pos * u_pscale;\n"
    "  gl_Position = vec4(p * u_scale + u_offset, 0.0, 1.0);\n"
    "  v_uv = a_uv; v_col = a_col;\n"
    "}\n";

static const char *kHudFS =
    "#version 330 core\n"
    "in vec2 v_uv; in vec4 v_col; out vec4 o_col;\n"
    "uniform sampler2D u_tex;\n"
    "void main() { o_col = vec4(v_col.rgb, v_col.a * texture(u_tex, v_uv).a); }\n";

static GLuint_t
gl_compile(GLenum_t type, const char *src)
{
    const GLchar_t *p = src;
    GLuint_t s = p_glCreateShader(type);

    p_glShaderSource(s, 1, &p, NULL);
    p_glCompileShader(s);

    int ok = 0;
    p_glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[1024];
        log[0] = 0;
        p_glGetShaderInfoLog(s, sizeof log - 1, NULL, log);
        SDL_Log("shader did not compile: %s", log);
        p_glDeleteShader(s);
        return 0;
    }
    return s;
}

static GLuint_t
gl_program(const char *vs, const char *fs)
{
    GLuint_t v = gl_compile(GL_VERTEX_SHADER, vs);
    GLuint_t f = gl_compile(GL_FRAGMENT_SHADER, fs);
    GLuint_t p;

    if (!v || !f) return 0;
    p = p_glCreateProgram();
    p_glAttachShader(p, v);
    p_glAttachShader(p, f);
    p_glLinkProgram(p);

    int ok = 0;
    p_glGetProgramiv(p, GL_LINK_STATUS, &ok);
    if (!ok) {
        char log[1024];
        log[0] = 0;
        p_glGetProgramInfoLog(p, sizeof log - 1, NULL, log);
        SDL_Log("program did not link: %s", log);
        return 0;
    }
    return p;
}

/* Byte offset of attribute `n`, for glVertexAttribPointer. */
#define VOFF(n) ((const void *)(size_t)(n))

/* The glyphs are cut out of SDL's own debug font rather than written out by
 * hand, so the HUD keeps exactly the characters it had. A software renderer
 * draws the printable set into a surface - no window, no GL context, nothing
 * for it to fight with - and the result becomes a GL texture. */
static bool
build_font_atlas(App *a)
{
    const int w = FONT_W * FONT_N;
    SDL_Surface *surf = SDL_CreateSurface(w, FONT_H, SDL_PIXELFORMAT_ARGB8888);
    SDL_Renderer *sw;
    unsigned char *px;
    int i, x, y;

    if (!surf) return false;
    sw = SDL_CreateSoftwareRenderer(surf);
    if (!sw) {
        SDL_DestroySurface(surf);
        return false;
    }
    SDL_SetRenderDrawColor(sw, 0, 0, 0, 0);
    SDL_RenderClear(sw);
    SDL_SetRenderDrawColor(sw, 255, 255, 255, 255);
    for (i = 0; i < FONT_N; ++i) {
        const char ch[2] = { (char)(FONT_FIRST + i), 0 };
        SDL_RenderDebugText(sw, (float)(i * FONT_W), 0.0f, ch);
    }
    /* SDL's renderers queue their commands and run them at present, so
     * without this the surface is still empty and every glyph comes out
     * blank. Measured: 0 of 760 columns filled before, 563 after. */
    SDL_RenderPresent(sw);
    SDL_DestroyRenderer(sw);

    /* ARGB8888 is B,G,R,A in memory. The glyphs are white, so the brightest
     * channel is the coverage; alpha is written as well but not by every
     * rasteriser, so take whichever of the two says more. */
    px = (unsigned char *)SDL_malloc((size_t)w * FONT_H * 4);
    if (!px) {
        SDL_DestroySurface(surf);
        return false;
    }
    for (y = 0; y < FONT_H; ++y) {
        const unsigned char *row = (const unsigned char *)surf->pixels
                                 + (size_t)y * surf->pitch;
        for (x = 0; x < w; ++x) {
            unsigned char v = row[x * 4 + 2];
            unsigned char *o = px + ((size_t)y * w + x) * 4;

            if (row[x * 4 + 1] > v) v = row[x * 4 + 1];
            if (row[x * 4 + 0] > v) v = row[x * 4 + 0];
            if (row[x * 4 + 3] > v) v = row[x * 4 + 3];
            o[0] = 255; o[1] = 255; o[2] = 255; o[3] = v;
        }
    }
    SDL_DestroySurface(surf);

    p_glGenTextures(1, &a->hudTex);
    p_glBindTexture(GL_TEXTURE_2D, a->hudTex);
    p_glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    p_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, FONT_H, 0, GL_RGBA,
                   GL_UNSIGNED_BYTE, px);
    p_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    p_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    p_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    p_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    SDL_free(px);
    return true;
}

static bool
gl_init(App *a)
{
    a->curveProg = gl_program(kCurveVS, kCurveFS);
    a->hudProg = gl_program(kHudVS, kHudFS);
    if (!a->curveProg || !a->hudProg) return false;

    a->curveScale = p_glGetUniformLocation(a->curveProg, "u_scale");
    a->curveOffset = p_glGetUniformLocation(a->curveProg, "u_offset");
    a->hudScale = p_glGetUniformLocation(a->hudProg, "u_scale");
    a->hudOffset = p_glGetUniformLocation(a->hudProg, "u_offset");
    a->hudPointScale = p_glGetUniformLocation(a->hudProg, "u_pscale");
    a->hudTexUni = p_glGetUniformLocation(a->hudProg, "u_tex");

    p_glGenVertexArrays(1, &a->curveVao);
    p_glBindVertexArray(a->curveVao);
    p_glGenBuffers(1, &a->curveVbo);
    p_glBindBuffer(GL_ARRAY_BUFFER, a->curveVbo);
    p_glEnableVertexAttribArray(0);
    p_glVertexAttribPointer(0, 2, GL_FLOAT, 0, CURVE_FLOATS * (GLsizei_t)sizeof(float), VOFF(0));
    p_glEnableVertexAttribArray(1);
    p_glVertexAttribPointer(1, 2, GL_FLOAT, 0, CURVE_FLOATS * (GLsizei_t)sizeof(float), VOFF(2 * sizeof(float)));
    p_glEnableVertexAttribArray(2);
    p_glVertexAttribPointer(2, 1, GL_FLOAT, 0, CURVE_FLOATS * (GLsizei_t)sizeof(float), VOFF(4 * sizeof(float)));
    p_glEnableVertexAttribArray(3);
    p_glVertexAttribPointer(3, 4, GL_FLOAT, 0, CURVE_FLOATS * (GLsizei_t)sizeof(float), VOFF(5 * sizeof(float)));

    p_glGenVertexArrays(1, &a->hudVao);
    p_glBindVertexArray(a->hudVao);
    p_glGenBuffers(1, &a->hudVbo);
    p_glBindBuffer(GL_ARRAY_BUFFER, a->hudVbo);
    p_glEnableVertexAttribArray(0);
    p_glVertexAttribPointer(0, 2, GL_FLOAT, 0, HUD_FLOATS * (GLsizei_t)sizeof(float), VOFF(0));
    p_glEnableVertexAttribArray(1);
    p_glVertexAttribPointer(1, 2, GL_FLOAT, 0, HUD_FLOATS * (GLsizei_t)sizeof(float), VOFF(2 * sizeof(float)));
    p_glEnableVertexAttribArray(2);
    p_glVertexAttribPointer(2, 4, GL_FLOAT, 0, HUD_FLOATS * (GLsizei_t)sizeof(float), VOFF(4 * sizeof(float)));

    p_glBindVertexArray(0);
    p_glBindBuffer(GL_ARRAY_BUFFER, 0);

    if (!build_font_atlas(a)) {
        SDL_Log("could not build the font atlas; the HUD would be blank");
        return false;
    }
    p_glDisable(GL_DEPTH_TEST);
    p_glEnable(GL_BLEND);
    p_glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    return true;
}

/* The vertex scratch follows the sample count, which the user can double and
 * halve, so it grows on demand like the point buffers do. */
static bool
ensure_curve_verts(App *a, int verts)
{
    float *v;

    if (verts <= a->vcap) return true;
    v = (float *)realloc(a->vbuf,
                         sizeof(float) * CURVE_FLOATS * (size_t)verts);
    if (!v) return false;
    a->vbuf = v;
    a->vcap = verts;
    return true;
}

static bool
ensure_hud_verts(App *a, int verts)
{
    float *v;

    if (verts <= a->hcap) return true;
    v = (float *)realloc(a->hbuf,
                         sizeof(float) * HUD_FLOATS * (size_t)verts);
    if (!v) return false;
    a->hbuf = v;
    a->hcap = verts;
    return true;
}

/* The whole curve as one triangle strip: two vertices per drawn point, each
 * carrying half the stroke folded into the normal, so the shader only has to
 * push the pair apart, and the colour on every vertex.
 *
 * That last part is what the runs were for. SDL_RenderLines takes one colour
 * per call, so the rainbow had to be cut into stretches of one hue and the run
 * boundaries were arithmetic on `didx`; a shader takes the colour per vertex,
 * so the hue is simply the point's own and there is nothing to cut. The hue
 * itself is unchanged - the sample index in the parametric mode, the direction
 * from the centre in the other. */
static void
draw_curve_gl(App *a)
{
    const int m = a->ndisp;
    const bool thick = a->width > 1;
    const bool rainbow = a->rainbow;
    const bool byAngle = rainbow && !a->rainbowParam;
    const long long n = a->p.samples;
    float *v;
    int i, k = 0;

    if (m < 2 || !ensure_curve_verts(a, m * 2)) return;
    v = a->vbuf;

    /* The strip only changes when something it is built from does, and
     * nothing it is built from is per-frame - so it is built and uploaded
     * once and then drawn from the VBO until a flag says otherwise. That is
     * the difference between the draw costing what the geometry costs and it
     * costing what the upload of 58 MB costs: an idle frame at a million
     * samples is one glDrawArrays and no CPU work at all.
     *
     * A benchmark forces the curve dirty every frame, so it always takes the
     * first branch - the worst case, which is what it is for. */
    if (a->vertsDirty) {
        for (i = 0; i < m; ++i) {
            float cr = 148.0f / 255.0f, cg = 0.0f, cb = 211.0f / 255.0f;

            if (rainbow) {
                const long long q = byAngle
                                  ? (long long)a->rbq[i]
                                  : (long long)a->didx[i] * RAINBOW_STEPS / n;
                Uint8 r, g, b;

                hsv_to_rgb(360.0f * ((float)q + 0.5f) / (float)RAINBOW_STEPS,
                           &r, &g, &b);
                cr = (float)r / 255.0f;
                cg = (float)g / 255.0f;
                cb = (float)b / 255.0f;
            }

            {
                const float half = 0.5f * (float)(thick ? a->wid[i] : 1)
                                 * STROKE_HALF_BIAS;
                const float hx = a->nrm[i].x * half, hy = a->nrm[i].y * half;

                for (int side = 0; side < 2; ++side) {
                    v[k++] = a->disp[i].x;
                    v[k++] = a->disp[i].y;
                    v[k++] = hx;
                    v[k++] = hy;
                    v[k++] = side ? -1.0f : 1.0f;
                    v[k++] = cr; v[k++] = cg; v[k++] = cb; v[k++] = 1.0f;
                }
            }
        }

        p_glBindVertexArray(a->curveVao);
        p_glBindBuffer(GL_ARRAY_BUFFER, a->curveVbo);
        p_glBufferData(GL_ARRAY_BUFFER,
                       (GLsizeiptr_t)k * (GLsizeiptr_t)sizeof(float),
                       v, GL_DYNAMIC_DRAW);
        a->vertsDirty = false;
    }

    p_glUseProgram(a->curveProg);
    p_glUniform2f(a->curveScale, 2.0f / (float)a->outW, -2.0f / (float)a->outH);
    p_glUniform2f(a->curveOffset, -1.0f, 1.0f);
    p_glBindVertexArray(a->curveVao);
    p_glDrawArrays(GL_TRIANGLE_STRIP, 0, m * 2);
}

/* One glyph: a quad in window points, with the atlas's columns for that
 * character. Six vertices, because the strip the curve uses is no use here -
 * the glyphs do not touch. */
static void
hud_glyph(App *a, int *k, float x, float y, int g)
{
    static const float q[6][4] = {
        { 0.0f, 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 1.0f, 0.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f },
        { 0.0f, 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f },
        { 0.0f, 1.0f, 0.0f, 1.0f },
    };
    const float u0 = (float)(g * FONT_W) / (float)(FONT_W * FONT_N);
    const float u1 = (float)((g + 1) * FONT_W) / (float)(FONT_W * FONT_N);

    for (int i = 0; i < 6; ++i) {
        float *v = a->hbuf + (size_t)(*k) * HUD_FLOATS;

        v[0] = x + q[i][0] * (float)FONT_W;
        v[1] = y + q[i][1] * (float)FONT_H;
        v[2] = q[i][2] ? u1 : u0;
        v[3] = q[i][3];
        v[4] = 1.0f; v[5] = 1.0f; v[6] = 1.0f; v[7] = 1.0f;
        ++*k;
    }
}

static void
hud_text(App *a, int *k, float x, float y, const char *text)
{
    for (; *text; ++text) {
        const unsigned char ch = (unsigned char)*text;

        if (ch >= FONT_FIRST && ch <= FONT_LAST)
            hud_glyph(a, k, x, y, ch - FONT_FIRST);
        x += (float)FONT_W;
    }
}

static int
hud_text_quads(const char *text)
{
    const char *c;
    int n = 0;

    for (c = text; *c; ++c) ++n;
    return n * 6;
}

/* ---------------------------------------------------------------- render */

/* The rainbow's hue bucket at each drawn point, taken from the direction the
 * point lies in as seen from the centre of the figure.
 *
 * Not from the sample index, which is what the hue used to follow. A
 * spirograph comes back over its own path - at N=1048576 the curve is some
 * 1.4M pixels of arc drawn into ~35k distinct ones - and the runs are drawn
 * in hue order, so the last ones repaint the ones before them: the picture
 * came out 99% in the last 45 degrees of the wheel, one colour. Two passes
 * over the same place have the same direction, so colouring by direction is
 * the same colour on top of itself and survives.
 *
 * The direction is not measured with atan2. `|dy| / (|dx| + |dy|)` runs 0..1
 * across each octant and the signs pick which, which is monotone round the
 * circle, exact at the eight points and about 4 degrees out between them -
 * invisible in a hue, and it is a divide and a test instead of a libm call
 * per point. */
static void
build_rainbow_buckets(App *a)
{
    const int m = a->ndisp;
    const float cx = (float)a->outW * 0.5f;
    const float cy = (float)a->outH * 0.5f;

    for (int i = 0; i < m; ++i) {
        const float dx = a->disp[i].x - cx;
        const float dy = a->disp[i].y - cy;
        const float ax = fabsf(dx), ay = fabsf(dy);
        const float sum = ax + ay;
        const float u = sum > 0.0f ? ay / sum : 0.0f;
        float f;

        if (dy >= 0.0f) f = (dx >= 0.0f) ? u : 2.0f - u;
        else            f = (dx <  0.0f) ? 2.0f + u : 4.0f - u;

        int q = (int)(f * (float)RAINBOW_STEPS * 0.25f);
        if (q < 0) q = 0;
        if (q >= RAINBOW_STEPS) q = RAINBOW_STEPS - 1;
        a->rbq[i] = q;
    }
}

/* One run of the polyline, `w` device pixels wide, as `w` copies of the thin
 * line the driver API draws, pushed across the curve by whole pixels.
 *
 * Whole pixels and not a fraction of one, because SDL's line rasteriser snaps
 * to the pixel grid: two copies half a pixel apart land on the same pixels and
 * the stroke comes out uneven. The offsets go outwards from the centre - 0,
 * -1, +1, -2, +2 - so a stroke of width w is symmetric about the curve and w=1
 * is exactly the single line it always was. */
static void
draw_stroked(App *a, int s, int e, int w)
{
    const int count = e - s + 2;        /* points in the run */

    if (w <= 1) {
        SDL_RenderLines(a->renderer, a->disp + s, count);
        return;
    }

    for (int k = 0; k < w; ++k) {
        const float off = (float)((k + 1) / 2) * ((k & 1) ? -1.0f : 1.0f);

        if (off == 0.0f) {
            SDL_RenderLines(a->renderer, a->disp + s, count);
            continue;
        }
        for (int i = s; i <= e + 1; ++i) {
            a->thk[i].x = a->disp[i].x + a->nrm[i].x * off;
            a->thk[i].y = a->disp[i].y + a->nrm[i].y * off;
        }
        SDL_RenderLines(a->renderer, a->thk + s, count);
    }
}

static void
draw_curve(App *a)
{
    const int m = a->ndisp;
    const bool thick = a->width > 1;

    if (m < 2) return;

    /* Runs of constant colour and constant width, one polyline each. The hue
     * rises with the sample index, so the runs are long; the speed mode is
     * smoothed in build_widths() for the same reason.
     *
     * The colour boundary is the first sample whose bucket is past q, which is
     * ceil((q+1)*n/RAINBOW_STEPS) - worked out once per run rather than
     * dividing by `n` once per drawn point. That division was 42 cycles a
     * point, and at six figures it was most of the frame. */
    const long long n = a->p.samples;
    const bool rainbow = a->rainbow;
    const bool byAngle = rainbow && !a->rainbowParam;
    /* Shortest run a colour change is allowed to cut, so the call count
     * cannot run away; 0 at the default sample count, where the colour is
     * meant to be per point. */
    const int minSpan = byAngle ? m / MAX_RAINBOW_RUNS : 0;
    int s = 0;

    while (s < m - 1) {
        long long limit = n;                /* no colour boundary when plain */
        long long q = 0;

        if (rainbow && !byAngle) {
            q = (long long)a->didx[s + 1] * RAINBOW_STEPS / n;
            limit = ((q + 1) * n + RAINBOW_STEPS - 1) / RAINBOW_STEPS;
        }
        if (byAngle)
            q = a->rbq[s + 1];

        const int w = thick ? a->wid[s + 1] : 1;

        /* didx never reaches n, so a limit of n is "no boundary" without a
         * second test in the loop. */
        int e = s;
        while (e < m - 2
               && (!thick || a->wid[e + 2] == w)
               && ((byAngle
                    ? ((e - s + 2) < minSpan || a->rbq[e + 2] == q)
                    : (long long)a->didx[e + 2] < limit)))
            ++e;

        if (rainbow) {
            Uint8 r, g, b;
            hsv_to_rgb(360.0f * ((float)q + 0.5f) / (float)RAINBOW_STEPS, &r, &g, &b);
            SDL_SetRenderDrawColor(a->renderer, r, g, b, 255);
        } else {
            SDL_SetRenderDrawColor(a->renderer, 148, 0, 211, 255);
        }

        draw_stroked(a, s, e, w);
        s = e + 1;
    }
}

static void
render_frame(App *a)
{
    if (a->rawgl) {
        p_glViewport(0, 0, a->outW, a->outH);
        p_glClearColor(8.0f / 255.0f, 8.0f / 255.0f, 12.0f / 255.0f, 1.0f);
        p_glClear(GL_COLOR_BUFFER_BIT);
        draw_curve_gl(a);
        return;
    }
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

/* The help overlay, in one place because both drawing paths show it. */
static const char *const help_lines[] = {
    "1 2 3        select wheel",
    "Up/Down      wheel teeth n",
    "Left/Right   phase offset s",
    "[  ]         wheel radius a",
    "+  -         samples (halve/double)",
    "space        rotate wheels (animate)",
    ",  .         stroke width (thinner/thicker)",
    "<  >         waves along the curve (fewer/more)",
    "v            stroke variation (flat/taper/speed/wave)",
    "c            rainbow colour",
    "r            randomise",
    "f            fullscreen",
    "s            save SVG",
    "b            save BMP",
    "h            hide help",
    "Esc / q      quit",
};
#define HELP_N (sizeof help_lines / sizeof help_lines[0])

static void
draw_help(const App *a)
{
    SDL_SetRenderDrawColor(a->renderer, 255, 255, 255, 255);
    float y = 40.0f;
    for (size_t i = 0; i < HELP_N; ++i) {
        SDL_RenderDebugText(a->renderer, 12.0f, y, help_lines[i]);
        y += SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE + 4.0f;
    }
}

/* ----------------------------------------------------------- status line */

static void
format_status(const App *a, char *buf, size_t bufsz)
{
    /* The wave count only means anything in the wave mode, so it is only
     * shown there rather than sitting in the line as a number that does
     * nothing. */
    char stroke[48];

    if (a->widthMode == WIDTH_WAVE)
        snprintf(stroke, sizeof stroke, "w=%d %s/%d",
                 a->width, width_mode_names[a->widthMode], a->waveCount);
    else
        snprintf(stroke, sizeof stroke, "w=%d %s",
                 a->width, width_mode_names[a->widthMode]);

    snprintf(buf, bufsz,
             "wheel[%d] n=%3.0f n=%3.0f n=%3.0f   a=(%.2f %.2f %.2f)  "
             "s=(%.3f %.3f %.3f)   samples=%d (drawing %d)%s   "
             "%s   %dx%d@%.0fx",
             a->sel,
             a->p.n[0], a->p.n[1], a->p.n[2],
             a->p.a[0], a->p.a[1], a->p.a[2],
             a->p.s[0], a->p.s[1], a->p.s[2],
             a->p.samples, a->ndisp,
             a->rainbow ? "  rainbow" : "",
             stroke,
             a->winW, a->winH, (double)a->scale);
}

/* The status line and the help overlay stay in window points, so the renderer
 * goes back to the display scale for them and back to 1 for the curve: one
 * device pixel per point is what the curve wants, and a point per pixel is
 * what the 8x8-pixel debug font needs to keep its size on any display.
 *
 * The raw GL path does the same thing with a uniform, and draws both lots of
 * text in one call. */
static void
draw_overlays(App *a)
{
    char status[320];
    int ntext, k = 0;

    format_status(a, status, sizeof status);

    if (a->rawgl) {
        ntext = hud_text_quads(status);
        for (size_t i = 0; a->help && i < HELP_N; ++i)
            ntext += hud_text_quads(help_lines[i]);
        if (!ensure_hud_verts(a, ntext)) return;

        hud_text(a, &k, 12.0f, 12.0f, status);
        if (a->help) {
            float y = 40.0f;
            for (size_t i = 0; i < HELP_N; ++i) {
                hud_text(a, &k, 12.0f, y, help_lines[i]);
                y += (float)FONT_H + 4.0f;
            }
        }

        p_glUseProgram(a->hudProg);
        p_glUniform2f(a->hudScale, 2.0f / (float)a->outW, -2.0f / (float)a->outH);
        p_glUniform2f(a->hudOffset, -1.0f, 1.0f);
        p_glUniform1f(a->hudPointScale, a->scale);
        p_glUniform1i(a->hudTexUni, 0);
        p_glActiveTexture(GL_TEXTURE0);
        p_glBindTexture(GL_TEXTURE_2D, a->hudTex);
        p_glBindVertexArray(a->hudVao);
        p_glBindBuffer(GL_ARRAY_BUFFER, a->hudVbo);
        /* `k` counts vertices here, not floats - the curve's builder counts
         * floats, this one does not. */
        p_glBufferData(GL_ARRAY_BUFFER,
                       (GLsizeiptr_t)k * HUD_FLOATS * (GLsizeiptr_t)sizeof(float),
                       a->hbuf, GL_DYNAMIC_DRAW);
        p_glDrawArrays(GL_TRIANGLES, 0, k);
        return;
    }

    SDL_SetRenderScale(a->renderer, a->scale, a->scale);
    draw_debug_text(a, status);
    if (a->help)
        draw_help(a);
    SDL_SetRenderScale(a->renderer, 1.0f, 1.0f);
}

/* One line per PROFILE_REPORT_FRAMES frames: the per-frame averages since the
 * start, so a run that has not settled yet shows it. */
static void
report_profile(const App *a)
{
    const double f = a->frames > 0 ? (double)a->frames : 1.0;
    char stroke[48];

    if (a->widthMode == WIDTH_WAVE)
        snprintf(stroke, sizeof stroke, "%d/%s/%d",
                 a->width, width_mode_names[a->widthMode], a->waveCount);
    else
        snprintf(stroke, sizeof stroke, "%d/%s",
                 a->width, width_mode_names[a->widthMode]);

    SDL_Log("bench %6d frames  %dx%d pts scale %.2f  samples=%7d drawn=%7d  "
            "stroke=%s%s | "
            "kernel %8.3f  layout %7.3f  thin %7.3f | "
            "draw %8.3f  hud %6.3f  present %8.3f | total %8.3f ms",
            a->frames, a->winW, a->winH, (double)a->scale,
            a->p.samples, a->ndisp,
            stroke,
            a->rainbow ? (a->rainbowParam ? " rainbow/param" : " rainbow/angle") : "",
            a->tKernel / f, a->tLayout / f, a->tThin / f,
            a->tDraw / f, a->tHud / f, a->tPresent / f, a->tTotal / f);
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
    case SDLK_COMMA:
    case SDLK_PERIOD:
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

    /* A benchmark is driven by the environment, so any key arriving during one
     * is news - it means something outside is typing into the window and the
     * run is not measuring what it says it is. */
    if (a->benchFrames > 0)
        SDL_Log("bench: key %s during a benchmark run", SDL_GetKeyName(ke->key));

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
        a->vertsDirty = true;       /* the colour is on the vertices now */
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

    /* The stroke. `,` and `.` are the width, `v` cycles how it varies, and
     * shift with those two is the wave's frequency - the one mode with a
     * shape parameter of its own. The plain width is one pixel, and a mode
     * that varies a one-pixel line has nothing to vary, so `v` gives it
     * enough room to be visible on the way in. */
    case SDLK_COMMA:
        if (shifted) {
            if (a->waveCount > 1) {
                --a->waveCount;
                a->strokeDirty = true;
            }
        } else if (a->width > 1) {
            --a->width;
            a->strokeDirty = true;
        }
        break;
    case SDLK_PERIOD:
        if (shifted) {
            if (a->waveCount < MAX_WAVES) {
                ++a->waveCount;
                a->strokeDirty = true;
            }
        } else if (a->width < MAX_WIDTH) {
            ++a->width;
            a->strokeDirty = true;
        }
        break;
    case SDLK_V:
        a->widthMode = (a->widthMode + 1) % WIDTH_MODE_COUNT;
        if (a->width <= 1)          /* a variation of a 1-pixel line is 1 pixel */
            a->width = DEFAULT_VARIED_WIDTH;
        a->strokeDirty = true;
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
    app.rainbowParam = true;        /* the long-standing rainbow, not the new one */
    app.animate = false;
    app.help = true;
    app.width = 1;                  /* the plain one-pixel stroke */
    app.widthMode = WIDTH_FLAT;
    app.waveCount = DEFAULT_WAVE_COUNT;

    /* Drawing the curve through raw GL is the default; FLOWERY_DRAW=lines
     * asks for the SDL renderer and the line drawing it does instead, for an
     * A/B and for a machine where the shaders will not run. */
    app.rawgl = true;
    if (SDL_getenv("FLOWERY_DRAW"))
        app.rawgl = SDL_strcasecmp(SDL_getenv("FLOWERY_DRAW"), "lines") != 0;

    /* The stroke can be set from the environment as well as from the keys,
     * which is what lets a benchmark measure a width without a keyboard. */
    if (SDL_getenv("FLOWERY_WIDTH"))
        app.width = atoi(SDL_getenv("FLOWERY_WIDTH"));
    if (SDL_getenv("FLOWERY_WIDTH_MODE")) {
        const char *want = SDL_getenv("FLOWERY_WIDTH_MODE");
        for (int i = 0; i < WIDTH_MODE_COUNT; ++i)
            if (SDL_strcasecmp(want, width_mode_names[i]) == 0)
                app.widthMode = i;
    }
    if (SDL_getenv("FLOWERY_WAVE"))
        app.waveCount = atoi(SDL_getenv("FLOWERY_WAVE"));
    /* Which of the two rainbows. The parametric one - hue walking the wheel
     * with the sample index - is what the app has always drawn and what the
     * default is; hue-by-direction is behind this flag so the two can be
     * compared in the app rather than only in a benchmark. */
    if (SDL_getenv("FLOWERY_RAINBOW_MODE"))
        app.rainbowParam = SDL_strcasecmp(
            SDL_getenv("FLOWERY_RAINBOW_MODE"), "angle") != 0;
    if (app.width < 1) app.width = 1;
    if (app.width > MAX_WIDTH) app.width = MAX_WIDTH;
    if (app.waveCount < 1) app.waveCount = 1;
    if (app.waveCount > MAX_WAVES) app.waveCount = MAX_WAVES;

    /* A benchmark run is a batch job that happens to need a window: it holds
     * the curve still, drops the help overlay (which is not what we are
     * timing), and quits by itself after FLOWERY_BENCH frames. */
    app.benchFrames = SDL_getenv("FLOWERY_BENCH")
                    ? atoi(SDL_getenv("FLOWERY_BENCH")) : 0;
    app.profile = app.benchFrames > 0 || SDL_getenv("FLOWERY_PROFILE") != NULL;
    if (app.benchFrames > 0) {
        app.help = false;
        app.rainbow = SDL_getenv("FLOWERY_RAINBOW") != NULL;
        app.benchBmp = SDL_getenv("FLOWERY_BENCH_BMP");
        /* What the run was asked for, before anything can change it: a bench
         * that reports a different stroke at the end had it changed under it. */
        SDL_Log("stroke: width=%d mode=%s waves=%d rainbow=%d/%s",
                app.width, width_mode_names[app.widthMode], app.waveCount,
                (int)app.rainbow, app.rainbowParam ? "param" : "angle");
    }

    srand((unsigned)time(NULL));

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }
    atexit(SDL_Quit);

    app.window = SDL_CreateWindow(
        "flowery — spirograph (SDL3)",
        900, 900,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY
        | (app.rawgl ? SDL_WINDOW_OPENGL : 0));
    if (!app.window) {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        return 1;
    }
    /* Drawing through raw GL, or through SDL's renderer as before. The two do
     * not mix - SDL's text stops drawing once our GL state has been bound -
     * so this is decided once, here, and nothing below switches. */
    if (app.rawgl) {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                            SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        app.glctx = SDL_GL_CreateContext(app.window);
        if (!app.glctx) {
            SDL_Log("SDL_GL_CreateContext failed: %s", SDL_GetError());
            return 1;
        }
        if (!SDL_GL_MakeCurrent(app.window, app.glctx)) {
            SDL_Log("SDL_GL_MakeCurrent failed: %s", SDL_GetError());
            return 1;
        }
        if (!load_gl() || !gl_init(&app)) {
            SDL_Log("could not set up the GL drawing path; "
                    "run with FLOWERY_DRAW=lines to use SDL's renderer instead");
            return 1;
        }
    } else {
        /* Left alone, SDL draws a line as one quad per line *pixel*; at
         * 2880x1800 that is 20x the cost of the driver's own line API, which
         * is only used at render scale 1. An explicit SDL_RENDER_LINE_METHOD
         * wins, so the other methods stay reachable for comparison. Must
         * precede the renderer. */
        if (!SDL_GetHint(SDL_HINT_RENDER_LINE_METHOD))
            SDL_SetHint(SDL_HINT_RENDER_LINE_METHOD, "2");

        /* Vulkan draws these lines about ten times faster than the GL driver
         * (NOTES.md has the measurements). It was the default for that reason
         * and it is not any more, because on this machine it also breaks the
         * frame on the way to the screen: with a large sample count the HUD
         * comes out smeared, and the smear cannot be caught in a screenshot,
         * so the rendered frame is right and the presentation is not - a
         * swapchain problem in Mesa's Vulkan driver for Gen7.5, which says so
         * itself at every start:
         *
         *     MESA-INTEL: warning: Haswell Vulkan support is incomplete
         *
         * An explicit SDL_RENDER_DRIVER still wins, so both stay reachable. */
        if (!SDL_GetHint(SDL_HINT_RENDER_DRIVER))
            app.renderer = SDL_CreateRenderer(app.window, "opengl");
        if (!app.renderer)
            app.renderer = SDL_CreateRenderer(app.window, NULL);
        if (!app.renderer) {
            SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
            return 1;
        }
    }

    /* The window's pixel size is only known once there is something to draw
     * with, and the first pixel-size event may well arrive after the first
     * frame. */
    update_sizes(&app);
    if (app.rawgl)
        SDL_Log("window %dx%d points, raw GL %dx%d pixels (scale %.2f)",
                app.winW, app.winH, app.outW, app.outH, (double)app.scale);
    else
        SDL_Log("window %dx%d points, renderer %s %dx%d pixels (scale %.2f)",
                app.winW, app.winH, SDL_GetRendererName(app.renderer),
                app.outW, app.outH, (double)app.scale);

    /* Fullscreen on startup, so `run.sh` can put the whole panel behind the
     * curve without anyone pressing `f`. On this display that is the only way
     * to get the native 2880x1800 back buffer - a windowed one is whatever
     * Hyprland decides, and it resizes it underneath us regardless.
     * FLOWERY_BENCH_FULLSCREEN stays as it was for the benchmarks; this one is
     * for the app. `f` still toggles either way. */
    if (SDL_getenv("FLOWERY_FULLSCREEN")
        && atoi(SDL_getenv("FLOWERY_FULLSCREEN")) != 0) {
        app.fullscreen = true;
        SDL_SetWindowFullscreen(app.window, true);
    }
    if (app.benchFrames > 0 && SDL_getenv("FLOWERY_BENCH_FULLSCREEN")) {
        app.fullscreen = true;
        SDL_SetWindowFullscreen(app.window, true);
    }

    /* Pace the loop off the display when we can (it also stops the tearing
     * an 8 ms sleep never did); only fall back to sleeping if vsync is not
     * available, as it is not for the offscreen driver. A benchmark turns it
     * off: timing a frame that is waiting for vblank times the display. */
    if (app.rawgl)
        app.vsync = SDL_GL_SetSwapInterval(app.benchFrames > 0 ? 0 : 1) == 0;
    else
        app.vsync = SDL_SetRenderVSync(app.renderer, app.benchFrames > 0 ? 0 : 1);
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
        /* A benchmark wants the worst case every frame, not the idle one. */
        if (app.benchFrames > 0)
            app.dirtyCurve = true;
        app.lastTicks = now;

        ++app.seen;
        const bool counted = !app.benchFrames || app.seen > BENCH_WARMUP_FRAMES;

        const Uint64 t0 = SDL_GetPerformanceCounter();
        if (!update_geometry(&app)) continue;
        const Uint64 t1 = SDL_GetPerformanceCounter();
        render_frame(&app);
        const Uint64 t2 = SDL_GetPerformanceCounter();
        draw_overlays(&app);
        const Uint64 t3 = SDL_GetPerformanceCounter();

        /* One frame left behind for the eye - and for a pixel-by-pixel
         * comparison of two builds. The last frame, not the first: by then
         * the window has stopped being resized underneath us. Reads the back
         * buffer while it is still the current one, so it precedes the swap. */
        if (app.benchBmp && app.benchFrames > 0
            && app.seen >= app.benchFrames + BENCH_WARMUP_FRAMES)
            save_bmp(&app, app.benchBmp);

        if (app.rawgl)
            SDL_GL_SwapWindow(app.window);
        else
            SDL_RenderPresent(app.renderer);
        const Uint64 t4 = SDL_GetPerformanceCounter();

        if (counted) {
            app.tDraw    += perf_ms(t1, t2);
            app.tHud     += perf_ms(t2, t3);
            app.tPresent += perf_ms(t3, t4);
            app.tTotal   += perf_ms(t0, t4);
            ++app.frames;

            if (app.profile && app.frames % PROFILE_REPORT_FRAMES == 0)
                report_profile(&app);
        }

        if (app.benchFrames > 0
            && app.seen >= app.benchFrames + BENCH_WARMUP_FRAMES) {
            report_profile(&app);
            return 0;
        }

        if (!app.vsync)
            SDL_Delay(8);
    }
}
