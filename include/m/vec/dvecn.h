#ifndef M_VEC_DBL_VECN_H
#define M_VEC_DBL_VECN_H

/**
 * @file vec/dvecn.h
 *
 * functions working on double dvectors with a given size
 */



#include "m/sca/dbl.h"

/** macro to cast a dvector into a double dvector */
#define dvecn_cast_into(dst, from, n) \
do {                                                                           \
    for(int dvecn_cast_into_i_=0; dvecn_cast_into_i_<(n); dvecn_cast_into_i_++) { \
        (dst)[dvecn_cast_into_i_] = (double) (from)[dvecn_cast_into_i_];          \
    }                                                                          \
} while(0)


/** dst = v / 255 */
O_INLINE
void dvecn_cast_byte_1(double *dst_v, const obyte *dvec, int n)
{
    for (int i = 0; i < n; i++) {
        dst_v[i] = (double) dvec[i] / (double) 255;
    }
}


/** a == b */
O_INLINE
bool dvecn_equals(const double *a, double b, int n)
{
    for (int i = 0; i < n; i++) {
        if (a[i] != b) {
            return false;
        }
    }
    return true;
}

/** a == b */
O_INLINE
bool dvecn_equals_v(const double *a, const double *b, int n)
{
    for (int i = 0; i < n; i++) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

/** a ~= b */
O_INLINE
bool dvecn_equals_eps(const double *a, double b, double eps, int n)
{
    for (int i = 0; i < n; i++) {
        if (o_abs(a[i] - b) > eps) {
            return false;
        }
    }
    return true;
}

/** a ~= b */
O_INLINE
bool dvecn_equals_eps_v(const double *a, const double *b, double eps, int n)
{
    for (int i = 0; i < n; i++) {
        if (o_abs(a[i] - b[i]) > eps) {
            return false;
        }
    }
    return true;
}

/** dst = v */
O_INLINE
void dvecn_clone(double *dst, const double *v, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = v[i];
    }
}

/** dst = s */
O_INLINE
void dvecn_set(double *dst, double s, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = s;
    }
}

/** dst = unit_x */
O_INLINE
void dvecn_unit_x(double *dst, int n)
{
    dvecn_set(dst, 0, n);
    dst[0] = 1;
}

/** dst = unit_y */
O_INLINE
void dvecn_unit_y(double *dst, int n)
{
    assert(n >= 2 && "m dvec*_unit_z");
    dvecn_set(dst, 0, n);
    dst[1] = 1;
}

/** assert(n>=3); dst = unit_z */
O_INLINE
void dvecn_unit_z(double *dst, int n)
{
    assert(n >= 3 && "m dvec*_unit_z");
    dvecn_set(dst, 0, n);
    dst[2] = 1;
}

/** assert(n>=4); dst = unit_w */
O_INLINE
void dvecn_unit_w(double *dst, int n)
{
    assert(n >= 4 && "m dvec*_unit_w");
    dvecn_set(dst, 0, n);
    dst[3] = 1;
}

/** dst = -v */
O_INLINE
void dvecn_neg(double *dst, const double *v, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = -v[i];
    }
}

/** dst = a + b */
O_INLINE
void dvecn_add_v(double *dst, const double *a, const double *b, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = a[i] + b[i];
    }
}

/** dst = a + b */
O_INLINE
void dvecn_add(double *dst, const double *a, double b, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = a[i] + b;
    }
}

/** dst = a - b */
O_INLINE
void dvecn_sub_v(double *dst, const double *a, const double *b, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = a[i] - b[i];
    }
}

/** dst = a - b */
O_INLINE
void dvecn_sub(double *dst, const double *a, double b, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = a[i] - b;
    }
}

/** dst = a * b */
O_INLINE
void dvecn_scale_v(double *dst, const double *a, const double *b, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = a[i] * b[i];
    }
}

/** dst = a * b */
O_INLINE
void dvecn_scale(double *dst, const double *a, double b, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = a[i] * b;
    }
}

/** dst = a / b */
O_INLINE
void dvecn_div_v(double *dst, const double *a, const double *b, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = a[i] / b[i];
    }
}

/** dst = a / b */
O_INLINE
void dvecn_div(double *dst, const double *a, double b, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = a[i] / b;
    }
}

/** dst = a + b * c */
O_INLINE
void dvecn_add_scaled_v(double *dst, const double *a, const double *b, const double *c, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = a[i] + b[i] * c[i];
    }
}

