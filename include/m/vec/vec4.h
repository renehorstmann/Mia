#ifndef M_VEC_FLT_VEC4_H
#define M_VEC_FLT_VEC4_H

/**
 * @file vec/vec4.h
 *
 * functions working with vec4
 */


#include "vecn.h"
#include "m/m_types/flt.h"

/** dst = v0, v1, ... */
O_INLINE
m_vec4 vec4_new(float v0, float v1, float v2, float v3)
{
    m_vec4 self;
    self.v0 = v0;
    self.v1 = v1;
    self.v2 = v2;
    self.v3 = v3;
    return self;
}

///** dst = (m_vec4) v */
//O_INLINE
//m_vec4 vec4_cast_float(const float *cast)
//{
//    m_vec4 res;
//    vecn_cast_into(res.v, cast, 4);
//    return res;
//}

/** dst = (m_vec4) v */
O_INLINE
m_vec4 vec4_cast_double(const double *cast)
{
    m_vec4 res;
    vecn_cast_into(res.v, cast, 4);
    return res;
}

/** dst = (m_vec4) v */
O_INLINE
m_vec4 vec4_cast_int(const int *cast)
{
    m_vec4 res;
    vecn_cast_into(res.v, cast, 4);
    return res;
}

/** dst = (m_vec4) v */
O_INLINE
m_vec4 vec4_cast_byte(const obyte *cast)
{
    m_vec4 res;
    vecn_cast_into(res.v, cast, 4);
    return res;
}

/** dst = v / 255 */
O_INLINE
m_vec4 vec4_cast_byte_1(const obyte *cast)
{
    m_vec4 res;
    vecn_cast_byte_1(res.v, cast, 4);
    return res;
}



/** a == b */
O_INLINE
bool vec4_equals(m_vec4 a, float b)
{
    return vecn_equals(a.v, b, 4);
}

/** a == b */
O_INLINE
bool vec4_equals_v(m_vec4 a, m_vec4 b)
{
    return vecn_equals_v(a.v, b.v, 4);
}

/** a ~= b */
O_INLINE
bool vec4_equals_eps(m_vec4 a, float b, float eps)
{
    return vecn_equals_eps(a.v, b, eps, 4);
}

/** a ~= b */
O_INLINE
bool vec4_equals_eps_v(m_vec4 a, m_vec4 b, float eps)
{
    return vecn_equals_eps_v(a.v, b.v, eps, 4);
}


/** dst = s */
O_INLINE
m_vec4 vec4_set(float s)
{
    m_vec4 res;
    vecn_set(res.v, s, 4);
    return res;
}

/** dst = unit_x */
O_INLINE
m_vec4 vec4_unit_x(void)
{
    m_vec4 res;
    vecn_unit_x(res.v, 4);
    return res;
}


/** dst = unit_y */
O_INLINE
m_vec4 vec4_unit_y(void)
{
    m_vec4 res;
    vecn_unit_y(res.v, 4);
    return res;
}


/** dst = unit_z */
O_INLINE
m_vec4 vec4_unit_z(void)
{
    m_vec4 res;
    vecn_unit_z(res.v, 4);
    return res;
}


/** dst = unit_w */
O_INLINE
m_vec4 vec4_unit_w(void)
{
    m_vec4 res;
    vecn_unit_w(res.v, 4);
    return res;
}


/** dst = -v */
O_INLINE
m_vec4 vec4_neg(m_vec4 v)
{
    m_vec4 res;
    vecn_neg(res.v, v.v, 4);
    return res;
}


/** dst = a + b */
O_INLINE
m_vec4 vec4_add_v(m_vec4 a, m_vec4 b)
{
    m_vec4 res;
    vecn_add_v(res.v, a.v, b.v, 4);
    return res;
}


/** dst = a + b */
O_INLINE
m_vec4 vec4_add(m_vec4 a, float b)
{
    m_vec4 res;
    vecn_add(res.v, a.v, b, 4);
    return res;
}


/** dst = a - b */
O_INLINE
m_vec4 vec4_sub_v(m_vec4 a, m_vec4 b)
{
    m_vec4 res;
    vecn_sub_v(res.v, a.v, b.v, 4);
    return res;
}


/** dst = a - b */
O_INLINE
m_vec4 vec4_sub(m_vec4 a, float b)
{
    m_vec4 res;
    vecn_sub(res.v, a.v, b, 4);
    return res;
}


