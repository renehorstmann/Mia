#ifndef M_MAT_DBLMAT4_H
#define M_MAT_DBLMAT4_H

/**
 * @file mat/m_dmat4.h
 *
 * functions working with m_dmat4
 */


#include "dmatn.h"
#include "m/m_types/dbl.h"


/** dst = v0, v1, ... */
O_INLINE
m_dmat4 dmat4_new(double v0, double v1, double v2, double v3,
                  double v4, double v5, double v6, double v7,
                  double v8, double v9, double v10, double v11,
                  double v12, double v13, double v14, double v15)
{
    m_dmat4 self;
    self.v0 = v0;
    self.v1 = v1;
    self.v2 = v2;
    self.v3 = v3;
    self.v4 = v4;
    self.v5 = v5;
    self.v6 = v6;
    self.v7 = v7;
    self.v8 = v8;
    self.v9 = v9;
    self.v10 = v10;
    self.v11 = v11;
    self.v12 = v12;
    self.v13 = v13;
    self.v14 = v14;
    self.v15 = v15;
    return self;
}


/** dst = (m_dmat4) v */
O_INLINE
m_dmat4 dmat4_cast_float(const float *cast)
{
    m_dmat4 res;
    dmatn_cast_into(res.v, cast, 4);
    return res;
}

///** dst = (m_dmat4) v */
//O_INLINE
//m_dmat4 dmat4_cast_double(const double *cast) {
//    m_dmat4 res;
//    dmatn_cast_into(res.v, cast, 4);
//    return res;
//}


/** dst = r==c ? 1 : 0 (identity)  */
O_INLINE
m_dmat4 dmat4_eye(void)
{
    m_dmat4 res;
    dmatn_eye(res.v, 4);
    return res;
}

/** dst = m[row][:] */
O_INLINE
m_dvec4 dmat4_get_row(m_dmat4 m, int row)
{
    m_dvec4 res;
    dmatn_get_row(res.v, m.v, row, 4);
    return res;
}


/** dst = m[:][col] */
O_INLINE
m_dvec4 dmat4_get_col(m_dmat4 m, int col)
{
    m_dvec4 res;
    dmatn_get_col(res.v, m.v, col, 4);
    return res;
}


/** dst = m; dst[row][:] = v */
O_INLINE
m_dmat4 dmat4_set_row(m_dmat4 m, m_dvec4 v, int row)
{
    dmatn_set_row(m.v, v.v, row, 4);
    return m;
}

/** m[row][:] = v; dst = m */
O_INLINE
m_dmat4 dmat4_set_this_row(m_dmat4 *m, m_dvec4 v, int row)
{
    dmatn_set_row(m->v, v.v, row, 4);
    return *m;
}

/** dst = m; dst[:][col] = v */
O_INLINE
m_dmat4 dmat4_set_col(m_dmat4 m, m_dvec4 v, int col)
{
    dmatn_set_col(m.v, v.v, col, 4);
    return m;
}

/** dmat[:][col] = v; dst = m */
O_INLINE
m_dmat4 dmat4_set_this_col(m_dmat4 *dmat, m_dvec4 v, int col)
{
    dmatn_set_col(dmat->v, v.v, col, 4);
    return *dmat;
}


/** dst = m; dst[row][:] = s */
O_INLINE
m_dmat4 dmat4_set_row_m(m_dmat4 m, double s, int row)
{
    dmatn_set_row_m(m.v, s, row, 4);
    return m;
}

/** m[row][:] = s; dst = m */
O_INLINE
m_dmat4 dmat4_set_this_row_m(m_dmat4 *m, double s, int row)
{
    dmatn_set_row_m(m->v, s, row, 4);
    return *m;
}


/** dst = m; dst[:][col] = s */
O_INLINE
m_dmat4 dmat4_set_col_m(m_dmat4 m, double s, int col)
{
    dmatn_set_col_m(m.v, s, col, 4);
    return m;
}

/** m[:][col] = s; dst = m */
O_INLINE
m_dmat4 dmat4_set_this_col_m(m_dmat4 *m, double s, int col)
{
    dmatn_set_col_m(m->v, s, col, 4);
    return *m;
}


/** returns sum of diagonal form upper left to lower right */
O_INLINE
double dmat4_trace(m_dmat4 m)
{
    return dmatn_trace(m.v, 4);
}


/** dst = m^t */
O_INLINE
m_dmat4 dmat4_transpose(m_dmat4 m)
{
    m_dmat4 res;
    dmatn_transpose_no_alias(res.v, m.v, 4);
    return res;
}


