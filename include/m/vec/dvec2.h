#ifndef M_VEC_DBL_VEC2_H
#define M_VEC_DBL_VEC2_H

/**
 * @file vec/dvec2.h
 *
 * functions working with dvec2
 */



#include "dvecn.h"
#include "m/m_types/dbl.h"

/** dst = v0, v1, ... */
O_INLINE
m_dvec2 dvec2_new(double v0, double v1)
{
    m_dvec2 self;
    self.v0 = v0;
    self.v1 = v1;
    return self;
}

/** dst = (m_dvec2) v */
O_INLINE
m_dvec2 dvec2_cast_float(const float *cast)
{
    m_dvec2 res;
    dvecn_cast_into(res.v, cast, 2);
    return res;
}

///** dst = (m_dvec2) v */
//O_INLINE
//m_dvec2 dvec2_cast_double(const double *cast)
//{
//    m_dvec2 res;
//    dvecn_cast_into(res.v, cast, 2);
//    return res;
//}

/** dst = (m_dvec2) v */
O_INLINE
m_dvec2 dvec2_cast_int(const int *cast)
{
    m_dvec2 res;
    dvecn_cast_into(res.v, cast, 2);
    return res;
}

/** dst = (m_dvec2) v */
O_INLINE
m_dvec2 dvec2_cast_byte(const obyte *cast)
{
    m_dvec2 res;
    dvecn_cast_into(res.v, cast, 2);
    return res;
}

/** dst = v / 255 */
O_INLINE
m_dvec2 dvec2_cast_byte_1(const obyte *cast)
{
    m_dvec2 res;
    dvecn_cast_byte_1(res.v, cast, 2);
    return res;
}



/** a == b */
O_INLINE
bool dvec2_equals(m_dvec2 a, double b)
{
    return dvecn_equals(a.v, b, 2);
}

/** a == b */
O_INLINE
bool dvec2_equals_v(m_dvec2 a, m_dvec2 b)
{
    return dvecn_equals_v(a.v, b.v, 2);
}

/** a ~= b */
O_INLINE
bool dvec2_equals_eps(m_dvec2 a, double b, double eps)
{
    return dvecn_equals_eps(a.v, b, eps, 2);
}

/** a ~= b */
O_INLINE
bool dvec2_equals_eps_v(m_dvec2 a, m_dvec2 b, double eps)
{
    return dvecn_equals_eps_v(a.v, b.v, eps, 2);
}


/** dst = s */
O_INLINE
m_dvec2 dvec2_set(double s)
{
    m_dvec2 res;
    dvecn_set(res.v, s, 2);
    return res;
}

/** dst = unit_x */
O_INLINE
m_dvec2 dvec2_unit_x(void)
{
    m_dvec2 res;
    dvecn_unit_x(res.v, 2);
    return res;
}


/** dst = unit_y */
O_INLINE
m_dvec2 dvec2_unit_y(void)
{
    m_dvec2 res;
    dvecn_unit_y(res.v, 2);
    return res;
}


/** dst = unit_z */
O_INLINE
m_dvec2 dvec2_unit_z(void)
{
    m_dvec2 res;
    dvecn_unit_z(res.v, 2);
    return res;
}


/** dst = unit_w */
O_INLINE
m_dvec2 dvec2_unit_w(void)
{
    m_dvec2 res;
    dvecn_unit_w(res.v, 2);
    return res;
}


/** dst = -v */
O_INLINE
m_dvec2 dvec2_neg(m_dvec2 v)
{
    m_dvec2 res;
    dvecn_neg(res.v, v.v, 2);
    return res;
}


/** dst = a + b */
O_INLINE
m_dvec2 dvec2_add_v(m_dvec2 a, m_dvec2 b)
{
    m_dvec2 res;
    dvecn_add_v(res.v, a.v, b.v, 2);
    return res;
}


/** dst = a + b */
O_INLINE
m_dvec2 dvec2_add(m_dvec2 a, double b)
{
    m_dvec2 res;
    dvecn_add(res.v, a.v, b, 2);
    return res;
}


/** dst = a - b */
O_INLINE
m_dvec2 dvec2_sub_v(m_dvec2 a, m_dvec2 b)
{
    m_dvec2 res;
    dvecn_sub_v(res.v, a.v, b.v, 2);
    return res;
}


/** dst = a - b */
O_INLINE
m_dvec2 dvec2_sub(m_dvec2 a, double b)
{
    m_dvec2 res;
    dvecn_sub(res.v, a.v, b, 2);
    return res;
}


/** dst = a * b */
O_INLINE
m_dvec2 dvec2_scale_v(m_dvec2 a, m_dvec2 b)
{
    m_dvec2 res;
    dvecn_scale_v(res.v, a.v, b.v, 2);
    return res;
}


