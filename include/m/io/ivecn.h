#ifndef M_IO_INT_IVECN_H
#define M_IO_INT_IVECN_H

/**
 * @file io/ivecn.h
 *
 * print int vectors to buffer.
 * json parser.
 */

#include "m/common.h"

/**
 * prints into the given string buffer
 * In the style:
 *     typed && n>=2 && n<=4:   "(ivec<n>) {{ 1, 2, ... , n }}"
 *     else:                    "(int[5]) { 1, 2, 3, 4, 5 }"
 * @param v the int vector
 * @param n the size
 * @param colored true to use color codes for a terminal
 * @param typed if true, prints with the ivec<n> type (2.)
 * @return characters written
 */
O_EXTERN
osize ivecn_buffer(char *str, osize size, const int *v, int n, bool colored, bool typed);


/**
 * prints into a new allocated string.
 * In the style:
 *     typed && n>=2 && n<=4:   "(ivec<n>) {{ 1, 2, ... , n }}"
 *     else:                    "(int[5]) { 1, 2, 3, 4, 5 }"
 * @param parent object to allocate on
 * @param v the int vector
 * @param n the size
 * @param colored true to use color codes for a terminal
 * @param typed if true, prints with the ivec<n> type (2.)
 */
O_EXTERN
char *ivecn_str(oobj parent, const int *v, int n, bool colored, bool typed);

//
// print to stdout
//


/**
 * Calls ivecn_buffer and prints to stdout with colors (typed)
 * In the style:
 *     n>=2 && n<=4:   "(ivec<n>) {{ 1, 2, ... , n }}"
 *     else:           "(int[5]) { 1, 2, 3, 4, 5 }"
 * @param v the int vector
 * @param n the size
 */
O_EXTERN
osize ivecn_print(const int *v, int n);


/**
 * Calls ivecn_buffer and prints to stdout with colors (typed).
 * Adds a newline at the end.
 * In the style:
 *     n>=2 && n<=4:   "(ivec<n>) {{ 1, 2, ... , n }}"
 *     else:           "(int[5]) { 1, 2, 3, 4, 5 }"
 * @param v the int vector
 * @param n the size
 */
O_EXTERN
osize ivecn_println(const int *v, int n);



//
// json stuff
//


/**
 * Tries to read the json hierarchy array to read n int numbers
 * @param out_v int vector to read in
 * @param n size of the vector
 * @param json_array OJson object type OJson_TYPE_ARRAY
 * @return true if valid
 * @example json: [1, 2, 3, 4]
 */
O_EXTERN
bool ivecn_json_read(int *out_v, int n, oobj json_array);

/**
 * Creates a new OJson OJson_TYPE_ARRAY, representing the given vector
 * @param parent to inherit from
 * @param name if the parent is an object, NULL safe
 * @param v int vector to create the json array from
 * @param n size of the vector
 * @return new allocated OJson OJson_TYPE_ARRAY object as child of parent
 * @example json: [1, 2, 3, 4]
 */
O_EXTERN
oobj ivecn_json_new(oobj parent, const char *name, const int *v, int n);


#endif //M_IO_INT_IVECN_H
