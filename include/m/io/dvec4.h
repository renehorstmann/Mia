#ifndef M_IO_DBL_DVEC4_H
#define M_IO_DBL_DVEC4_H

/**
 * @file io/dvec4.h
 *
 * wrappers to io/dvecn.h with types
 *
 * to generator other Xdvec stuff, use the regex: (?<!=)(?=vec)|(?=dmat)(?!t)
 */

#include "dvecn.h"
#include "m/m_types/dbl.h"


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
O_INLINE
osize dvec4_buffer(char *str, osize size, m_dvec4 dvec, bool colored, bool typed)
{
    return dvecn_buffer(str, size, dvec.v, 4, colored, typed);
}


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
O_INLINE
char *dvec4_str(oobj parent, m_dvec4 dvec, bool colored, bool typed)
{
    return dvecn_str(parent, dvec.v, 4, colored, typed);
}

//
// print to stdout
//


/**
 * Calls dvec4_buffer and prints to stdout with colors (typed)
 * In the style:
 *     n>=2 && n<=4:   "(dvec<n>) {{ 1.1, 2.2, ... , n }}"
 *     else:           "(double[5]) { 1.1, 2.2, 3.3, 4.4, 5.5 }"
 * @param v the double vector
 * @param n the size
 */
O_INLINE
osize dvec4_print(m_dvec4 dvec)
{
    return dvecn_print(dvec.v, 4);
}


/**
 * Calls dvec4_buffer and prints to stdout with colors (typed).
 * Adds a newline at the end.
 * In the style:
 *     n>=2 && n<=4:   "(dvec<n>) {{ 1.1, 2.2, ... , n }}"
 *     else:           "(double[5]) { 1.1, 2.2, 3.3, 4.4, 5.5 }"
 * @param v the double vector
 * @param n the size
 */
O_INLINE
osize dvec4_println(m_dvec4 dvec)
{
    return dvecn_println(dvec.v, 4);
}


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
O_INLINE
bool dvec4_json_read(m_dvec4 *out_dvec, oobj json_array)
{
    return dvecn_json_read(out_dvec->v, 4, json_array);
}

/**
 * Creates a new OJson OJson_TYPE_ARRAY, representing the given vector
 * @param parent to inherit from
 * @param name if the parent is an object, NULL safe
 * @param v double vector to create the json array from
 * @param n size of the vector
 * @return new allocated OJson OJson_TYPE_ARRAY object as child of parent
 * @example json: [1.1, 2.2, 3.3, 4.4]
 */
O_INLINE
oobj dvec4_json_new(oobj parent, const char *name, m_dvec4 dvec)
{
    return dvecn_json_new(parent, name, dvec.v, 4);
}


#endif //M_IO_DBL_DVEC4_H
