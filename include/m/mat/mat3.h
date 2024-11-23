#ifndef M_MAT_FLTMAT3_H
#define M_MAT_FLTMAT3_H

/**
 * @file mat/m_mat3.h
 *
 * functions working with m_mat3
 */


#include "matn.h"
#include "m/m_types/flt.h"


/** dst = v0, v1, ... */
O_INLINE
m_mat3 mat3_new(float v0, float v1, float v2,
                float v3, float v4, float v5,
                float v6, float v7, float v8)
{
    m_mat3 self;
    self.v0 = v0;
    self.v1 = v1;
    self.v2 = v2;
    self.v3 = v3;
    self.v4 = v4;
    self.v5 = v5;
    self.v6 = v6;
    self.v7 = v7;
    self.v8 = v8;
    return self;
}


///** dst = (m_mat3) v */
//O_INLINE
//m_mat3 mat3_cast_float(const float *cast) {
//    m_mat3 res;
//    matn_cast_into(res.v, cast, 3);
//    return res;
//}

/** dst = (m_mat3) v */
O_INLINE
m_mat3 mat3_cast_double(const double *cast)
{
    m_mat3 res;
    matn_cast_into(res.v, cast, 3);
    return res;
}


/** dst = r==c ? 1 : 0 (identity)  */
O_INLINE
m_mat3 mat3_eye(void)
{
    m_mat3 res;
    matn_eye(res.v, 3);
    return res;
}

/** dst = m[row][:] */
O_INLINE
m_vec3 mat3_get_row(m_mat3 m, int row)
{
    m_vec3 res;
    matn_get_row(res.v, m.v, row, 3);
    return res;
}


/** dst = m[:][col] */
O_INLINE
m_vec3 mat3_get_col(m_mat3 m, int col)
{
    m_vec3 res;
    matn_get_col(res.v, m.v, col, 3);
    return res;
}


/** dst = m; dst[row][:] = v */
O_INLINE
m_mat3 mat3_set_row(m_mat3 m, m_vec3 v, int row)
{
    matn_set_row(m.v, v.v, row, 3);
    return m;
}

/** m[row][:] = v; dst = m */
O_INLINE
m_mat3 mat3_set_this_row(m_mat3 *m, m_vec3 v, int row)
{
    matn_set_row(m->v, v.v, row, 3);
    return *m;
}

/** dst = m; dst[:][col] = v */
O_INLINE
m_mat3 mat3_set_col(m_mat3 m, m_vec3 v, int col)
{
    matn_set_col(m.v, v.v, col, 3);
    return m;
}

/** mat[:][col] = v; dst = m */
O_INLINE
m_mat3 mat3_set_this_col(m_mat3 *mat, m_vec3 v, int col)
{
    matn_set_col(mat->v, v.v, col, 3);
    return *mat;
}


/** dst = m; dst[row][:] = s */
O_INLINE
m_mat3 mat3_set_row_m(m_mat3 m, float s, int row)
{
    matn_set_row_m(m.v, s, row, 3);
    return m;
}

/** m[row][:] = s; dst = m */
O_INLINE
m_mat3 mat3_set_this_row_m(m_mat3 *m, float s, int row)
{
    matn_set_row_m(m->v, s, row, 3);
    return *m;
}


/** dst = m; dst[:][col] = s */
O_INLINE
m_mat3 mat3_set_col_m(m_mat3 m, float s, int col)
{
    matn_set_col_m(m.v, s, col, 3);
    return m;
}

/** m[:][col] = s; dst = m */
O_INLINE
m_mat3 mat3_set_this_col_m(m_mat3 *m, float s, int col)
{
    matn_set_col_m(m->v, s, col, 3);
    return *m;
}


/** returns sum of diagonal form upper left to lower right */
O_INLINE
float mat3_trace(m_mat3 m)
{
    return matn_trace(m.v, 3);
}


/** dst = m^t */
O_INLINE
m_mat3 mat3_transpose(m_mat3 m)
{
    m_mat3 res;
    matn_transpose_no_alias(res.v, m.v, 3);
    return res;
}


