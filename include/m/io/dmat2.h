#ifndef M_IO_DBL_DMAT2_H
#define M_IO_DBL_DMAT2_H

/**
 * @file io/dmat2.h
 *
 * wrappers to io/dmatn.h with types
 *
 * to generator other Xdvec|dmat stuff, use the regex: (?<!=)(?=vec)|(?=dmat)(?!t)
 */

#include "dmatn.h"
#include "m/m_types/dbl.h"


/**
 * prints into the given string buffer
 * In the style:
 * multiline && typed && n>=2 && n<=4:   "(dmat<n>)
 *                                        {{ 1.1, 2.2,
 *                                           ... , n*n }}"
 * multiline ^^ else:                    "(double[4])
 *                                        { 1.1, 2.2,
 *                                          3.3, 4.4 }"
 *           typed && n>=2 && n<=4:      "(dmat<n>) {{ 1.1, 2.2,   ... , n*n }}"
 *           else:                       "(double[4]) { 1.1, 2.2,   3.3, 4.4 }"
 * @param m the double square dmatrix
 * @param n the size (rows=n, cols=n)
 * @param colored true to use color codes for a terminal
 * @param typed if true, prints with the dvec<n> type (2..4)
 * @param multiline if true, prints with newlines, else in a single line
 * @return characters written
 */
O_INLINE
osize dmat2_buffer(char *str, osize size, m_dmat2 dmat, bool colored, bool typed, bool multiline)
{
    return dmatn_buffer(str, size, dmat.v, 2, colored, typed, multiline);
}


/**
 * prints into a new allocated string.
 * In the style:
 * multiline && typed && n>=2 && n<=4:   "(dmat<n>)
 *                                        {{ 1.1, 2.2,
 *                                           ... , n*n }}"
 * multiline ^^ else:                    "(double[4])
 *                                        { 1.1, 2.2,
 *                                          3.3, 4.4 }"
 *           typed && n>=2 && n<=4:      "(dmat<n>) {{ 1.1, 2.2,   ... , n*n }}"
 *           else:                       "(double[4]) { 1.1, 2.2,   3.3, 4.4 }"
 * @param parent object to allocate on
 * @param m the double square dmatrix
 * @param n the size (rows=n, cols=n)
 * @param colored true to use color codes for a terminal
 * @param typed if true, prints with the dvec<n> type (2..4)
 * @param multiline if true, prints with newlines, else in a single line
 * @return characters written
 */
O_INLINE
char *dmat2_str(oobj parent, m_dmat2 dmat, bool colored, bool typed, bool multiline)
{
    return dmatn_str(parent, dmat.v, 2, colored, typed, multiline);
}


//
// print to stdout
//


/**
 * Calls dmat2_buffer and prints to stdout with colors (typed)
 * In the style:
 * multiline&& n>=2 && n<=4:   "(dmat<n>)
 *                             {{ 1.1, 2.2,
 *                                ... , n*n }}"
 * multiline ^^ else:          "(double[4])
 *                              { 1.1, 2.2,
 *                                3.3, 4.4 }"
 *            n>=2 && n<=4:    "(dmat<n>) {{ 1.1, 2.2,   ... , n*n }}"
 *           else:             "(double[4]) { 1.1, 2.2,   3.3, 4.4 }"
 * @param m the double vector
 * @param n the size
 * @param multiline if true, prints with newlines, else in a single line
 */
O_INLINE
osize dmat2_print(m_dmat2 dmat, bool multiline)
{
    return dmatn_print(dmat.v, 2, multiline);
}


/**
 * Calls dmat2_buffer and prints to stdout with colors (typed)
 * Adds a newline at the end.
 * In the style:
 * multiline&& n>=2 && n<=4:   "(dmat<n>)
 *                             {{ 1.1, 2.2,
 *                                ... , n*n }}"
 * multiline ^^ else:          "(double[4])
 *                              { 1.1, 2.2,
 *                                3.3, 4.4 }"
 *            n>=2 && n<=4:    "(dmat<n>) {{ 1.1, 2.2,   ... , n*n }}"
 *           else:             "(double[4]) { 1.1, 2.2,   3.3, 4.4 }"
 * @param m the double square dmatrix
 * @param n the size
 * @param multiline if true, prints with newlines, else in a single line
 */
O_INLINE
osize dmat2_println(m_dmat2 dmat, bool multiline)
{
    return dmatn_println(dmat.v, 2, multiline);
}


//
// json stuff
//


/**
 * Tries to read the json hierarchy array to read n OJson_TYPE_ARRAY in which n double numbers are read
 * @param out_m double square dmatrix to read in
 * @param n size of the square dmatrix
 * @param json_array OJson object type OJson_TYPE_ARRAY
 * @return true if valid
 * @example json: [[1.1, 2.2], [3.3, 4.4]]
 */
O_INLINE
bool dmat2_json_read(m_dmat2 *out_dmat, oobj json_array)
{
    return dmatn_json_read(out_dmat->v, 2, json_array);
}


/**
 * Creates a new OJson OJson_TYPE_ARRAY, representing the given dmatrix
 * @param parent to inherit from
 * @param name if the parent is an object, NULL safe
 * @param m double square dmatrix to create the json array from
 * @param n size of the dmatrix
 * @return new allocated OJson OJson_TYPE_ARRAY object as child of parent
 * @example json: [[1.1, 2.2], [3.3, 4.4]]
 */
O_INLINE
oobj dmat2_json_new(oobj parent, const char *name, m_dmat2 dmat)
{
    return dmatn_json_new(parent, name, dmat.v, 2);
}

#endif //M_IO_DBL_DMAT2_H
