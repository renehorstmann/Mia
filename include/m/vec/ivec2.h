#ifndef M_VEC_INT_VEC2_H
#define M_VEC_INT_VEC2_H

/**
 * @file vec/ivec2.h
 *
 * functions working with ivec2
 */



#include "ivecn.h"
#include "m/m_types/int.h"

/** dst = v0, v1, ... */
O_INLINE
m_ivec2 ivec2_new(int v0, int v1)
{
    m_ivec2 self;
    self.v0 = v0;
    self.v1 = v1;
    return self;
}

///** dst = (m_ivec2) v */
//O_INLINE
//m_ivec2 ivec2_cast_int(const int *cast)
//{
//    m_ivec2 res;
//    ivecn_cast_into(res.v, cast, 2);
//    return res;
//}

/** dst = (m_ivec2) v */
O_INLINE
m_ivec2 ivec2_cast_double(const double *cast)
{
    m_ivec2 res;
    ivecn_cast_into(res.v, cast, 2);
    return res;
}

/** dst = (m_ivec2) v */
O_INLINE
m_ivec2 ivec2_cast_float(const float *cast)
{
    m_ivec2 res;
    ivecn_cast_into(res.v, cast, 2);
    return res;
}

/** dst = (m_ivec2) v */
O_INLINE
m_ivec2 ivec2_cast_byte(const obyte *cast)
{
    m_ivec2 res;
    ivecn_cast_into(res.v, cast, 2);
    return res;
}

/** dst = v / 255 */
O_INLINE
m_ivec2 ivec2_cast_byte_1(const obyte *cast)
{
    m_ivec2 res;
    ivecn_cast_byte_1(res.v, cast, 2);
    return res;
}



/** a == b */
O_INLINE
bool ivec2_equals(m_ivec2 a, int b)
{
    return ivecn_equals(a.v, b, 2);
}

/** a == b */
O_INLINE
bool ivec2_equals_v(m_ivec2 a, m_ivec2 b)
{
    return ivecn_equals_v(a.v, b.v, 2);
}

/** a ~= b */
O_INLINE
bool ivec2_equals_eps(m_ivec2 a, int b, int eps)
{
    return ivecn_equals_eps(a.v, b, eps, 2);
}

/** a ~= b */
O_INLINE
bool ivec2_equals_eps_v(m_ivec2 a, m_ivec2 b, int eps)
{
    return ivecn_equals_eps_v(a.v, b.v, eps, 2);
}


/** dst = s */
O_INLINE
m_ivec2 ivec2_set(int s)
{
    m_ivec2 res;
    ivecn_set(res.v, s, 2);
    return res;
}

/** dst = unit_x */
O_INLINE
m_ivec2 ivec2_unit_x(void)
{
    m_ivec2 res;
    ivecn_unit_x(res.v, 2);
    return res;
}


/** dst = unit_y */
O_INLINE
m_ivec2 ivec2_unit_y(void)
{
    m_ivec2 res;
    ivecn_unit_y(res.v, 2);
    return res;
}


/** dst = unit_z */
O_INLINE
m_ivec2 ivec2_unit_z(void)
{
    m_ivec2 res;
    ivecn_unit_z(res.v, 2);
    return res;
}


/** dst = unit_w */
O_INLINE
m_ivec2 ivec2_unit_w(void)
{
    m_ivec2 res;
    ivecn_unit_w(res.v, 2);
    return res;
}


/** dst = -v */
O_INLINE
m_ivec2 ivec2_neg(m_ivec2 v)
{
    m_ivec2 res;
    ivecn_neg(res.v, v.v, 2);
    return res;
}


/** dst = a + b */
O_INLINE
m_ivec2 ivec2_add_v(m_ivec2 a, m_ivec2 b)
{
    m_ivec2 res;
    ivecn_add_v(res.v, a.v, b.v, 2);
    return res;
}


/** dst = a + b */
O_INLINE
m_ivec2 ivec2_add(m_ivec2 a, int b)
{
    m_ivec2 res;
    ivecn_add(res.v, a.v, b, 2);
    return res;
}


/** dst = a - b */
O_INLINE
m_ivec2 ivec2_sub_v(m_ivec2 a, m_ivec2 b)
{
    m_ivec2 res;
    ivecn_sub_v(res.v, a.v, b.v, 2);
    return res;
}


/** dst = a - b */
O_INLINE
m_ivec2 ivec2_sub(m_ivec2 a, int b)
{
    m_ivec2 res;
    ivecn_sub(res.v, a.v, b, 2);
    return res;
}


/** dst = a * b */
O_INLINE
m_ivec2 ivec2_scale_v(m_ivec2 a, m_ivec2 b)
{
    m_ivec2 res;
    ivecn_scale_v(res.v, a.v, b.v, 2);
    return res;
}


