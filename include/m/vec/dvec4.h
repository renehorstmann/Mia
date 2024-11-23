#ifndef M_VEC_DBL_VEC4_H
#define M_VEC_DBL_VEC4_H

/**
 * @file vec/dvec4.h
 *
 * functions working with dvec4
 */


#include "dvecn.h"
#include "m/m_types/dbl.h"

/** dst = v0, v1, ... */
O_INLINE
m_dvec4 dvec4_new(double v0, double v1, double v2, double v3)
{
    m_dvec4 self;
    self.v0 = v0;
    self.v1 = v1;
    self.v2 = v2;
    self.v3 = v3;
    return self;
}

/** dst = (m_dvec4) v */
O_INLINE
m_dvec4 dvec4_cast_float(const float *cast)
{
    m_dvec4 res;
    dvecn_cast_into(res.v, cast, 4);
    return res;
}

///** dst = (m_dvec4) v */
//O_INLINE
//m_dvec4 dvec4_cast_double(const double *cast)
//{
//    m_dvec4 res;
//    dvecn_cast_into(res.v, cast, 4);
//    return res;
//}

/** dst = (m_dvec4) v */
O_INLINE
m_dvec4 dvec4_cast_int(const int *cast)
{
    m_dvec4 res;
    dvecn_cast_into(res.v, cast, 4);
    return res;
}

/** dst = (m_dvec4) v */
O_INLINE
m_dvec4 dvec4_cast_byte(const obyte *cast)
{
    m_dvec4 res;
    dvecn_cast_into(res.v, cast, 4);
    return res;
}

/** dst = v / 255 */
O_INLINE
m_dvec4 dvec4_cast_byte_1(const obyte *cast)
{
    m_dvec4 res;
    dvecn_cast_byte_1(res.v, cast, 4);
    return res;
}



/** a == b */
O_INLINE
bool dvec4_equals(m_dvec4 a, double b)
{
    return dvecn_equals(a.v, b, 4);
}

/** a == b */
O_INLINE
bool dvec4_equals_v(m_dvec4 a, m_dvec4 b)
{
    return dvecn_equals_v(a.v, b.v, 4);
}

/** a ~= b */
O_INLINE
bool dvec4_equals_eps(m_dvec4 a, double b, double eps)
{
    return dvecn_equals_eps(a.v, b, eps, 4);
}

/** a ~= b */
O_INLINE
bool dvec4_equals_eps_v(m_dvec4 a, m_dvec4 b, double eps)
{
    return dvecn_equals_eps_v(a.v, b.v, eps, 4);
}


/** dst = s */
O_INLINE
m_dvec4 dvec4_set(double s)
{
    m_dvec4 res;
    dvecn_set(res.v, s, 4);
    return res;
}

/** dst = unit_x */
O_INLINE
m_dvec4 dvec4_unit_x(void)
{
    m_dvec4 res;
    dvecn_unit_x(res.v, 4);
    return res;
}


/** dst = unit_y */
O_INLINE
m_dvec4 dvec4_unit_y(void)
{
    m_dvec4 res;
    dvecn_unit_y(res.v, 4);
    return res;
}


/** dst = unit_z */
O_INLINE
m_dvec4 dvec4_unit_z(void)
{
    m_dvec4 res;
    dvecn_unit_z(res.v, 4);
    return res;
}


/** dst = unit_w */
O_INLINE
m_dvec4 dvec4_unit_w(void)
{
    m_dvec4 res;
    dvecn_unit_w(res.v, 4);
    return res;
}


/** dst = -v */
O_INLINE
m_dvec4 dvec4_neg(m_dvec4 v)
{
    m_dvec4 res;
    dvecn_neg(res.v, v.v, 4);
    return res;
}


/** dst = a + b */
O_INLINE
m_dvec4 dvec4_add_v(m_dvec4 a, m_dvec4 b)
{
    m_dvec4 res;
    dvecn_add_v(res.v, a.v, b.v, 4);
    return res;
}


/** dst = a + b */
O_INLINE
m_dvec4 dvec4_add(m_dvec4 a, double b)
{
    m_dvec4 res;
    dvecn_add(res.v, a.v, b, 4);
    return res;
}


/** dst = a - b */
O_INLINE
m_dvec4 dvec4_sub_v(m_dvec4 a, m_dvec4 b)
{
    m_dvec4 res;
    dvecn_sub_v(res.v, a.v, b.v, 4);
    return res;
}


/** dst = a - b */
O_INLINE
m_dvec4 dvec4_sub(m_dvec4 a, double b)
{
    m_dvec4 res;
    dvecn_sub(res.v, a.v, b, 4);
    return res;
}


