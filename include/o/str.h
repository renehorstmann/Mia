#ifndef O_STR_H
#define O_STR_H

/**
 * @file str.h
 *
 * Some default library functions for strings, etc.
 * Working with allocated object resources
 *
 * @note The functions should typically be NULL safe, unlike the C-Versions
 */

#include "str_range.h"

/**
 * Clones a (sub) string into the reallocated memory
 * @param obj to allocate the resource
 * @param mem the old memory to realloc (restrict: may NOT alias with to_clone!)
 * @param to_clone string to clone (null terminated) or NULL, which will return NULL
 * @param start start character index of to_clone (<=0 for beginning)
 * @param len num of characters to clone (osize_MAX for up to the end)
 *            <=0 returns NULL.
 *            will be truncated to the maximal cloneable size.
 * @return The new string as resource from obj, or NULL if to_clone is NULL
 * @note asserts mem != to_clone
 * @threadsafe (because allocations with objects are threadsafe)
 */
O_EXTERN
char *o_str_clone_sub_realloc(oobj obj, void *restrict mem, const char *to_clone, osize start, osize len);

/**
 * Clones a (sub) string into the new allocated memory
 * @param obj to allocate the resource
 * @param to_clone string to clone (null terminated) or NULL, which will return NULL
 * @param start start character index of to_clone (<=0 for beginning)
 * @param len num of characters to clone (osize_MAX for up to the end)
 *            <=0 returns NULL.
 *            will be truncated to the maximal cloneable size.
 * @return The new string as resource from obj, or NULL if to_clone is NULL
 * @threadsafe (because allocations with objects are threadsafe)
 */
O_INLINE
char *o_str_clone_sub(oobj obj, const char *to_clone, osize start, osize len)
{
    return o_str_clone_sub_realloc(obj, NULL, to_clone, start, len);
}

/**
 * Clones a (sub) string into the new allocated memory
 * @param obj to allocate the resource
 * @param to_clone string to clone (null terminated) or NULL, which will return NULL
 * @param range to be cloned
 * @return The new string as resource from obj, or NULL if to_clone is NULL
 * @threadsafe (because allocations with objects are threadsafe)
 */
O_INLINE
char *o_str_clone_range(oobj obj, const char *to_clone, struct o_str_range range)
{
    return o_str_clone_sub(obj, to_clone, range.start, range.len);
}

/**
 * Clones a string into the reallocated memory
 * @param obj to allocate the resource
 * @param mem the old memory to realloc (restrict: may NOT alias with to_clone!)
 * @param to_clone string to clone (null terminated) or NULL, which will return NULL
 * @return The new string as resource from obj, or NULL if to_clone is NULL
 * @note asserts mem != to_clone
 * @threadsafe (because allocations with objects are threadsafe)
 */
O_INLINE
char *o_str_clone_realloc(oobj obj, void *restrict mem, const char *to_clone)
{
    return o_str_clone_sub_realloc(obj, mem, to_clone, 0, osize_MAX);
}

/**
 * Clones a string into the new allocated memory
 * @param obj to allocate the resource
 * @param to_clone string to clone (null terminated) or NULL, which will return NULL
 * @return The new string as resource from obj, or NULL if to_clone is NULL
 * @threadsafe (because allocations with objects are threadsafe)
 */
O_INLINE
char *o_str_clone(oobj obj, const char *to_clone)
{
    return o_str_clone_sub(obj, to_clone, 0, osize_MAX);
}

/**
 * Copies a string into the given buffer, that buffer needs to be big enough!
 * @param dst buffer to copy into (null safe)
 * @param to_clone string to clone (null terminated) or NULL, which will return 0
 * @return copied bytes: o_strlen(to_clone) + 1 (added null terminator)
 * @threadsafe
 */
O_INLINE
osize o_str_copy(char *dst, const char *to_clone)
{
    if(!dst || !to_clone) {
        return 0;
    }
    // add null terminator... (+1)
    osize len = o_strlen(to_clone) + 1;
    o_memcpy(dst, to_clone, 1, len);
    return len;
}

/**
 * Creates a new string from the given printf style format into the new allocated memory.
 * va_list version.
 * @param obj to allocate the resource
 * @param format printf style format (null terminated, must NOT be NULL)
 * @param args va_args object
 * @return The new string as resource from obj
 * @threadsafe (because allocations with objects are threadsafe)
 */
O_EXTERN
char *o_strf_va(oobj obj, const char *format, va_list args);


/**
 * Creates a new string from the given printf style format into the new allocated memory.
 * @param obj to allocate the resource
 * @param format printf style format (null terminated, must NOT be NULL)
 * @return The new string as resource from obj
 * @threadsafe (because allocations with objects are threadsafe)
 */
