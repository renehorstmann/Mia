#ifndef O_STR_RANGE_H
#define O_STR_RANGE_H

/**
 * @file str_range.h
 *
 * string ranges and lists for ranges (used for OPattern captures)
 */

#include "common.h"

/**
 * Range based on a str (or something)
 */
struct o_str_range {
    osize start;
    osize len;
};

/**
 * List of ranges
 */
struct o_str_range_list {
    struct o_str_range *list;
    osize len;
    
    // may be NULL
    oobj parent;
};

/**
 * List of list of ranges
 * Used by OPattern captures for find all
 */
struct o_str_range_list_list {
    struct o_str_range_list *list;
    osize len;
    
    // may be NULL
    oobj parent;
};

/**
 * Allocates a new cleared list
 * @param obj object to allocate on
 * @param list_len size of the list
 * @return o_str_range_list object with an element cleared list
 * @note list will be cleared to 0
 */
O_EXTERN
struct o_str_range_list o_str_range_list_new(oobj obj, osize list_len);

/**
 * moves the full list into another object
 * @param list to be moved
 * @param into the new object to move the list into
 * @note just calls o_move_res(obj, into, list.list)
 */
O_EXTERN
void o_str_range_list_move(struct o_str_range_list *list, oobj into);

/**
 * frees the full list
 * @param list to free
 * @note just calls o_free(obj, list.list)
 */
O_EXTERN
void o_str_range_list_free(struct o_str_range_list *list);

/**
 * Allocates a new cleared list list
 * @param obj object to allocate on
 * @param list_list_len size of the outer listlist
 * @param list_len size of the inner lists
 * @return o_str_range_list_list object with element cleared lists
 * @note lists will be cleared to 0
 */
O_EXTERN
struct o_str_range_list_list o_str_range_list_list_new(oobj obj, osize list_list_len, osize list_len);

/**
 * movex the full list list into another object
 * @param list_list to be moved
 * @param into the new object to move the list list into
 */
O_EXTERN
void o_str_range_list_list_move(struct o_str_range_list_list *list_list, oobj into);

/**
 * frees the full list list
 * @param list_list to free
 */
O_EXTERN
void o_str_range_list_list_free(struct o_str_range_list_list *list_list);


//
// struct functions
//


/**
 * @param ranges list is cleared terminated (last element is 0), NULL safe
 * @return the size of elements in this list
 */
O_INLINE
osize o_str_ranges_size(const struct o_str_range *ranges)
{
    osize cnt = 0;
    while (ranges && ranges++->len != 0) {
        cnt++;
    }
    return cnt;
}

/**
 * @param ranges list is cleared terminated (last element is 0), NULL safe
 * @param parent OObj on which ranges was allocated, or NULL
 * @return a range list element with the same memory
 */
O_INLINE
struct o_str_range_list o_str_ranges_as_list(struct o_str_range *ranges, oobj parent)
{
    return (struct o_str_range_list) {ranges, o_str_ranges_size(ranges), parent};
}

/**
 * Returns a list of strings, all allocated on obj (list and strings)
 * So to free manually, traverse list and free each entry, then free the list, too
 *  -> Or just use an container / temp OObj and delete that...
 * @param obj to allocate the resources
 * @param str to be cloned from, NULL safe
 * @param ranges ranges for the str, NULL safe, clamped to str boundaries
 * @param opt_len if <0, the ranges list is element cleared (last is 0)
 * @return An allocated null terminated list of allocated split strings.
 *         The list is always allocated, but may be empty (first entry is NULL)
 * @threadsafe (because allocations with objects are threadsafe)
 */
O_EXTERN
char **o_str_ranges_to_strings(oobj obj, const char *str, const struct o_str_range *ranges, osize opt_len);

/**
 * Replaces each range in str with the replace string, allocated on obj
 * @param obj to allocate the resource
 * @param str to be replaces from, NULL safe
 * @param ranges ranges for the str, NULL safe, clamped to str boundaries.
 *               MUST BE ORDERED ( next.start >= last.start+last.len )
 * @param opt_len if <0, the ranges list is element cleared (last is 0)
 * @param replace string to replace for the ranges
 * @return The new string as resource from obj, or NULL if str is NULL
 * @note asserts ranges is ordered
 * @threadsafe (because allocations with objects are threadsafe)
 */
O_EXTERN
char *o_str_ranges_replace(oobj obj, const char *str,
                           const struct o_str_range *ranges, osize opt_len,
                           const char *replace);

#endif //O_STR_RANGE_H
