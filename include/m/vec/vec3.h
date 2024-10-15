#ifndef M_VEC_FLT_VEC3_H
#define M_VEC_FLT_VEC3_H

/**
 * @file vec/vec3.h
 *
 * functions working with vec3
 */



#include "vecn.h"
#include "m/m_types/flt.h"

/** dst = v0, v1, ... */
O_INLINE
m_vec3 vec3_new(float v0, float v1, float v2)
{
    m_vec3 self;
    self.v0 = v0;
    self.v1 = v1;
    self.v2 = v2;
    return self;
}

///** dst = (m_vec3) v */
//O_INLINE
//m_vec3 vec3_cast_float(const float *cast)
//{
//    m_vec3 res;
//    vecn_cast_into(res.v, cast, 3);
//    return res;
//}

/** dst = (m_vec3) v */
O_INLINE
m_vec3 vec3_cast_double(const double *cast)
{
    m_vec3 res;
    vecn_cast_into(res.v, cast, 3);
    return res;
}

/** dst = (m_vec3) v */
O_INLINE
m_vec3 vec3_cast_int(const int *cast)
{
    m_vec3 res;
    vecn_cast_into(res.v, cast, 3);
    return res;
}

/** dst = (m_vec3) v */
O_INLINE
m_vec3 vec3_cast_byte(const obyte *cast)
{
    m_vec3 res;
    vecn_cast_into(res.v, cast, 3);
    return res;
}

/** dst = v / 255 */
O_INLINE
m_vec3 vec3_cast_byte_1(const obyte *cast)
{
    m_vec3 res;
    vecn_cast_byte_1(res.v, cast, 3);
    return res;
}



/** a == b */
O_INLINE
bool vec3_equals(m_vec3 a, float b)
{
    return vecn_equals(a.v, b, 3);
}

/** a == b */
O_INLINE
bool vec3_equals_v(m_vec3 a, m_vec3 b)
{
    return vecn_equals_v(a.v, b.v, 3);
}

/** a ~= b */
O_INLINE
bool vec3_equals_eps(m_vec3 a, float b, float eps)
{
    return vecn_equals_eps(a.v, b, eps, 3);
}

/** a ~= b */
O_INLINE
bool vec3_equals_eps_v(m_vec3 a, m_vec3 b, float eps)
{
    return vecn_equals_eps_v(a.v, b.v, eps, 3);
}


/** dst = s */
O_INLINE
m_vec3 vec3_set(float s)
{
    m_vec3 res;
    vecn_set(res.v, s, 3);
    return res;
}

/** dst = unit_x */
O_INLINE
m_vec3 vec3_unit_x(void)
{
    m_vec3 res;
    vecn_unit_x(res.v, 3);
    return res;
}


/** dst = unit_y */
O_INLINE
m_vec3 vec3_unit_y(void)
{
    m_vec3 res;
    vecn_unit_y(res.v, 3);
    return res;
}


/** dst = unit_z */
O_INLINE
m_vec3 vec3_unit_z(void)
{
    m_vec3 res;
    vecn_unit_z(res.v, 3);
    return res;
}


/** dst = unit_w */
O_INLINE
m_vec3 vec3_unit_w(void)
{
    m_vec3 res;
    vecn_unit_w(res.v, 3);
    return res;
}


/** dst = -v */
O_INLINE
m_vec3 vec3_neg(m_vec3 v)
{
    m_vec3 res;
    vecn_neg(res.v, v.v, 3);
    return res;
}


/** dst = a + b */
O_INLINE
m_vec3 vec3_add_v(m_vec3 a, m_vec3 b)
{
    m_vec3 res;
    vecn_add_v(res.v, a.v, b.v, 3);
    return res;
}


/** dst = a + b */
O_INLINE
m_vec3 vec3_add(m_vec3 a, float b)
{
    m_vec3 res;
    vecn_add(res.v, a.v, b, 3);
    return res;
}


/** dst = a - b */
O_INLINE
m_vec3 vec3_sub_v(m_vec3 a, m_vec3 b)
{
    m_vec3 res;
    vecn_sub_v(res.v, a.v, b.v, 3);
    return res;
}


/** dst = a - b */
O_INLINE
m_vec3 vec3_sub(m_vec3 a, float b)
{
    m_vec3 res;
    vecn_sub(res.v, a.v, b, 3);
    return res;
}


/** dst = a * b */
O_INLINE
m_vec3 vec3_scale_v(m_vec3 a, m_vec3 b)
{
    m_vec3 res;
    vecn_scale_v(res.v, a.v, b.v, 3);
    return res;
}


