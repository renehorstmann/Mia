#ifndef M_VEC_INT_VECN_H
#define M_VEC_INT_VECN_H

/**
 * @file vec/ivecn.h
 *
 * functions working on int ivectors with a given size
 */



#include "m/sca/int.h"

/** macro to cast a ivector into a int ivector */
#define ivecn_cast_into(dst, from, n) \
do {                                                                           \
    for(int ivecn_cast_into_i_=0; ivecn_cast_into_i_<(n); ivecn_cast_into_i_++) { \
        (dst)[ivecn_cast_into_i_] = (int) (from)[ivecn_cast_into_i_];          \
    }                                                                          \
} while(0)


/** dst = v / 255 */
O_INLINE
void ivecn_cast_byte_1(int *dst_v, const obyte *ivec, int n)
{
    for (int i = 0; i < n; i++) {
        dst_v[i] = (int) ivec[i] / (int) 255;
    }
}


/** a == b */
O_INLINE
bool ivecn_equals(const int *a, int b, int n)
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
bool ivecn_equals_v(const int *a, const int *b, int n)
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
bool ivecn_equals_eps(const int *a, int b, int eps, int n)
{
    for (int i = 0; i < n; i++) {
        if (mi_abs(a[i] - b) > eps) {
            return false;
        }
    }
    return true;
}

/** a ~= b */
O_INLINE
bool ivecn_equals_eps_v(const int *a, const int *b, int eps, int n)
{
    for (int i = 0; i < n; i++) {
        if (mi_abs(a[i] - b[i]) > eps) {
            return false;
        }
    }
    return true;
}

/** dst = v */
O_INLINE
void ivecn_clone(int *dst, const int *v, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = v[i];
    }
}

/** dst = s */
O_INLINE
void ivecn_set(int *dst, int s, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = s;
    }
}

/** dst = unit_x */
O_INLINE
void ivecn_unit_x(int *dst, int n)
{
    ivecn_set(dst, 0, n);
    dst[0] = 1;
}

/** dst = unit_y */
O_INLINE
void ivecn_unit_y(int *dst, int n)
{
    assert(n >= 2 && "m ivec*_unit_z");
    ivecn_set(dst, 0, n);
    dst[1] = 1;
}

/** assert(n>=3); dst = unit_z */
O_INLINE
void ivecn_unit_z(int *dst, int n)
{
    assert(n >= 3 && "m ivec*_unit_z");
    ivecn_set(dst, 0, n);
    dst[2] = 1;
}

/** assert(n>=4); dst = unit_w */
O_INLINE
void ivecn_unit_w(int *dst, int n)
{
    assert(n >= 4 && "m ivec*_unit_w");
    ivecn_set(dst, 0, n);
    dst[3] = 1;
}

/** dst = -v */
O_INLINE
void ivecn_neg(int *dst, const int *v, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = -v[i];
    }
}

/** dst = a + b */
O_INLINE
void ivecn_add_v(int *dst, const int *a, const int *b, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = a[i] + b[i];
    }
}

/** dst = a + b */
O_INLINE
void ivecn_add(int *dst, const int *a, int b, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = a[i] + b;
    }
}

/** dst = a - b */
O_INLINE
void ivecn_sub_v(int *dst, const int *a, const int *b, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = a[i] - b[i];
    }
}

/** dst = a - b */
O_INLINE
void ivecn_sub(int *dst, const int *a, int b, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = a[i] - b;
    }
}

/** dst = a * b */
O_INLINE
void ivecn_scale_v(int *dst, const int *a, const int *b, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = a[i] * b[i];
    }
}

/** dst = a * b */
O_INLINE
void ivecn_scale(int *dst, const int *a, int b, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = a[i] * b;
    }
}

/** dst = a / b */
O_INLINE
void ivecn_div_v(int *dst, const int *a, const int *b, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = a[i] / b[i];
    }
}

/** dst = a / b */
O_INLINE
void ivecn_div(int *dst, const int *a, int b, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = a[i] / b;
    }
}

/** dst = a + b * c */
O_INLINE
void ivecn_add_scaled_v(int *dst, const int *a, const int *b, const int *c, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = a[i] + b[i] * c[i];
    }
}

/** dst = a + b * c */
O_INLINE
void ivecn_add_scaled(int *dst, const int *a, const int *b, int c, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = a[i] + b[i] * c;
    }
}

/** dst = abs(x) */
O_INLINE
void ivecn_abs(int *dst, const int *x, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = mi_abs(x[i]);
    }
}

/** dst = x > 0 ? 1 : (x < 0 ? -1 : 0) */
O_INLINE
void ivecn_sign(int *dst, const int *x, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = mi_sign(x[i]);
    }
}

/** dst = x % y as positive version for integers */
O_INLINE
void ivecn_mod(int *dst, const int *x, int y, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = mi_mod(x[i], y);
    }
}

/** dst = x % y as positive version for integers */
O_INLINE
void ivecn_mod_v(int *dst, const int *x, const int *y, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = mi_mod(x[i], y[i]);
    }
}

/** dst = a < b ? a : b */
O_INLINE
void ivecn_min(int *dst, const int *a, int b, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = mi_min(a[i], b);
    }
}

/** dst = a < b ? a : b */
O_INLINE
void ivecn_min_v(int *dst, const int *a, const int *b, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = mi_min(a[i], b[i]);
    }
}

/** dst = a > b ? a : b */
O_INLINE
void ivecn_max(int *dst, const int *a, int b, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = mi_max(a[i], b);
    }
}

/** dst = a > b ? a : b */
O_INLINE
void ivecn_max_v(int *dst, const int *a, const int *b, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = mi_max(a[i], b[i]);
    }
}

/** returns the index of the min v value */
O_INLINE
int ivecn_min_index(const int *v, int n)
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
int ivecn_max_index(const int *v, int n)
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
void ivecn_clamp(int *dst, const int *x, int min, int max, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = mi_clamp(x[i], min, max);
    }
}

/** dst = x < min ? min : (x > max ? max : x) */
O_INLINE
void ivecn_clamp_v(int *dst, const int *x, const int *min, const int *max, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = mi_clamp(x[i], min[i], max[i]);
    }
}

/** dst = a * (1-t) + b * t */
O_INLINE
void ivecn_mix(int *dst, const int *a, const int *b, float t, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = mi_mix(a[i], b[i], t);
    }
}

/** dst = a * (1-t) + b * t */
O_INLINE
void ivecn_mix_v(int *dst, const int *a, const int *b, const float *t, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = mi_mix(a[i], b[i], t[i]);
    }
}

/** returns v[0] + v[1] + ... + v[n-1] */
O_INLINE
int ivecn_sum(const int *v, int n)
{
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += v[i];
    }
    return sum;
}

#endif //M_VEC_INT_VECN_H
