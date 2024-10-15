#ifndef M_VEC_INT_VEC4_H
#define M_VEC_INT_VEC4_H

/**
 * @file vec/ivec4.h
 *
 * functions working with ivec4
 */


#include "ivecn.h"
#include "m/m_types/int.h"

/** dst = v0, v1, ... */
O_INLINE
m_ivec4 ivec4_new(int v0, int v1, int v2, int v3)
{
    m_ivec4 self;
    self.v0 = v0;
    self.v1 = v1;
    self.v2 = v2;
    self.v3 = v3;
    return self;
}

///** dst = (m_ivec4) v */
//O_INLINE
//m_ivec4 ivec4_cast_int(const int *cast)
//{
//    m_ivec4 res;
//    ivecn_cast_into(res.v, cast, 4);
//    return res;
//}

/** dst = (m_ivec4) v */
O_INLINE
m_ivec4 ivec4_cast_double(const double *cast)
{
    m_ivec4 res;
    ivecn_cast_into(res.v, cast, 4);
    return res;
}

/** dst = (m_ivec4) v */
O_INLINE
m_ivec4 ivec4_cast_float(const float *cast)
{
    m_ivec4 res;
    ivecn_cast_into(res.v, cast, 4);
    return res;
}

/** dst = (m_ivec4) v */
O_INLINE
m_ivec4 ivec4_cast_byte(const obyte *cast)
{
    m_ivec4 res;
    ivecn_cast_into(res.v, cast, 4);
    return res;
}

/** dst = v / 255 */
O_INLINE
m_ivec4 ivec4_cast_byte_1(const obyte *cast)
{
    m_ivec4 res;
    ivecn_cast_byte_1(res.v, cast, 4);
    return res;
}



/** a == b */
O_INLINE
bool ivec4_equals(m_ivec4 a, int b)
{
    return ivecn_equals(a.v, b, 4);
}

/** a == b */
O_INLINE
bool ivec4_equals_v(m_ivec4 a, m_ivec4 b)
{
    return ivecn_equals_v(a.v, b.v, 4);
}

/** a ~= b */
O_INLINE
bool ivec4_equals_eps(m_ivec4 a, int b, int eps)
{
    return ivecn_equals_eps(a.v, b, eps, 4);
}

/** a ~= b */
O_INLINE
bool ivec4_equals_eps_v(m_ivec4 a, m_ivec4 b, int eps)
{
    return ivecn_equals_eps_v(a.v, b.v, eps, 4);
}


/** dst = s */
O_INLINE
m_ivec4 ivec4_set(int s)
{
    m_ivec4 res;
    ivecn_set(res.v, s, 4);
    return res;
}

/** dst = unit_x */
O_INLINE
m_ivec4 ivec4_unit_x(void)
{
    m_ivec4 res;
    ivecn_unit_x(res.v, 4);
    return res;
}


/** dst = unit_y */
O_INLINE
m_ivec4 ivec4_unit_y(void)
{
    m_ivec4 res;
    ivecn_unit_y(res.v, 4);
    return res;
}


/** dst = unit_z */
O_INLINE
m_ivec4 ivec4_unit_z(void)
{
    m_ivec4 res;
    ivecn_unit_z(res.v, 4);
    return res;
}


/** dst = unit_w */
O_INLINE
m_ivec4 ivec4_unit_w(void)
{
    m_ivec4 res;
    ivecn_unit_w(res.v, 4);
    return res;
}


/** dst = -v */
O_INLINE
m_ivec4 ivec4_neg(m_ivec4 v)
{
    m_ivec4 res;
    ivecn_neg(res.v, v.v, 4);
    return res;
}


/** dst = a + b */
O_INLINE
m_ivec4 ivec4_add_v(m_ivec4 a, m_ivec4 b)
{
    m_ivec4 res;
    ivecn_add_v(res.v, a.v, b.v, 4);
    return res;
}


/** dst = a + b */
O_INLINE
m_ivec4 ivec4_add(m_ivec4 a, int b)
{
    m_ivec4 res;
    ivecn_add(res.v, a.v, b, 4);
    return res;
}


/** dst = a - b */
O_INLINE
m_ivec4 ivec4_sub_v(m_ivec4 a, m_ivec4 b)
{
    m_ivec4 res;
    ivecn_sub_v(res.v, a.v, b.v, 4);
    return res;
}


/** dst = a - b */
O_INLINE
m_ivec4 ivec4_sub(m_ivec4 a, int b)
{
    m_ivec4 res;
    ivecn_sub(res.v, a.v, b, 4);
    return res;
}


/** dst = a * b */
O_INLINE
m_ivec4 ivec4_scale_v(m_ivec4 a, m_ivec4 b)
{
    m_ivec4 res;
    ivecn_scale_v(res.v, a.v, b.v, 4);
    return res;
}


