#ifndef M_VEC_FLT_VECN_H
#define M_VEC_FLT_VECN_H

/**
 * @file vec/vecn.h
 *
 * functions working on float vectors with a given size
 */



#include "m/sca/flt.h"

/** macro to cast a vector into a float vector */
#define vecn_cast_into(dst, from, n) \
do {                                                                           \
    for(int vecn_cast_into_i_=0; vecn_cast_into_i_<(n); vecn_cast_into_i_++) { \
        (dst)[vecn_cast_into_i_] = (float) (from)[vecn_cast_into_i_];          \
    }                                                                          \
} while(0)


/** dst = v / 255 */
O_INLINE
void vecn_cast_byte_1(float *dst_v, const obyte *vec, int n)
{
    for (int i = 0; i < n; i++) {
        dst_v[i] = (float) vec[i] / (float) 255;
    }
}


/** a == b */
O_INLINE
bool vecn_equals(const float *a, float b, int n)
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
bool vecn_equals_v(const float *a, const float *b, int n)
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
bool vecn_equals_eps(const float *a, float b, float eps, int n)
{
    for (int i = 0; i < n; i++) {
        if (m_abs(a[i] - b) > eps) {
            return false;
        }
    }
    return true;
}

/** a ~= b */
O_INLINE
bool vecn_equals_eps_v(const float *a, const float *b, float eps, int n)
{
    for (int i = 0; i < n; i++) {
        if (m_abs(a[i] - b[i]) > eps) {
            return false;
        }
    }
    return true;
}

/** dst = v */
O_INLINE
void vecn_clone(float *dst, const float *v, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = v[i];
    }
}

/** dst = s */
O_INLINE
void vecn_set(float *dst, float s, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = s;
    }
}

/** dst = unit_x */
O_INLINE
void vecn_unit_x(float *dst, int n)
{
    vecn_set(dst, 0, n);
    dst[0] = 1;
}

/** dst = unit_y */
O_INLINE
void vecn_unit_y(float *dst, int n)
{
    assert(n >= 2 && "m vec*_unit_z");
    vecn_set(dst, 0, n);
    dst[1] = 1;
}

/** assert(n>=3); dst = unit_z */
O_INLINE
void vecn_unit_z(float *dst, int n)
{
    assert(n >= 3 && "m vec*_unit_z");
    vecn_set(dst, 0, n);
    dst[2] = 1;
}

/** assert(n>=4); dst = unit_w */
O_INLINE
void vecn_unit_w(float *dst, int n)
{
    assert(n >= 4 && "m vec*_unit_w");
    vecn_set(dst, 0, n);
    dst[3] = 1;
}

/** dst = -v */
O_INLINE
void vecn_neg(float *dst, const float *v, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = -v[i];
    }
}

/** dst = a + b */
O_INLINE
void vecn_add_v(float *dst, const float *a, const float *b, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = a[i] + b[i];
    }
}

/** dst = a + b */
O_INLINE
void vecn_add(float *dst, const float *a, float b, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = a[i] + b;
    }
}

/** dst = a - b */
O_INLINE
void vecn_sub_v(float *dst, const float *a, const float *b, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = a[i] - b[i];
    }
}

/** dst = a - b */
O_INLINE
void vecn_sub(float *dst, const float *a, float b, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = a[i] - b;
    }
}

/** dst = a * b */
O_INLINE
void vecn_scale_v(float *dst, const float *a, const float *b, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = a[i] * b[i];
    }
}

/** dst = a * b */
O_INLINE
void vecn_scale(float *dst, const float *a, float b, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = a[i] * b;
    }
}

/** dst = a / b */
O_INLINE
void vecn_div_v(float *dst, const float *a, const float *b, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = a[i] / b[i];
    }
}

/** dst = a / b */
O_INLINE
void vecn_div(float *dst, const float *a, float b, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = a[i] / b;
    }
}

/** dst = a + b * c */
O_INLINE
void vecn_add_scaled_v(float *dst, const float *a, const float *b, const float *c, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = a[i] + b[i] * c[i];
    }
}

/** dst = a + b * c */
O_INLINE
void vecn_add_scaled(float *dst, const float *a, const float *b, float c, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = a[i] + b[i] * c;
    }
}

