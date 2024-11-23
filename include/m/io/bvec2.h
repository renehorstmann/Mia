#ifndef M_IO_BYTE_BVEC2_H
#define M_IO_BYTE_BVEC2_H

/**
 * @file io/bvec2.h
 *
 * wrappers to io/bvecn.h with types
 *
 * to generator other Xbvec stuff, use the regex: (?<!=)(?=vec)|(?=bimat)(?!t)
 */

#include "bvecn.h"
#include "m/m_types/byte.h"


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
O_INLINE
osize bvec2_buffer(char *str, osize size, m_bvec2 bvec, bool colored, bool typed)
{
    return bvecn_buffer(str, size, bvec.v, 2, colored, typed);
}


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
O_INLINE
char *bvec2_str(oobj parent, m_bvec2 bvec, bool colored, bool typed)
{
    return bvecn_str(parent, bvec.v, 2, colored, typed);
}



//
// print to stdout
//


/**
 * Calls bvec2_buffer and prints to stdout with colors (typed)
 * In the style:
 *     n>=2 && n<=4:   "(bvec<n>) {{ 1, 2, ... , n }}"
 *     else:           "(obyte[5]) { 1, 2, 3, 4, 5 }"
 * @param v the obyte vector
 * @param n the size
 */
O_INLINE
osize bvec2_print(m_bvec2 bvec)
{
    return bvecn_print(bvec.v, 2);
}


/**
 * Calls bvec2_buffer and prints to stdout with colors (typed).
 * Adds a newline at the end.
 * In the style:
 *     n>=2 && n<=4:   "(bvec<n>) {{ 1, 2, ... , n }}"
 *     else:           "(obyte[5]) { 1, 2, 3, 4, 5 }"
 * @param v the obyte vector
 * @param n the size
 */
O_INLINE
osize bvec2_println(m_bvec2 bvec)
{
    return bvecn_println(bvec.v, 2);
}


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
O_INLINE
bool bvec2_json_read(m_bvec2 *out_bvec, oobj json_array)
{
    return bvecn_json_read(out_bvec->v, 2, json_array);
}

/**
 * Creates a new OJson OJson_TYPE_ARRAY, representing the given vector
 * @param parent to inherit from
 * @param name if the parent is an object, NULL safe
 * @param v obyte vector to create the json array from
 * @param n size of the vector
 * @return new allocated OJson OJson_TYPE_ARRAY object as child of parent
 * @example json: [1, 2, 3, 4]
 */
O_INLINE
oobj bvec2_json_new(oobj parent, const char *name, m_bvec2 bvec)
{
    return bvecn_json_new(parent, name, bvec.v, 2);
}

#endif //M_IO_BYTE_BVEC2_H
