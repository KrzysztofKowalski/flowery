/* flowery.c — implementation of the Farris wheel curve.
 *
 * The curve is a sum of up to three co-rotating circular wheels:
 *
 *     z(t) = sum_k A_k * e^(i * 2*pi * (n_k*t + s_k))
 *
 * x(t) = Re(z(t)) = sum_k A_k * cos(2*pi*(n_k*t + s_k))
 * y(t) = Im(z(t)) = sum_k A_k * sin(2*pi*(n_k*t + s_k))
 *
 * Ported from legacy/cx.sh which fed the same function to gnuplot.
 */
#include "flowery.h"

#include <math.h>

void
flowery_points(const FloweryParams *p, double *xs, double *ys)
{
    const int n = p->samples > 0 ? p->samples : 1;
    int k, i;

    /* gnuplot `set samples N` evaluates t = i/(N-1) for i in 0..N-1,
     * so the first and last point coincide (a closed loop). */
    for (i = 0; i < n; ++i) {
        const double denom = n > 1 ? (double)(n - 1) : 1.0;
        const double t = (double)i / denom; /* t in [0, 1] */
        double x = 0.0, y = 0.0;
        for (k = 0; k < 3; ++k) {
            /* 2*pi = 4*asin(1.0) */
            const double ang = 4.0 * asin(1.0) * (p->n[k] * t + p->s[k]);
            x += p->a[k] * cos(ang);
            y += p->a[k] * sin(ang);
        }
        xs[i] = x;
        ys[i] = y;
    }
}

void
flowery_bbox(const double *xs, const double *ys, int n,
             double *minx, double *miny, double *maxx, double *maxy)
{
    int i;
    if (n <= 0) {
        *minx = *maxx = *miny = *maxy = 0.0;
        return;
    }
    double loX = xs[0], hiX = xs[0];
    double loY = ys[0], hiY = ys[0];
    for (i = 1; i < n; ++i) {
        if (xs[i] < loX) loX = xs[i];
        if (xs[i] > hiX) hiX = xs[i];
        if (ys[i] < loY) loY = ys[i];
        if (ys[i] > hiY) hiY = ys[i];
    }
    *minx = loX; *maxx = hiX;
    *miny = loY; *maxy = hiY;
}
