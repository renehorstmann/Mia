#ifndef M_VEC_INT_VEC3_H
#define M_VEC_INT_VEC3_H

/**
 * @file vec/ivec3.h
 *
 * functions working with ivec3
 */



#include "ivecn.h"
#include "m/m_types/int.h"

/** dst = v0, v1, ... */
O_INLINE
m_ivec3 ivec3_new(int v0, int v1, int v2)
{
    m_ivec3 self;
    self.v0 = v0;
    self.v1 = v1;
    self.v2 = v2;
    return self;
}

///** dst = (m_ivec3) v */
//O_INLINE
//m_ivec3 ivec3_cast_int(const int *cast)
//{
//    m_ivec3 res;
//    ivecn_cast_into(res.v, cast, 3);
//    return res;
//}

/** dst = (m_ivec3) v */
O_INLINE
m_ivec3 ivec3_cast_double(const double *cast)
{
    m_ivec3 res;
    ivecn_cast_into(res.v, cast, 3);
    return res;
}

/** dst = (m_ivec3) v */
O_INLINE
m_ivec3 ivec3_cast_float(const float *cast)
{
    m_ivec3 res;
    ivecn_cast_into(res.v, cast, 3);
    return res;
}

/** dst = (m_ivec3) v */
O_INLINE
m_ivec3 ivec3_cast_byte(const obyte *cast)
{
    m_ivec3 res;
    ivecn_cast_into(res.v, cast, 3);
    return res;
}

/** dst = v / 255 */
O_INLINE
m_ivec3 ivec3_cast_byte_1(const obyte *cast)
{
    m_ivec3 res;
    ivecn_cast_byte_1(res.v, cast, 3);
    return res;
}



/** a == b */
O_INLINE
bool ivec3_equals(m_ivec3 a, int b)
{
    return ivecn_equals(a.v, b, 3);
}

/** a == b */
O_INLINE
bool ivec3_equals_v(m_ivec3 a, m_ivec3 b)
{
    return ivecn_equals_v(a.v, b.v, 3);
}

/** a ~= b */
O_INLINE
bool ivec3_equals_eps(m_ivec3 a, int b, int eps)
{
    return ivecn_equals_eps(a.v, b, eps, 3);
}

/** a ~= b */
O_INLINE
bool ivec3_equals_eps_v(m_ivec3 a, m_ivec3 b, int eps)
{
    return ivecn_equals_eps_v(a.v, b.v, eps, 3);
}


/** dst = s */
O_INLINE
m_ivec3 ivec3_set(int s)
{
    m_ivec3 res;
    ivecn_set(res.v, s, 3);
    return res;
}

/** dst = unit_x */
O_INLINE
m_ivec3 ivec3_unit_x(void)
{
    m_ivec3 res;
    ivecn_unit_x(res.v, 3);
    return res;
}


/** dst = unit_y */
O_INLINE
m_ivec3 ivec3_unit_y(void)
{
    m_ivec3 res;
    ivecn_unit_y(res.v, 3);
    return res;
}


/** dst = unit_z */
O_INLINE
m_ivec3 ivec3_unit_z(void)
{
    m_ivec3 res;
    ivecn_unit_z(res.v, 3);
    return res;
}


/** dst = unit_w */
O_INLINE
m_ivec3 ivec3_unit_w(void)
{
    m_ivec3 res;
    ivecn_unit_w(res.v, 3);
    return res;
}


/** dst = -v */
O_INLINE
m_ivec3 ivec3_neg(m_ivec3 v)
{
    m_ivec3 res;
    ivecn_neg(res.v, v.v, 3);
    return res;
}


/** dst = a + b */
O_INLINE
m_ivec3 ivec3_add_v(m_ivec3 a, m_ivec3 b)
{
    m_ivec3 res;
    ivecn_add_v(res.v, a.v, b.v, 3);
    return res;
}


/** dst = a + b */
O_INLINE
m_ivec3 ivec3_add(m_ivec3 a, int b)
{
    m_ivec3 res;
    ivecn_add(res.v, a.v, b, 3);
    return res;
}


/** dst = a - b */
O_INLINE
m_ivec3 ivec3_sub_v(m_ivec3 a, m_ivec3 b)
{
    m_ivec3 res;
    ivecn_sub_v(res.v, a.v, b.v, 3);
    return res;
}


/** dst = a - b */
O_INLINE
m_ivec3 ivec3_sub(m_ivec3 a, int b)
{
    m_ivec3 res;
    ivecn_sub(res.v, a.v, b, 3);
    return res;
}


/** dst = a * b */
O_INLINE
m_ivec3 ivec3_scale_v(m_ivec3 a, m_ivec3 b)
{
    m_ivec3 res;
    ivecn_scale_v(res.v, a.v, b.v, 3);
    return res;
}


