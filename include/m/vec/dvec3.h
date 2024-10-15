#ifndef M_VEC_DBL_VEC3_H
#define M_VEC_DBL_VEC3_H

/**
 * @file vec/dvec3.h
 *
 * functions working with dvec3
 */



#include "dvecn.h"
#include "m/m_types/dbl.h"

/** dst = v0, v1, ... */
O_INLINE
m_dvec3 dvec3_new(double v0, double v1, double v2)
{
    m_dvec3 self;
    self.v0 = v0;
    self.v1 = v1;
    self.v2 = v2;
    return self;
}

/** dst = (m_dvec3) v */
O_INLINE
m_dvec3 dvec3_cast_float(const float *cast)
{
    m_dvec3 res;
    dvecn_cast_into(res.v, cast, 3);
    return res;
}

///** dst = (m_dvec3) v */
//O_INLINE
//m_dvec3 dvec3_cast_double(const double *cast)
//{
//    m_dvec3 res;
//    dvecn_cast_into(res.v, cast, 3);
//    return res;
//}

/** dst = (m_dvec3) v */
O_INLINE
m_dvec3 dvec3_cast_int(const int *cast)
{
    m_dvec3 res;
    dvecn_cast_into(res.v, cast, 3);
    return res;
}

/** dst = (m_dvec3) v */
O_INLINE
m_dvec3 dvec3_cast_byte(const obyte *cast)
{
    m_dvec3 res;
    dvecn_cast_into(res.v, cast, 3);
    return res;
}

/** dst = v / 255 */
O_INLINE
m_dvec3 dvec3_cast_byte_1(const obyte *cast)
{
    m_dvec3 res;
    dvecn_cast_byte_1(res.v, cast, 3);
    return res;
}



/** a == b */
O_INLINE
bool dvec3_equals(m_dvec3 a, double b)
{
    return dvecn_equals(a.v, b, 3);
}

/** a == b */
O_INLINE
bool dvec3_equals_v(m_dvec3 a, m_dvec3 b)
{
    return dvecn_equals_v(a.v, b.v, 3);
}

/** a ~= b */
O_INLINE
bool dvec3_equals_eps(m_dvec3 a, double b, double eps)
{
    return dvecn_equals_eps(a.v, b, eps, 3);
}

/** a ~= b */
O_INLINE
bool dvec3_equals_eps_v(m_dvec3 a, m_dvec3 b, double eps)
{
    return dvecn_equals_eps_v(a.v, b.v, eps, 3);
}


/** dst = s */
O_INLINE
m_dvec3 dvec3_set(double s)
{
    m_dvec3 res;
    dvecn_set(res.v, s, 3);
    return res;
}

/** dst = unit_x */
O_INLINE
m_dvec3 dvec3_unit_x(void)
{
    m_dvec3 res;
    dvecn_unit_x(res.v, 3);
    return res;
}


/** dst = unit_y */
O_INLINE
m_dvec3 dvec3_unit_y(void)
{
    m_dvec3 res;
    dvecn_unit_y(res.v, 3);
    return res;
}


/** dst = unit_z */
O_INLINE
m_dvec3 dvec3_unit_z(void)
{
    m_dvec3 res;
    dvecn_unit_z(res.v, 3);
    return res;
}


/** dst = unit_w */
O_INLINE
m_dvec3 dvec3_unit_w(void)
{
    m_dvec3 res;
    dvecn_unit_w(res.v, 3);
    return res;
}


/** dst = -v */
O_INLINE
m_dvec3 dvec3_neg(m_dvec3 v)
{
    m_dvec3 res;
    dvecn_neg(res.v, v.v, 3);
    return res;
}


/** dst = a + b */
O_INLINE
m_dvec3 dvec3_add_v(m_dvec3 a, m_dvec3 b)
{
    m_dvec3 res;
    dvecn_add_v(res.v, a.v, b.v, 3);
    return res;
}


/** dst = a + b */
O_INLINE
m_dvec3 dvec3_add(m_dvec3 a, double b)
{
    m_dvec3 res;
    dvecn_add(res.v, a.v, b, 3);
    return res;
}


/** dst = a - b */
O_INLINE
m_dvec3 dvec3_sub_v(m_dvec3 a, m_dvec3 b)
{
    m_dvec3 res;
    dvecn_sub_v(res.v, a.v, b.v, 3);
    return res;
}


/** dst = a - b */
O_INLINE
m_dvec3 dvec3_sub(m_dvec3 a, double b)
{
    m_dvec3 res;
    dvecn_sub(res.v, a.v, b, 3);
    return res;
}


/** dst = a * b */
O_INLINE
m_dvec3 dvec3_scale_v(m_dvec3 a, m_dvec3 b)
{
    m_dvec3 res;
    dvecn_scale_v(res.v, a.v, b.v, 3);
    return res;
}