/** dst = a + b * c */
O_INLINE
void dvecn_add_scaled(double *dst, const double *a, const double *b, double c, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = a[i] + b[i] * c;
    }
}

/** dst = a * M_PI / 180 */
O_INLINE
void dvecn_radians(double *dst, const double *deg, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = md_radians(deg[i]);
    }
}

/** dst = a * 180 / M_PI */
O_INLINE
void dvecn_degrees(double *dst, const double *rad, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = md_degrees(rad[i]);
    }
}

/** dst = abs(x) */
O_INLINE
void dvecn_abs(double *dst, const double *x, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = md_abs(x[i]);
    }
}

/** dst = x > 0 ? 1 : (x < 0 ? -1 : 0) */
O_INLINE
void dvecn_sign(double *dst, const double *x, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = md_sign(x[i]);
    }
}

/** dst = round(x) */
O_INLINE
void dvecn_round(double *dst, const double *x, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = md_round(x[i]);
    }
}

/** dst = floor(x) */
O_INLINE
void dvecn_floor(double *dst, const double *x, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = md_floor(x[i]);
    }
}

/** dst = ceil(x) */
O_INLINE
void dvecn_ceil(double *dst, const double *x, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = md_ceil(x[i]);
    }
}

/** dst = x - floor(x) */
O_INLINE
void dvecn_fract(double *dst, const double *x, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = md_fract(x[i]);
    }
}

/** dst = x - y * floor(x/y) */
O_INLINE
void dvecn_mod(double *dst, const double *x, double y, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = md_mod(x[i], y);
    }
}

/** dst = x - y * floor(x/y) */
O_INLINE
void dvecn_mod_v(double *dst, const double *x, const double *y, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = md_mod(x[i], y[i]);
    }
}

/** dst = a < b ? a : b */
O_INLINE
void dvecn_min(double *dst, const double *a, double b, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = md_min(a[i], b);
    }
}

/** dst = a < b ? a : b */
O_INLINE
void dvecn_min_v(double *dst, const double *a, const double *b, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = md_min(a[i], b[i]);
    }
}

/** dst = a > b ? a : b */
O_INLINE
void dvecn_max(double *dst, const double *a, double b, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = md_max(a[i], b);
    }
}

/** dst = a > b ? a : b */
O_INLINE
void dvecn_max_v(double *dst, const double *a, const double *b, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = md_max(a[i], b[i]);
    }
}

/** returns the index of the min v value */
O_INLINE
int dvecn_min_index(const double *v, int n)
{
    int idx = 0;
    for (int i = 1; i < n; i++) {
        if (v[i] < v[idx]) {
            idx = i;
        }
    }
    return idx;
}

/** returns the index of the max v value */
O_INLINE
int dvecn_max_index(const double *v, int n)
{
    int idx = 0;
    for (int i = 1; i < n; i++) {
        if (v[i] > v[idx]) {
            idx = i;
        }
    }
    return idx;
}

/** dst = x < min ? min : (x > max ? max : x) */
O_INLINE
void dvecn_clamp(double *dst, const double *x, double min, double max, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = md_clamp(x[i], min, max);
    }
}

/** dst = x < min ? min : (x > max ? max : x) */
O_INLINE
void dvecn_clamp_v(double *dst, const double *x, const double *min, const double *max, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = md_clamp(x[i], min[i], max[i]);
    }
}

/** dst = a * (1-t) + b * t */
O_INLINE
void dvecn_mix(double *dst, const double *a, const double *b, double t, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = md_mix(a[i], b[i], t);
    }
}

/** dst = a * (1-t) + b * t */
O_INLINE
void dvecn_mix_v(double *dst, const double *a, const double *b, const double *t, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = md_mix(a[i], b[i], t[i]);
    }
}

/** returns v[0] + v[1] + ... + v[n-1] */
O_INLINE
double dvecn_sum(const double *v, int n)
{
    double sum = 0;
    for (int i = 0; i < n; i++) {
        sum += v[i];
    }
    return sum;
}

/** returns a dot b */
O_INLINE
double dvecn_dot(const double *a, const double *b, int n)
{
    double dot = 0;
    for (int i = 0; i < n; i++) {
        dot += a[i] * b[i];
    }
    return dot;
}