/** dst = a * b */
O_INLINE
m_dvec2 dvec2_scale(m_dvec2 a, double b)
{
    m_dvec2 res;
    dvecn_scale(res.v, a.v, b, 2);
    return res;
}


/** dst = a / b */
O_INLINE
m_dvec2 dvec2_div_v(m_dvec2 a, m_dvec2 b)
{
    m_dvec2 res;
    dvecn_div_v(res.v, a.v, b.v, 2);
    return res;
}


/** dst = a / b */
O_INLINE
m_dvec2 dvec2_div(m_dvec2 a, double b)
{
    m_dvec2 res;
    dvecn_div(res.v, a.v, b, 2);
    return res;
}

/** dst = a + b * c */
O_INLINE
m_dvec2 dvec2_add_scaled_v(m_dvec2 a, m_dvec2 b, m_dvec2 c)
{
    m_dvec2 res;
    dvecn_add_scaled_v(res.v, a.v, b.v, c.v, 2);
    return res;
}

/** dst = a + b * c */
O_INLINE
m_dvec2 dvec2_add_scaled(m_dvec2 a, m_dvec2 b, double c)
{
    m_dvec2 res;
    dvecn_add_scaled(res.v, a.v, b.v, c, 2);
    return res;
}


/** dst = a * M_PI / 180 */
O_INLINE
m_dvec2 dvec2_radians(m_dvec2 deg)
{
    m_dvec2 res;
    dvecn_radians(res.v, deg.v, 2);
    return res;
}


/** dst = a * 180 / M_PI */
O_INLINE
m_dvec2 dvec2_degrees(m_dvec2 rad)
{
    m_dvec2 res;
    dvecn_degrees(res.v, rad.v, 2);
    return res;
}


/** dst = abs(x) */
O_INLINE
m_dvec2 dvec2_abs(m_dvec2 x)
{
    m_dvec2 res;
    dvecn_abs(res.v, x.v, 2);
    return res;
}


/** dst = x > 0 ? 1 : (x < 0 ? -1 : 0) */
O_INLINE
m_dvec2 dvec2_sign(m_dvec2 x)
{
    m_dvec2 res;
    dvecn_sign(res.v, x.v, 2);
    return res;
}


/** dst = round(x) */
O_INLINE
m_dvec2 dvec2_round(m_dvec2 x)
{
    m_dvec2 res;
    dvecn_round(res.v, x.v, 2);
    return res;
}


/** dst = floor(x) */
O_INLINE
m_dvec2 dvec2_floor(m_dvec2 x)
{
    m_dvec2 res;
    dvecn_floor(res.v, x.v, 2);
    return res;
}


/** dst = ceil(x) */
O_INLINE
m_dvec2 dvec2_ceil(m_dvec2 x)
{
    m_dvec2 res;
    dvecn_ceil(res.v, x.v, 2);
    return res;
}


/** dst = x - floor(x) */
O_INLINE
m_dvec2 dvec2_fract(m_dvec2 x)
{
    m_dvec2 res;
    dvecn_fract(res.v, x.v, 2);
    return res;
}


/** dst = x - y * floor(x/y) */
O_INLINE
m_dvec2 dvec2_mod(m_dvec2 x, double y)
{
    m_dvec2 res;
    dvecn_mod(res.v, x.v, y, 2);
    return res;
}

/** dst = x - y * floor(x/y) */
O_INLINE
m_dvec2 dvec2_mod_v(m_dvec2 x, m_dvec2 y)
{
    m_dvec2 res;
    dvecn_mod_v(res.v, x.v, y.v, 2);
    return res;
}


/** dst = a < b ? a : b */
O_INLINE
m_dvec2 dvec2_min(m_dvec2 x, double y)
{
    m_dvec2 res;
    dvecn_min(res.v, x.v, y, 2);
    return res;
}


/** dst = a < b ? a : b */
O_INLINE
m_dvec2 dvec2_min_v(m_dvec2 x, m_dvec2 y)
{
    m_dvec2 res;
    dvecn_min_v(res.v, x.v, y.v, 2);
    return res;
}


/** dst = a > b ? a : b */
O_INLINE
m_dvec2 dvec2_max(m_dvec2 x, double y)
{
    m_dvec2 res;
    dvecn_max(res.v, x.v, y, 2);
    return res;
}


/** dst = a > b ? a : b */
O_INLINE
m_dvec2 dvec2_max_v(m_dvec2 x, m_dvec2 y)
{
    m_dvec2 res;
    dvecn_max_v(res.v, x.v, y.v, 2);
    return res;
}


/** returns the index of the min v value */
O_INLINE
int dvec2_min_index(m_dvec2 dvec)
{
    return dvecn_min_index(dvec.v, 2);
}


