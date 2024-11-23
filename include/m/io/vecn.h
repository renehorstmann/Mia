#ifndef M_IO_FLT_VECN_H
#define M_IO_FLT_VECN_H

/**
 * @file io/vecn.h
 *
 * print float vectors to buffer.
 * json parser.
 */

#include "m/common.h"

/**
 * prints into the given string buffer
 * In the style:
 *     typed && n>=2 && n<=4:   "(vec<n>) {{ 1.1, 2.2, ... , n }}"
 *     else:                    "(float[5]) { 1.1, 2.2, 3.3, 4.4, 5.5 }"
 * @param v the float vector
 * @param n the size
 * @param colored true to use color codes for a terminal
 * @param typed if true, prints with the vec<n> type (2..4)
 * @return characters written
 */
O_EXTERN
osize vecn_buffer(char *str, osize size, const float *v, int n, bool colored, bool typed);


/**
 * prints into a new allocated string.
 * In the style:
 *     typed && n>=2 && n<=4:   "(vec<n>) {{ 1.1, 2.2, ... , n }}"
 *     else:                    "(float[5]) { 1.1, 2.2, 3.3, 4.4, 5.5 }"
 * @param parent object to allocate on
 * @param v the float vector
 * @param n the size
 * @param colored true to use color codes for a terminal
 * @param typed if true, prints with the vec<n> type (2..4)
 */
O_EXTERN
char *vecn_str(oobj parent, const float *v, int n, bool colored, bool typed);

//
// print to stdout
//


/**
 * Calls vecn_buffer and prints to stdout with colors (typed)
 * In the style:
 *     n>=2 && n<=4:   "(vec<n>) {{ 1.1, 2.2, ... , n }}"
 *     else:           "(float[5]) { 1.1, 2.2, 3.3, 4.4, 5.5 }"
 * @param v the float vector
 * @param n the size
 */
O_EXTERN
osize vecn_print(const float *v, int n);


/**
 * Calls vecn_buffer and prints to stdout with colors (typed).
 * Adds a newline at the end.
 * In the style:
 *     n>=2 && n<=4:   "(vec<n>) {{ 1.1, 2.2, ... , n }}"
 *     else:           "(float[5]) { 1.1, 2.2, 3.3, 4.4, 5.5 }"
 * @param v the float vector
 * @param n the size
 */
O_EXTERN
osize vecn_println(const float *v, int n);



//
// json stuff
//


/**
 * Tries to read the json hierarchy array to read n float numbers
 * @param out_v float vector to read in
 * @param n size of the vector
 * @param json_array OJson object type OJson_TYPE_ARRAY
 * @return true if valid
 * @example json: [1.1, 2.2, 3.3, 4.4]
 */
O_EXTERN
bool vecn_json_read(float *out_v, int n, oobj json_array);

/**
 * Creates a new OJson OJson_TYPE_ARRAY, representing the given vector
 * @param parent to inherit from
 * @param name if the parent is an object, NULL safe
 * @param v float vector to create the json array from
 * @param n size of the vector
 * @return new allocated OJson OJson_TYPE_ARRAY object as child of parent
 * @example json: [1.1, 2.2, 3.3, 4.4]
 */
O_EXTERN
oobj vecn_json_new(oobj parent, const char *name, const float *v, int n);


#endif //M_IO_FLT_VECN_H