/** dst = a * b */
O_INLINE
m_vec4 vec4_scale_v(m_vec4 a, m_vec4 b)
{
    m_vec4 res;
    vecn_scale_v(res.v, a.v, b.v, 4);
    return res;
}


/** dst = a * b */
O_INLINE
m_vec4 vec4_scale(m_vec4 a, float b)
{
    m_vec4 res;
    vecn_scale(res.v, a.v, b, 4);
    return res;
}


/** dst = a / b */
O_INLINE
m_vec4 vec4_div_v(m_vec4 a, m_vec4 b)
{
    m_vec4 res;
    vecn_div_v(res.v, a.v, b.v, 4);
    return res;
}


/** dst = a / b */
O_INLINE
m_vec4 vec4_div(m_vec4 a, float b)
{
    m_vec4 res;
    vecn_div(res.v, a.v, b, 4);
    return res;
}

/** dst = a + b * c */
O_INLINE
m_vec4 vec4_add_scaled_v(m_vec4 a, m_vec4 b, m_vec4 c)
{
    m_vec4 res;
    vecn_add_scaled_v(res.v, a.v, b.v, c.v, 4);
    return res;
}

/** dst = a + b * c */
O_INLINE
m_vec4 vec4_add_scaled(m_vec4 a, m_vec4 b, float c)
{
    m_vec4 res;
    vecn_add_scaled(res.v, a.v, b.v, c, 4);
    return res;
}


/** dst = a * M_PI / 180 */
O_INLINE
m_vec4 vec4_radians(m_vec4 deg)
{
    m_vec4 res;
    vecn_radians(res.v, deg.v, 4);
    return res;
}


/** dst = a * 180 / M_PI */
O_INLINE
m_vec4 vec4_degrees(m_vec4 rad)
{
    m_vec4 res;
    vecn_degrees(res.v, rad.v, 4);
    return res;
}


/** dst = abs(x) */
O_INLINE
m_vec4 vec4_abs(m_vec4 x)
{
    m_vec4 res;
    vecn_abs(res.v, x.v, 4);
    return res;
}


/** dst = x > 0 ? 1 : (x < 0 ? -1 : 0) */
O_INLINE
m_vec4 vec4_sign(m_vec4 x)
{
    m_vec4 res;
    vecn_sign(res.v, x.v, 4);
    return res;
}


/** dst = round(x) */
O_INLINE
m_vec4 vec4_round(m_vec4 x)
{
    m_vec4 res;
    vecn_round(res.v, x.v, 4);
    return res;
}


/** dst = floor(x) */
O_INLINE
m_vec4 vec4_floor(m_vec4 x)
{
    m_vec4 res;
    vecn_floor(res.v, x.v, 4);
    return res;
}


/** dst = ceil(x) */
O_INLINE
m_vec4 vec4_ceil(m_vec4 x)
{
    m_vec4 res;
    vecn_ceil(res.v, x.v, 4);
    return res;
}


/** dst = x - floor(x) */
O_INLINE
m_vec4 vec4_fract(m_vec4 x)
{
    m_vec4 res;
    vecn_fract(res.v, x.v, 4);
    return res;
}


/** dst = x - y * floor(x/y) */
O_INLINE
m_vec4 vec4_mod(m_vec4 x, float y)
{
    m_vec4 res;
    vecn_mod(res.v, x.v, y, 4);
    return res;
}

/** dst = x - y * floor(x/y) */
O_INLINE
m_vec4 vec4_mod_v(m_vec4 x, m_vec4 y)
{
    m_vec4 res;
    vecn_mod_v(res.v, x.v, y.v, 4);
    return res;
}


/** dst = a < b ? a : b */
O_INLINE
m_vec4 vec4_min(m_vec4 x, float y)
{
    m_vec4 res;
    vecn_min(res.v, x.v, y, 4);
    return res;
}


/** dst = a < b ? a : b */
O_INLINE
m_vec4 vec4_min_v(m_vec4 x, m_vec4 y)
{
    m_vec4 res;
    vecn_min_v(res.v, x.v, y.v, 4);
    return res;
}


/** dst = a > b ? a : b */
O_INLINE
m_vec4 vec4_max(m_vec4 x, float y)
{
    m_vec4 res;
    vecn_max(res.v, x.v, y, 4);
    return res;
}


/** dst = a > b ? a : b */
O_INLINE
m_vec4 vec4_max_v(m_vec4 x, m_vec4 y)
{
    m_vec4 res;
    vecn_max_v(res.v, x.v, y.v, 4);
    return res;
}


/** returns the index of the min v value */
O_INLINE
int vec4_min_index(m_vec4 vec)
{
    return vecn_min_index(vec.v, 4);
}


