#ifndef M_IO_DBL_DVECN_H
#define M_IO_DBL_DVECN_H

/**
 * @file io/dvecn.h
 *
 * print double vectors to buffer.
 * json parser.
 */

#include "m/common.h"

/**
 * prints into the given string buffer
 * In the style:
 *     typed && n>=2 && n<=4:   "(dvec<n>) {{ 1.1, 2.2, ... , n }}"
 *     else:                    "(double[5]) { 1.1, 2.2, 3.3, 4.4, 5.5 }"
 * @param v the double vector
 * @param n the size
 * @param colored true to use color codes for a terminal
 * @param typed if true, prints with the dvec<n> type (2..4)
 * @return characters written
 */
O_EXTERN
osize dvecn_buffer(char *str, osize size, const double *v, int n, bool colored, bool typed);


/**
 * prints into a new allocated string.
 * In the style:
 *     typed && n>=2 && n<=4:   "(dvec<n>) {{ 1.1, 2.2, ... , n }}"
 *     else:                    "(double[5]) { 1.1, 2.2, 3.3, 4.4, 5.5 }"
 * @param parent object to allocate on
 * @param v the double vector
 * @param n the size
 * @param colored true to use color codes for a terminal
 * @param typed if true, prints with the dvec<n> type (2..4)
 */
O_EXTERN
char *dvecn_str(oobj parent, const double *v, int n, bool colored, bool typed);

//
// print to stdout
//


/**
 * Calls dvecn_buffer and prints to stdout with colors (typed)
 * In the style:
 *     n>=2 && n<=4:   "(dvec<n>) {{ 1.1, 2.2, ... , n }}"
 *     else:           "(double[5]) { 1.1, 2.2, 3.3, 4.4, 5.5 }"
 * @param v the double vector
 * @param n the size
 */
O_EXTERN
osize dvecn_print(const double *v, int n);


/**
 * Calls dvecn_buffer and prints to stdout with colors (typed).
 * Adds a newline at the end.
 * In the style:
 *     n>=2 && n<=4:   "(dvec<n>) {{ 1.1, 2.2, ... , n }}"
 *     else:           "(double[5]) { 1.1, 2.2, 3.3, 4.4, 5.5 }"
 * @param v the double vector
 * @param n the size
 */
O_EXTERN
osize dvecn_println(const double *v, int n);



//
// json stuff
//


/**
 * Tries to read the json hierarchy array to read n double numbers
 * @param out_v double vector to read in
 * @param n size of the vector
 * @param json_array OJson object type OJson_TYPE_ARRAY
 * @return true if valid
 * @example json: [1.1, 2.2, 3.3, 4.4]
 */
O_EXTERN
bool dvecn_json_read(double *out_v, int n, oobj json_array);

/**
 * Creates a new OJson OJson_TYPE_ARRAY, representing the given vector
 * @param parent to inherit from
 * @param name if the parent is an object, NULL safe
 * @param v double vector to create the json array from
 * @param n size of the vector
 * @return new allocated OJson OJson_TYPE_ARRAY object as child of parent
 * @example json: [1.1, 2.2, 3.3, 4.4]
 */
O_EXTERN
oobj dvecn_json_new(oobj parent, const char *name, const double *v, int n);


#endif //M_IO_DBL_DVECN_H
