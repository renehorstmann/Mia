#ifndef M_MAT_DBL_MATN_H
#define M_MAT_DBL_MATN_H

/**
 * @file mat/dmatn.h
 *
 * functions working on double matrices with a given size
 */



#include "m/sca/dbl.h"

/** macro to cast a dmattor into a double dmattor */
#define dmatn_cast_into(dst, from, n) \
do {                                                                                 \
    for(int dmatn_cast_into_i_=0; dmatn_cast_into_i_<((n)*(n)); dmatn_cast_into_i_++) { \
        (dst)[dmatn_cast_into_i_] = (double) (from)[dmatn_cast_into_i_];                \
    }                                                                                \
} while(0)


/** dst = r==c ? 1 : 0 (identity) */
O_INLINE
void dmatn_eye(double *dst, int n)
{
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            dst[r * n + c] = r == c ? 1 : 0;
        }
    }
}

/** dst = m[row][:] */
O_INLINE
void dmatn_get_row(double *dst_v, const double *m, int row, int n)
{
    assert(row >= 0 && row < n);
    for (int c = 0; c < n; c++) {
        dst_v[c] = m[c * n + row];
    }
}

/** dst = m[:][col] */
O_INLINE
void dmatn_get_col(double *dst_v, const double *m, int col, int n)
{
    assert(col >= 0 && col < n);
    for (int r = 0; r < n; r++) {
        dst_v[r] = m[col * n + r];
    }
}

/** dst[row][:] = v */
O_INLINE
void dmatn_set_row(double *dst, const double *v, int row, int n)
{
    assert(row >= 0 && row < n);
    for (int c = 0; c < n; c++) {
        dst[c * n + row] = v[c];
    }
}

/** dst[:][col] = v */
O_INLINE
void dmatn_set_col(double *dst, const double *v, int col, int n)
{
    assert(col >= 0 && col < n);
    for (int r = 0; r < n; r++) {
        dst[col * n + r] = v[r];
    }
}

/** dst[row][:] = s */
O_INLINE
void dmatn_set_row_m(double *dst, double s, int row, int n)
{
    assert(row >= 0 && row < n);
    for (int c = 0; c < n; c++) {
        dst[c * n + row] = s;
    }
}

/** dst[:][col] = s */
O_INLINE
void dmatn_set_col_m(double *dst, double s, int col, int n)
{
    assert(col >= 0 && col < n);
    for (int r = 0; r < n; r++) {
        dst[col * n + r] = s;
    }
}

/** returns sum of diagonal from upper left to lower right */
O_INLINE
double dmatn_trace(const double *dmat, int n)
{
    double sum = 0;
    for (int i = 0; i < n; i++) {
        sum += dmat[i * n + i];
    }
    return sum;
}

/** dst = m^t  (restrict data) */
O_INLINE
void dmatn_transpose_no_alias(double *restrict dst, const double *restrict m, int n)
{
    for (int c = 0; c < n; c++) {
        for (int r = 0; r < n; r++) {
            dst[c * n + r] = m[r * n + c];
        }
    }
}

/** dst = m^t */
O_INLINE
void dmatn_transpose(double *dst, const double *m, int n)
{
    assert(n <= M_MAX_SIZE);
    double tmp[M_MAX_SIZE * M_MAX_SIZE];
    dmatn_transpose_no_alias(tmp, m, n);
    for (int i = 0; i < n * n; i++) {
        dst[i] = tmp[i];
    }
}

/** dst = a @ b  (restrict data) */
O_INLINE
void dmatn_mul_mat_no_alias(double *restrict dst, const double *restrict a,
                            const double *restrict b, int n)
{
    for (int c = 0; c < n; c++) {
        for (int r = 0; r < n; r++) {
            dst[c * n + r] = 0;
            for (int k = 0; k < n; k++) {
                dst[c * n + r] += a[k * n + r] * b[c * n + k];
            }
        }
    }
}

/** dst = a @ b */
O_INLINE
void dmatn_mul_mat(double *dst, const double *a, const double *b, int n)
{
    assert(n <= M_MAX_SIZE);
    double tmp[M_MAX_SIZE * M_MAX_SIZE];
    dmatn_mul_mat_no_alias(tmp, a, b, n);
    for (int i = 0; i < n * n; i++) {
        dst[i] = tmp[i];
    }
}

/** dst = a @ b  (restrict data) */
O_INLINE
void dmatn_mul_vec_no_alias(double *restrict dst_v, const double *restrict a,
                            const double *restrict b, int n)
{
    for (int r = 0; r < n; r++) {
        dst_v[r] = 0;
        for (int c = 0; c < n; c++) {
            dst_v[r] += a[c * n + r] * b[c];
        }
    }
}

/** dst = a @ b */
O_INLINE
void dmatn_mul_vec(double *dst_v, const double *a, const double *b, int n)
{
    assert(n <= M_MAX_SIZE);
    double tmp[M_MAX_SIZE];
    dmatn_mul_vec_no_alias(tmp, a, b, n);
    for (int i = 0; i < n; i++) {
        dst_v[i] = tmp[i];
    }
}

/** dst = a @ b  (restrict data) */
O_INLINE
void dvecn_mul_mat_no_alias(double *restrict dst_v, const double *restrict a,
                            const double *restrict b, int n)
{
    for (int c = 0; c < n; c++) {
        dst_v[c] = 0;
        for (int r = 0; r < n; r++) {
            dst_v[c] += b[c * n + r] * a[r];
        }
    }
}

/** dst = a @ b */
O_INLINE
void dvecn_mul_mat(double *dst_v, const double *a, const double *b, int n)
{
    assert(n <= M_MAX_SIZE);
    double tmp[M_MAX_SIZE];
    dvecn_mul_mat_no_alias(tmp, a, b, n);
    for (int i = 0; i < n; i++) {
        dst_v[i] = tmp[i];
    }
}

/**
 * returns = determinant mm
 * @note asserts n>=2 && n<=4
 */
O_EXTERN
double dmatn_det(const double *m, int n);

/**
 * dst = inverted mm
 * @note asserts n>=2 && n<=4
 */
O_EXTERN
void dmatn_inv(double *out_inv, const double *restrict m, int n);


/** block<block_n*block_n> = m<n*n>[col:col+block_n, row:row+block_n] */
O_INLINE
void dmatn_get_block(double *restrict dst_block, const double *restrict m, int row, int col, int block_n, int n)
{
    assert(row >= 0 && row + block_n <= n);
    assert(col >= 0 && col + block_n <= n);
    for (int c = 0; c < block_n; c++) {
        for (int r = 0; r < block_n; r++) {
            dst_block[c * block_n + r] = m[c * n + r];
        }
    }
}

/** dst<n*n>[col:col+block_n, row:row+block_n] = block<block_n*block_n> */
O_INLINE
void dmatn_set_block(double *restrict dst, const double *restrict block, int row, int col, int block_n, int n)
{
    assert(row >= 0 && row + block_n <= n);
    assert(col >= 0 && col + block_n <= n);
    for (int c = 0; c < block_n; c++) {
        for (int r = 0; r < block_n; r++) {
            dst[c * n + r] = block[c * block_n + r];
        }
    }
}

#endif //M_MAT_DBL_MATN_H