/** returns the index of the max v value */
O_INLINE
int vec4_max_index(m_vec4 vec)
{
    return vecn_max_index(vec.v, 4);
}

/** dst = x < min ? min : (x > max ? max : x) */
O_INLINE
m_vec4 vec4_clamp(m_vec4 x, float min, float max)
{
    m_vec4 res;
    vecn_clamp(res.v, x.v, min, max, 4);
    return res;
}


/** dst = x < min ? min : (x > max ? max : x) */
O_INLINE
m_vec4 vec4_clamp_v(m_vec4 x, m_vec4 min, m_vec4 max)
{
    m_vec4 res;
    vecn_clamp_v(res.v, x.v, min.v, max.v, 4);
    return res;
}


/** dst = a * (1-t) + b * t */
O_INLINE
m_vec4 vec4_mix(m_vec4 a, m_vec4 b, float t)
{
    m_vec4 res;
    vecn_mix(res.v, a.v, b.v, t, 4);
    return res;
}

/** returns v[0] + v[1] + ... + v[n-1] */
O_INLINE
float vec4_sum(m_vec4 v)
{
    return vecn_sum(v.v, 4);
}


/** returns a dot b */
O_INLINE
float vec4_dot(m_vec4 a, m_vec4 b)
{
    return vecn_dot(a.v, b.v, 4);
}


/** dst = a x b , dst.w... = 0 */
O_INLINE
m_vec4 vec4_cross(m_vec4 a, m_vec4 b)
{
    m_vec4 res;
    vecn_cross(res.v, a.v, b.v, 4);
    return res;
}


/** returns ||v||_4 */
O_INLINE
float vec4_norm(m_vec4 v)
{
    return vecn_norm(v.v, 4);
}


/** returns ||v||_p */
O_INLINE
float vec4_norm_p(m_vec4 v, float p)
{
    return vecn_norm_p(v.v, p, 4);
}


/** returns ||v||_1 */
O_INLINE
float vec4_norm_1(m_vec4 v)
{
    return vecn_norm_1(v.v, 4);
}


/** returns ||v||_inf */
O_INLINE
float vec4_norm_inf(m_vec4 v)
{
    return vecn_norm_inf(v.v, 4);
}


/** dst = v / norm(v) */
O_INLINE
m_vec4 vec4_normalize_unsafe(m_vec4 v)
{
    m_vec4 res;
    vecn_normalize_unsafe(res.v, v.v, 4);
    return res;
}


/** dst = v / (norm(v) > 0 ? norm(v) : 1) */
O_INLINE
m_vec4 vec4_normalize(m_vec4 v)
{
    m_vec4 res;
    vecn_normalize(res.v, v.v, 4);
    return res;
}

/** dst = normalize(cross(a, b)) */
O_INLINE
m_vec4 vec4_cross_normalized(m_vec4 a, m_vec4 b)
{
    m_vec4 res;
    vecn_cross_normalized(res.v, a.v, b.v, 4);
    return res;
}


/** returns length of a vector, see vecn_norm. Just here to match glsl */
O_INLINE
float vec4_length(m_vec4 v)
{
    return vecn_length(v.v, 4);
}


/** returns norm(b-a) */
O_INLINE
float vec4_distance(m_vec4 a, m_vec4 b)
{
    return vecn_distance(a.v, b.v, 4);
}


/** returns dot(b-a) */
O_INLINE
float vec4_sqr_distance(m_vec4 a, m_vec4 b)
{
    return vecn_sqr_distance(a.v, b.v, 4);
}


///** dst = dot(i, nref) < 0 ? N : -N */
//O_INLINE
//m_vec4 vec4_faceforward(m_vec4 n, m_vec4 i, m_vec4 nref)
//{
//    m_vec4 res;
//    vecn_faceforward(res.v, n.v, i.v, nref.v, 4);
//    return res;
//}
//
//
///** dst = i - 2.0 * n * dot(n,i) */
//O_INLINE
//m_vec4 vec4_reflect(m_vec4 i, m_vec4 n)
//{
//    m_vec4 res;
//    vecn_reflect(res.v, i.v, n.v, 4);
//    return res;
//}
//
//
//O_INLINE
//m_vec4 vec4_refract(m_vec4 i, m_vec4 n, float eta)
//{
//    m_vec4 res;
//    vecn_refract(res.v, i.v, n.v, eta, 4);
//    return res;
//}

#endif //M_VEC_FLT_VEC4_H