O_EXTERN
char *o_strf(oobj obj, const char *format, ...);

/**
 * Creates a new string from the given printf style format into the new allocated memory.
 * Reallocs the old string, which may be used in the format
 * va_list version.
 * @param obj to allocate the resource
 * @param old previosly allocated string or NULL (realloc mem), can be used in the format
 * @param format printf style format (null terminated, must NOT be NULL)
 * @param args va_args object
 * @return The new string as resource from obj
 * @threadsafe (because allocations with objects are threadsafe)
 */
O_EXTERN
char *o_strf_realloc_va(oobj obj, char *old, const char *format, va_list args);


/**
 * Creates a new string from the given printf style format into the new allocated memory.
 * Reallocs the old string, which may be used in the format
 * @param obj to allocate the resource
 * @param old previosly allocated string or NULL (realloc mem), can be used in the format
 * @param format printf style format (null terminated, must NOT be NULL)
 * @return The new string as resource from obj
 * @threadsafe (because allocations with objects are threadsafe)
 */
O_EXTERN
char *o_strf_realloc(oobj obj, char *old, const char *format, ...);


/**
 * Calls snprintf with buf and sizeof buf
 * @threadsafe
 */
#define o_strf_buf(buf, ...) snprintf((buf), sizeof (buf), __VA_ARGS__)

/**
 * Joins multiple (n) strings into a new allocated string
 * @param obj to allocate the resource
 * @param list_of_strings list of strings to concatenate
 * @param n Number os strings in the list
 * @param opt_seperator optional seperator string between each repetition
 * @return The new string as resource from obj, or NULL if the resulting string length is 0
 * @threadsafe (because allocations with objects are threadsafe)
 */
O_EXTERN
char *o_str_join(oobj obj, char **list_of_strings, osize n, const char *opt_seperator);


/**
 * Concatenates string a and b into a new allocated string
 * @param obj to allocate the resource
 * @param a first part of the new string
 * @param b second part of the new string
 * @return The new string as resource from obj, or NULL if the resulting string length is 0
 * @threadsafe (because allocations with objects are threadsafe)
 */
O_INLINE
char *o_str_cat(oobj obj, const char *a, const char *b)
{
    return o_str_join(obj, (char *[]) {(char *) a, (char *) b}, 2, NULL);
}

/**
 * Creates a new repetition into a new allocated string
 * @param obj to allocate the resource
 * @param str String to repeat, or NULL which will return NULL
 * @param n number of repetitions
 * @param opt_seperator optional seperator string between each repetition
 * @return The new string as resource from obj, or NULL if str is NULL
 * @threadsafe (because allocations with objects are threadsafe)
 * @example
 *      o_str_rep(obj, "hi", 3, NULL);    // -> "hihihi"
 *      o_str_rep(obj, "hi", 3, "");      // -> "hihihi"
 *      o_str_rep(obj, "hi", 3, ", ");    // -> "hi, hi, hi"
 */
O_EXTERN
char *o_str_rep(oobj obj, const char *str, osize n, const char *opt_seperator);

/**
 * @return True if both strings equal (also in length), NULL safe (returns false)
 */
O_INLINE
bool o_str_equals(const char *a, const char *b)
{
    if (!a || !b) {
        return false;
    }
    return strcmp(a, b) == 0;
}

/**
 * Matches the start of a string
 * @param str String to check, NULL safe (returns false)
 * @param begin String to match at the beginning, NULL safe (returns false)
 * @return True on match
 */
O_EXTERN
bool o_str_begins(const char *str, const char *begin);

/**
 * Matches the end of a string
 * @param str String to check, NULL safe (returns false)
 * @param end String to match at the end, NULL safe (returns false)
 * @return True on match
 */
O_EXTERN
bool o_str_ends(const char *str, const char *end);

/**
 * Searches for the first occurrence of search in the string
 * @param str String to check, NULL safe (returns -1)
 * @param search String to search for, NULL safe (returns -1)
 * @return The position of search or -1 if not found
 */
O_EXTERN
osize o_str_find(const char *str, const char *search);

/**
 * Searches for all occurrences of search in the string (no aliasing, so if string if "JJJ" and search "JJ", 1 result
 * Ranges list is allocated
 * @param obj to allocate the resource
 * @param str String to check, NULL safe (returns -1)
 * @param search String to search for, NULL safe (returns -1)
 * @return an element cleared (0) terminated list of ranges.
 *         Except for the termination, .len=o_strlen(search)
 * @threadsafe (because allocations with objects are threadsafe)
 */