/** dst = a * b */
O_INLINE
m_ivec3 ivec3_scale(m_ivec3 a, int b)
{
    m_ivec3 res;
    ivecn_scale(res.v, a.v, b, 3);
    return res;
}


/** dst = a / b */
O_INLINE
m_ivec3 ivec3_div_v(m_ivec3 a, m_ivec3 b)
{
    m_ivec3 res;
    ivecn_div_v(res.v, a.v, b.v, 3);
    return res;
}


/** dst = a / b */
O_INLINE
m_ivec3 ivec3_div(m_ivec3 a, int b)
{
    m_ivec3 res;
    ivecn_div(res.v, a.v, b, 3);
    return res;
}

/** dst = a + b * c */
O_INLINE
m_ivec3 ivec3_add_scaled_v(m_ivec3 a, m_ivec3 b, m_ivec3 c)
{
    m_ivec3 res;
    ivecn_add_scaled_v(res.v, a.v, b.v, c.v, 3);
    return res;
}

/** dst = a + b * c */
O_INLINE
m_ivec3 ivec3_add_scaled(m_ivec3 a, m_ivec3 b, int c)
{
    m_ivec3 res;
    ivecn_add_scaled(res.v, a.v, b.v, c, 3);
    return res;
}


/** dst = abs(x) */
O_INLINE
m_ivec3 ivec3_abs(m_ivec3 x)
{
    m_ivec3 res;
    ivecn_abs(res.v, x.v, 3);
    return res;
}


/** dst = x > 0 ? 1 : (x < 0 ? -1 : 0) */
O_INLINE
m_ivec3 ivec3_sign(m_ivec3 x)
{
    m_ivec3 res;
    ivecn_sign(res.v, x.v, 3);
    return res;
}

/** dst = x % y as positive version for integers */
O_INLINE
m_ivec3 ivec3_mod(m_ivec3 x, int y)
{
    m_ivec3 res;
    ivecn_mod(res.v, x.v, y, 3);
    return res;
}

/** dst = x % y as positive version for integers */
O_INLINE
m_ivec3 ivec3_mod_v(m_ivec3 x, m_ivec3 y)
{
    m_ivec3 res;
    ivecn_mod_v(res.v, x.v, y.v, 3);
    return res;
}


/** dst = a < b ? a : b */
O_INLINE
m_ivec3 ivec3_min(m_ivec3 x, int y)
{
    m_ivec3 res;
    ivecn_min(res.v, x.v, y, 3);
    return res;
}


/** dst = a < b ? a : b */
O_INLINE
m_ivec3 ivec3_min_v(m_ivec3 x, m_ivec3 y)
{
    m_ivec3 res;
    ivecn_min_v(res.v, x.v, y.v, 3);
    return res;
}


/** dst = a > b ? a : b */
O_INLINE
m_ivec3 ivec3_max(m_ivec3 x, int y)
{
    m_ivec3 res;
    ivecn_max(res.v, x.v, y, 3);
    return res;
}


/** dst = a > b ? a : b */
O_INLINE
m_ivec3 ivec3_max_v(m_ivec3 x, m_ivec3 y)
{
    m_ivec3 res;
    ivecn_max_v(res.v, x.v, y.v, 3);
    return res;
}


/** returns the index of the min v value */
O_INLINE
int ivec3_min_index(m_ivec3 ivec)
{
    return ivecn_min_index(ivec.v, 3);
}


/** returns the index of the max v value */
O_INLINE
int ivec3_max_index(m_ivec3 ivec)
{
    return ivecn_max_index(ivec.v, 3);
}

/** dst = x < min ? min : (x > max ? max : x) */
O_INLINE
m_ivec3 ivec3_clamp(m_ivec3 x, int min, int max)
{
    m_ivec3 res;
    ivecn_clamp(res.v, x.v, min, max, 3);
    return res;
}


/** dst = x < min ? min : (x > max ? max : x) */
O_INLINE
m_ivec3 ivec3_clamp_v(m_ivec3 x, m_ivec3 min, m_ivec3 max)
{
    m_ivec3 res;
    ivecn_clamp_v(res.v, x.v, min.v, max.v, 3);
    return res;
}


/** dst = a * (1-t) + b * t */
O_INLINE
m_ivec3 ivec3_mix(m_ivec3 a, m_ivec3 b, float t)
{
    m_ivec3 res;
    ivecn_mix(res.v, a.v, b.v, t, 3);
    return res;
}

/** returns v[0] + v[1] + ... + v[n-1] */
O_INLINE
int ivec3_sum(m_ivec3 v)
{
    return ivecn_sum(v.v, 3);
}


#endif //M_VEC_INT_VEC3_H
