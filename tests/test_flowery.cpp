/* test_flowery.cpp — tests for the flowery curve math (src/flowery.cpp).
 *
 *   ./test_flowery                 analytic properties of the curve
 *   ./test_flowery --svg FILE...   compare against gnuplot output
 *
 * The `--svg` mode uses the SVG files produced by the original pipeline
 * (legacy/cx.sh) as the reference implementation. The wheel counts are read
 * back from the file name (spiro-N1-N2-N3-SAMPLES.svg), and since cx.sh
 * always used a=1, s=0 those are the parameters used here. The two point
 * sets are compared after a least-squares similarity fit, which absorbs the
 * things that legitimately differ between the two renderers: the position of
 * gnuplot's plot window inside its 777x777 canvas, its uniform scale, and
 * the fact that SVG y grows downwards.
 */
#define _POSIX_C_SOURCE 200809L   /* scandir, alphasort */

#include <dirent.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "flowery.h"

static int checks, failures;

static void
fail(int line, const char *msg)
{
    ++failures;
    printf("FAIL (line %d): %s\n", line, msg);
}

#define CHECK(cond, msg) do { ++checks; if (!(cond)) fail(__LINE__, msg); } while (0)

static FloweryParams
params(double n1, double n2, double n3,
       double a1, double a2, double a3,
       double s1, double s2, double s3, int samples)
{
    FloweryParams p;
    p.n[0] = n1; p.n[1] = n2; p.n[2] = n3;
    p.a[0] = a1; p.a[1] = a2; p.a[2] = a3;
    p.s[0] = s1; p.s[1] = s2; p.s[2] = s3;
    p.samples = samples;
    return p;
}

/* ------------------------------------------------------ analytic properties */

/* One wheel with a single tooth and radius 1 traces the unit circle. With
 * samples=5 the curve is evaluated at t = 0, 1/4, 1/2, 3/4, 1, which lands
 * on the four axis points and closes the loop. */
static void
test_single_wheel(void)
{
    const double ex[5] = { 1, 0, -1, 0, 1 };
    const double ey[5] = { 0, 1, 0, -1, 0 };
    double xs[5], ys[5];
    FloweryParams p = params(1, 0, 0, 1, 0, 0, 0, 0, 0, 5);
    int i;

    flowery_points(&p, xs, ys);
    for (i = 0; i < 5; ++i) {
        char msg[160];
        snprintf(msg, sizeof msg,
                 "single wheel, point %d: got (%g, %g), want (%g, %g)",
                 i, xs[i], ys[i], ex[i], ey[i]);
        CHECK(fabs(xs[i] - ex[i]) < 1e-12 && fabs(ys[i] - ey[i]) < 1e-12, msg);
    }
}

/* gnuplot's `set samples N` evaluates t = i/(N-1), so the last point falls
 * on t=1 and coincides with the first one. */
static void
test_loop_is_closed(void)
{
    double xs[2001], ys[2001];
    FloweryParams p = params(7, 11, 13, 1, 1, 1, 0, 0, 0, 2001);

    flowery_points(&p, xs, ys);
    CHECK(fabs(xs[2000] - xs[0]) < 1e-12, "loop is not closed in x");
    CHECK(fabs(ys[2000] - ys[0]) < 1e-12, "loop is not closed in y");
    CHECK(fabs(xs[0] - 3.0) < 1e-12 && fabs(ys[0]) < 1e-12,
          "loop does not start at (3, 0)");
}

/* Wheels with equal counts and no phase offset just add up: the point at
 * parameter t is 3*exp(i*2*pi*t). */
static void
test_wheels_superpose(void)
{
    double xs[101], ys[101];
    FloweryParams p = params(1, 1, 1, 1, 1, 1, 0, 0, 0, 101);
    int i;

    flowery_points(&p, xs, ys);
    for (i = 0; i < 101; ++i) {
        const double t = (double)i / 100.0;
        const double ang = 4.0 * asin(1.0) * t;
        char msg[160];
        snprintf(msg, sizeof msg, "superposition at t=%g is off", t);
        CHECK(fabs(xs[i] - 3.0 * cos(ang)) < 1e-12 &&
              fabs(ys[i] - 3.0 * sin(ang)) < 1e-12, msg);
    }
}

/* Three unit wheels with equal counts whose phases are 1/3 apart sum to
 * zero, at every point of the curve. */
static void
test_phases_cancel(void)
{
    double xs[64], ys[64];
    FloweryParams p = params(5, 5, 5, 1, 1, 1, 0.0, 1.0 / 3.0, 2.0 / 3.0, 64);
    int i;

    flowery_points(&p, xs, ys);
    for (i = 0; i < 64; ++i) {
        char msg[160];
        snprintf(msg, sizeof msg, "cancelling phases, point %d is (%g, %g)",
                 i, xs[i], ys[i]);
        CHECK(fabs(xs[i]) < 1e-12 && fabs(ys[i]) < 1e-12, msg);
    }
}