O_EXTERN
struct o_str_range_list o_str_find_all(oobj obj, const char *str, const char *search);

/**
 * Searches for the first occurrence of one character of set in the string
 * @param str String to check, may be NULL and returns -1 then
 * @param set characters in a set string, may be NULL and returns -1 then)
 * @return The position of a char from set or -1 if not found
 */
O_EXTERN
osize o_str_find_set(const char *str, const char *set);

/**
 * Searches for the first occurrence of a character that is not in set, in the string
 * @param str String to check, may be NULL and returns -1 then
 * @param set characters in a set string, may be NULL and returns -1 then)
 * @return The position of search or -1 if not found
 */
O_EXTERN
osize o_str_find_set_inv(const char *str, const char *set);

/**
 * Searches for the first occurrence of the char search in the string
 * @param str String to check, may be NULL and returns -1 then
 * @param search character to search for
 * @return The position of search or -1 if not found
 */
O_INLINE
osize o_str_find_char(const char *str, char search)
{
    return o_str_find_set(str, (char[2]) {search, '\0'});
}

/**
 * Searches for the first occurrence of a whitespace (isspace) in the string
 * @param str String to check, may be NULL and returns -1 then
 * @return The position of the whitespace or -1 if not found
 */
O_INLINE
osize o_str_find_space(const char *str)
{
    return o_str_find_set(str, O_ISSPACE_SET);
}

/**
 * Searches for the last occurrence of search in the string
 * @param str String to check, NULL safe (returns -1)
 * @param search String to search for, NULL safe (returns -1)
 * @return The position of search or -1 if not found
 */
O_EXTERN
osize o_str_find_back(const char *str, const char *search);

/**
 * Searches for the last occurrence of one character of set in the string
 * @param str String to check, may be NULL and returns -1 then
 * @param set characters in a set string, may be NULL and returns -1 then)
 * @return The position of a char from set or -1 if not found
 */
O_EXTERN
osize o_str_find_back_set(const char *str, const char *set);

/**
 * Searches for the last occurrence of a character that is not in set, in the string
 * @param str String to check, may be NULL and returns -1 then
 * @param set characters in a set string, may be NULL and returns -1 then)
 * @return The position of search or -1 if not found
 */
O_EXTERN
osize o_str_find_back_set_inv(const char *str, const char *set);

/**
 * Searches for the last occurrence of the char search in the string
 * @param str String to check, may be NULL and returns -1 then
 * @param search character to search for
 * @return The position of search or -1 if not found
 */
O_INLINE
osize o_str_find_back_char(const char *str, char search)
{
    return o_str_find_back_set(str, (char[2]) {search, '\0'});
}

/**
 * Searches for the last occurrence of a whitespace (isspace) in the string
 * @param str String to check, may be NULL and returns -1 then
 * @return The position of the whitespace or -1 if not found
 */
O_INLINE
osize o_str_find_back_space(const char *str)
{
    return o_str_find_back_set(str, O_ISSPACE_SET);
}

/**
 * Returns the amount of "search" found in the string.
 * If str is smth like "aaa" and search is "aa", the result is 1 (no aliasing).
 * @param str String to check, NULL safe (returns 0)
 * @param search String to search, NULL safe (returns 0)
 * @return the amount of matches
 */
O_EXTERN
osize o_str_count(const char *str, const char *search);

/**
 * Replaces each occurrence of search in str with replace
 * @param in_out_str to replace in, NULL safe
 * @param search character to be replaced
 * @param replace replaces search
 * @param replaced characters
 */
O_EXTERN
osize o_str_replace_char_this(char *in_out_str, char search, char replace);

/**
 * Creates a new string with a replaced version of str
 * @param obj to allocate the resource
 * @param str to replace from, NULL safe
 * @param search character to be replaced
 * @param replace replaces search
 * @param out_opt_replaced optional replaced characters
 * @return The new string as resource from obj
 * @threadsafe (because allocations with objects are threadsafe)
 */
O_EXTERN
char *o_str_replace_char(oobj obj, const char *str, char search, char replace, osize *out_opt_replaced);

/**
 * Replaces each occurrence of search in str with replace, allocating a new string as result
 * @param obj to allocate the resource
 * @param str to replace from, NULL safe
 * @param search string to search in str, NULL safe
 * @param replace matches are replaced by this string, NULL safe (uses "" instead)
 * @param out_opt_replaced optional replaced strings
 * @return The new string as resource from obj, or NULL if str was NULL
 * @threadsafe (because allocations with objects are threadsafe)
 */
O_EXTERN
char *o_str_replace(oobj obj, const char *str, const char *search, const char *replace, osize *out_opt_replaced);