/** dst = a * M_PI / 180 */
O_INLINE
void vecn_radians(float *dst, const float *deg, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = m_radians(deg[i]);
    }
}

/** dst = a * 180 / M_PI */
O_INLINE
void vecn_degrees(float *dst, const float *rad, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = m_degrees(rad[i]);
    }
}

/** dst = abs(x) */
O_INLINE
void vecn_abs(float *dst, const float *x, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = m_abs(x[i]);
    }
}

/** dst = x > 0 ? 1 : (x < 0 ? -1 : 0) */
O_INLINE
void vecn_sign(float *dst, const float *x, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = m_sign(x[i]);
    }
}

/** dst = round(x) */
O_INLINE
void vecn_round(float *dst, const float *x, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = m_round(x[i]);
    }
}

/** dst = floor(x) */
O_INLINE
void vecn_floor(float *dst, const float *x, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = m_floor(x[i]);
    }
}

/** dst = ceil(x) */
O_INLINE
void vecn_ceil(float *dst, const float *x, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = m_ceil(x[i]);
    }
}

/** dst = x - floor(x) */
O_INLINE
void vecn_fract(float *dst, const float *x, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = m_fract(x[i]);
    }
}

/** dst = x - y * floor(x/y) */
O_INLINE
void vecn_mod(float *dst, const float *x, float y, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = m_mod(x[i], y);
    }
}

/** dst = x - y * floor(x/y) */
O_INLINE
void vecn_mod_v(float *dst, const float *x, const float *y, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = m_mod(x[i], y[i]);
    }
}

/** dst = a < b ? a : b */
O_INLINE
void vecn_min(float *dst, const float *a, float b, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = m_min(a[i], b);
    }
}

/** dst = a < b ? a : b */
O_INLINE
void vecn_min_v(float *dst, const float *a, const float *b, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = m_min(a[i], b[i]);
    }
}

/** dst = a > b ? a : b */
O_INLINE
void vecn_max(float *dst, const float *a, float b, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = m_max(a[i], b);
    }
}

/** dst = a > b ? a : b */
O_INLINE
void vecn_max_v(float *dst, const float *a, const float *b, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = m_max(a[i], b[i]);
    }
}

/** returns the index of the min v value */
O_INLINE
int vecn_min_index(const float *v, int n)
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
int vecn_max_index(const float *v, int n)
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
void vecn_clamp(float *dst, const float *x, float min, float max, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = m_clamp(x[i], min, max);
    }
}

/** dst = x < min ? min : (x > max ? max : x) */
O_INLINE
void vecn_clamp_v(float *dst, const float *x, const float *min, const float *max, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = m_clamp(x[i], min[i], max[i]);
    }
}

/** dst = a * (1-t) + b * t */
O_INLINE
void vecn_mix(float *dst, const float *a, const float *b, float t, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = m_mix(a[i], b[i], t);
    }
}

/** dst = a * (1-t) + b * t */
O_INLINE
void vecn_mix_v(float *dst, const float *a, const float *b, const float *t, int n)
{
    for (int i = 0; i < n; i++) {
        dst[i] = m_mix(a[i], b[i], t[i]);
    }
}

/** returns (double) v[0] + v[1] + ... + v[n-1] */
O_INLINE
double vecn_sum_d(const float *v, int n)
{
    double sum = 0;
    for (int i = 0; i < n; i++) {
        sum += v[i];
    }
    return sum;
}

/** returns v[0] + v[1] + ... + v[n-1] */
O_INLINE
float vecn_sum(const float *v, int n)
{
    return (float) vecn_sum_d(v, n);
}


/** returns a dot b */
O_INLINE
float vecn_dot(const float *a, const float *b, int n)
{
    float dot = 0;
    for (int i = 0; i < n; i++) {
        dot += a[i] * b[i];
    }
    return dot;
}

/** assert(n>=3) ; dst = a x b , dst.w... = 0 */
O_INLINE
void vecn_cross(float *dst, const float *a, const float *b, int n)
{
    assert(n >= 3 && "m vec*_cross only in 3D");
    dst[0] = a[1] * b[2] - a[2] * b[1];
    dst[1] = a[2] * b[0] - a[0] * b[2];
    dst[2] = a[0] * b[1] - a[1] * b[0];
    for (int i = 3; i < n; i++)
        dst[i] = 0;
}

