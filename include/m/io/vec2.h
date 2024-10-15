#ifndef M_IO_FLT_VEC2_H
#define M_IO_FLT_VEC2_H

/**
 * @file io/vec2.h
 *
 * wrappers to io/vecn.h with types
 *
 * to generator other Xvec stuff, use the regex: (?<!=)(?=vec)|(?=mat)(?!t)
 */

#include "vecn.h"
#include "m/m_types/flt.h"


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
O_INLINE
osize vec2_buffer(char *str, osize size, m_vec2 vec, bool colored, bool typed)
{
    return vecn_buffer(str, size, vec.v, 2, colored, typed);
}


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
O_INLINE
char *vec2_str(oobj parent, m_vec2 vec, bool colored, bool typed)
{
    return vecn_str(parent, vec.v, 2, colored, typed);
}



//
// print to stdout
//


/**
 * Calls vec2_buffer and prints to stdout with colors (typed)
 * In the style:
 *     n>=2 && n<=4:   "(vec<n>) {{ 1.1, 2.2, ... , n }}"
 *     else:           "(float[5]) { 1.1, 2.2, 3.3, 4.4, 5.5 }"
 * @param v the float vector
 * @param n the size
 */
O_INLINE
osize vec2_print(m_vec2 vec)
{
    return vecn_print(vec.v, 2);
}


/**
 * Calls vec2_buffer and prints to stdout with colors (typed).
 * Adds a newline at the end.
 * In the style:
 *     n>=2 && n<=4:   "(vec<n>) {{ 1.1, 2.2, ... , n }}"
 *     else:           "(float[5]) { 1.1, 2.2, 3.3, 4.4, 5.5 }"
 * @param v the float vector
 * @param n the size
 */
O_INLINE
osize vec2_println(m_vec2 vec)
{
    return vecn_println(vec.v, 2);
}


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
O_INLINE
bool vec2_json_read(m_vec2 *out_vec, oobj json_array)
{
    return vecn_json_read(out_vec->v, 2, json_array);
}

/**
 * Creates a new OJson OJson_TYPE_ARRAY, representing the given vector
 * @param parent to inherit from
 * @param name if the parent is an object, NULL safe
 * @param v float vector to create the json array from
 * @param n size of the vector
 * @return new allocated OJson OJson_TYPE_ARRAY object as child of parent
 * @example json: [1.1, 2.2, 3.3, 4.4]
 */
O_INLINE
oobj vec2_json_new(oobj parent, const char *name, m_vec2 vec)
{
    return vecn_json_new(parent, name, vec.v, 2);
}

#endif //M_IO_FLT_VEC2_H
