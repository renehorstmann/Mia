#ifndef M_VEC_BYTE_VEC2_H
#define M_VEC_BYTE_VEC2_H

/**
 * @file vec/bvec2.h
 *
 * functions working with bvec2
 */



#include "bvecn.h"
#include "m/m_types/byte.h"

/** dst = v0, v1, ... */
O_INLINE
m_bvec2 bvec2_new(obyte v0, obyte v1)
{
    m_bvec2 self;
    self.v0 = v0;
    self.v1 = v1;
    return self;
}

/** dst = (m_bvec2) v */
O_INLINE
m_bvec2 bvec2_cast_float(const float *cast)
{
    m_bvec2 res;
    bvecn_cast_into(res.v, cast, 2);
    return res;
}

/** dst = (m_bvec2) v */
O_INLINE
m_bvec2 bvec2_cast_double(const double *cast)
{
    m_bvec2 res;
    bvecn_cast_into(res.v, cast, 2);
    return res;
}

/** dst = (m_bvec2) v */
O_INLINE
m_bvec2 bvec2_cast_int(const int *cast)
{
    m_bvec2 res;
    bvecn_cast_into(res.v, cast, 2);
    return res;
}

/** dst = v * 255 */
O_INLINE
m_bvec2 bvec2_cast_float_1(const float *cast)
{
    m_bvec2 res;
    bvecn_cast_float_1(res.v, cast, 2);
    return res;
}

/** dst = v * 255 */
O_INLINE
m_bvec2 bvec2_cast_double_1(const double *cast)
{
    m_bvec2 res;
    bvecn_cast_double_1(res.v, cast, 2);
    return res;
}



/** a == b */
O_INLINE
bool bvec2_equals(m_bvec2 a, obyte b)
{
    return bvecn_equals(a.v, b, 2);
}

/** a == b */
O_INLINE
bool bvec2_equals_v(m_bvec2 a, m_bvec2 b)
{
    return bvecn_equals_v(a.v, b.v, 2);
}

/** a ~= b */
O_INLINE
bool bvec2_equals_eps(m_bvec2 a, obyte b, obyte eps)
{
    return bvecn_equals_eps(a.v, b, eps, 2);
}

/** a ~= b */
O_INLINE
bool bvec2_equals_eps_v(m_bvec2 a, m_bvec2 b, obyte eps)
{
    return bvecn_equals_eps_v(a.v, b.v, eps, 2);
}


/** dst = s */
O_INLINE
m_bvec2 bvec2_set(obyte s)
{
    m_bvec2 res;
    bvecn_set(res.v, s, 2);
    return res;
}

/** dst = unit_x */
O_INLINE
m_bvec2 bvec2_unit_x(void)
{
    m_bvec2 res;
    bvecn_unit_x(res.v, 2);
    return res;
}


/** dst = unit_y */
O_INLINE
m_bvec2 bvec2_unit_y(void)
{
    m_bvec2 res;
    bvecn_unit_y(res.v, 2);
    return res;
}


/** dst = unit_z */
O_INLINE
m_bvec2 bvec2_unit_z(void)
{
    m_bvec2 res;
    bvecn_unit_z(res.v, 2);
    return res;
}


/** dst = unit_w */
O_INLINE
m_bvec2 bvec2_unit_w(void)
{
    m_bvec2 res;
    bvecn_unit_w(res.v, 2);
    return res;
}


/** dst = a + b */
O_INLINE
m_bvec2 bvec2_add_v(m_bvec2 a, m_bvec2 b)
{
    m_bvec2 res;
    bvecn_add_v(res.v, a.v, b.v, 2);
    return res;
}


/** dst = a + b */
O_INLINE
m_bvec2 bvec2_add(m_bvec2 a, obyte b)
{
    m_bvec2 res;
    bvecn_add(res.v, a.v, b, 2);
    return res;
}


/** dst = a - b */
O_INLINE
m_bvec2 bvec2_sub_v(m_bvec2 a, m_bvec2 b)
{
    m_bvec2 res;
    bvecn_sub_v(res.v, a.v, b.v, 2);
    return res;
}


/** dst = a - b */
O_INLINE
m_bvec2 bvec2_sub(m_bvec2 a, obyte b)
{
    m_bvec2 res;
    bvecn_sub(res.v, a.v, b, 2);
    return res;
}