/** dst = a * b */
O_INLINE
m_ivec2 ivec2_scale(m_ivec2 a, int b)
{
    m_ivec2 res;
    ivecn_scale(res.v, a.v, b, 2);
    return res;
}


/** dst = a / b */
O_INLINE
m_ivec2 ivec2_div_v(m_ivec2 a, m_ivec2 b)
{
    m_ivec2 res;
    ivecn_div_v(res.v, a.v, b.v, 2);
    return res;
}


/** dst = a / b */
O_INLINE
m_ivec2 ivec2_div(m_ivec2 a, int b)
{
    m_ivec2 res;
    ivecn_div(res.v, a.v, b, 2);
    return res;
}

/** dst = a + b * c */
O_INLINE
m_ivec2 ivec2_add_scaled_v(m_ivec2 a, m_ivec2 b, m_ivec2 c)
{
    m_ivec2 res;
    ivecn_add_scaled_v(res.v, a.v, b.v, c.v, 2);
    return res;
}

/** dst = a + b * c */
O_INLINE
m_ivec2 ivec2_add_scaled(m_ivec2 a, m_ivec2 b, int c)
{
    m_ivec2 res;
    ivecn_add_scaled(res.v, a.v, b.v, c, 2);
    return res;
}

/** dst = abs(x) */
O_INLINE
m_ivec2 ivec2_abs(m_ivec2 x)
{
    m_ivec2 res;
    ivecn_abs(res.v, x.v, 2);
    return res;
}


/** dst = x > 0 ? 1 : (x < 0 ? -1 : 0) */
O_INLINE
m_ivec2 ivec2_sign(m_ivec2 x)
{
    m_ivec2 res;
    ivecn_sign(res.v, x.v, 2);
    return res;
}


/** dst = x % y as positive version for integers */
O_INLINE
m_ivec2 ivec2_mod(m_ivec2 x, int y)
{
    m_ivec2 res;
    ivecn_mod(res.v, x.v, y, 2);
    return res;
}

/** dst = x % y as positive version for integers */
O_INLINE
m_ivec2 ivec2_mod_v(m_ivec2 x, m_ivec2 y)
{
    m_ivec2 res;
    ivecn_mod_v(res.v, x.v, y.v, 2);
    return res;
}


/** dst = a < b ? a : b */
O_INLINE
m_ivec2 ivec2_min(m_ivec2 x, int y)
{
    m_ivec2 res;
    ivecn_min(res.v, x.v, y, 2);
    return res;
}


/** dst = a < b ? a : b */
O_INLINE
m_ivec2 ivec2_min_v(m_ivec2 x, m_ivec2 y)
{
    m_ivec2 res;
    ivecn_min_v(res.v, x.v, y.v, 2);
    return res;
}


/** dst = a > b ? a : b */
O_INLINE
m_ivec2 ivec2_max(m_ivec2 x, int y)
{
    m_ivec2 res;
    ivecn_max(res.v, x.v, y, 2);
    return res;
}


/** dst = a > b ? a : b */
O_INLINE
m_ivec2 ivec2_max_v(m_ivec2 x, m_ivec2 y)
{
    m_ivec2 res;
    ivecn_max_v(res.v, x.v, y.v, 2);
    return res;
}


/** returns the index of the min v value */
O_INLINE
int ivec2_min_index(m_ivec2 ivec)
{
    return ivecn_min_index(ivec.v, 2);
}


/** returns the index of the max v value */
O_INLINE
int ivec2_max_index(m_ivec2 ivec)
{
    return ivecn_max_index(ivec.v, 2);
}

/** dst = x < min ? min : (x > max ? max : x) */
O_INLINE
m_ivec2 ivec2_clamp(m_ivec2 x, int min, int max)
{
    m_ivec2 res;
    ivecn_clamp(res.v, x.v, min, max, 2);
    return res;
}


/** dst = x < min ? min : (x > max ? max : x) */
O_INLINE
m_ivec2 ivec2_clamp_v(m_ivec2 x, m_ivec2 min, m_ivec2 max)
{
    m_ivec2 res;
    ivecn_clamp_v(res.v, x.v, min.v, max.v, 2);
    return res;
}


/** dst = a * (1-t) + b * t */
O_INLINE
m_ivec2 ivec2_mix(m_ivec2 a, m_ivec2 b, float t)
{
    m_ivec2 res;
    ivecn_mix(res.v, a.v, b.v, t, 2);
    return res;
}

/** returns v[0] + v[1] + ... + v[n-1] */
O_INLINE
int ivec2_sum(m_ivec2 v)
{
    return ivecn_sum(v.v, 2);
}


#endif //M_VEC_INT_VEC2_H