/** dst = a * b */
O_INLINE
m_vec3 vec3_scale(m_vec3 a, float b)
{
    m_vec3 res;
    vecn_scale(res.v, a.v, b, 3);
    return res;
}


/** dst = a / b */
O_INLINE
m_vec3 vec3_div_v(m_vec3 a, m_vec3 b)
{
    m_vec3 res;
    vecn_div_v(res.v, a.v, b.v, 3);
    return res;
}


/** dst = a / b */
O_INLINE
m_vec3 vec3_div(m_vec3 a, float b)
{
    m_vec3 res;
    vecn_div(res.v, a.v, b, 3);
    return res;
}

/** dst = a + b * c */
O_INLINE
m_vec3 vec3_add_scaled_v(m_vec3 a, m_vec3 b, m_vec3 c)
{
    m_vec3 res;
    vecn_add_scaled_v(res.v, a.v, b.v, c.v, 3);
    return res;
}

/** dst = a + b * c */
O_INLINE
m_vec3 vec3_add_scaled(m_vec3 a, m_vec3 b, float c)
{
    m_vec3 res;
    vecn_add_scaled(res.v, a.v, b.v, c, 3);
    return res;
}


/** dst = a * M_PI / 180 */
O_INLINE
m_vec3 vec3_radians(m_vec3 deg)
{
    m_vec3 res;
    vecn_radians(res.v, deg.v, 3);
    return res;
}


/** dst = a * 180 / M_PI */
O_INLINE
m_vec3 vec3_degrees(m_vec3 rad)
{
    m_vec3 res;
    vecn_degrees(res.v, rad.v, 3);
    return res;
}


/** dst = abs(x) */
O_INLINE
m_vec3 vec3_abs(m_vec3 x)
{
    m_vec3 res;
    vecn_abs(res.v, x.v, 3);
    return res;
}


/** dst = x > 0 ? 1 : (x < 0 ? -1 : 0) */
O_INLINE
m_vec3 vec3_sign(m_vec3 x)
{
    m_vec3 res;
    vecn_sign(res.v, x.v, 3);
    return res;
}


/** dst = round(x) */
O_INLINE
m_vec3 vec3_round(m_vec3 x)
{
    m_vec3 res;
    vecn_round(res.v, x.v, 3);
    return res;
}


/** dst = floor(x) */
O_INLINE
m_vec3 vec3_floor(m_vec3 x)
{
    m_vec3 res;
    vecn_floor(res.v, x.v, 3);
    return res;
}


/** dst = ceil(x) */
O_INLINE
m_vec3 vec3_ceil(m_vec3 x)
{
    m_vec3 res;
    vecn_ceil(res.v, x.v, 3);
    return res;
}


/** dst = x - floor(x) */
O_INLINE
m_vec3 vec3_fract(m_vec3 x)
{
    m_vec3 res;
    vecn_fract(res.v, x.v, 3);
    return res;
}


/** dst = x - y * floor(x/y) */
O_INLINE
m_vec3 vec3_mod(m_vec3 x, float y)
{
    m_vec3 res;
    vecn_mod(res.v, x.v, y, 3);
    return res;
}

/** dst = x - y * floor(x/y) */
O_INLINE
m_vec3 vec3_mod_v(m_vec3 x, m_vec3 y)
{
    m_vec3 res;
    vecn_mod_v(res.v, x.v, y.v, 3);
    return res;
}


/** dst = a < b ? a : b */
O_INLINE
m_vec3 vec3_min(m_vec3 x, float y)
{
    m_vec3 res;
    vecn_min(res.v, x.v, y, 3);
    return res;
}


/** dst = a < b ? a : b */
O_INLINE
m_vec3 vec3_min_v(m_vec3 x, m_vec3 y)
{
    m_vec3 res;
    vecn_min_v(res.v, x.v, y.v, 3);
    return res;
}


/** dst = a > b ? a : b */
O_INLINE
m_vec3 vec3_max(m_vec3 x, float y)
{
    m_vec3 res;
    vecn_max(res.v, x.v, y, 3);
    return res;
}


/** dst = a > b ? a : b */
O_INLINE
m_vec3 vec3_max_v(m_vec3 x, m_vec3 y)
{
    m_vec3 res;
    vecn_max_v(res.v, x.v, y.v, 3);
    return res;
}


