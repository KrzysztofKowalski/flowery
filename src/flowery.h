/* flowery.h — math module for the SDL3 port.
 *
 * This is a straight port of the curve computed in legacy/cx.sh:
 *
 *     z(t) = a1*exp(i*2*pi*(n1*t + s1))
 *          + a2*exp(i*2*pi*(n2*t + s2))
 *          + a3*exp(i*2*pi*(n3*t + s3))
 *
 * drawn parametrically for t in [0,1] with `samples` points.
 * n[] are the wheel teeth (integer), a[] the wheel radii,
 * s[] the phase offsets in turns (0..1).
 */
#ifndef FLOWERY_H
#define FLOWERY_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    double n[3];   /* wheel gear counts */
    double a[3];   /* wheel radii       */
    double s[3];   /* phase offsets     */
    int    samples; /* number of t samples */
} FloweryParams;

/* Sample the curve into xs[]/ys[] (world coordinates). Both arrays must
 * have room for `samples` doubles. */
void flowery_points(const FloweryParams *p, double *xs, double *ys);

/* Bounding box of a point set. */
void flowery_bbox(const double *xs, const double *ys, int n,
                  double *minx, double *miny, double *maxx, double *maxy);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif /* FLOWERY_H */
