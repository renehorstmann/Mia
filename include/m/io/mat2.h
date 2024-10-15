#ifndef M_IO_FLT_MAT2_H
#define M_IO_FLT_MAT2_H

/**
 * @file io/mat2.h
 *
 * wrappers to io/matn.h with types
 *
 * to generator other Xvec|mat stuff, use the regex: (?<!=)(?=vec)|(?=mat)(?!t)
 */

#include "matn.h"
#include "m/m_types/flt.h"


/**
 * prints into the given string buffer
 * In the style:
 * multiline && typed && n>=2 && n<=4:   "(mat<n>)
 *                                        {{ 1.1, 2.2,
 *                                           ... , n*n }}"
 * multiline ^^ else:                    "(float[4])
 *                                        { 1.1, 2.2,
 *                                          3.3, 4.4 }"
 *           typed && n>=2 && n<=4:      "(mat<n>) {{ 1.1, 2.2,   ... , n*n }}"
 *           else:                       "(float[4]) { 1.1, 2.2,   3.3, 4.4 }"
 * @param m the float square matrix
 * @param n the size (rows=n, cols=n)
 * @param colored true to use color codes for a terminal
 * @param typed if true, prints with the vec<n> type (2..4)
 * @param multiline if true, prints with newlines, else in a single line
 * @return characters written
 */
O_INLINE
osize mat2_buffer(char *str, osize size, m_mat2 mat, bool colored, bool typed, bool multiline)
{
    return matn_buffer(str, size, mat.v, 2, colored, typed, multiline);
}


/**
 * prints into a new allocated string.
 * In the style:
 * multiline && typed && n>=2 && n<=4:   "(mat<n>)
 *                                        {{ 1.1, 2.2,
 *                                           ... , n*n }}"
 * multiline ^^ else:                    "(float[4])
 *                                        { 1.1, 2.2,
 *                                          3.3, 4.4 }"
 *           typed && n>=2 && n<=4:      "(mat<n>) {{ 1.1, 2.2,   ... , n*n }}"
 *           else:                       "(float[4]) { 1.1, 2.2,   3.3, 4.4 }"
 * @param parent object to allocate on
 * @param m the float square matrix
 * @param n the size (rows=n, cols=n)
 * @param colored true to use color codes for a terminal
 * @param typed if true, prints with the vec<n> type (2..4)
 * @param multiline if true, prints with newlines, else in a single line
 * @return characters written
 */
O_INLINE
char *mat2_str(oobj parent, m_mat2 mat, bool colored, bool typed, bool multiline)
{
    return matn_str(parent, mat.v, 2, colored, typed, multiline);
}


//
// print to stdout
//


/**
 * Calls mat2_buffer and prints to stdout with colors (typed)
 * In the style:
 * multiline&& n>=2 && n<=4:   "(mat<n>)
 *                             {{ 1.1, 2.2,
 *                                ... , n*n }}"
 * multiline ^^ else:          "(float[4])
 *                              { 1.1, 2.2,
 *                                3.3, 4.4 }"
 *            n>=2 && n<=4:    "(mat<n>) {{ 1.1, 2.2,   ... , n*n }}"
 *           else:             "(float[4]) { 1.1, 2.2,   3.3, 4.4 }"
 * @param m the float vector
 * @param n the size
 * @param multiline if true, prints with newlines, else in a single line
 */
O_INLINE
osize mat2_print(m_mat2 mat, bool multiline)
{
    return matn_print(mat.v, 2, multiline);
}


/**
 * Calls mat2_buffer and prints to stdout with colors (typed)
 * Adds a newline at the end.
 * In the style:
 * multiline&& n>=2 && n<=4:   "(mat<n>)
 *                             {{ 1.1, 2.2,
 *                                ... , n*n }}"
 * multiline ^^ else:          "(float[4])
 *                              { 1.1, 2.2,
 *                                3.3, 4.4 }"
 *            n>=2 && n<=4:    "(mat<n>) {{ 1.1, 2.2,   ... , n*n }}"
 *           else:             "(float[4]) { 1.1, 2.2,   3.3, 4.4 }"
 * @param m the float square matrix
 * @param n the size
 * @param multiline if true, prints with newlines, else in a single line
 */
O_INLINE
osize mat2_println(m_mat2 mat, bool multiline)
{
    return matn_println(mat.v, 2, multiline);
}


//
// json stuff
//


/**
 * Tries to read the json hierarchy array to read n OJson_TYPE_ARRAY in which n float numbers are read
 * @param out_m float square matrix to read in
 * @param n size of the square matrix
 * @param json_array OJson object type OJson_TYPE_ARRAY
 * @return true if valid
 * @example json: [[1.1, 2.2], [3.3, 4.4]]
 */
O_INLINE
bool mat2_json_read(m_mat2 *out_mat, oobj json_array)
{
    return matn_json_read(out_mat->v, 2, json_array);
}


/**
 * Creates a new OJson OJson_TYPE_ARRAY, representing the given matrix
 * @param parent to inherit from
 * @param name if the parent is an object, NULL safe
 * @param m float square matrix to create the json array from
 * @param n size of the matrix
 * @return new allocated OJson OJson_TYPE_ARRAY object as child of parent
 * @example json: [[1.1, 2.2], [3.3, 4.4]]
 */
O_INLINE
oobj mat2_json_new(oobj parent, const char *name, m_mat2 mat)
{
    return matn_json_new(parent, name, mat.v, 2);
}

#endif //M_IO_FLT_MAT2_H