/**
 * Positions for o_str_trim* functions
 */
enum o_str_trim_whence {
    o_str_trim_LEFT,
    o_str_trim_RIGHT,
    o_str_trim_BOTH,
    o_str_trim_NUM_WHENCE,
};

/**
 * Trims all characters in set from the string, returning a new allocated string
 * @param obj to allocate the resource
 * @param str to replace from, NULL safe
 * @param set characters in a set string, may be NULL and returns -1 then)
 * @param whence trim left, right or both (left + right)
 * @return The new string as resource from obj, or NULL if str was NULL
 * @threadsafe (because allocations with objects are threadsafe)
 */
O_EXTERN
char *o_str_trim_set(oobj obj, const char *str, const char *set, enum o_str_trim_whence whence);

/**
 * Trims the given char from the string, returning a new allocated string
 * @param obj to allocate the resource
 * @param str to replace from, NULL safe
 * @param trim character to trim
 * @param whence trim left, right or both (left + right)
 * @return The new string as resource from obj, or NULL if str was NULL
 * @threadsafe (because allocations with objects are threadsafe)
 */
O_INLINE
char *o_str_trim_char(oobj obj, const char *str, char trim, enum o_str_trim_whence whence)
{
    return o_str_trim_set(obj, str, (char[2]) {trim, '\0'}, whence);
}

/**
 * Trims spaces from the string, returning a new allocated string
 * @param obj to allocate the resource
 * @param str to replace from, NULL safe
 * @param whence trim left, right or both (left + right)
 * @return The new string as resource from obj, or NULL if str was NULL
 * @threadsafe (because allocations with objects are threadsafe)
 */
O_INLINE
char *o_str_trim(oobj obj, const char *str, enum o_str_trim_whence whence)
{
    return o_str_trim_set(obj, str, O_ISSPACE_SET, whence);
}

/**
 * Splits a string into a list of ranges, allocated on object
 * terminated with a cleared element to 0 {0, 0}
 * @param obj to allocate the resource
 * @param str to split from, NULL safe
 * @param split delimiter string, NULL safe
 * @return An allocated element cleared terminated list of allocated ranges.
 *         The list is always allocated, but may be empty (first entry is cleared)
 * @note ignores empty splits
 * @threadsafe (because allocations with objects are threadsafe)
 */
O_EXTERN
struct o_str_range_list o_str_split_ranges(oobj obj, const char *str, const char *split);

/**
 * Splits a string into a list of strings (all allocated on obj)
 * So to free manually, traverse list and free each entry, then free the list, too
 *  -> Or just use an container / temp OObj and delete that...
 * @param obj to allocate the resource
 * @param str to split from, NULL safe
 * @param split delimiter string, NULL safe
 * @return An allocated null terminated list of allocated split strings.
 *         The list is always allocated, but may be empty (first entry is NULL)
 * @note ignores empty splits
 * @threadsafe (because allocations with objects are threadsafe)
 */
O_EXTERN
char **o_str_split(oobj obj, const char *str, const char *split);


/**
 * @return true if all chars of the given str are lower (NULL safe, returns true)
 * @note not exactly islower, implemented as !isupper...
 */
O_EXTERN
bool o_str_islower(const char *str);

/**
 * @return true if all chars of the given str are upper (NULL safe, returns true)
 * @note not exactly isupper, implemented as !islower...
 */
O_EXTERN
bool o_str_isupper(const char *str);

/**
 * Replaces each character in str with the lower version
 * @param str inout string to lower, NULL safe
 */
O_EXTERN
void o_str_tolower_this(char *str);

/**
 * Replaces each character in str with the upper version
 * @param str inout string to upper, NULL safe
 */
O_EXTERN
void o_str_toupper_this(char *str);

/**
 * Creates a new string with a lower version of str
 * @param obj to allocate the resource
 * @param str string to clone into a lower version, NULL safe
 * @return The new string as resource from obj
 * @threadsafe (because allocations with objects are threadsafe)
 */
O_EXTERN
char *o_str_tolower(oobj obj, const char *str);


/**
 * Creates a new string with an upper version of str
 * @param obj to allocate the resource
 * @param str string to clone into an upper version, NULL safe
 * @return The new string as resource from obj
 * @threadsafe (because allocations with objects are threadsafe)
 */
O_EXTERN
char *o_str_toupper(oobj obj, const char *str);


/**
 * Simple string hashing
 * @param str to be hashed, NULL sage
 * @return an unsigned 32 bit hash
 */
O_EXTERN
ou32 o_str_hash(const char *str);