/** dst = a * b */
O_INLINE
m_ivec4 ivec4_scale(m_ivec4 a, int b)
{
    m_ivec4 res;
    ivecn_scale(res.v, a.v, b, 4);
    return res;
}


/** dst = a / b */
O_INLINE
m_ivec4 ivec4_div_v(m_ivec4 a, m_ivec4 b)
{
    m_ivec4 res;
    ivecn_div_v(res.v, a.v, b.v, 4);
    return res;
}


/** dst = a / b */
O_INLINE
m_ivec4 ivec4_div(m_ivec4 a, int b)
{
    m_ivec4 res;
    ivecn_div(res.v, a.v, b, 4);
    return res;
}

/** dst = a + b * c */
O_INLINE
m_ivec4 ivec4_add_scaled_v(m_ivec4 a, m_ivec4 b, m_ivec4 c)
{
    m_ivec4 res;
    ivecn_add_scaled_v(res.v, a.v, b.v, c.v, 4);
    return res;
}

/** dst = a + b * c */
O_INLINE
m_ivec4 ivec4_add_scaled(m_ivec4 a, m_ivec4 b, int c)
{
    m_ivec4 res;
    ivecn_add_scaled(res.v, a.v, b.v, c, 4);
    return res;
}

/** dst = abs(x) */
O_INLINE
m_ivec4 ivec4_abs(m_ivec4 x)
{
    m_ivec4 res;
    ivecn_abs(res.v, x.v, 4);
    return res;
}


/** dst = x > 0 ? 1 : (x < 0 ? -1 : 0) */
O_INLINE
m_ivec4 ivec4_sign(m_ivec4 x)
{
    m_ivec4 res;
    ivecn_sign(res.v, x.v, 4);
    return res;
}

/** dst = x % y as positive version for integers */
O_INLINE
m_ivec4 ivec4_mod(m_ivec4 x, int y)
{
    m_ivec4 res;
    ivecn_mod(res.v, x.v, y, 4);
    return res;
}

/** dst = x % y as positive version for integers */
O_INLINE
m_ivec4 ivec4_mod_v(m_ivec4 x, m_ivec4 y)
{
    m_ivec4 res;
    ivecn_mod_v(res.v, x.v, y.v, 4);
    return res;
}


/** dst = a < b ? a : b */
O_INLINE
m_ivec4 ivec4_min(m_ivec4 x, int y)
{
    m_ivec4 res;
    ivecn_min(res.v, x.v, y, 4);
    return res;
}


/** dst = a < b ? a : b */
O_INLINE
m_ivec4 ivec4_min_v(m_ivec4 x, m_ivec4 y)
{
    m_ivec4 res;
    ivecn_min_v(res.v, x.v, y.v, 4);
    return res;
}


/** dst = a > b ? a : b */
O_INLINE
m_ivec4 ivec4_max(m_ivec4 x, int y)
{
    m_ivec4 res;
    ivecn_max(res.v, x.v, y, 4);
    return res;
}


/** dst = a > b ? a : b */
O_INLINE
m_ivec4 ivec4_max_v(m_ivec4 x, m_ivec4 y)
{
    m_ivec4 res;
    ivecn_max_v(res.v, x.v, y.v, 4);
    return res;
}


/** returns the index of the min v value */
O_INLINE
int ivec4_min_index(m_ivec4 ivec)
{
    return ivecn_min_index(ivec.v, 4);
}


/** returns the index of the max v value */
O_INLINE
int ivec4_max_index(m_ivec4 ivec)
{
    return ivecn_max_index(ivec.v, 4);
}

/** dst = x < min ? min : (x > max ? max : x) */
O_INLINE
m_ivec4 ivec4_clamp(m_ivec4 x, int min, int max)
{
    m_ivec4 res;
    ivecn_clamp(res.v, x.v, min, max, 4);
    return res;
}


/** dst = x < min ? min : (x > max ? max : x) */
O_INLINE
m_ivec4 ivec4_clamp_v(m_ivec4 x, m_ivec4 min, m_ivec4 max)
{
    m_ivec4 res;
    ivecn_clamp_v(res.v, x.v, min.v, max.v, 4);
    return res;
}


/** dst = a * (1-t) + b * t */
O_INLINE
m_ivec4 ivec4_mix(m_ivec4 a, m_ivec4 b, float t)
{
    m_ivec4 res;
    ivecn_mix(res.v, a.v, b.v, t, 4);
    return res;
}

/** returns v[0] + v[1] + ... + v[n-1] */
O_INLINE
int ivec4_sum(m_ivec4 v)
{
    return ivecn_sum(v.v, 4);
}


#endif //M_VEC_INT_VEC4_H