/* Wheels stopped at phase 0 all point along +x. */
static void
test_zero_wheels(void)
{
    double xs[8], ys[8];
    FloweryParams p = params(0, 0, 0, 1, 1, 1, 0, 0, 0, 8);
    int i;

    flowery_points(&p, xs, ys);
    for (i = 0; i < 8; ++i) {
        char msg[160];
        snprintf(msg, sizeof msg, "stationary wheels, point %d is (%g, %g)",
                 i, xs[i], ys[i]);
        CHECK(fabs(xs[i] - 3.0) < 1e-12 && fabs(ys[i]) < 1e-12, msg);
    }
}

/* Radii scale the curve linearly. */
static void
test_radius_scales(void)
{
    double ax[200], ay[200], bx[200], by[200];
    FloweryParams p1 = params(5, 7, 3, 1, 1, 1, 0, 0, 0, 200);
    FloweryParams p2 = params(5, 7, 3, 2, 2, 2, 0, 0, 0, 200);
    int i;

    flowery_points(&p1, ax, ay);
    flowery_points(&p2, bx, by);
    for (i = 0; i < 200; ++i) {
        char msg[160];
        snprintf(msg, sizeof msg, "radius does not scale at point %d", i);
        CHECK(fabs(bx[i] - 2.0 * ax[i]) < 1e-12 &&
              fabs(by[i] - 2.0 * ay[i]) < 1e-12, msg);
    }
}

static void
test_a_single_sample(void)
{
    double xs[1] = { 0.0 }, ys[1] = { 0.0 };
    FloweryParams p = params(1, 1, 1, 1, 1, 1, 0, 0, 0, 1);

    flowery_points(&p, xs, ys);      /* must not divide by zero */
    CHECK(fabs(xs[0] - 3.0) < 1e-12 && fabs(ys[0]) < 1e-12,
          "a single sample should evaluate the curve at t=0");
}

static void
test_bbox(void)
{
    const double xs[3] = { 3.0, -1.0, 2.0 };
    const double ys[3] = { 0.0, 5.0, -2.0 };
    double minx, miny, maxx, maxy;

    flowery_bbox(xs, ys, 3, &minx, &miny, &maxx, &maxy);
    CHECK(minx == -1.0 && maxx == 3.0, "bbox x range is wrong");
    CHECK(miny == -2.0 && maxy == 5.0, "bbox y range is wrong");

    flowery_bbox(xs, ys, 0, &minx, &miny, &maxx, &maxy);
    CHECK(minx == 0.0 && maxx == 0.0 && miny == 0.0 && maxy == 0.0,
          "an empty point set should give an empty bbox");
}

/* The default wheel from legacy/samples.sh must stay finite and sweep a
 * usable amount of the plane for every sample count we let the UI pick. */
static void
test_default_wheel_is_sane(void)
{
    static const int counts[] = { 32, 2001, 100000 };
    int c, i;

    for (c = 0; c < 3; ++c) {
        const int n = counts[c];
        double *xs = (double *)malloc(sizeof(double) * n);
        double *ys = (double *)malloc(sizeof(double) * n);
        FloweryParams p = params(480, 648, 816, 1, 1, 1, 0, 0, 0, n);
        double minx, miny, maxx, maxy;
        char msg[160];

        if (!xs || !ys) { CHECK(0, "out of memory"); free(xs); free(ys); return; }
        flowery_points(&p, xs, ys);
        for (i = 0; i < n; ++i) {
            if (!isfinite(xs[i]) || !isfinite(ys[i])) {
                snprintf(msg, sizeof msg, "non-finite point %d at samples=%d", i, n);
                CHECK(0, msg);
                break;
            }
        }
        flowery_bbox(xs, ys, n, &minx, &miny, &maxx, &maxy);
        snprintf(msg, sizeof msg, "degenerate bbox at samples=%d", n);
        CHECK(maxx - minx > 0.1 && maxy - miny > 0.1, msg);
        free(xs); free(ys);
    }
}

static void
test_analytic(void)
{
    test_single_wheel();
    test_loop_is_closed();
    test_wheels_superpose();
    test_phases_cancel();
    test_zero_wheels();
    test_radius_scales();
    test_a_single_sample();
    test_bbox();
    test_default_wheel_is_sane();
}

/* --------------------------------------------------- legacy gnuplot oracle */

