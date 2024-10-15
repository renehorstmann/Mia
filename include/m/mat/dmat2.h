#ifndef M_MAT_DBLMAT2_H
#define M_MAT_DBLMAT2_H

/**
 * @file mat/m_dmat2.h
 *
 * functions working with m_dmat2
 */


#include "dmatn.h"
#include "m/m_types/dbl.h"


/** dst = v0, v1, ... */
O_INLINE
m_dmat2 dmat2_new(double v0, double v1,
                  double v2, double v3)
{
    m_dmat2 self;
    self.v0 = v0;
    self.v1 = v1;
    self.v2 = v2;
    self.v3 = v3;
    return self;
}


/** dst = (m_dmat2) v */
O_INLINE
m_dmat2 dmat2_cast_float(const float *cast)
{
    m_dmat2 res;
    dmatn_cast_into(res.v, cast, 2);
    return res;
}

///** dst = (m_dmat2) v */
//O_INLINE
//m_dmat2 dmat2_cast_double(const double *cast) {
//    m_dmat2 res;
//    dmatn_cast_into(res.v, cast, 2);
//    return res;
//}

/** dst = r==c ? 1 : 0 (identity)  */
O_INLINE
m_dmat2 dmat2_eye(void)
{
    m_dmat2 res;
    dmatn_eye(res.v, 2);
    return res;
}

/** dst = m[row][:] */
O_INLINE
m_dvec2 dmat2_get_row(m_dmat2 m, int row)
{
    m_dvec2 res;
    dmatn_get_row(res.v, m.v, row, 2);
    return res;
}


/** dst = m[:][col] */
O_INLINE
m_dvec2 dmat2_get_col(m_dmat2 m, int col)
{
    m_dvec2 res;
    dmatn_get_col(res.v, m.v, col, 2);
    return res;
}


/** dst = m; dst[row][:] = v */
O_INLINE
m_dmat2 dmat2_set_row(m_dmat2 m, m_dvec2 v, int row)
{
    dmatn_set_row(m.v, v.v, row, 2);
    return m;
}

/** m[row][:] = v; dst = m */
O_INLINE
m_dmat2 dmat2_set_this_row(m_dmat2 *m, m_dvec2 v, int row)
{
    dmatn_set_row(m->v, v.v, row, 2);
    return *m;
}

/** dst = m; dst[:][col] = v */
O_INLINE
m_dmat2 dmat2_set_col(m_dmat2 m, m_dvec2 v, int col)
{
    dmatn_set_col(m.v, v.v, col, 2);
    return m;
}

/** dmat[:][col] = v; dst = m */
O_INLINE
m_dmat2 dmat2_set_this_col(m_dmat2 *dmat, m_dvec2 v, int col)
{
    dmatn_set_col(dmat->v, v.v, col, 2);
    return *dmat;
}


/** dst = m; dst[row][:] = s */
O_INLINE
m_dmat2 dmat2_set_row_m(m_dmat2 m, double s, int row)
{
    dmatn_set_row_m(m.v, s, row, 2);
    return m;
}

/** m[row][:] = s; dst = m */
O_INLINE
m_dmat2 dmat2_set_this_row_m(m_dmat2 *m, double s, int row)
{
    dmatn_set_row_m(m->v, s, row, 2);
    return *m;
}


/** dst = m; dst[:][col] = s */
O_INLINE
m_dmat2 dmat2_set_col_m(m_dmat2 m, double s, int col)
{
    dmatn_set_col_m(m.v, s, col, 2);
    return m;
}

/** m[:][col] = s; dst = m */
O_INLINE
m_dmat2 dmat2_set_this_col_m(m_dmat2 *m, double s, int col)
{
    dmatn_set_col_m(m->v, s, col, 2);
    return *m;
}


/** returns sum of diagonal form upper left to lower right */
O_INLINE
double dmat2_trace(m_dmat2 m)
{
    return dmatn_trace(m.v, 2);
}


/** dst = m^t */
O_INLINE
m_dmat2 dmat2_transpose(m_dmat2 m)
{
    m_dmat2 res;
    dmatn_transpose_no_alias(res.v, m.v, 2);
    return res;
}