/** dst = a * b */
O_INLINE
m_dvec4 dvec4_scale_v(m_dvec4 a, m_dvec4 b)
{
    m_dvec4 res;
    dvecn_scale_v(res.v, a.v, b.v, 4);
    return res;
}


/** dst = a * b */
O_INLINE
m_dvec4 dvec4_scale(m_dvec4 a, double b)
{
    m_dvec4 res;
    dvecn_scale(res.v, a.v, b, 4);
    return res;
}


/** dst = a / b */
O_INLINE
m_dvec4 dvec4_div_v(m_dvec4 a, m_dvec4 b)
{
    m_dvec4 res;
    dvecn_div_v(res.v, a.v, b.v, 4);
    return res;
}


/** dst = a / b */
O_INLINE
m_dvec4 dvec4_div(m_dvec4 a, double b)
{
    m_dvec4 res;
    dvecn_div(res.v, a.v, b, 4);
    return res;
}

/** dst = a + b * c */
O_INLINE
m_dvec4 dvec4_add_scaled_v(m_dvec4 a, m_dvec4 b, m_dvec4 c)
{
    m_dvec4 res;
    dvecn_add_scaled_v(res.v, a.v, b.v, c.v, 4);
    return res;
}

/** dst = a + b * c */
O_INLINE
m_dvec4 dvec4_add_scaled(m_dvec4 a, m_dvec4 b, double c)
{
    m_dvec4 res;
    dvecn_add_scaled(res.v, a.v, b.v, c, 4);
    return res;
}


/** dst = a * M_PI / 180 */
O_INLINE
m_dvec4 dvec4_radians(m_dvec4 deg)
{
    m_dvec4 res;
    dvecn_radians(res.v, deg.v, 4);
    return res;
}


/** dst = a * 180 / M_PI */
O_INLINE
m_dvec4 dvec4_degrees(m_dvec4 rad)
{
    m_dvec4 res;
    dvecn_degrees(res.v, rad.v, 4);
    return res;
}


/** dst = abs(x) */
O_INLINE
m_dvec4 dvec4_abs(m_dvec4 x)
{
    m_dvec4 res;
    dvecn_abs(res.v, x.v, 4);
    return res;
}


/** dst = x > 0 ? 1 : (x < 0 ? -1 : 0) */
O_INLINE
m_dvec4 dvec4_sign(m_dvec4 x)
{
    m_dvec4 res;
    dvecn_sign(res.v, x.v, 4);
    return res;
}


/** dst = round(x) */
O_INLINE
m_dvec4 dvec4_round(m_dvec4 x)
{
    m_dvec4 res;
    dvecn_round(res.v, x.v, 4);
    return res;
}


/** dst = floor(x) */
O_INLINE
m_dvec4 dvec4_floor(m_dvec4 x)
{
    m_dvec4 res;
    dvecn_floor(res.v, x.v, 4);
    return res;
}


/** dst = ceil(x) */
O_INLINE
m_dvec4 dvec4_ceil(m_dvec4 x)
{
    m_dvec4 res;
    dvecn_ceil(res.v, x.v, 4);
    return res;
}


/** dst = x - floor(x) */
O_INLINE
m_dvec4 dvec4_fract(m_dvec4 x)
{
    m_dvec4 res;
    dvecn_fract(res.v, x.v, 4);
    return res;
}


/** dst = x - y * floor(x/y) */
O_INLINE
m_dvec4 dvec4_mod(m_dvec4 x, double y)
{
    m_dvec4 res;
    dvecn_mod(res.v, x.v, y, 4);
    return res;
}

/** dst = x - y * floor(x/y) */
O_INLINE
m_dvec4 dvec4_mod_v(m_dvec4 x, m_dvec4 y)
{
    m_dvec4 res;
    dvecn_mod_v(res.v, x.v, y.v, 4);
    return res;
}


/** dst = a < b ? a : b */
O_INLINE
m_dvec4 dvec4_min(m_dvec4 x, double y)
{
    m_dvec4 res;
    dvecn_min(res.v, x.v, y, 4);
    return res;
}


/** dst = a < b ? a : b */
O_INLINE
m_dvec4 dvec4_min_v(m_dvec4 x, m_dvec4 y)
{
    m_dvec4 res;
    dvecn_min_v(res.v, x.v, y.v, 4);
    return res;
}


/** dst = a > b ? a : b */
O_INLINE
m_dvec4 dvec4_max(m_dvec4 x, double y)
{
    m_dvec4 res;
    dvecn_max(res.v, x.v, y, 4);
    return res;
}


/** dst = a > b ? a : b */
O_INLINE
m_dvec4 dvec4_max_v(m_dvec4 x, m_dvec4 y)
{
    m_dvec4 res;
    dvecn_max_v(res.v, x.v, y.v, 4);
    return res;
}


