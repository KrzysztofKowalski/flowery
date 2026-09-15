/* flowery.cpp — implementation of the Farris wheel curve.
 *
 * The curve is a sum of up to three co-rotating circular wheels:
 *
 *     z(t) = sum_k A_k * e^(i * 2*pi * (n_k*t + s_k))
 *
 * x(t) = Re(z(t)) = sum_k A_k * cos(2*pi*(n_k*t + s_k))
 * y(t) = Im(z(t)) = sum_k A_k * sin(2*pi*(n_k*t + s_k))
 *
 * Ported from legacy/cx.sh which fed the same function to gnuplot.
 *
 * The sampling loop is vectorised four samples at a time with AVX2 + FMA
 * (see sincos4 below). The arithmetic is arranged to match the scalar path
 * operation for operation, so the two agree to within the accuracy of the
 * sin/cos implementation itself; NOTES.md has the measured figures.
 */
#include "flowery.h"

#include <cmath>

#if defined(__AVX2__) && defined(__FMA__)
#  include <immintrin.h>
#  define FLOWERY_HAVE_AVX2 1
#endif

namespace {

/* 2*pi. The original shell version wrote 4*asin(1), which the compiler folds
 * to exactly this double, so the value is unchanged. */
constexpr double kTau = 6.283185307179586476925286766559;

#ifdef FLOWERY_HAVE_AVX2

/* Largest |angle| the fixed Cody-Waite reduction below stays exact for.
 *
 * The reduction needs fn * kPio2_i to be exactly representable, and fn is
 * about |x| / (pi/2), so the width of the leading terms caps the range: with
 * 25 significant bits there, fn may go up to 2^28. 4e8 / (pi/2) = 2.55e8,
 * just under 2^28, so this is the comfortable end of that range - a wheel
 * with 63 million teeth. Past it flowery_points falls back to libm, which
 * reduces exactly. (This used to be 1e6, which a held Shift+Up reached in
 * ten minutes and which cost a 6.7x cliff when it did.) */
constexpr double kVectorAngleLimit = 4.0e8;

/* pi/2 split into four parts, high to low: 25 significant bits, 24, 25, and
 * the rest as a full double. Subtracting them one at a time (Cody-Waite)
 * keeps the reduced argument exact, which reducing by a single rounded pi/2
 * would not - that loses about log2(x) bits and is what makes a naive vector
 * sin/cos useless for large arguments.
 *
 * The widths are the whole trick, and they are narrower than fdlibm's for a
 * reason. A term with b significant bits makes fn * term exact only while
 * fn < 2^(53-b): at fn = 2^28 even the 25-bit leading term needs 53 bits, so
 * there is no room for a wider one. fdlibm's pio2_1 / pio2_2 / pio2_3 are a
 * 33-bit split, which is exact only to fn < 2^20; at fn = 2^28 its first
 * product rounds at ulp(4e8)/2 = 3e-8, eight orders of magnitude worse than
 * the double it is subtracted from, and the reduced argument is ruined. What
 * the extra terms buy is coverage: four of them sum to pi/2 to 2.3e-41, so
 * four is what 81 bits (53 + 28) of accuracy at the top of the range costs.
 *
 * The split is derived from pi/2 rather than copied. fdlibm's pio2_1t /
 * pio2_2t / pio2_3t look like further terms and are actually *alternatives*
 * used by its two-step refinement; adding all six subtracts pi/2 three times
 * over, and the symptom is a visibly wrong curve, not a slightly off one. */
constexpr double kPio2_1 = 1.57079631090164184570e+00;
constexpr double kPio2_2 = 1.58932547122958567343e-08;
constexpr double kPio2_3 = 6.12323393205359425102e-17;
constexpr double kPio2_4 = 6.36831716351094990796e-25;

/* 2/pi, to 53 bits. Rounding fn = nint(x * this) can only go wrong when the
 * exact x*(2/pi) sits within ~|x|*2^-53 of a half-integer, and there the two
 * candidate fn values both leave |r| <= pi/4 - one lands on +pi/4, the other
 * on -pi/4 - so the quadrant fixup still returns the right answer. No second
 * 2/pi term is needed for that. */
constexpr double kTwoOverPi = 6.36619772367581382433e-01;

/* Widen a vector of 0 / -1 int32 lanes into a 0.0 / all-ones double mask. */
inline __m256d mask4(__m128i v)
{
    return _mm256_castsi256_pd(_mm256_cvtepi32_epi64(v));
}

/* sin(r) and cos(r) for |r| <= pi/4, as odd/even polynomials in r^2.
 *
 * The coefficients are fdlibm's (__kernel_sin / __kernel_cos), which are a
 * minimax fit to better than 1 ULP over this interval. A truncated Taylor
 * series is not good enough here: stopping at r^11 already costs ~7e-12 at
 * r = pi/4, which is four orders of magnitude worse than double rounding and
 * would show up in the curve. */
inline __m256d poly_sin(__m256d r)
{
    __m256d z = _mm256_mul_pd(r, r);
    __m256d p = _mm256_set1_pd( 1.58969099521155010221e-10);
    p = _mm256_fmadd_pd(p, z, _mm256_set1_pd(-2.50507602534068634195e-08));
    p = _mm256_fmadd_pd(p, z, _mm256_set1_pd( 2.75573137070700676789e-06));
    p = _mm256_fmadd_pd(p, z, _mm256_set1_pd(-1.98412698298579493134e-04));
    p = _mm256_fmadd_pd(p, z, _mm256_set1_pd( 8.33333333332248946124e-03));
    p = _mm256_fmadd_pd(p, z, _mm256_set1_pd(-1.66666666666666324348e-01));
    return _mm256_fmadd_pd(_mm256_mul_pd(r, z), p, r);      /* r + r*z*p */
}

inline __m256d poly_cos(__m256d r)
{
    __m256d z = _mm256_mul_pd(r, r);
    __m256d p = _mm256_set1_pd(-1.13596475577881948265e-11);
    p = _mm256_fmadd_pd(p, z, _mm256_set1_pd( 2.08757232129817482790e-09));
    p = _mm256_fmadd_pd(p, z, _mm256_set1_pd(-2.75573143513906633035e-07));
    p = _mm256_fmadd_pd(p, z, _mm256_set1_pd( 2.48015872894767294178e-05));
    p = _mm256_fmadd_pd(p, z, _mm256_set1_pd(-1.38888888888741095749e-03));
    p = _mm256_fmadd_pd(p, z, _mm256_set1_pd( 4.16666666666666019037e-02));
    p = _mm256_fmadd_pd(p, z, _mm256_set1_pd(-5.0e-1));
    return _mm256_fmadd_pd(p, z, _mm256_set1_pd(1.0));      /* 1 + z*p */
}

/* sin and cos of four doubles at once.
 *
 * x = q*(pi/2) + r with |r| <= pi/4, then the quadrant picks between the two
 * polynomials and their signs. The reduction subtracts all four parts of
 * pi/2 unconditionally: the later terms are no-ops in the common case and
 * cost three FMAs, which is cheaper than branching per lane. */
inline void sincos4(__m256d x, __m256d *sinp, __m256d *cosp)
{
    const __m256d invpio2 = _mm256_set1_pd(kTwoOverPi);

    __m256d fn = _mm256_round_pd(_mm256_mul_pd(x, invpio2),
                                 _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
    __m256d r = _mm256_fnmadd_pd(fn, _mm256_set1_pd(kPio2_1), x);
    r = _mm256_fnmadd_pd(fn, _mm256_set1_pd(kPio2_2), r);
    r = _mm256_fnmadd_pd(fn, _mm256_set1_pd(kPio2_3), r);
    r = _mm256_fnmadd_pd(fn, _mm256_set1_pd(kPio2_4), r);

    __m128i q  = _mm_and_si128(_mm256_cvttpd_epi32(fn), _mm_set1_epi32(3));
    __m128i q0 = _mm_and_si128(q, _mm_set1_epi32(1));   /* low bit  */
    __m128i q1 = _mm_and_si128(q, _mm_set1_epi32(2));   /* high bit */
    __m256d m0 = mask4(_mm_cmpgt_epi32(q0, _mm_setzero_si128()));
    __m256d m1 = mask4(_mm_cmpgt_epi32(q1, _mm_setzero_si128()));

    __m256d s = poly_sin(r), c = poly_cos(r);
    const __m256d signbit = _mm256_set1_pd(-0.0);

    /* sin: cos(r) in odd quadrants, negated in the top half */
    __m256d sv = _mm256_blendv_pd(s, c, m0);
    sv = _mm256_xor_pd(sv, _mm256_and_pd(m1, signbit));

    /* cos: sin(r) in odd quadrants, negated where the two bits disagree */
    __m256d cv = _mm256_blendv_pd(c, s, m0);
    cv = _mm256_xor_pd(cv, _mm256_and_pd(_mm256_xor_pd(m0, m1), signbit));

    *sinp = sv;
    *cosp = cv;
}

#endif /* FLOWERY_HAVE_AVX2 */

} /* namespace */

void
flowery_points(const FloweryParams *p, double *xs, double *ys)
{
    const int n = p->samples > 0 ? p->samples : 1;
    /* gnuplot `set samples N` evaluates t = i/(N-1) for i in 0..N-1,
     * so the first and last point coincide (a closed loop). */
    const double denom = n > 1 ? (double)(n - 1) : 1.0;
    int i;

#ifdef FLOWERY_HAVE_AVX2
    {
        double maxang = 0.0;
        for (int k = 0; k < 3; ++k) {
            const double m = (std::fabs(p->n[k]) + std::fabs(p->s[k])) * kTau;
            if (m > maxang) maxang = m;
        }

        if (maxang <= kVectorAngleLimit) {
            const __m256d taud = _mm256_set1_pd(kTau);
            const __m256d dend = _mm256_set1_pd(denom);
            const __m256d lane = _mm256_set_pd(3.0, 2.0, 1.0, 0.0);

            for (i = 0; i + 4 <= n; i += 4) {
                const __m256d t =
                    _mm256_div_pd(_mm256_add_pd(_mm256_set1_pd((double)i), lane),
                                  dend);
                __m256d x = _mm256_setzero_pd(), y = _mm256_setzero_pd();

                for (int k = 0; k < 3; ++k) {
                    const __m256d ang = _mm256_mul_pd(
                        taud,
                        _mm256_add_pd(
                            _mm256_mul_pd(_mm256_set1_pd(p->n[k]), t),
                            _mm256_set1_pd(p->s[k])));
                    __m256d s, c;
                    sincos4(ang, &s, &c);
                    const __m256d a = _mm256_set1_pd(p->a[k]);
                    x = _mm256_add_pd(x, _mm256_mul_pd(a, c));
                    y = _mm256_add_pd(y, _mm256_mul_pd(a, s));
                }
                _mm256_storeu_pd(xs + i, x);
                _mm256_storeu_pd(ys + i, y);
            }
            for (; i < n; ++i) {
                const double t = (double)i / denom;
                double x = 0.0, y = 0.0;
                for (int k = 0; k < 3; ++k) {
                    const double ang = 4.0 * std::asin(1.0)
                                     * (p->n[k] * t + p->s[k]);
                    x += p->a[k] * std::cos(ang);
                    y += p->a[k] * std::sin(ang);
                }
                xs[i] = x;
                ys[i] = y;
            }
            return;
        }
    }
#endif

    for (i = 0; i < n; ++i) {
        const double t = (double)i / denom; /* t in [0, 1] */
        double x = 0.0, y = 0.0;
        for (int k = 0; k < 3; ++k) {
            /* 2*pi = 4*asin(1.0) */
            const double ang = 4.0 * std::asin(1.0) * (p->n[k] * t + p->s[k]);
            x += p->a[k] * std::cos(ang);
            y += p->a[k] * std::sin(ang);
        }
        xs[i] = x;
        ys[i] = y;
    }
}

void
flowery_bbox(const double *xs, const double *ys, int n,
             double *minx, double *miny, double *maxx, double *maxy)
{
    if (n <= 0) {
        *minx = *maxx = *miny = *maxy = 0.0;
        return;
    }

    double loX = xs[0], hiX = xs[0];
    double loY = ys[0], hiY = ys[0];
    int i;

#ifdef FLOWERY_HAVE_AVX2
    /* min/max are exact, so this is bit-identical to the scalar version. */
    __m256d vloX = _mm256_set1_pd(xs[0]), vhiX = vloX;
    __m256d vloY = _mm256_set1_pd(ys[0]), vhiY = vloY;
    for (i = 1; i + 4 <= n; i += 4) {
        const __m256d x = _mm256_loadu_pd(xs + i);
        const __m256d y = _mm256_loadu_pd(ys + i);
        vloX = _mm256_min_pd(vloX, x);
        vhiX = _mm256_max_pd(vhiX, x);
        vloY = _mm256_min_pd(vloY, y);
        vhiY = _mm256_max_pd(vhiY, y);
    }
    {
        double b[4];
        _mm256_storeu_pd(b, vloX); for (int j = 0; j < 4; ++j) if (b[j] < loX) loX = b[j];
        _mm256_storeu_pd(b, vhiX); for (int j = 0; j < 4; ++j) if (b[j] > hiX) hiX = b[j];
        _mm256_storeu_pd(b, vloY); for (int j = 0; j < 4; ++j) if (b[j] < loY) loY = b[j];
        _mm256_storeu_pd(b, vhiY); for (int j = 0; j < 4; ++j) if (b[j] > hiY) hiY = b[j];
    }
#else
    i = 1;
#endif

    for (; i < n; ++i) {
        if (xs[i] < loX) loX = xs[i];
        if (xs[i] > hiX) hiX = xs[i];
        if (ys[i] < loY) loY = ys[i];
        if (ys[i] > hiY) hiY = ys[i];
    }

    *minx = loX; *maxx = hiX;
    *miny = loY; *maxy = hiY;
}