/** assert(n>=3) ; dst = a x b , dst.w... = 0 */
O_INLINE
void dvecn_cross(double *dst, const double *a, const double *b, int n)
{
    assert(n >= 3 && "m dvec*_cross only in 3D");
    dst[0] = a[1] * b[2] - a[2] * b[1];
    dst[1] = a[2] * b[0] - a[0] * b[2];
    dst[2] = a[0] * b[1] - a[1] * b[0];
    for (int i = 3; i < n; i++)
        dst[i] = 0;
}

/** returns ||v||_2 */
O_INLINE
double dvecn_norm(const double *v, int n)
{
    return md_sqrt(dvecn_dot(v, v, n));
}

/** returns ||v||_p */
O_INLINE
double dvecn_norm_p(const double *v, double p, int n)
{
    double sum = 0;
    for (int i = 0; i < n; i++) {
        sum += md_pow(md_abs(v[i]), p);
    }
    return md_pow(sum, (double) 1 / p);
}

/** returns ||v||_1 */
O_INLINE
double dvecn_norm_1(const double *v, int n)
{
    double sum = 0;
    for (int i = 0; i < n; i++) {
        sum += md_abs(v[i]);
    }
    return sum;
}

/** returns ||v||_inf */
O_INLINE
double dvecn_norm_inf(const double *v, int n)
{
    double max = 0;
    for (int i = 0; i < n; i++) {
        if (max < md_abs(v[i])) {
            max = md_abs(v[i]);
        }
    }
    return max;
}

/** dst = v / norm(v) */
O_INLINE
void dvecn_normalize_unsafe(double *dst, const double *v, int n)
{
    dvecn_scale(dst, v, (double) 1 / dvecn_norm(v, n), n);
}

/** dst = v / (norm(v) > 0 ? norm(v) : 1) */
O_INLINE
void dvecn_normalize(double *dst, const double *v, int n)
{
    double norm = dvecn_norm(v, n);
    dvecn_scale(dst, v, (double) 1 / (norm > (double) 0 ? norm : (double) 1), n);
}

/** dst = normalize(cross(a, b)) */
O_INLINE
void dvecn_cross_normalized(double *dst, const double *a, const double *b, int n)
{
    dvecn_cross(dst, a, b, n);
    dvecn_normalize(dst, dst, n);
}

/** returns length of a dvector, see dvecn_norm. Just here to match glsl */
O_INLINE
double dvecn_length(const double *v, int n)
{
    return dvecn_norm(v, n);
}

/** returns norm(b-a) */
O_INLINE
double dvecn_distance(const double *a, const double *b, int n)
{
    assert(n <= M_MAX_SIZE);
    double tmp[M_MAX_SIZE];
    dvecn_sub_v(tmp, b, a, n);
    return dvecn_norm(tmp, n);
}

/** returns dot(b-a) */
O_INLINE
double dvecn_sqr_distance(const double *a, const double *b, int n)
{
    assert(n <= M_MAX_SIZE);
    double tmp[M_MAX_SIZE];
    dvecn_sub_v(tmp, b, a, n);
    return dvecn_dot(tmp, tmp, n);
}

/** dst = dot(i_v, nref_v) < 0 ? n_v : -n_v */
O_INLINE
void dvecn_faceforward(double *dst, const double *n_v, const double *i_v, const double *nref_v, int n)
{
    if (dvecn_dot(i_v, nref_v, n) < 0) {
        dvecn_clone(dst, n_v, n);
    } else {
        dvecn_neg(dst, n_v, n);
    }
}

/** dst = i_v - 2.0 * n_v * dot(n_v,i_v) */
O_INLINE
void dvecn_reflect(double *dst, const double *i_v, const double *n_v, int n)
{
    dvecn_scale(dst, n_v, (double) 2 * dvecn_dot(n_v, i_v, n), n);
    dvecn_sub_v(dst, i_v, dst, n);
}

O_INLINE
void dvecn_refract(double *dst, const double *i_v, const double *n_v, double eta, int n)
{
    // implementation from example implementation: https://developer.download.nvidia.com/cg/refract.html
    dvecn_neg(dst, i_v, n);
    double cosi = dvecn_dot(dst, n_v, n);
    double cost2 = (double) 1 - eta * eta * ((double) 1 - cosi * cosi);
    dvecn_scale(dst, n_v, eta * cosi - md_sqrt(md_abs(cost2)), n);
    double t[3];
    dvecn_scale(t, i_v, eta, n);
    dvecn_add_v(t, t, dst, n);
    dvecn_scale(dst, t, md_step(0, cost2), n);
}

#endif //M_VEC_DBL_VECN_H
