#ifndef M_IO_BYTE_BVECN_H
#define M_IO_BYTE_BVECN_H

/**
 * @file io/bvecn.h
 *
 * print obyte vectors to buffer.
 * json parser.
 */

#include "m/common.h"

/**
 * prints into the given string buffer
 * In the style:
 *     typed && n>=2 && n<=4:   "(bvec<n>) {{ 1, 2, ... , n }}"
 *     else:                    "(obyte[5]) { 1, 2, 3, 4, 5 }"
 * @param v the obyte vector
 * @param n the size
 * @param colored true to use color codes for a terminal
 * @param typed if true, prints with the bvec<n> type (2.)
 * @return characters written
 */
O_EXTERN
osize bvecn_buffer(char *str, osize size, const obyte *v, int n, bool colored, bool typed);


/**
 * prints into a new allocated string.
 * In the style:
 *     typed && n>=2 && n<=4:   "(bvec<n>) {{ 1, 2, ... , n }}"
 *     else:                    "(obyte[5]) { 1, 2, 3, 4, 5 }"
 * @param parent object to allocate on
 * @param v the obyte vector
 * @param n the size
 * @param colored true to use color codes for a terminal
 * @param typed if true, prints with the bvec<n> type (2.)
 */
O_EXTERN
char *bvecn_str(oobj parent, const obyte *v, int n, bool colored, bool typed);

//
// print to stdout
//


/**
 * Calls bvecn_buffer and prints to stdout with colors (typed)
 * In the style:
 *     n>=2 && n<=4:   "(bvec<n>) {{ 1, 2, ... , n }}"
 *     else:           "(obyte[5]) { 1, 2, 3, 4, 5 }"
 * @param v the obyte vector
 * @param n the size
 */
O_EXTERN
osize bvecn_print(const obyte *v, int n);


/**
 * Calls bvecn_buffer and prints to stdout with colors (typed).
 * Adds a newline at the end.
 * In the style:
 *     n>=2 && n<=4:   "(bvec<n>) {{ 1, 2, ... , n }}"
 *     else:           "(obyte[5]) { 1, 2, 3, 4, 5 }"
 * @param v the obyte vector
 * @param n the size
 */
O_EXTERN
osize bvecn_println(const obyte *v, int n);



//
// json stuff
//


/**
 * Tries to read the json hierarchy array to read n obyte numbers
 * @param out_v obyte vector to read in
 * @param n size of the vector
 * @param json_array OJson object type OJson_TYPE_ARRAY
 * @return true if valid
 * @example json: [1, 2, 3, 4]
 */
O_EXTERN
bool bvecn_json_read(obyte *out_v, int n, oobj json_array);

/**
 * Creates a new OJson OJson_TYPE_ARRAY, representing the given vector
 * @param parent to inherit from
 * @param name if the parent is an object, NULL safe
 * @param v obyte vector to create the json array from
 * @param n size of the vector
 * @return new allocated OJson OJson_TYPE_ARRAY object as child of parent
 * @example json: [1, 2, 3, 4]
 */
O_EXTERN
oobj bvecn_json_new(oobj parent, const char *name, const obyte *v, int n);


#endif //M_IO_BYTE_BVECN_H
