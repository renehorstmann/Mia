#ifndef M_VEC_BYTE_VEC4_H
#define M_VEC_BYTE_VEC4_H

/**
 * @file vec/bvec4.h
 *
 * functions working with bvec4
 */


#include "bvecn.h"
#include "m/m_types/byte.h"

/** dst = v0, v1, ... */
O_INLINE
m_bvec4 bvec4_new(obyte v0, obyte v1, obyte v2, obyte v3)
{
    m_bvec4 self;
    self.v0 = v0;
    self.v1 = v1;
    self.v2 = v2;
    self.v3 = v3;
    return self;
}


/** dst = (m_bvec4) v */
O_INLINE
m_bvec4 bvec4_cast_float(const float *cast)
{
    m_bvec4 res;
    bvecn_cast_into(res.v, cast, 4);
    return res;
}

/** dst = (m_bvec4) v */
O_INLINE
m_bvec4 bvec4_cast_double(const double *cast)
{
    m_bvec4 res;
    bvecn_cast_into(res.v, cast, 4);
    return res;
}

/** dst = (m_bvec4) v */
O_INLINE
m_bvec4 bvec4_cast_int(const int *cast)
{
    m_bvec4 res;
    bvecn_cast_into(res.v, cast, 4);
    return res;
}

/** dst = v * 455 */
O_INLINE
m_bvec4 bvec4_cast_float_1(const float *cast)
{
    m_bvec4 res;
    bvecn_cast_float_1(res.v, cast, 4);
    return res;
}

/** dst = v * 455 */
O_INLINE
m_bvec4 bvec4_cast_double_1(const double *cast)
{
    m_bvec4 res;
    bvecn_cast_double_1(res.v, cast, 4);
    return res;
}



/** a == b */
O_INLINE
bool bvec4_equals(m_bvec4 a, obyte b)
{
    return bvecn_equals(a.v, b, 4);
}

/** a == b */
O_INLINE
bool bvec4_equals_v(m_bvec4 a, m_bvec4 b)
{
    return bvecn_equals_v(a.v, b.v, 4);
}

/** a ~= b */
O_INLINE
bool bvec4_equals_eps(m_bvec4 a, obyte b, obyte eps)
{
    return bvecn_equals_eps(a.v, b, eps, 4);
}

/** a ~= b */
O_INLINE
bool bvec4_equals_eps_v(m_bvec4 a, m_bvec4 b, obyte eps)
{
    return bvecn_equals_eps_v(a.v, b.v, eps, 4);
}


/** dst = s */
O_INLINE
m_bvec4 bvec4_set(obyte s)
{
    m_bvec4 res;
    bvecn_set(res.v, s, 4);
    return res;
}

/** dst = unit_x */
O_INLINE
m_bvec4 bvec4_unit_x(void)
{
    m_bvec4 res;
    bvecn_unit_x(res.v, 4);
    return res;
}


/** dst = unit_y */
O_INLINE
m_bvec4 bvec4_unit_y(void)
{
    m_bvec4 res;
    bvecn_unit_y(res.v, 4);
    return res;
}


/** dst = unit_z */
O_INLINE
m_bvec4 bvec4_unit_z(void)
{
    m_bvec4 res;
    bvecn_unit_z(res.v, 4);
    return res;
}


/** dst = unit_w */
O_INLINE
m_bvec4 bvec4_unit_w(void)
{
    m_bvec4 res;
    bvecn_unit_w(res.v, 4);
    return res;
}


/** dst = a + b */
O_INLINE
m_bvec4 bvec4_add_v(m_bvec4 a, m_bvec4 b)
{
    m_bvec4 res;
    bvecn_add_v(res.v, a.v, b.v, 4);
    return res;
}


/** dst = a + b */
O_INLINE
m_bvec4 bvec4_add(m_bvec4 a, obyte b)
{
    m_bvec4 res;
    bvecn_add(res.v, a.v, b, 4);
    return res;
}


/** dst = a - b */
O_INLINE
m_bvec4 bvec4_sub_v(m_bvec4 a, m_bvec4 b)
{
    m_bvec4 res;
    bvecn_sub_v(res.v, a.v, b.v, 4);
    return res;
}


/** dst = a - b */
O_INLINE
m_bvec4 bvec4_sub(m_bvec4 a, obyte b)
{
    m_bvec4 res;
    bvecn_sub(res.v, a.v, b, 4);
    return res;
}