/** returns the index of the min v value */
O_INLINE
int dvec4_min_index(m_dvec4 dvec)
{
    return dvecn_min_index(dvec.v, 4);
}


/** returns the index of the max v value */
O_INLINE
int dvec4_max_index(m_dvec4 dvec)
{
    return dvecn_max_index(dvec.v, 4);
}

/** dst = x < min ? min : (x > max ? max : x) */
O_INLINE
m_dvec4 dvec4_clamp(m_dvec4 x, double min, double max)
{
    m_dvec4 res;
    dvecn_clamp(res.v, x.v, min, max, 4);
    return res;
}


/** dst = x < min ? min : (x > max ? max : x) */
O_INLINE
m_dvec4 dvec4_clamp_v(m_dvec4 x, m_dvec4 min, m_dvec4 max)
{
    m_dvec4 res;
    dvecn_clamp_v(res.v, x.v, min.v, max.v, 4);
    return res;
}


/** dst = a * (1-t) + b * t */
O_INLINE
m_dvec4 dvec4_mix(m_dvec4 a, m_dvec4 b, double t)
{
    m_dvec4 res;
    dvecn_mix(res.v, a.v, b.v, t, 4);
    return res;
}

/** returns v[0] + v[1] + ... + v[n-1] */
O_INLINE
double dvec4_sum(m_dvec4 v)
{
    return dvecn_sum(v.v, 4);
}


/** returns a dot b */
O_INLINE
double dvec4_dot(m_dvec4 a, m_dvec4 b)
{
    return dvecn_dot(a.v, b.v, 4);
}


/** dst = a x b , dst.w... = 0 */
O_INLINE
m_dvec4 dvec4_cross(m_dvec4 a, m_dvec4 b)
{
    m_dvec4 res;
    dvecn_cross(res.v, a.v, b.v, 4);
    return res;
}


/** returns ||v||_4 */
O_INLINE
double dvec4_norm(m_dvec4 v)
{
    return dvecn_norm(v.v, 4);
}


/** returns ||v||_p */
O_INLINE
double dvec4_norm_p(m_dvec4 v, double p)
{
    return dvecn_norm_p(v.v, p, 4);
}


/** returns ||v||_1 */
O_INLINE
double dvec4_norm_1(m_dvec4 v)
{
    return dvecn_norm_1(v.v, 4);
}


/** returns ||v||_inf */
O_INLINE
double dvec4_norm_inf(m_dvec4 v)
{
    return dvecn_norm_inf(v.v, 4);
}


/** dst = v / norm(v) */
O_INLINE
m_dvec4 dvec4_normalize_unsafe(m_dvec4 v)
{
    m_dvec4 res;
    dvecn_normalize_unsafe(res.v, v.v, 4);
    return res;
}


/** dst = v / (norm(v) > 0 ? norm(v) : 1) */
O_INLINE
m_dvec4 dvec4_normalize(m_dvec4 v)
{
    m_dvec4 res;
    dvecn_normalize(res.v, v.v, 4);
    return res;
}

/** dst = normalize(cross(a, b)) */
O_INLINE
m_dvec4 dvec4_cross_normalized(m_dvec4 a, m_dvec4 b)
{
    m_dvec4 res;
    dvecn_cross_normalized(res.v, a.v, b.v, 4);
    return res;
}


/** returns length of a dvector, see dvecn_norm. Just here to match glsl */
O_INLINE
double dvec4_length(m_dvec4 v)
{
    return dvecn_length(v.v, 4);
}


/** returns norm(b-a) */
O_INLINE
double dvec4_distance(m_dvec4 a, m_dvec4 b)
{
    return dvecn_distance(a.v, b.v, 4);
}


/** returns dot(b-a) */
O_INLINE
double dvec4_sqr_distance(m_dvec4 a, m_dvec4 b)
{
    return dvecn_sqr_distance(a.v, b.v, 4);
}


///** dst = dot(i, nref) < 0 ? N : -N */
//O_INLINE
//m_dvec4 dvec4_faceforward(m_dvec4 n, m_dvec4 i, m_dvec4 nref)
//{
//    m_dvec4 res;
//    dvecn_faceforward(res.v, n.v, i.v, nref.v, 4);
//    return res;
//}
//
//
///** dst = i - 2.0 * n * dot(n,i) */
//O_INLINE
//m_dvec4 dvec4_reflect(m_dvec4 i, m_dvec4 n)
//{
//    m_dvec4 res;
//    dvecn_reflect(res.v, i.v, n.v, 4);
//    return res;
//}
//
//
//O_INLINE
//m_dvec4 dvec4_refract(m_dvec4 i, m_dvec4 n, double eta)
//{
//    m_dvec4 res;
//    dvecn_refract(res.v, i.v, n.v, eta, 4);
//    return res;
//}

#endif //M_VEC_DBL_VEC4_H