static char *
read_file(const char *path, size_t *len)
{
    FILE *fp = fopen(path, "rb");
    long sz;
    char *buf;
    size_t got;

    if (!fp) return NULL;
    if (fseek(fp, 0, SEEK_END) != 0) { fclose(fp); return NULL; }
    sz = ftell(fp);
    if (sz < 0) { fclose(fp); return NULL; }
    rewind(fp);
    buf = (char *)malloc((size_t)sz + 1);
    if (!buf) { fclose(fp); return NULL; }
    got = fread(buf, 1, (size_t)sz, fp);
    fclose(fp);
    buf[got] = '\0';
    if (len) *len = got;
    return buf;
}

/* spiro-N1-N2-N3-SAMPLES.svg -> the four numbers. */
static int
parse_sample_name(const char *path, int *n1, int *n2, int *n3, int *samples)
{
    const char *base = strrchr(path, '/');
    base = base ? base + 1 : path;
    if (strncmp(base, "spiro-", 6) != 0) return 0;
    return sscanf(base + 6, "%d-%d-%d-%d", n1, n2, n3, samples) == 4;
}

/* gnuplot writes the whole curve as a single <path stroke='rgb(...)'
 * d='M x,y L x,y ... '/>. The point markers in <defs> also contain M/L, so
 * the search has to anchor on the stroke attribute. */
static int
parse_svg_path(const char *svg, double *xs, double *ys, int max)
{
    const char *p = strstr(svg, "<path stroke=");
    int count = 0;

    if (!p) return -1;
    p = strstr(p, "d='");
    if (!p) return -1;
    p += 3;

    while (*p && *p != '\'') {
        if (*p == 'M' || *p == 'L') {
            char *end;
            double x, y;
            ++p;
            x = strtod(p, &end);
            if (end == p) return -2;
            p = end;
            while (*p == ' ' || *p == ',') ++p;
            y = strtod(p, &end);
            if (end == p) return -2;
            p = end;
            if (count >= max) return -3;
            xs[count] = x;
            ys[count] = y;
            ++count;
        } else {
            ++p;
        }
    }
    return count;
}

/* Least-squares fit of  u = ox + s*x,  v = oy - s*y  (the y flip is what
 * turns gnuplot's screen coordinates back into the curve's own frame).
 * Returns the scale, and writes the residuals in pixels. */
static double
fit_similarity(const double *xs, const double *ys,
               const double *ux, const double *uy, int n,
               double *ox, double *oy, double *avg, double *max)
{
    double mx = 0, my = 0, mu = 0, mv = 0, num = 0, den = 0, s = 0;
    double sum = 0, worst = 0;
    int i;

    for (i = 0; i < n; ++i) {
        mx += xs[i]; my += ys[i];
        mu += ux[i]; mv += uy[i];
    }
    mx /= n; my /= n; mu /= n; mv /= n;

    for (i = 0; i < n; ++i) {
        const double X = xs[i] - mx, Y = -(ys[i] - my);
        num += X * (ux[i] - mu) + Y * (uy[i] - mv);
        den += X * X + Y * Y;
    }
    if (den > 0.0) s = num / den;

    *ox = mu - s * mx;
    *oy = mv + s * my;   /* mean of -y is -mean of y */

    for (i = 0; i < n; ++i) {
        const double X = xs[i], Y = -ys[i];
        const double ex = *ox + s * X - ux[i];
        const double ey = *oy + s * Y - uy[i];
        const double r = sqrt(ex * ex + ey * ey);
        sum += r;
        if (r > worst) worst = r;
    }
    *avg = sum / n;
    *max = worst;
    return s;
}

/* Residuals this small mean the SVG is rounded coordinates only; ~0.04px
 * measured over the files in legacy/samples. */
#define TOLERANCE_AVG_PX 0.5
#define TOLERANCE_MAX_PX 2.0
#define MAX_NOTES 20

typedef struct {
    int    files;       /* files that follow the naming convention */
    int    matched;     /* compared point by point, within tolerance */
    int    skipped;     /* fewer points than samples: not comparable */
    int    failed;
    int    notes;
    double worst_avg, worst_max;
    char   worst_avg_file[512];
    char   worst_max_file[512];
} Oracle;

