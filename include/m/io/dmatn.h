#ifndef M_IO_DBL_DMATN_H
#define M_IO_DBL_DMATN_H

/**
 * @file io/dmatn.h
 *
 * print double  dmatrices to buffer.
 * json parser.
 */

#include "m/common.h"



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
O_EXTERN
osize dmatn_buffer(char *str, osize size, const double *m, int n, bool colored, bool typed, bool multiline);

//
// print to stdout
//



/**
 * Calls dmatn_buffer and prints to stdout with colors (typed)
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
O_EXTERN
osize dmatn_print(const double *m, int n, bool multiline);


/**
 * Calls dmatn_buffer and prints to stdout with colors (typed)
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
O_EXTERN
osize dmatn_println(const double *m, int n, bool multiline);

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
O_EXTERN
char *dmatn_str(oobj parent, const double *m, int n, bool colored, bool typed, bool multiline);



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
O_EXTERN
bool dmatn_json_read(double *out_m, int n, oobj json_array);


/**
 * Creates a new OJson OJson_TYPE_ARRAY, representing the given dmatrix
 * @param parent to inherit from
 * @param name if the parent is an object, NULL safe
 * @param m double square dmatrix to create the json array from
 * @param n size of the dmatrix
 * @return new allocated OJson OJson_TYPE_ARRAY object as child of parent
 * @example json: [[1.1, 2.2], [3.3, 4.4]]
 */
O_EXTERN
oobj dmatn_json_new(oobj parent, const char *name, const double *m, int n);

#endif //M_IO_DBL_DMATN_H