/** dst = a @ b */
O_INLINE
m_dmat2 dmat2_mul_mat(m_dmat2 a, m_dmat2 b)
{
    m_dmat2 res;
    dmatn_mul_mat_no_alias(res.v, a.v, b.v, 2);
    return res;
}


/** dst = a @ b */
O_INLINE
m_dvec2 dmat2_mul_vec(m_dmat2 a, m_dvec2 b)
{
    m_dvec2 res;
    dmatn_mul_vec_no_alias(res.v, a.v, b.v, 2);
    return res;
}


/** dst = a @ b */
O_INLINE
m_dvec2 dvec2_mul_mat(m_dvec2 a, m_dmat2 b)
{
    m_dvec2 res;
    dvecn_mul_mat_no_alias(res.v, a.v, b.v, 2);
    return res;
}


/** returns = determinant mm */
O_INLINE
double dmat2_det(m_dmat2 m)
{
    return dmatn_det(m.v, 2);
}


/** dst = inverted mm */
O_INLINE
m_dmat2 dmat2_inv(m_dmat2 m)
{
    m_dmat2 res;
    dmatn_inv(res.v, m.v, 2);
    return res;
}

/** dst = m[col:col+2, row:row+2] */
O_INLINE
m_dmat2 dmat2_get_block2(m_dmat2 m, int row, int col)
{
    m_dmat2 res;
    dmatn_get_block(res.v, m.v, row, col, 2, 2);
    return res;
}


/** dst = m; dst[col:col+2, row:row+2] = block */
O_INLINE
m_dmat2 dmat2_set_block2(m_dmat2 m, m_dmat2 block_2, int row, int col)
{
    dmatn_set_block(m.v, block_2.v, row, col, 2, 2);
    return m;
}


/** m[col:col+2, row:row+2] = block; dst = m */
O_INLINE
m_dmat2 dmat2_set_this_block2(m_dmat2 *m, m_dmat2 block_2, int row, int col)
{
    dmatn_set_block(m->v, block_2.v, row, col, 2, 2);
    return *m;
}


/** dst = m[col:col+2, row:row+2] */
O_INLINE
m_dmat3 dmat2_get_block3(m_dmat2 m, int row, int col)
{
    m_dmat3 res;
    dmatn_get_block(res.v, m.v, row, col, 3, 2);
    return res;
}


/** dst = m; dst[col:col+2, row:row+2] = block */
O_INLINE
m_dmat2 dmat2_set_block3(m_dmat2 m, m_dmat3 block_3, int row, int col)
{
    dmatn_set_block(m.v, block_3.v, row, col, 3, 2);
    return m;
}

/** m[col:col+2, row:row+2] = block; dst = m */
O_INLINE
m_dmat2 dmat2_set_this_block3(m_dmat2 *m, m_dmat3 block_3, int row, int col)
{
    dmatn_set_block(m->v, block_3.v, row, col, 3, 2);
    return *m;
}


/** dst = m[:2, :2] */
O_INLINE
m_dmat2 dmat2_get_upper_left2(m_dmat2 m)
{
    return dmat2_get_block2(m, 0, 0);
}


/** dst = m; dst[:2, :2] = block */
O_INLINE
m_dmat2 dmat2_set_upper_left2(m_dmat2 m, m_dmat2 block_2)
{
    return dmat2_set_block2(m, block_2, 0, 0);
}

/** m[:2, :2] = block; dst = m */
O_INLINE
m_dmat2 dmat2_set_this_upper_left2(m_dmat2 *m, m_dmat2 block_2)
{
    return dmat2_set_this_block2(m, block_2, 0, 0);
}


/** dst = m[:3, :3] */
O_INLINE
m_dmat3 dmat2_get_upper_left3(m_dmat2 m)
{
    return dmat2_get_block3(m, 0, 0);
}


/** dst = m; dst[:3, :3] = block */
O_INLINE
m_dmat2 dmat2_set_upper_left3(m_dmat2 m, m_dmat3 block_3)
{
    return dmat2_set_block3(m, block_3, 0, 0);
}

/** m[:3, :3] = block; dst = m */
O_INLINE
m_dmat2 dmat2_set_this_upper_left3(m_dmat2 *m, m_dmat3 block_3)
{
    return dmat2_set_this_block3(m, block_3, 0, 0);
}

#endif //M_MAT_DBLMAT2_H
