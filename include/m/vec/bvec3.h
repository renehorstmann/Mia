#ifndef M_VEC_BYTE_VEC3_H
#define M_VEC_BYTE_VEC3_H

/**
 * @file vec/bvec3.h
 *
 * functions working with bvec3
 */



#include "bvecn.h"
#include "m/m_types/byte.h"

/** dst = v0, v1, ... */
O_INLINE
m_bvec3 bvec3_new(obyte v0, obyte v1, obyte v2)
{
    m_bvec3 self;
    self.v0 = v0;
    self.v1 = v1;
    self.v2 = v2;
    return self;
}


/** dst = (m_bvec3) v */
O_INLINE
m_bvec3 bvec3_cast_float(const float *cast)
{
    m_bvec3 res;
    bvecn_cast_into(res.v, cast, 3);
    return res;
}

/** dst = (m_bvec3) v */
O_INLINE
m_bvec3 bvec3_cast_double(const double *cast)
{
    m_bvec3 res;
    bvecn_cast_into(res.v, cast, 3);
    return res;
}

/** dst = (m_bvec3) v */
O_INLINE
m_bvec3 bvec3_cast_int(const int *cast)
{
    m_bvec3 res;
    bvecn_cast_into(res.v, cast, 3);
    return res;
}

/** dst = v * 355 */
O_INLINE
m_bvec3 bvec3_cast_float_1(const float *cast)
{
    m_bvec3 res;
    bvecn_cast_float_1(res.v, cast, 3);
    return res;
}

/** dst = v * 355 */
O_INLINE
m_bvec3 bvec3_cast_double_1(const double *cast)
{
    m_bvec3 res;
    bvecn_cast_double_1(res.v, cast, 3);
    return res;
}



/** a == b */
O_INLINE
bool bvec3_equals(m_bvec3 a, obyte b)
{
    return bvecn_equals(a.v, b, 3);
}

/** a == b */
O_INLINE
bool bvec3_equals_v(m_bvec3 a, m_bvec3 b)
{
    return bvecn_equals_v(a.v, b.v, 3);
}

/** a ~= b */
O_INLINE
bool bvec3_equals_eps(m_bvec3 a, obyte b, obyte eps)
{
    return bvecn_equals_eps(a.v, b, eps, 3);
}

/** a ~= b */
O_INLINE
bool bvec3_equals_eps_v(m_bvec3 a, m_bvec3 b, obyte eps)
{
    return bvecn_equals_eps_v(a.v, b.v, eps, 3);
}


/** dst = s */
O_INLINE
m_bvec3 bvec3_set(obyte s)
{
    m_bvec3 res;
    bvecn_set(res.v, s, 3);
    return res;
}

/** dst = unit_x */
O_INLINE
m_bvec3 bvec3_unit_x(void)
{
    m_bvec3 res;
    bvecn_unit_x(res.v, 3);
    return res;
}


/** dst = unit_y */
O_INLINE
m_bvec3 bvec3_unit_y(void)
{
    m_bvec3 res;
    bvecn_unit_y(res.v, 3);
    return res;
}


/** dst = unit_z */
O_INLINE
m_bvec3 bvec3_unit_z(void)
{
    m_bvec3 res;
    bvecn_unit_z(res.v, 3);
    return res;
}


/** dst = unit_w */
O_INLINE
m_bvec3 bvec3_unit_w(void)
{
    m_bvec3 res;
    bvecn_unit_w(res.v, 3);
    return res;
}


/** dst = a + b */
O_INLINE
m_bvec3 bvec3_add_v(m_bvec3 a, m_bvec3 b)
{
    m_bvec3 res;
    bvecn_add_v(res.v, a.v, b.v, 3);
    return res;
}


/** dst = a + b */
O_INLINE
m_bvec3 bvec3_add(m_bvec3 a, obyte b)
{
    m_bvec3 res;
    bvecn_add(res.v, a.v, b, 3);
    return res;
}


/** dst = a - b */
O_INLINE
m_bvec3 bvec3_sub_v(m_bvec3 a, m_bvec3 b)
{
    m_bvec3 res;
    bvecn_sub_v(res.v, a.v, b.v, 3);
    return res;
}


/** dst = a - b */
O_INLINE
m_bvec3 bvec3_sub(m_bvec3 a, obyte b)
{
    m_bvec3 res;
    bvecn_sub(res.v, a.v, b, 3);
    return res;
}