/** dst = a * b */
O_INLINE
m_dvec3 dvec3_scale(m_dvec3 a, double b)
{
    m_dvec3 res;
    dvecn_scale(res.v, a.v, b, 3);
    return res;
}


/** dst = a / b */
O_INLINE
m_dvec3 dvec3_div_v(m_dvec3 a, m_dvec3 b)
{
    m_dvec3 res;
    dvecn_div_v(res.v, a.v, b.v, 3);
    return res;
}


/** dst = a / b */
O_INLINE
m_dvec3 dvec3_div(m_dvec3 a, double b)
{
    m_dvec3 res;
    dvecn_div(res.v, a.v, b, 3);
    return res;
}

/** dst = a + b * c */
O_INLINE
m_dvec3 dvec3_add_scaled_v(m_dvec3 a, m_dvec3 b, m_dvec3 c)
{
    m_dvec3 res;
    dvecn_add_scaled_v(res.v, a.v, b.v, c.v, 3);
    return res;
}

/** dst = a + b * c */
O_INLINE
m_dvec3 dvec3_add_scaled(m_dvec3 a, m_dvec3 b, double c)
{
    m_dvec3 res;
    dvecn_add_scaled(res.v, a.v, b.v, c, 3);
    return res;
}


/** dst = a * M_PI / 180 */
O_INLINE
m_dvec3 dvec3_radians(m_dvec3 deg)
{
    m_dvec3 res;
    dvecn_radians(res.v, deg.v, 3);
    return res;
}


/** dst = a * 180 / M_PI */
O_INLINE
m_dvec3 dvec3_degrees(m_dvec3 rad)
{
    m_dvec3 res;
    dvecn_degrees(res.v, rad.v, 3);
    return res;
}


/** dst = abs(x) */
O_INLINE
m_dvec3 dvec3_abs(m_dvec3 x)
{
    m_dvec3 res;
    dvecn_abs(res.v, x.v, 3);
    return res;
}


/** dst = x > 0 ? 1 : (x < 0 ? -1 : 0) */
O_INLINE
m_dvec3 dvec3_sign(m_dvec3 x)
{
    m_dvec3 res;
    dvecn_sign(res.v, x.v, 3);
    return res;
}


/** dst = round(x) */
O_INLINE
m_dvec3 dvec3_round(m_dvec3 x)
{
    m_dvec3 res;
    dvecn_round(res.v, x.v, 3);
    return res;
}


/** dst = floor(x) */
O_INLINE
m_dvec3 dvec3_floor(m_dvec3 x)
{
    m_dvec3 res;
    dvecn_floor(res.v, x.v, 3);
    return res;
}


/** dst = ceil(x) */
O_INLINE
m_dvec3 dvec3_ceil(m_dvec3 x)
{
    m_dvec3 res;
    dvecn_ceil(res.v, x.v, 3);
    return res;
}


/** dst = x - floor(x) */
O_INLINE
m_dvec3 dvec3_fract(m_dvec3 x)
{
    m_dvec3 res;
    dvecn_fract(res.v, x.v, 3);
    return res;
}


/** dst = x - y * floor(x/y) */
O_INLINE
m_dvec3 dvec3_mod(m_dvec3 x, double y)
{
    m_dvec3 res;
    dvecn_mod(res.v, x.v, y, 3);
    return res;
}

/** dst = x - y * floor(x/y) */
O_INLINE
m_dvec3 dvec3_mod_v(m_dvec3 x, m_dvec3 y)
{
    m_dvec3 res;
    dvecn_mod_v(res.v, x.v, y.v, 3);
    return res;
}


/** dst = a < b ? a : b */
O_INLINE
m_dvec3 dvec3_min(m_dvec3 x, double y)
{
    m_dvec3 res;
    dvecn_min(res.v, x.v, y, 3);
    return res;
}


/** dst = a < b ? a : b */
O_INLINE
m_dvec3 dvec3_min_v(m_dvec3 x, m_dvec3 y)
{
    m_dvec3 res;
    dvecn_min_v(res.v, x.v, y.v, 3);
    return res;
}


/** dst = a > b ? a : b */
O_INLINE
m_dvec3 dvec3_max(m_dvec3 x, double y)
{
    m_dvec3 res;
    dvecn_max(res.v, x.v, y, 3);
    return res;
}


/** dst = a > b ? a : b */
O_INLINE
m_dvec3 dvec3_max_v(m_dvec3 x, m_dvec3 y)
{
    m_dvec3 res;
    dvecn_max_v(res.v, x.v, y.v, 3);
    return res;
}