/** dst = a @ b */
O_INLINE
m_dmat4 dmat4_mul_mat(m_dmat4 a, m_dmat4 b)
{
    m_dmat4 res;
    dmatn_mul_mat_no_alias(res.v, a.v, b.v, 4);
    return res;
}


/** dst = a @ b */
O_INLINE
m_dvec4 dmat4_mul_vec(m_dmat4 a, m_dvec4 b)
{
    m_dvec4 res;
    dmatn_mul_vec_no_alias(res.v, a.v, b.v, 4);
    return res;
}


/** dst = a @ b */
O_INLINE
m_dvec4 dvec4_mul_mat(m_dvec4 a, m_dmat4 b)
{
    m_dvec4 res;
    dvecn_mul_mat_no_alias(res.v, a.v, b.v, 4);
    return res;
}


/** returns = determinant mm */
O_INLINE
double dmat4_det(m_dmat4 m)
{
    return dmatn_det(m.v, 4);
}


/** dst = inverted mm */
O_INLINE
m_dmat4 dmat4_inv(m_dmat4 m)
{
    m_dmat4 res;
    dmatn_inv(res.v, m.v, 4);
    return res;
}

/** dst = m[col:col+2, row:row+2] */
O_INLINE
m_dmat2 dmat4_get_block2(m_dmat4 m, int row, int col)
{
    m_dmat2 res;
    dmatn_get_block(res.v, m.v, row, col, 2, 4);
    return res;
}


/** dst = m; dst[col:col+2, row:row+2] = block */
O_INLINE
m_dmat4 dmat4_set_block2(m_dmat4 m, m_dmat2 block_2, int row, int col)
{
    dmatn_set_block(m.v, block_2.v, row, col, 2, 4);
    return m;
}


/** m[col:col+2, row:row+2] = block; dst = m */
O_INLINE
m_dmat4 dmat4_set_this_block2(m_dmat4 *m, m_dmat2 block_2, int row, int col)
{
    dmatn_set_block(m->v, block_2.v, row, col, 2, 4);
    return *m;
}


/** dst = m[col:col+2, row:row+2] */
O_INLINE
m_dmat3 dmat4_get_block3(m_dmat4 m, int row, int col)
{
    m_dmat3 res;
    dmatn_get_block(res.v, m.v, row, col, 3, 4);
    return res;
}


/** dst = m; dst[col:col+2, row:row+2] = block */
O_INLINE
m_dmat4 dmat4_set_block3(m_dmat4 m, m_dmat3 block_3, int row, int col)
{
    dmatn_set_block(m.v, block_3.v, row, col, 3, 4);
    return m;
}

/** m[col:col+2, row:row+2] = block; dst = m */
O_INLINE
m_dmat4 dmat4_set_this_block3(m_dmat4 *m, m_dmat3 block_3, int row, int col)
{
    dmatn_set_block(m->v, block_3.v, row, col, 3, 4);
    return *m;
}


/** dst = m[:2, :2] */
O_INLINE
m_dmat2 dmat4_get_upper_left2(m_dmat4 m)
{
    return dmat4_get_block2(m, 0, 0);
}


/** dst = m; dst[:2, :2] = block */
O_INLINE
m_dmat4 dmat4_set_upper_left2(m_dmat4 m, m_dmat2 block_2)
{
    return dmat4_set_block2(m, block_2, 0, 0);
}

/** m[:2, :2] = block; dst = m */
O_INLINE
m_dmat4 dmat4_set_this_upper_left2(m_dmat4 *m, m_dmat2 block_2)
{
    return dmat4_set_this_block2(m, block_2, 0, 0);
}


/** dst = m[:3, :3] */
O_INLINE
m_dmat3 dmat4_get_upper_left3(m_dmat4 m)
{
    return dmat4_get_block3(m, 0, 0);
}


/** dst = m; dst[:3, :3] = block */
O_INLINE
m_dmat4 dmat4_set_upper_left3(m_dmat4 m, m_dmat3 block_3)
{
    return dmat4_set_block3(m, block_3, 0, 0);
}

/** m[:3, :3] = block; dst = m */
O_INLINE
m_dmat4 dmat4_set_this_upper_left3(m_dmat4 *m, m_dmat3 block_3)
{
    return dmat4_set_this_block3(m, block_3, 0, 0);
}

#endif //M_MAT_DBLMAT4_H