/**
 * Escapes the string: a real newline will be replaced with "\n", etc.
 * Escape list according to javascript / json: https://en.wikipedia.org/wiki/Escape_character
 * @param obj to allocate the resource
 * @param str to create the escaped version, NULL safe
 * @return the new allocated escaped string, or NULL if str is NULL *
 * @threadsafe (because allocations with objects are threadsafe)
 */
O_EXTERN
char *o_str_escape(oobj obj, const char *str);

/**
 * Unescapes the string: a "\n" will get a real newline.
 * Escape list according to javascript / json: https://en.wikipedia.org/wiki/Escape_character
 * @param obj to allocate the resource
 * @param str to create the unescaped version, NULL safe
 * @return the new allocated unescaped string, or NULL if str is NULL
 * @threadsafe (because allocations with objects are threadsafe)
 */
O_EXTERN
char *o_str_unescape(oobj obj, const char *str);

/**
 * Encodes binary data to Base64 representation.
 * @param obj to allocate the resource
 * @param data binary data to encode, NULL safe
 * @param size size of the data buffer, <=0 safe
 * @return The new string containing the Base64 representation as a resource from obj,
 *         or NULL if data is NULL or size is 0.
 * @threadsafe (because allocations with objects are threadsafe)
 */
O_EXTERN
char *o_str_base64_encode(oobj obj, const void *data, osize size);

/**
 * Decodes a Base64 encoded string to binary data.
 * @param obj to allocate the resource
 * @param base64_str Base64 encoded string to decode, NULL safe
 * @param out_size Pointer to a variable to store the size of the decoded data.
 * @return The new buffer containing the decoded binary data as a resource from obj,
 *         or NULL if base64_str is NULL or decoding fails.
 * @note The caller is responsible for freeing the returned buffer.
 * @threadsafe (because allocations with objects are threadsafe)
 */
O_EXTERN
void *o_str_base64_decode(oobj obj, osize *out_size, const char *base64_str);

/**
 * Finds a lua-like pattern on the string and returns the first position.
 * Uses OPattern internally
 * @param obj to allocate the resources (out_opt_captures list and internal usage)
 * @param pattern to be compiled with OPattern
 * @param str to be matched
 * @param out_opt_captures if not NULL, a list of captures will be allocated on obj
´´ * @return the range of the next match, or a cleared element (.len = 0) for not found
 * @sa OPattern, o_str_range_list_move, o_str_range_list_free
 */
O_EXTERN
struct o_str_range o_str_pattern_find(oobj obj, const char *pattern, const char *str,
                                      struct o_str_range_list *out_opt_captures);

/**
 * Matches a lua-like pattern on the string (the full string must match to the pattern).
 * Uses OPattern internally
 * @param obj to allocate the resources (out_opt_captures list and internal usage)
 * @param pattern to be compiled with OPattern
 * @param str to be matched
 * @param out_opt_captures if not NULL, a list of captures will be allocated on obj
 * @return true if matched
 * @sa OPattern, o_str_range_list_move, o_str_range_list_free
 */
O_EXTERN
bool o_str_pattern_match(oobj obj, const char *pattern, const char *str,
                         struct o_str_range_list *out_opt_captures);

/**
 * Find a lua-like pattern on the string and returns all findings.
 * Uses OPattern internally.
 * @param obj to allocate the resource
 * @param out_opt_matches if not NULL, a list of ranges is created, containing all matches
 *                        element cleared terminated, or use the return value for its size
 *
 * @param pattern to be compiled with OPattern
 * @param str to be matched, NULL safe
 * @param out_opt_captures_list if not NULL, a list of a list of captures is created
 * @return findings counter, equal to the generated lists sizes for: out_opt_matches, out_opt_captures_list
 * @sa OPattern, o_str_range_list_list_move, o_str_range_list_list_free
 */
O_EXTERN
osize o_str_pattern_find_all(oobj obj, struct o_str_range_list *out_opt_matches,
                             const char *pattern, const char *str,
                             struct o_str_range_list_list *out_opt_captures_list);

/**
 * Replaces all lua-like pattern findings with the replace string.
 * Uses OPattern internally.
 * @param obj to allocate the resource
 * @param pattern to be compiled with OPattern
 * @param str to be matched
 * @param replace substitutes all matches with this string
 * @param out_opt_replaced if not NULL, will be set to the amount of replaced matches
 * @return A new allocated string with all matches replaces (allocated on obj)
 * @threadsafe (because allocations with objects are threadsafe)
 * @sa OPattern
 */
O_EXTERN
char *o_str_pattern_replace(oobj obj, const char *pattern, const char *str, const char *replace,
                            osize *out_opt_replaced);


#endif //O_STR_H