/** returns ||v||_2 */
O_INLINE
float vecn_norm(const float *v, int n)
{
    return m_sqrt(vecn_dot(v, v, n));
}

/** returns ||v||_p */
O_INLINE
float vecn_norm_p(const float *v, float p, int n)
{
    float sum = 0;
    for (int i = 0; i < n; i++) {
        sum += m_pow(m_abs(v[i]), p);
    }
    return m_pow(sum, (float) 1 / p);
}

/** returns ||v||_1 */
O_INLINE
float vecn_norm_1(const float *v, int n)
{
    float sum = 0;
    for (int i = 0; i < n; i++) {
        sum += m_abs(v[i]);
    }
    return sum;
}

/** returns ||v||_inf */
O_INLINE
float vecn_norm_inf(const float *v, int n)
{
    float max = 0;
    for (int i = 0; i < n; i++) {
        if (max < m_abs(v[i])) {
            max = m_abs(v[i]);
        }
    }
    return max;
}

/** dst = v / norm(v) */
O_INLINE
void vecn_normalize_unsafe(float *dst, const float *v, int n)
{
    vecn_scale(dst, v, (float) 1 / vecn_norm(v, n), n);
}

/** dst = v / (norm(v) > 0 ? norm(v) : 1) */
O_INLINE
void vecn_normalize(float *dst, const float *v, int n)
{
    float norm = vecn_norm(v, n);
    vecn_scale(dst, v, (float) 1 / (norm > (float) 0 ? norm : (float) 1), n);
}

/** dst = normalize(cross(a, b)) */
O_INLINE
void vecn_cross_normalized(float *dst, const float *a, const float *b, int n)
{
    vecn_cross(dst, a, b, n);
    vecn_normalize(dst, dst, n);
}

/** returns length of a vector, see vecn_norm. Just here to match glsl */
O_INLINE
float vecn_length(const float *v, int n)
{
    return vecn_norm(v, n);
}

/** returns norm(b-a) */
O_INLINE
float vecn_distance(const float *a, const float *b, int n)
{
    assert(n <= M_MAX_SIZE);
    float tmp[M_MAX_SIZE];
    vecn_sub_v(tmp, b, a, n);
    return vecn_norm(tmp, n);
}

/** returns dot(b-a) */
O_INLINE
float vecn_sqr_distance(const float *a, const float *b, int n)
{
    assert(n <= M_MAX_SIZE);
    float tmp[M_MAX_SIZE];
    vecn_sub_v(tmp, b, a, n);
    return vecn_dot(tmp, tmp, n);
}

/** dst = dot(i_v, nref_v) < 0 ? n_v : -n_v */
O_INLINE
void vecn_faceforward(float *dst, const float *n_v, const float *i_v, const float *nref_v, int n)
{
    if (vecn_dot(i_v, nref_v, n) < 0) {
        vecn_clone(dst, n_v, n);
    } else {
        vecn_neg(dst, n_v, n);
    }
}

/** dst = i_v - 2.0 * n_v * dot(n_v,i_v) */
O_INLINE
void vecn_reflect(float *dst, const float *i_v, const float *n_v, int n)
{
    vecn_scale(dst, n_v, (float) 2 * vecn_dot(n_v, i_v, n), n);
    vecn_sub_v(dst, i_v, dst, n);
}

O_INLINE
void vecn_refract(float *dst, const float *i_v, const float *n_v, float eta, int n)
{
    // implementation from example implementation: https://developer.download.nvidia.com/cg/refract.html
    vecn_neg(dst, i_v, n);
    float cosi = vecn_dot(dst, n_v, n);
    float cost2 = (float) 1 - eta * eta * ((float) 1 - cosi * cosi);
    vecn_scale(dst, n_v, eta * cosi - m_sqrt(m_abs(cost2)), n);
    float t[3];
    vecn_scale(t, i_v, eta, n);
    vecn_add_v(t, t, dst, n);
    vecn_scale(dst, t, m_step(0, cost2), n);
}

#endif //M_VEC_FLT_VECN_H
