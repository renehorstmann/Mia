#ifndef M_IO_INT_IVEC2_H
#define M_IO_INT_IVEC2_H

/**
 * @file io/ivec2.h
 *
 * wrappers to io/ivecn.h with types
 *
 * to generator other Xivec stuff, use the regex: (?<!=)(?=vec)|(?=imat)(?!t)
 */

#include "ivecn.h"
#include "m/m_types/int.h"


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
O_INLINE
osize ivec2_buffer(char *str, osize size, m_ivec2 ivec, bool colored, bool typed)
{
    return ivecn_buffer(str, size, ivec.v, 2, colored, typed);
}


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
O_INLINE
char *ivec2_str(oobj parent, m_ivec2 ivec, bool colored, bool typed)
{
    return ivecn_str(parent, ivec.v, 2, colored, typed);
}



//
// print to stdout
//


/**
 * Calls ivec2_buffer and prints to stdout with colors (typed)
 * In the style:
 *     n>=2 && n<=4:   "(ivec<n>) {{ 1, 2, ... , n }}"
 *     else:           "(int[5]) { 1, 2, 3, 4, 5 }"
 * @param v the int vector
 * @param n the size
 */
O_INLINE
osize ivec2_print(m_ivec2 ivec)
{
    return ivecn_print(ivec.v, 2);
}


/**
 * Calls ivec2_buffer and prints to stdout with colors (typed).
 * Adds a newline at the end.
 * In the style:
 *     n>=2 && n<=4:   "(ivec<n>) {{ 1, 2, ... , n }}"
 *     else:           "(int[5]) { 1, 2, 3, 4, 5 }"
 * @param v the int vector
 * @param n the size
 */
O_INLINE
osize ivec2_println(m_ivec2 ivec)
{
    return ivecn_println(ivec.v, 2);
}


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
O_INLINE
bool ivec2_json_read(m_ivec2 *out_ivec, oobj json_array)
{
    return ivecn_json_read(out_ivec->v, 2, json_array);
}

/**
 * Creates a new OJson OJson_TYPE_ARRAY, representing the given vector
 * @param parent to inherit from
 * @param name if the parent is an object, NULL safe
 * @param v int vector to create the json array from
 * @param n size of the vector
 * @return new allocated OJson OJson_TYPE_ARRAY object as child of parent
 * @example json: [1, 2, 3, 4]
 */
O_INLINE
oobj ivec2_json_new(oobj parent, const char *name, m_ivec2 ivec)
{
    return ivecn_json_new(parent, name, ivec.v, 2);
}

#endif //M_IO_INT_IVEC2_H