/** returns the index of the min v value */
O_INLINE
int dvec3_min_index(m_dvec3 dvec)
{
    return dvecn_min_index(dvec.v, 3);
}


/** returns the index of the max v value */
O_INLINE
int dvec3_max_index(m_dvec3 dvec)
{
    return dvecn_max_index(dvec.v, 3);
}

/** dst = x < min ? min : (x > max ? max : x) */
O_INLINE
m_dvec3 dvec3_clamp(m_dvec3 x, double min, double max)
{
    m_dvec3 res;
    dvecn_clamp(res.v, x.v, min, max, 3);
    return res;
}


/** dst = x < min ? min : (x > max ? max : x) */
O_INLINE
m_dvec3 dvec3_clamp_v(m_dvec3 x, m_dvec3 min, m_dvec3 max)
{
    m_dvec3 res;
    dvecn_clamp_v(res.v, x.v, min.v, max.v, 3);
    return res;
}


/** dst = a * (1-t) + b * t */
O_INLINE
m_dvec3 dvec3_mix(m_dvec3 a, m_dvec3 b, double t)
{
    m_dvec3 res;
    dvecn_mix(res.v, a.v, b.v, t, 3);
    return res;
}

/** returns v[0] + v[1] + ... + v[n-1] */
O_INLINE
double dvec3_sum(m_dvec3 v)
{
    return dvecn_sum(v.v, 3);
}


/** returns a dot b */
O_INLINE
double dvec3_dot(m_dvec3 a, m_dvec3 b)
{
    return dvecn_dot(a.v, b.v, 3);
}


/** dst = a x b , dst.w... = 0 */
O_INLINE
m_dvec3 dvec3_cross(m_dvec3 a, m_dvec3 b)
{
    m_dvec3 res;
    dvecn_cross(res.v, a.v, b.v, 3);
    return res;
}


/** returns ||v||_3 */
O_INLINE
double dvec3_norm(m_dvec3 v)
{
    return dvecn_norm(v.v, 3);
}


/** returns ||v||_p */
O_INLINE
double dvec3_norm_p(m_dvec3 v, double p)
{
    return dvecn_norm_p(v.v, p, 3);
}


/** returns ||v||_1 */
O_INLINE
double dvec3_norm_1(m_dvec3 v)
{
    return dvecn_norm_1(v.v, 3);
}


/** returns ||v||_inf */
O_INLINE
double dvec3_norm_inf(m_dvec3 v)
{
    return dvecn_norm_inf(v.v, 3);
}


/** dst = v / norm(v) */
O_INLINE
m_dvec3 dvec3_normalize_unsafe(m_dvec3 v)
{
    m_dvec3 res;
    dvecn_normalize_unsafe(res.v, v.v, 3);
    return res;
}


/** dst = v / (norm(v) > 0 ? norm(v) : 1) */
O_INLINE
m_dvec3 dvec3_normalize(m_dvec3 v)
{
    m_dvec3 res;
    dvecn_normalize(res.v, v.v, 3);
    return res;
}

/** dst = normalize(cross(a, b)) */
O_INLINE
m_dvec3 dvec3_cross_normalized(m_dvec3 a, m_dvec3 b)
{
    m_dvec3 res;
    dvecn_cross_normalized(res.v, a.v, b.v, 3);
    return res;
}


/** returns length of a dvector, see dvecn_norm. Just here to match glsl */
O_INLINE
double dvec3_length(m_dvec3 v)
{
    return dvecn_length(v.v, 3);
}


/** returns norm(b-a) */
O_INLINE
double dvec3_distance(m_dvec3 a, m_dvec3 b)
{
    return dvecn_distance(a.v, b.v, 3);
}


/** returns dot(b-a) */
O_INLINE
double dvec3_sqr_distance(m_dvec3 a, m_dvec3 b)
{
    return dvecn_sqr_distance(a.v, b.v, 3);
}


/** dst = dot(i, nref) < 0 ? N : -N */
O_INLINE
m_dvec3 dvec3_faceforward(m_dvec3 n, m_dvec3 i, m_dvec3 nref)
{
    m_dvec3 res;
    dvecn_faceforward(res.v, n.v, i.v, nref.v, 3);
    return res;
}


/** dst = i - 2.0 * n * dot(n,i) */
O_INLINE
m_dvec3 dvec3_reflect(m_dvec3 i, m_dvec3 n)
{
    m_dvec3 res;
    dvecn_reflect(res.v, i.v, n.v, 3);
    return res;
}


O_INLINE
m_dvec3 dvec3_refract(m_dvec3 i, m_dvec3 n, double eta)
{
    m_dvec3 res;
    dvecn_refract(res.v, i.v, n.v, eta, 3);
    return res;
}

#endif //M_VEC_DBL_VEC3_H