/** dst = a @ b */
O_INLINE
m_mat3 mat3_mul_mat(m_mat3 a, m_mat3 b)
{
    m_mat3 res;
    matn_mul_mat_no_alias(res.v, a.v, b.v, 3);
    return res;
}


/** dst = a @ b */
O_INLINE
m_vec3 mat3_mul_vec(m_mat3 a, m_vec3 b)
{
    m_vec3 res;
    matn_mul_vec_no_alias(res.v, a.v, b.v, 3);
    return res;
}


/** dst = a @ b */
O_INLINE
m_vec3 vec3_mul_mat(m_vec3 a, m_mat3 b)
{
    m_vec3 res;
    vecn_mul_mat_no_alias(res.v, a.v, b.v, 3);
    return res;
}


/** returns = determinant mm */
O_INLINE
float mat3_det(m_mat3 m)
{
    return matn_det(m.v, 3);
}


/** dst = inverted mm */
O_INLINE
m_mat3 mat3_inv(m_mat3 m)
{
    m_mat3 res;
    matn_inv(res.v, m.v, 3);
    return res;
}

/** dst = m[col:col+2, row:row+2] */
O_INLINE
m_mat2 mat3_get_block2(m_mat3 m, int row, int col)
{
    m_mat2 res;
    matn_get_block(res.v, m.v, row, col, 2, 3);
    return res;
}


/** dst = m; dst[col:col+2, row:row+2] = block */
O_INLINE
m_mat3 mat3_set_block2(m_mat3 m, m_mat2 block_2, int row, int col)
{
    matn_set_block(m.v, block_2.v, row, col, 2, 3);
    return m;
}


/** m[col:col+2, row:row+2] = block; dst = m */
O_INLINE
m_mat3 mat3_set_this_block2(m_mat3 *m, m_mat2 block_2, int row, int col)
{
    matn_set_block(m->v, block_2.v, row, col, 2, 3);
    return *m;
}


/** dst = m[col:col+2, row:row+2] */
O_INLINE
m_mat3 mat3_get_block3(m_mat3 m, int row, int col)
{
    m_mat3 res;
    matn_get_block(res.v, m.v, row, col, 3, 3);
    return res;
}


/** dst = m; dst[col:col+2, row:row+2] = block */
O_INLINE
m_mat3 mat3_set_block3(m_mat3 m, m_mat3 block_3, int row, int col)
{
    matn_set_block(m.v, block_3.v, row, col, 3, 3);
    return m;
}

/** m[col:col+2, row:row+2] = block; dst = m */
O_INLINE
m_mat3 mat3_set_this_block3(m_mat3 *m, m_mat3 block_3, int row, int col)
{
    matn_set_block(m->v, block_3.v, row, col, 3, 3);
    return *m;
}


/** dst = m[:2, :2] */
O_INLINE
m_mat2 mat3_get_upper_left2(m_mat3 m)
{
    return mat3_get_block2(m, 0, 0);
}


/** dst = m; dst[:2, :2] = block */
O_INLINE
m_mat3 mat3_set_upper_left2(m_mat3 m, m_mat2 block_2)
{
    return mat3_set_block2(m, block_2, 0, 0);
}

/** m[:2, :2] = block; dst = m */
O_INLINE
m_mat3 mat3_set_this_upper_left2(m_mat3 *m, m_mat2 block_2)
{
    return mat3_set_this_block2(m, block_2, 0, 0);
}


/** dst = m[:3, :3] */
O_INLINE
m_mat3 mat3_get_upper_left3(m_mat3 m)
{
    return mat3_get_block3(m, 0, 0);
}


/** dst = m; dst[:3, :3] = block */
O_INLINE
m_mat3 mat3_set_upper_left3(m_mat3 m, m_mat3 block_3)
{
    return mat3_set_block3(m, block_3, 0, 0);
}

/** m[:3, :3] = block; dst = m */
O_INLINE
m_mat3 mat3_set_this_upper_left3(m_mat3 *m, m_mat3 block_3)
{
    return mat3_set_this_block3(m, block_3, 0, 0);
}

#endif //M_MAT_FLTMAT3_H