static void
check_svg(const char *path, Oracle *o)
{
    int n1, n2, n3, samples, count;
    double *ux = NULL, *uy = NULL, *xs = NULL, *ys = NULL;
    char *svg = NULL;
    FloweryParams p;
    double ox, oy, avg, max, s;
    char msg[600];

    if (!parse_sample_name(path, &n1, &n2, &n3, &samples) || samples < 2)
        return;                     /* no parameters in the name to check */

    ++o->files;
    svg = read_file(path, NULL);
    if (!svg) {
        printf("FAIL %s: cannot read\n", path);
        ++o->failed;
        goto done;
    }
    ux = (double *)malloc(sizeof(double) * samples);
    uy = (double *)malloc(sizeof(double) * samples);
    xs = (double *)malloc(sizeof(double) * samples);
    ys = (double *)malloc(sizeof(double) * samples);
    if (!ux || !uy || !xs || !ys) {
        printf("FAIL %s: out of memory\n", path);
        ++o->failed;
        goto done;
    }

    count = parse_svg_path(svg, ux, uy, samples);
    if (count < 0) {
        printf("FAIL %s: could not read the plotted path (error %d)\n",
               path, count);
        ++o->failed;
        goto done;
    }
    if (count > samples) {
        printf("FAIL %s: path has %d points for %d samples\n",
               path, count, samples);
        ++o->failed;
        goto done;
    }
    /* A few legacy files hold fewer points than the sample count in their
     * own name -- all of them curves whose neighbouring points crowd onto
     * the same terminal coordinate (they alias to a slow wheel, or bunch up
     * at a cusp). Whatever gnuplot dropped there, the remaining points no
     * longer line up one-to-one with the curve, so such a file can only be
     * reported, never compared. (count == samples - 1 is normal instead: the
     * closing point is sometimes not repeated.) */
    if (count < samples - 1) {
        if (o->notes < MAX_NOTES) {
            printf("note %s: path has %d of %d points, not comparable\n",
                   path, count, samples);
            ++o->notes;
        }
        ++o->skipped;
        goto done;
    }

    p = params(n1, n2, n3, 1, 1, 1, 0, 0, 0, samples);
    flowery_points(&p, xs, ys);
    s = fit_similarity(xs, ys, ux, uy, count, &ox, &oy, &avg, &max);

    if (!(s > 0.0)) {
        printf("FAIL %s: degenerate fit\n", path);
        ++o->failed;
        goto done;
    }
    if (avg > TOLERANCE_AVG_PX || max > TOLERANCE_MAX_PX) {
        snprintf(msg, sizeof msg,
                 "%s: deviation avg=%.3fpx max=%.3fpx (n=%d %d %d)",
                 path, avg, max, n1, n2, n3);
        printf("FAIL %s\n", msg);
        ++o->failed;
        goto done;
    }
    ++o->matched;
    if (avg > o->worst_avg) {
        o->worst_avg = avg;
        snprintf(o->worst_avg_file, sizeof o->worst_avg_file, "%s", path);
    }
    if (max > o->worst_max) {
        o->worst_max = max;
        snprintf(o->worst_max_file, sizeof o->worst_max_file, "%s", path);
    }

done:
    free(svg); free(ux); free(uy); free(xs); free(ys);
}

/* Directories are read here rather than expanded by the shell: a good many
 * of the legacy sample names contain spaces. */
static void
check_svg_path(const char *path, Oracle *o)
{
    struct stat st;

    if (stat(path, &st) != 0) {
        printf("FAIL %s: %s\n", path, strerror(errno));
        ++o->failed;
        return;
    }
    if (!S_ISDIR(st.st_mode)) {
        check_svg(path, o);
        return;
    }

    {
        struct dirent **list;
        int n = scandir(path, &list, NULL, alphasort), i;

        if (n < 0) {
            printf("FAIL %s: %s\n", path, strerror(errno));
            ++o->failed;
            return;
        }
        for (i = 0; i < n; ++i) {
            const char *name = list[i]->d_name;
            const size_t len = strlen(name);

            if (len > 4 && strcmp(name + len - 4, ".svg") == 0) {
                char full[4096];
                snprintf(full, sizeof full, "%s/%s", path, name);
                check_svg(full, o);
            }
            free(list[i]);
        }
        free(list);
    }
}

/* --------------------------------------------------------------- entry */

static void
usage(const char *prog)
{
    printf("usage: %s [--svg FILE|DIR ...]\n", prog);
}

static int
run_oracle(int argc, char *argv[])
{
    Oracle o = {};
    int i;

    for (i = 2; i < argc; ++i)
        check_svg_path(argv[i], &o);

    if (o.matched) {
        printf("gnuplot oracle: %d/%d files match "
               "(worst average %.3fpx, worst point %.3fpx)\n",
               o.matched, o.files, o.worst_avg, o.worst_max);
        printf("  worst average: %s\n", o.worst_avg_file);
        printf("  worst point:   %s\n", o.worst_max_file);
    } else {
        printf("gnuplot oracle: 0/%d files match\n", o.files);
    }
    if (o.skipped)
        printf("  %d file(s) hold fewer points than samples: not comparable\n",
               o.skipped);
    if (o.failed)
        printf("  %d file(s) FAILED\n", o.failed);

    return o.failed ? 1 : 0;
}

int
main(int argc, char *argv[])
{
    if (argc > 1 && strcmp(argv[1], "--svg") == 0) {
        if (argc < 3) { usage(argv[0]); return 2; }
        return run_oracle(argc, argv);
    }
    if (argc > 1) { usage(argv[0]); return 2; }

    test_analytic();
    printf("%d checks, %d failures\n", checks, failures);
    return failures ? 1 : 0;
}
