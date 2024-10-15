#ifndef M_MAT_FLTMAT4_H
#define M_MAT_FLTMAT4_H

/**
 * @file mat/m_mat4.h
 *
 * functions working with m_mat4
 */


#include "matn.h"
#include "m/m_types/flt.h"

/** dst = v0, v1, ... */
O_INLINE
m_mat4 mat4_new(float v0, float v1, float v2, float v3,
                float v4, float v5, float v6, float v7,
                float v8, float v9, float v10, float v11,
                float v12, float v13, float v14, float v15)
{
    m_mat4 self;
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


///** dst = (m_mat4) v */
//O_INLINE
//m_mat4 mat4_cast_float(const float *cast) {
//    m_mat4 res;
//    matn_cast_into(res.v, cast, 4);
//    return res;
//}

/** dst = (m_mat4) v */
O_INLINE
m_mat4 mat4_cast_double(const double *cast)
{
    m_mat4 res;
    matn_cast_into(res.v, cast, 4);
    return res;
}


/** dst = r==c ? 1 : 0 (identity)  */
O_INLINE
m_mat4 mat4_eye(void)
{
    m_mat4 res;
    matn_eye(res.v, 4);
    return res;
}

/** dst = m[row][:] */
O_INLINE
m_vec4 mat4_get_row(m_mat4 m, int row)
{
    m_vec4 res;
    matn_get_row(res.v, m.v, row, 4);
    return res;
}


/** dst = m[:][col] */
O_INLINE
m_vec4 mat4_get_col(m_mat4 m, int col)
{
    m_vec4 res;
    matn_get_col(res.v, m.v, col, 4);
    return res;
}


/** dst = m; dst[row][:] = v */
O_INLINE
m_mat4 mat4_set_row(m_mat4 m, m_vec4 v, int row)
{
    matn_set_row(m.v, v.v, row, 4);
    return m;
}

/** m[row][:] = v; dst = m */
O_INLINE
m_mat4 mat4_set_this_row(m_mat4 *m, m_vec4 v, int row)
{
    matn_set_row(m->v, v.v, row, 4);
    return *m;
}

/** dst = m; dst[:][col] = v */
O_INLINE
m_mat4 mat4_set_col(m_mat4 m, m_vec4 v, int col)
{
    matn_set_col(m.v, v.v, col, 4);
    return m;
}

/** mat[:][col] = v; dst = m */
O_INLINE
m_mat4 mat4_set_this_col(m_mat4 *mat, m_vec4 v, int col)
{
    matn_set_col(mat->v, v.v, col, 4);
    return *mat;
}


/** dst = m; dst[row][:] = s */
O_INLINE
m_mat4 mat4_set_row_m(m_mat4 m, float s, int row)
{
    matn_set_row_m(m.v, s, row, 4);
    return m;
}

/** m[row][:] = s; dst = m */
O_INLINE
m_mat4 mat4_set_this_row_m(m_mat4 *m, float s, int row)
{
    matn_set_row_m(m->v, s, row, 4);
    return *m;
}


/** dst = m; dst[:][col] = s */
O_INLINE
m_mat4 mat4_set_col_m(m_mat4 m, float s, int col)
{
    matn_set_col_m(m.v, s, col, 4);
    return m;
}

/** m[:][col] = s; dst = m */
O_INLINE
m_mat4 mat4_set_this_col_m(m_mat4 *m, float s, int col)
{
    matn_set_col_m(m->v, s, col, 4);
    return *m;
}


/** returns sum of diagonal form upper left to lower right */
O_INLINE
float mat4_trace(m_mat4 m)
{
    return matn_trace(m.v, 4);
}


/** dst = m^t */
O_INLINE
m_mat4 mat4_transpose(m_mat4 m)
{
    m_mat4 res;
    matn_transpose_no_alias(res.v, m.v, 4);
    return res;
}


/** dst = a @ b */
O_INLINE
m_mat4 mat4_mul_mat(m_mat4 a, m_mat4 b)
{
    m_mat4 res;
    matn_mul_mat_no_alias(res.v, a.v, b.v, 4);
    return res;
}


/** dst = a @ b */
O_INLINE
m_vec4 mat4_mul_vec(m_mat4 a, m_vec4 b)
{
    m_vec4 res;
    matn_mul_vec_no_alias(res.v, a.v, b.v, 4);
    return res;
}


/** dst = a @ b */
O_INLINE
m_vec4 vec4_mul_mat(m_vec4 a, m_mat4 b)
{
    m_vec4 res;
    vecn_mul_mat_no_alias(res.v, a.v, b.v, 4);
    return res;
}


/** returns = determinant mm */
O_INLINE
float mat4_det(m_mat4 m)
{
    return matn_det(m.v, 4);
}


/** dst = inverted mm */
O_INLINE
m_mat4 mat4_inv(m_mat4 m)
{
    m_mat4 res;
    matn_inv(res.v, m.v, 4);
    return res;
}

/** dst = m[col:col+2, row:row+2] */
O_INLINE
m_mat2 mat4_get_block2(m_mat4 m, int row, int col)
{
    m_mat2 res;
    matn_get_block(res.v, m.v, row, col, 2, 4);
    return res;
}


/** dst = m; dst[col:col+2, row:row+2] = block */
O_INLINE
m_mat4 mat4_set_block2(m_mat4 m, m_mat2 block_2, int row, int col)
{
    matn_set_block(m.v, block_2.v, row, col, 2, 4);
    return m;
}


/** m[col:col+2, row:row+2] = block; dst = m */
O_INLINE
m_mat4 mat4_set_this_block2(m_mat4 *m, m_mat2 block_2, int row, int col)
{
    matn_set_block(m->v, block_2.v, row, col, 2, 4);
    return *m;
}


/** dst = m[col:col+2, row:row+2] */
O_INLINE
m_mat3 mat4_get_block3(m_mat4 m, int row, int col)
{
    m_mat3 res;
    matn_get_block(res.v, m.v, row, col, 3, 4);
    return res;
}


/** dst = m; dst[col:col+2, row:row+2] = block */
O_INLINE
m_mat4 mat4_set_block3(m_mat4 m, m_mat3 block_3, int row, int col)
{
    matn_set_block(m.v, block_3.v, row, col, 3, 4);
    return m;
}

/** m[col:col+2, row:row+2] = block; dst = m */
O_INLINE
m_mat4 mat4_set_this_block3(m_mat4 *m, m_mat3 block_3, int row, int col)
{
    matn_set_block(m->v, block_3.v, row, col, 3, 4);
    return *m;
}


/** dst = m[:2, :2] */
O_INLINE
m_mat2 mat4_get_upper_left2(m_mat4 m)
{
    return mat4_get_block2(m, 0, 0);
}


/** dst = m; dst[:2, :2] = block */
O_INLINE
m_mat4 mat4_set_upper_left2(m_mat4 m, m_mat2 block_2)
{
    return mat4_set_block2(m, block_2, 0, 0);
}

/** m[:2, :2] = block; dst = m */
O_INLINE
m_mat4 mat4_set_this_upper_left2(m_mat4 *m, m_mat2 block_2)
{
    return mat4_set_this_block2(m, block_2, 0, 0);
}


/** dst = m[:3, :3] */
O_INLINE
m_mat3 mat4_get_upper_left3(m_mat4 m)
{
    return mat4_get_block3(m, 0, 0);
}


/** dst = m; dst[:3, :3] = block */
O_INLINE
m_mat4 mat4_set_upper_left3(m_mat4 m, m_mat3 block_3)
{
    return mat4_set_block3(m, block_3, 0, 0);
}

/** m[:3, :3] = block; dst = m */
O_INLINE
m_mat4 mat4_set_this_upper_left3(m_mat4 *m, m_mat3 block_3)
{
    return mat4_set_this_block3(m, block_3, 0, 0);
}

#endif //M_MAT_FLTMAT4_H