/** returns the index of the min v value */
O_INLINE
int vec3_min_index(m_vec3 vec)
{
    return vecn_min_index(vec.v, 3);
}


/** returns the index of the max v value */
O_INLINE
int vec3_max_index(m_vec3 vec)
{
    return vecn_max_index(vec.v, 3);
}

/** dst = x < min ? min : (x > max ? max : x) */
O_INLINE
m_vec3 vec3_clamp(m_vec3 x, float min, float max)
{
    m_vec3 res;
    vecn_clamp(res.v, x.v, min, max, 3);
    return res;
}


/** dst = x < min ? min : (x > max ? max : x) */
O_INLINE
m_vec3 vec3_clamp_v(m_vec3 x, m_vec3 min, m_vec3 max)
{
    m_vec3 res;
    vecn_clamp_v(res.v, x.v, min.v, max.v, 3);
    return res;
}


/** dst = a * (1-t) + b * t */
O_INLINE
m_vec3 vec3_mix(m_vec3 a, m_vec3 b, float t)
{
    m_vec3 res;
    vecn_mix(res.v, a.v, b.v, t, 3);
    return res;
}

/** returns v[0] + v[1] + ... + v[n-1] */
O_INLINE
float vec3_sum(m_vec3 v)
{
    return vecn_sum(v.v, 3);
}


/** returns a dot b */
O_INLINE
float vec3_dot(m_vec3 a, m_vec3 b)
{
    return vecn_dot(a.v, b.v, 3);
}


/** dst = a x b , dst.w... = 0 */
O_INLINE
m_vec3 vec3_cross(m_vec3 a, m_vec3 b)
{
    m_vec3 res;
    vecn_cross(res.v, a.v, b.v, 3);
    return res;
}


/** returns ||v||_3 */
O_INLINE
float vec3_norm(m_vec3 v)
{
    return vecn_norm(v.v, 3);
}


/** returns ||v||_p */
O_INLINE
float vec3_norm_p(m_vec3 v, float p)
{
    return vecn_norm_p(v.v, p, 3);
}


/** returns ||v||_1 */
O_INLINE
float vec3_norm_1(m_vec3 v)
{
    return vecn_norm_1(v.v, 3);
}


/** returns ||v||_inf */
O_INLINE
float vec3_norm_inf(m_vec3 v)
{
    return vecn_norm_inf(v.v, 3);
}


/** dst = v / norm(v) */
O_INLINE
m_vec3 vec3_normalize_unsafe(m_vec3 v)
{
    m_vec3 res;
    vecn_normalize_unsafe(res.v, v.v, 3);
    return res;
}


/** dst = v / (norm(v) > 0 ? norm(v) : 1) */
O_INLINE
m_vec3 vec3_normalize(m_vec3 v)
{
    m_vec3 res;
    vecn_normalize(res.v, v.v, 3);
    return res;
}

/** dst = normalize(cross(a, b)) */
O_INLINE
m_vec3 vec3_cross_normalized(m_vec3 a, m_vec3 b)
{
    m_vec3 res;
    vecn_cross_normalized(res.v, a.v, b.v, 3);
    return res;
}


/** returns length of a vector, see vecn_norm. Just here to match glsl */
O_INLINE
float vec3_length(m_vec3 v)
{
    return vecn_length(v.v, 3);
}


/** returns norm(b-a) */
O_INLINE
float vec3_distance(m_vec3 a, m_vec3 b)
{
    return vecn_distance(a.v, b.v, 3);
}


/** returns dot(b-a) */
O_INLINE
float vec3_sqr_distance(m_vec3 a, m_vec3 b)
{
    return vecn_sqr_distance(a.v, b.v, 3);
}


/** dst = dot(i, nref) < 0 ? N : -N */
O_INLINE
m_vec3 vec3_faceforward(m_vec3 n, m_vec3 i, m_vec3 nref)
{
    m_vec3 res;
    vecn_faceforward(res.v, n.v, i.v, nref.v, 3);
    return res;
}


/** dst = i - 2.0 * n * dot(n,i) */
O_INLINE
m_vec3 vec3_reflect(m_vec3 i, m_vec3 n)
{
    m_vec3 res;
    vecn_reflect(res.v, i.v, n.v, 3);
    return res;
}


O_INLINE
m_vec3 vec3_refract(m_vec3 i, m_vec3 n, float eta)
{
    m_vec3 res;
    vecn_refract(res.v, i.v, n.v, eta, 3);
    return res;
}

#endif //M_VEC_FLT_VEC3_H