/** dst = a * b */
O_INLINE
m_bvec4 bvec4_scale_v(m_bvec4 a, m_bvec4 b)
{
    m_bvec4 res;
    bvecn_scale_v(res.v, a.v, b.v, 4);
    return res;
}


/** dst = a * b */
O_INLINE
m_bvec4 bvec4_scale(m_bvec4 a, obyte b)
{
    m_bvec4 res;
    bvecn_scale(res.v, a.v, b, 4);
    return res;
}


/** dst = a / b */
O_INLINE
m_bvec4 bvec4_div_v(m_bvec4 a, m_bvec4 b)
{
    m_bvec4 res;
    bvecn_div_v(res.v, a.v, b.v, 4);
    return res;
}


/** dst = a / b */
O_INLINE
m_bvec4 bvec4_div(m_bvec4 a, obyte b)
{
    m_bvec4 res;
    bvecn_div(res.v, a.v, b, 4);
    return res;
}

/** dst = a + b * c */
O_INLINE
m_bvec4 bvec4_add_scaled_v(m_bvec4 a, m_bvec4 b, m_bvec4 c)
{
    m_bvec4 res;
    bvecn_add_scaled_v(res.v, a.v, b.v, c.v, 4);
    return res;
}

/** dst = a + b * c */
O_INLINE
m_bvec4 bvec4_add_scaled(m_bvec4 a, m_bvec4 b, obyte c)
{
    m_bvec4 res;
    bvecn_add_scaled(res.v, a.v, b.v, c, 4);
    return res;
}


/** dst = x % y as positive version for integers */
O_INLINE
m_bvec4 bvec4_mod(m_bvec4 x, obyte y)
{
    m_bvec4 res;
    bvecn_mod(res.v, x.v, y, 4);
    return res;
}

/** dst = x % y as positive version for integers */
O_INLINE
m_bvec4 bvec4_mod_v(m_bvec4 x, m_bvec4 y)
{
    m_bvec4 res;
    bvecn_mod_v(res.v, x.v, y.v, 4);
    return res;
}


/** dst = a < b ? a : b */
O_INLINE
m_bvec4 bvec4_min(m_bvec4 x, obyte y)
{
    m_bvec4 res;
    bvecn_min(res.v, x.v, y, 4);
    return res;
}


/** dst = a < b ? a : b */
O_INLINE
m_bvec4 bvec4_min_v(m_bvec4 x, m_bvec4 y)
{
    m_bvec4 res;
    bvecn_min_v(res.v, x.v, y.v, 4);
    return res;
}


/** dst = a > b ? a : b */
O_INLINE
m_bvec4 bvec4_max(m_bvec4 x, obyte y)
{
    m_bvec4 res;
    bvecn_max(res.v, x.v, y, 4);
    return res;
}


/** dst = a > b ? a : b */
O_INLINE
m_bvec4 bvec4_max_v(m_bvec4 x, m_bvec4 y)
{
    m_bvec4 res;
    bvecn_max_v(res.v, x.v, y.v, 4);
    return res;
}


/** returns the index of the min v value */
O_INLINE
int bvec4_min_index(m_bvec4 bvec)
{
    return bvecn_min_index(bvec.v, 4);
}


/** returns the index of the max v value */
O_INLINE
int bvec4_max_index(m_bvec4 bvec)
{
    return bvecn_max_index(bvec.v, 4);
}

/** dst = x < min ? min : (x > max ? max : x) */
O_INLINE
m_bvec4 bvec4_clamp(m_bvec4 x, obyte min, obyte max)
{
    m_bvec4 res;
    bvecn_clamp(res.v, x.v, min, max, 4);
    return res;
}


/** dst = x < min ? min : (x > max ? max : x) */
O_INLINE
m_bvec4 bvec4_clamp_v(m_bvec4 x, m_bvec4 min, m_bvec4 max)
{
    m_bvec4 res;
    bvecn_clamp_v(res.v, x.v, min.v, max.v, 4);
    return res;
}


/** dst = a * (1-t) + b * t */
O_INLINE
m_bvec4 bvec4_mix(m_bvec4 a, m_bvec4 b, obyte t)
{
    m_bvec4 res;
    bvecn_mix(res.v, a.v, b.v, t, 4);
    return res;
}


/** returns v[0] + v[1] + ... + v[n-1] */
O_INLINE
int bvec4_sum(m_bvec4 v)
{
    return bvecn_sum(v.v, 4);
}

#endif //M_VEC_BYTE_VEC4_H