/** returns the index of the max v value */
O_INLINE
int dvec2_max_index(m_dvec2 dvec)
{
    return dvecn_max_index(dvec.v, 2);
}

/** dst = x < min ? min : (x > max ? max : x) */
O_INLINE
m_dvec2 dvec2_clamp(m_dvec2 x, double min, double max)
{
    m_dvec2 res;
    dvecn_clamp(res.v, x.v, min, max, 2);
    return res;
}


/** dst = x < min ? min : (x > max ? max : x) */
O_INLINE
m_dvec2 dvec2_clamp_v(m_dvec2 x, m_dvec2 min, m_dvec2 max)
{
    m_dvec2 res;
    dvecn_clamp_v(res.v, x.v, min.v, max.v, 2);
    return res;
}


/** dst = a * (1-t) + b * t */
O_INLINE
m_dvec2 dvec2_mix(m_dvec2 a, m_dvec2 b, double t)
{
    m_dvec2 res;
    dvecn_mix(res.v, a.v, b.v, t, 2);
    return res;
}

/** returns v[0] + v[1] + ... + v[n-1] */
O_INLINE
double dvec2_sum(m_dvec2 v)
{
    return dvecn_sum(v.v, 2);
}


/** returns a dot b */
O_INLINE
double dvec2_dot(m_dvec2 a, m_dvec2 b)
{
    return dvecn_dot(a.v, b.v, 2);
}


///** dst = a x b , dst.w... = 0 */
//O_INLINE
//m_dvec2 dvec2_cross(m_dvec2 a, m_dvec2 b)
//{
//    m_dvec2 res;
//    dvecn_cross(res.v, a.v, b.v, 2);
//    return res;
//}


/** returns ||v||_2 */
O_INLINE
double dvec2_norm(m_dvec2 v)
{
    return dvecn_norm(v.v, 2);
}


/** returns ||v||_p */
O_INLINE
double dvec2_norm_p(m_dvec2 v, double p)
{
    return dvecn_norm_p(v.v, p, 2);
}


/** returns ||v||_1 */
O_INLINE
double dvec2_norm_1(m_dvec2 v)
{
    return dvecn_norm_1(v.v, 2);
}


/** returns ||v||_inf */
O_INLINE
double dvec2_norm_inf(m_dvec2 v)
{
    return dvecn_norm_inf(v.v, 2);
}


/** dst = v / norm(v) */
O_INLINE
m_dvec2 dvec2_normalize_unsafe(m_dvec2 v)
{
    m_dvec2 res;
    dvecn_normalize_unsafe(res.v, v.v, 2);
    return res;
}


/** dst = v / (norm(v) > 0 ? norm(v) : 1) */
O_INLINE
m_dvec2 dvec2_normalize(m_dvec2 v)
{
    m_dvec2 res;
    dvecn_normalize(res.v, v.v, 2);
    return res;
}

///** dst = normalize(cross(a, b)) */
//O_INLINE
//m_dvec2 dvec2_cross_normalized(m_dvec2 a, m_dvec2 b)
//{
//    m_dvec2 res;
//    dvecn_cross_normalized(res.v, a.v, b.v, 2);
//    return res;
//}


/** returns length of a dvector, see dvecn_norm. Just here to match glsl */
O_INLINE
double dvec2_length(m_dvec2 v)
{
    return dvecn_length(v.v, 2);
}


/** returns norm(b-a) */
O_INLINE
double dvec2_distance(m_dvec2 a, m_dvec2 b)
{
    return dvecn_distance(a.v, b.v, 2);
}


/** returns dot(b-a) */
O_INLINE
double dvec2_sqr_distance(m_dvec2 a, m_dvec2 b)
{
    return dvecn_sqr_distance(a.v, b.v, 2);
}


///** dst = dot(i, nref) < 0 ? N : -N */
//O_INLINE
//m_dvec2 dvec2_faceforward(m_dvec2 n, m_dvec2 i, m_dvec2 nref)
//{
//    m_dvec2 res;
//    dvecn_faceforward(res.v, n.v, i.v, nref.v, 2);
//    return res;
//}
//
//
///** dst = i - 2.0 * n * dot(n,i) */
//O_INLINE
//m_dvec2 dvec2_reflect(m_dvec2 i, m_dvec2 n)
//{
//    m_dvec2 res;
//    dvecn_reflect(res.v, i.v, n.v, 2);
//    return res;
//}
//
//
//O_INLINE
//m_dvec2 dvec2_refract(m_dvec2 i, m_dvec2 n, double eta)
//{
//    m_dvec2 res;
//    dvecn_refract(res.v, i.v, n.v, eta, 2);
//    return res;
//}

#endif //M_VEC_DBL_VEC2_H