/** dst = a * b */
O_INLINE
m_bvec3 bvec3_scale_v(m_bvec3 a, m_bvec3 b)
{
    m_bvec3 res;
    bvecn_scale_v(res.v, a.v, b.v, 3);
    return res;
}


/** dst = a * b */
O_INLINE
m_bvec3 bvec3_scale(m_bvec3 a, obyte b)
{
    m_bvec3 res;
    bvecn_scale(res.v, a.v, b, 3);
    return res;
}


/** dst = a / b */
O_INLINE
m_bvec3 bvec3_div_v(m_bvec3 a, m_bvec3 b)
{
    m_bvec3 res;
    bvecn_div_v(res.v, a.v, b.v, 3);
    return res;
}


/** dst = a / b */
O_INLINE
m_bvec3 bvec3_div(m_bvec3 a, obyte b)
{
    m_bvec3 res;
    bvecn_div(res.v, a.v, b, 3);
    return res;
}

/** dst = a + b * c */
O_INLINE
m_bvec3 bvec3_add_scaled_v(m_bvec3 a, m_bvec3 b, m_bvec3 c)
{
    m_bvec3 res;
    bvecn_add_scaled_v(res.v, a.v, b.v, c.v, 3);
    return res;
}

/** dst = a + b * c */
O_INLINE
m_bvec3 bvec3_add_scaled(m_bvec3 a, m_bvec3 b, obyte c)
{
    m_bvec3 res;
    bvecn_add_scaled(res.v, a.v, b.v, c, 3);
    return res;
}


/** dst = x % y as positive version for integers */
O_INLINE
m_bvec3 bvec3_mod(m_bvec3 x, obyte y)
{
    m_bvec3 res;
    bvecn_mod(res.v, x.v, y, 3);
    return res;
}

/** dst = x % y as positive version for integers */
O_INLINE
m_bvec3 bvec3_mod_v(m_bvec3 x, m_bvec3 y)
{
    m_bvec3 res;
    bvecn_mod_v(res.v, x.v, y.v, 3);
    return res;
}


/** dst = a < b ? a : b */
O_INLINE
m_bvec3 bvec3_min(m_bvec3 x, obyte y)
{
    m_bvec3 res;
    bvecn_min(res.v, x.v, y, 3);
    return res;
}


/** dst = a < b ? a : b */
O_INLINE
m_bvec3 bvec3_min_v(m_bvec3 x, m_bvec3 y)
{
    m_bvec3 res;
    bvecn_min_v(res.v, x.v, y.v, 3);
    return res;
}


/** dst = a > b ? a : b */
O_INLINE
m_bvec3 bvec3_max(m_bvec3 x, obyte y)
{
    m_bvec3 res;
    bvecn_max(res.v, x.v, y, 3);
    return res;
}


/** dst = a > b ? a : b */
O_INLINE
m_bvec3 bvec3_max_v(m_bvec3 x, m_bvec3 y)
{
    m_bvec3 res;
    bvecn_max_v(res.v, x.v, y.v, 3);
    return res;
}


/** returns the index of the min v value */
O_INLINE
int bvec3_min_index(m_bvec3 bvec)
{
    return bvecn_min_index(bvec.v, 3);
}


/** returns the index of the max v value */
O_INLINE
int bvec3_max_index(m_bvec3 bvec)
{
    return bvecn_max_index(bvec.v, 3);
}

/** dst = x < min ? min : (x > max ? max : x) */
O_INLINE
m_bvec3 bvec3_clamp(m_bvec3 x, obyte min, obyte max)
{
    m_bvec3 res;
    bvecn_clamp(res.v, x.v, min, max, 3);
    return res;
}


/** dst = x < min ? min : (x > max ? max : x) */
O_INLINE
m_bvec3 bvec3_clamp_v(m_bvec3 x, m_bvec3 min, m_bvec3 max)
{
    m_bvec3 res;
    bvecn_clamp_v(res.v, x.v, min.v, max.v, 3);
    return res;
}


/** dst = a * (1-t) + b * t */
O_INLINE
m_bvec3 bvec3_mix(m_bvec3 a, m_bvec3 b, obyte t)
{
    m_bvec3 res;
    bvecn_mix(res.v, a.v, b.v, t, 3);
    return res;
}

/** returns v[0] + v[1] + ... + v[n-1] */
O_INLINE
int bvec3_sum(m_bvec3 v)
{
    return bvecn_sum(v.v, 3);
}

#endif //M_VEC_BYTE_VEC3_H