/** dst = a * b */
O_INLINE
m_bvec2 bvec2_scale_v(m_bvec2 a, m_bvec2 b)
{
    m_bvec2 res;
    bvecn_scale_v(res.v, a.v, b.v, 2);
    return res;
}


/** dst = a * b */
O_INLINE
m_bvec2 bvec2_scale(m_bvec2 a, obyte b)
{
    m_bvec2 res;
    bvecn_scale(res.v, a.v, b, 2);
    return res;
}


/** dst = a / b */
O_INLINE
m_bvec2 bvec2_div_v(m_bvec2 a, m_bvec2 b)
{
    m_bvec2 res;
    bvecn_div_v(res.v, a.v, b.v, 2);
    return res;
}


/** dst = a / b */
O_INLINE
m_bvec2 bvec2_div(m_bvec2 a, obyte b)
{
    m_bvec2 res;
    bvecn_div(res.v, a.v, b, 2);
    return res;
}

/** dst = a + b * c */
O_INLINE
m_bvec2 bvec2_add_scaled_v(m_bvec2 a, m_bvec2 b, m_bvec2 c)
{
    m_bvec2 res;
    bvecn_add_scaled_v(res.v, a.v, b.v, c.v, 2);
    return res;
}

/** dst = a + b * c */
O_INLINE
m_bvec2 bvec2_add_scaled(m_bvec2 a, m_bvec2 b, obyte c)
{
    m_bvec2 res;
    bvecn_add_scaled(res.v, a.v, b.v, c, 2);
    return res;
}


/** dst = x % y as positive version for integers */
O_INLINE
m_bvec2 bvec2_mod(m_bvec2 x, obyte y)
{
    m_bvec2 res;
    bvecn_mod(res.v, x.v, y, 2);
    return res;
}

/** dst = x % y as positive version for integers */
O_INLINE
m_bvec2 bvec2_mod_v(m_bvec2 x, m_bvec2 y)
{
    m_bvec2 res;
    bvecn_mod_v(res.v, x.v, y.v, 2);
    return res;
}

/** dst = a < b ? a : b */
O_INLINE
m_bvec2 bvec2_min(m_bvec2 x, obyte y)
{
    m_bvec2 res;
    bvecn_min(res.v, x.v, y, 2);
    return res;
}


/** dst = a < b ? a : b */
O_INLINE
m_bvec2 bvec2_min_v(m_bvec2 x, m_bvec2 y)
{
    m_bvec2 res;
    bvecn_min_v(res.v, x.v, y.v, 2);
    return res;
}


/** dst = a > b ? a : b */
O_INLINE
m_bvec2 bvec2_max(m_bvec2 x, obyte y)
{
    m_bvec2 res;
    bvecn_max(res.v, x.v, y, 2);
    return res;
}


/** dst = a > b ? a : b */
O_INLINE
m_bvec2 bvec2_max_v(m_bvec2 x, m_bvec2 y)
{
    m_bvec2 res;
    bvecn_max_v(res.v, x.v, y.v, 2);
    return res;
}


/** returns the index of the min v value */
O_INLINE
int bvec2_min_index(m_bvec2 bvec)
{
    return bvecn_min_index(bvec.v, 2);
}


/** returns the index of the max v value */
O_INLINE
int bvec2_max_index(m_bvec2 bvec)
{
    return bvecn_max_index(bvec.v, 2);
}

/** dst = x < min ? min : (x > max ? max : x) */
O_INLINE
m_bvec2 bvec2_clamp(m_bvec2 x, obyte min, obyte max)
{
    m_bvec2 res;
    bvecn_clamp(res.v, x.v, min, max, 2);
    return res;
}


/** dst = x < min ? min : (x > max ? max : x) */
O_INLINE
m_bvec2 bvec2_clamp_v(m_bvec2 x, m_bvec2 min, m_bvec2 max)
{
    m_bvec2 res;
    bvecn_clamp_v(res.v, x.v, min.v, max.v, 2);
    return res;
}


/** dst = a * (1-t) + b * t */
O_INLINE
m_bvec2 bvec2_mix(m_bvec2 a, m_bvec2 b, obyte t)
{
    m_bvec2 res;
    bvecn_mix(res.v, a.v, b.v, t, 2);
    return res;
}


/** returns v[0] + v[1] + ... + v[n-1] */
O_INLINE
int bvec2_sum(m_bvec2 v)
{
    return bvecn_sum(v.v, 2);
}


#endif //M_VEC_BYTE_VEC2_H
