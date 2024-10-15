#include "o/str.h"
#include "o/OObj.h"
#include "o/OPattern.h"
#include "o/OObjRoot.h"  // for internal work
#include <stdlib.h>
#include <ctype.h>


//
// str_range.h
//

struct o_str_range_list o_str_range_list_new(oobj obj, osize list_len)
{
    return (struct o_str_range_list) {
            o_new0(obj, struct o_str_range, list_len + 1),
            list_len,
            obj
    };
}

void o_str_range_list_move(struct o_str_range_list *list, oobj into)
{
    o_mem_move(list->parent, into, list->list);
    list->parent = into;
}

void o_str_range_list_free(struct o_str_range_list *list)
{
    if(!list->parent) {
        return;
    }
    o_free(list->parent, list->list);
    *list = (struct o_str_range_list) {0};
}

struct o_str_range_list_list o_str_range_list_list_new(oobj obj, osize list_list_len, osize list_len)
{
    struct o_str_range_list_list ret = {
            o_new0(obj, struct o_str_range_list, list_list_len + 1),
            list_list_len,
            obj
    };
    for(osize i=0; i<list_len; i++) {
        ret.list[i] = o_str_range_list_new(obj, list_len);
    }
    return ret;
}

void o_str_range_list_list_move(struct o_str_range_list_list *list_list, oobj into)
{
    for (osize i = 0; i < list_list->len; i++) {
        o_str_range_list_move(&list_list->list[i], into);
    }
    o_mem_move(list_list->parent, into, list_list->list);
    list_list->parent = into;
}

void o_str_range_list_list_free(struct o_str_range_list_list *list_list)
{
    if(!list_list->parent) {
        return;
    }
    for (osize i = 0; i < list_list->len; i++) {
        o_str_range_list_free(&list_list->list[i]);
    }
    o_free(list_list->parent, list_list->list);
    *list_list = (struct o_str_range_list_list) {0};
}


//
// str.h
//


char *o_str_clone_sub_realloc(oobj obj, void *restrict mem, const char *to_clone, osize start, osize len)
{
    assert(!mem || mem != to_clone);

    // free?
    if (!to_clone) {
        o_free(obj, mem);
    }

    start = o_max(0, start);
    osize max_size = o_strlen(to_clone) - start;
    len = o_min(len, max_size);

    if (!to_clone || len <= 0) {
        return NULL;
    }
    char *str = o_renew(obj, mem, char, len + 1);
    o_memcpy(str, to_clone + start, 1, len);
    str[len] = '\0';
    return str;
}

char **o_str_ranges_to_strings(oobj obj, const char *str, const struct o_str_range *ranges, osize opt_len)
{
    if (opt_len < 0) {
        opt_len = o_str_ranges_size(ranges);
    }
    if (!str || !ranges || opt_len == 0) {
        return o_new0(obj, char *, 1);
    }
    char **list = o_new0(obj, char *, opt_len + 1);
    for (osize i = 0; i < opt_len; i++) {
        list[i] = o_str_clone_sub(obj, str, ranges[i].start, ranges[i].len);
    }
    return list;
}

char *o_str_ranges_replace(oobj obj, const char *str,
                           const struct o_str_range *ranges, osize opt_len,
                           const char *replace)
{
    if (opt_len < 0) {
        opt_len = o_str_ranges_size(ranges);
    }
    if (!str) {
        return NULL;
    }
    if (!ranges || opt_len == 0) {
        return o_str_clone(obj, str);
    }
    replace = o_or(replace, "");

    osize str_len = o_strlen(str);
    osize replace_len = o_strlen(replace);

    // calc result length
    osize res_len = str_len + opt_len * replace_len;
    for (osize i = 0; i < opt_len; i++) {
        res_len -= ranges[i].len;
    }

    char *res = o_new(obj, char, res_len + 1);

    osize res_start = 0;
    osize range_end = 0;
    for (osize i = 0; i < opt_len; i++) {
        assert(ranges[i].start >= range_end && "ranges must be ordered!");
        // copy bytes until the range
        o_memcpy(res + res_start, str + range_end, 1, ranges[i].start - range_end);
        res_start += (ranges[i].start - range_end);
        // copy replace
        o_memcpy(res + res_start, replace, 1, replace_len);
        res_start += replace_len;
        range_end = ranges[i].start + ranges[i].len;
    }
    // copy remaining of string
    o_memcpy(res + res_start, str + range_end, 1, str_len - range_end);
    res_start += (str_len - range_end);

    // don't forget the null terminator...
    res[res_start] = '\0';

    return res;
}

char *o_strf_va(oobj obj, const char *format, va_list args)
{
    va_list args2;
    va_copy(args2, args);

    osize n = vsnprintf(NULL, 0, format, args);
    char *res = o_new(obj, char, n + 1);
    vsnprintf(res, n + 1, format, args2);

    va_end(args2);

    return res;
}


char *o_strf(oobj obj, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    char *res = o_strf_va(obj, format, args);
    va_end(args);
    return res;
}

char *o_strf_realloc_va(oobj obj, char *old, const char *format, va_list args)
{
    // create a new formated text, so the old string may be used in the format
    char *text = o_strf_va(obj, format, args);

    // clone text into the reallocated old string
    char *res = o_renew(obj, old, char, o_strlen(text) + 1);
    o_memcpy(res, text, 1, o_strlen(text) + 1);

    // free the now unnecessary text
    o_free(obj, text);
    return res;
}

char *o_strf_realloc(oobj obj, char *old, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    char *res = o_strf_realloc_va(obj, old, format, args);
    va_end(args);
    return res;
}

char *o_str_join(oobj obj, char **list_of_strings, osize n, const char *opt_seperator)
{
    if (!opt_seperator) {
        opt_seperator = "";
    }

    osize len = 0;
    osize sep_len = o_strlen(opt_seperator);
    osize *slen = o_new(obj, osize, n);

    for (osize i = 0; i < n; i++) {
        slen[i] = o_strlen(list_of_strings[i]);
        len += slen[i];
    }

    len += sep_len * (n - 1);

    char *res = NULL;
    if (len <= 0) {
        goto CLEAN_UP;
    }

    res = o_new(obj, char, len + 1);
    len = 0;
    for (int i = 0; i < n; i++) {
        o_memcpy(res + len, list_of_strings[i], 1, slen[i]);
        len += slen[i];
        if(i<n-1) {
            o_memcpy(res + len, opt_seperator, 1, sep_len);
            len += sep_len;
        }
    }

    // don`t forget the null terminator
    res[len] = '\0';

    CLEAN_UP:
    o_free(obj, slen);

    return res;
}

char *o_str_rep(oobj obj, const char *str, osize n, const char *opt_seperator)
{
    if (!str) {
        return NULL;
    }

    char **strlist = o_new(obj, char *, n);

    for (osize i = 0; i < n; i++) {
        strlist[i] = (char *) str;
    }
    char *res = o_str_join(obj, strlist, n, opt_seperator);

    o_free(obj, strlist);

    return res;
}

bool o_str_begins(const char *str, const char *begin)
{
    if (!str || !begin) {
        return false;
    }
    osize str_len = o_strlen(str);
    osize begin_len = o_strlen(begin);
    if (begin_len > str_len) {
        return false;
    }
    return memcmp(str, begin, begin_len) == 0;
}

bool o_str_ends(const char *str, const char *end)
{
    if (!str || !end) {
        return false;
    }
    osize str_len = o_strlen(str);
    osize end_len = o_strlen(end);
    if (end_len > str_len) {
        return false;
    }
    return memcmp(str + str_len - end_len, end, end_len) == 0;
}

osize o_str_find(const char *str, const char *search)
{
    if (!str || !search) {
        return -1;
    }
    osize str_len = o_strlen(str);
    osize search_len = o_strlen(search);
    if (search_len > str_len) {
        return -1;
    }

    // +1 because if the str is "hi" and search equals str ("hi"), the loop should check position 0
    for (osize i = 0; i < 1 + str_len - search_len; i++) {
        if (memcmp(str + i, search, search_len) == 0) {
            return i;
        }
    }
    return -1;
}

struct o_str_range_list o_str_find_all(oobj obj, const char *str, const char *search)
{
    // null safe code
    osize len = o_str_count(str, search);
    osize search_len = o_strlen(search);
    struct o_str_range *ranges = o_new0(obj, struct o_str_range, len + 1);
    osize start = 0;
    for (osize i = 0; i < len; i++) {
        osize next = o_str_find(str + start, search);
        assert(next >= 0);
        start += next;
        ranges[i].start = start;
        ranges[i].len = search_len;
        start += search_len;
    }
    return (struct o_str_range_list) {ranges, len, obj};
}

osize o_str_find_set(const char *str, const char *set)
{
    // the code is NULL safe, o_strlen return 0 on NULL
    osize str_len = o_strlen(str);
    osize set_len = o_strlen(set);
    for (osize i = 0; i < str_len; i++) {
        for (osize s = 0; s < set_len; s++) {
            if (str[i] == set[s]) {
                return i;
            }
        }
    }
    return -1;
}

osize o_str_find_set_inv(const char *str, const char *set)
{
    // the code is NULL safe, o_strlen return 0 on NULL
    osize str_len = o_strlen(str);
    osize set_len = o_strlen(set);
    for (osize i = 0; i < str_len; i++) {
        bool found = false;
        for (osize s = 0; s < set_len; s++) {
            if (str[i] == set[s]) {
                found = true;
                break;
            }
        }
        if (!found) {
            return i;
        }
    }
    return -1;
}

osize o_str_find_back(const char *str, const char *search)
{
    if (!str || !search) {
        return -1;
    }
    osize str_len = o_strlen(str);
    osize search_len = o_strlen(search);
    if (search_len > str_len) {
        return -1;
    }

    for (osize i = str_len - search_len; i >= 0; i--) {
        if (memcmp(str + i, search, search_len) == 0) {
            return i;
        }
    }
    return -1;
}

osize o_str_find_back_set(const char *str, const char *set)
{
    // the code is NULL safe, o_strlen return 0 on NULL
    osize str_len = o_strlen(str);
    osize set_len = o_strlen(set);
    for (osize i = str_len - 1; i >= 0; i--) {
        for (osize s = 0; s < set_len; s++) {
            if (str[i] == set[s]) {
                return i;
            }
        }
    }
    return -1;
}

osize o_str_find_back_set_inv(const char *str, const char *set)
{
    // the code is NULL safe, o_strlen return 0 on NULL
    osize str_len = o_strlen(str);
    osize set_len = o_strlen(set);
    for (osize i = str_len - 1; i >= 0; i--) {
        bool found = false;
        for (osize s = 0; s < set_len; s++) {
            if (str[i] == set[s]) {
                found = true;
                break;
            }
        }
        if (!found) {
            return i;
        }
    }
    return -1;
}

osize o_str_count(const char *str, const char *search)
{
    osize cnt = 0;
    osize start = 0;
    osize search_len = o_strlen(search);
    for (;;) {
        osize next = o_str_find(str + start, search);
        if (next < 0) {
            break;
        }
        start += next + search_len;
        cnt++;
    }
    return cnt;
}

osize o_str_replace_char_this(char *in_out_str, char search, char replace)
{
    osize cnt = 0;
    for (osize i = 0; i < o_strlen(in_out_str); i++) {
        if (in_out_str[i] == search) {
            in_out_str[i] = replace;
            cnt++;
        }
    }
    return cnt;
}

char *o_str_replace_char(oobj obj, const char *str, char search, char replace, osize *out_opt_replaced)
{
    // the code is NULL safe, o_str_clone returns NULL on NULL
    char *res = o_str_clone(obj, str);
    osize cnt = o_str_replace_char_this(res, search, replace);
    o_opt_set(out_opt_replaced, cnt);
    return res;
}


char *o_str_replace(oobj obj, const char *str, const char *search, const char *replace, osize *out_opt_replaced)
{
    struct o_str_range_list ranges = o_str_find_all(obj, str, search);
    char *res = o_str_ranges_replace(obj, str, ranges.list, ranges.len, replace);
    o_opt_set(out_opt_replaced, ranges.len);
    o_str_range_list_free(&ranges);
    return res;
}

char *o_str_trim_set(oobj obj, const char *str, const char *set, enum o_str_trim_whence whence)
{
    // the code should be fine for passing NULLs
    osize start, end;
    if (whence == o_str_trim_LEFT || whence == o_str_trim_BOTH) {
        start = o_str_find_set_inv(str, set);
        start = o_max(0, start);
    } else {
        start = 0;
    }
    if (whence == o_str_trim_RIGHT || whence == o_str_trim_BOTH) {
        end = o_str_find_back_set_inv(str, set);
        end = o_min(end, o_strlen(str));
    } else {
        end = o_strlen(str);
    }

    // +1 because if start and end are the same indexed character, that character should be copied
    osize len = 1 + end - start;
    // safe for len < 0
    return o_str_clone_sub(obj, str, start, len);
}

struct o_str_range_list o_str_split_ranges(oobj obj, const char *str, const char *split)
{
    // NULL safe, returns 0, -> 1
    osize splits = o_str_count(str, split) + 1;

    // null terminated list
    struct o_str_range *ranges = o_new0(obj, struct o_str_range, splits + 1);

    osize split_len = o_strlen(split);

    struct o_str_range *it = ranges;
    osize start = 0;
    for (osize i = 0; i < splits; i++) {
        osize len = o_str_find(str + start, split);
        len = len >= 0 ? len : o_strlen(str + start);
        if (len > 0) {
            // ignore empty strings
            *it++ = (struct o_str_range) {start, len};
        }
        start += len + split_len;
    }
    return (struct o_str_range_list) {ranges, splits, obj};
}

char **o_str_split(oobj obj, const char *str, const char *split)
{
    struct o_str_range_list ranges = o_str_split_ranges(obj, str, split);
    char **list = o_str_ranges_to_strings(obj, str, ranges.list, ranges.len);
    o_str_range_list_free(&ranges);
    return list;
}

bool o_str_islower(const char *str)
{
    // the code is NULL safe, o_strlen return 0 on NULL
    osize str_len = o_strlen(str);
    for (osize i = 0; i < str_len; i++) {
        if(isupper(str[i])) {
            return false;
        }
    }
    return true;
}

bool o_str_isupper(const char *str)
{
    // the code is NULL safe, o_strlen return 0 on NULL
    osize str_len = o_strlen(str);
    for (osize i = 0; i < str_len; i++) {
        if(islower(str[i])) {
            return false;
        }
    }
    return true;
}

void o_str_tolower_this(char *str)
{
    // the code is NULL safe, o_strlen return 0 on NULL
    osize str_len = o_strlen(str);
    for (osize i = 0; i < str_len; i++) {
        str[i] = (char) tolower(str[i]);
    }
}

void o_str_toupper_this(char *str)
{
    // the code is NULL safe, o_strlen return 0 on NULL
    osize str_len = o_strlen(str);
    for (osize i = 0; i < str_len; i++) {
        str[i] = (char) toupper(str[i]);
    }
}

char *o_str_tolower(oobj obj, const char *str)
{
    // the code is NULL safe, o_str_clone returns NULL on NULL
    char *res = o_str_clone(obj, str);
    o_str_tolower_this(res);
    return res;
}

char *o_str_toupper(oobj obj, const char *str)
{
    // the code is NULL safe, o_str_clone returns NULL on NULL
    char *res = o_str_clone(obj, str);
    o_str_toupper_this(res);
    return res;
}

ou32 o_str_hash(const char *str)
{
    ou32 hash = 5381;
    if (!str) {
        return hash;
    }
    int c;
    while ((c = (int) *str++))
        hash = ((hash << 5u) + hash) + c; /* hash * 33 + c */
    return hash;
}


/**
 * Internal function
 * @param hex_byte_chars string of which the first 2 chars are used
 * @return the byte value
 */
O_STATIC
obyte obyte_from_hex(const char *hex_byte_chars)
{
    if (!hex_byte_chars || *hex_byte_chars == '\0')
        return 0;
    char hex[3] = {hex_byte_chars[0], hex_byte_chars[1], '\0'};
    return (obyte) strtoul(hex, NULL, 16);
}

char *o_str_escape(oobj obj, const char *str)
{
    if (!str) {
        return NULL;
    }

    osize str_len = o_strlen(str);

    // the new string is either the same size or max 4x bigger (each not known byte -> "\xFF" (1 byte -> 4 bytes)
    char *res = o_new(obj, char, str_len * 4 + 1);

    char *it = res;
    while (*str) {
        if (*str == '\'') {
            *it++ = '\\';
            *it++ = '\'';
        } else if (*str == '\"') {
            *it++ = '\\';
            *it++ = '\"';
        } else if (*str == '\\') {
            *it++ = '\\';
            *it++ = '\\';
        } else if (*str == '\n') {
            *it++ = '\\';
            *it++ = 'n';
        } else if (*str == '\r') {
            *it++ = '\\';
            *it++ = 'r';
        } else if (*str == '\t') {
            *it++ = '\\';
            *it++ = 't';
        } else if (*str == '\b') {
            *it++ = '\\';
            *it++ = 'b';
        } else if (*str == '\f') {
            *it++ = '\\';
            *it++ = 'f';
        } else if (*str >= '\x00' && *str <= '\x1F') {
            // Handle characters in the range \x00-\x1F
            *it++ = '\\';
            *it++ = 'x';
            *it++ = "0123456789ABCDEF"[(*str >> 4) & 0xF];
            *it++ = "0123456789ABCDEF"[*str & 0xF];
        } else {
            *it++ = *str;
        }
        str++;
    }
    // null terminate
    *it = '\0';
    osize res_len = it - res;
    assert(res_len <= str_len * 4);
    return res;
}

char *o_str_unescape(oobj obj, const char *str)
{
    if (!str) {
        return NULL;
    }

    osize str_len = o_strlen(str);

    // the new string is either the same size or smaller "\n": 2 bytes -> 1 byte
    char *res = o_new(obj, char, str_len + 1);

    char *it = res;
    while (*str) {
        if (*str == '\\' && str[1] != '\0') {
            str++;  // Skip the backslash

            switch (*str) {
                case '\'':
                    *it++ = '\'';
                    break;
                case '\"':
                    *it++ = '\"';
                    break;
                case '\\':
                    *it++ = '\\';
                    break;
                case 'n':
                    *it++ = '\n';
                    break;
                case 'r':
                    *it++ = '\r';
                    break;
                case 't':
                    *it++ = '\t';
                    break;
                case 'b':
                    *it++ = '\b';
                    break;
                case 'f':
                    *it++ = '\f';
                    break;
                case 'x':
                    *it++ = (char) obyte_from_hex(str);
                    str += 2;
                    break;
                default:
                    // Invalid escape sequence, treat the backslash and following character as literal
                    *it++ = '\\';
                    *it++ = *str;
                    break;
            }
        } else {
            *it++ = *str;
        }
        str++;
    }

    // null terminate
    *it = '\0';
    osize res_len = it - res;
    assert(res_len <= str_len);
    return res;
}


/** Base64 character set for encoding */
static const char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/**
 * Helper function to check if a character is a valid Base64 character.
 */
O_STATIC
bool is_base64_char(char c)
{
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '+' || c == '/';
}

char *o_str_base64_encode(oobj obj, const void *data, osize size)
{
    if (!data || size == 0)
        return NULL;

    const obyte *input = (const obyte *) data;
    osize output_size = ((size + 2) / 3) * 4; // Calculate the size of the Base64 output string
    char *output = o_new(obj, char, output_size + 1); // Allocate memory for the output string

    if (!output)
        return NULL;

    osize out_pos = 0;
    ou32 bits = 0;
    int bit_count = 0;

    for (osize i = 0; i < size; ++i) {
        bits = (bits << 8) | input[i];
        bit_count += 8;

        while (bit_count >= 6) {
            bit_count -= 6;
            output[out_pos++] = base64_chars[(bits >> bit_count) & 0x3F];
        }
    }

    if (bit_count > 0) {
        bits <<= (6 - bit_count);
        output[out_pos++] = base64_chars[bits & 0x3F];
    }

    while (out_pos < output_size)
        output[out_pos++] = '=';

    output[out_pos] = '\0';
    return output;
}


void *o_str_base64_decode(oobj obj, osize *out_size, const char *base64_str)
{
    if (!base64_str)
        return NULL;

    osize input_len = o_strlen(base64_str);
    if (input_len == 0)
        return NULL;

    // Calculate the size of the decoded binary data
    osize output_size = (input_len * 3 + 3) / 4;
    obyte *output = o_new(obj, obyte, output_size + 1);

    if (!output)
        return NULL;

    ou32 bits = 0;
    int bit_count = 0;
    osize out_pos = 0;

    for (osize i = 0; i < input_len; ++i) {
        char c = base64_str[i];
        if (!is_base64_char(c))
            continue;

        bits = (bits << 6) | (strchr(base64_chars, c) - base64_chars);
        bit_count += 6;

        if (bit_count >= 8) {
            bit_count -= 8;
            output[out_pos++] = (bits >> bit_count) & 0xFF;
        }
    }

    // null terminator just in case the data is a string...
    output[output_size] = '\0';

    *out_size = out_pos;
    return output;
}


struct o_str_range o_str_pattern_find(oobj obj, const char *pattern, const char *str,
                                      struct o_str_range_list *out_opt_captures)
{
    oobj p = OPattern_new(obj, pattern);
    struct o_str_range_list captures = o_str_range_list_new(obj, OPattern_captures(p));
    struct o_str_range range = OPattern_find(p, str, captures.list);
    if (out_opt_captures) {
        *out_opt_captures = captures;
    } else {
        o_str_range_list_free(&captures);
    }
    o_del(p);
    return range;
}

bool o_str_pattern_match(oobj obj, const char *pattern, const char *str,
                         struct o_str_range_list *out_opt_captures)
{
    oobj p = OPattern_new(obj, pattern);
    struct o_str_range_list captures = o_str_range_list_new(obj, OPattern_captures(p));
    bool matched = OPattern_match(p, str, captures.list);
    if (out_opt_captures) {
        *out_opt_captures = captures;
    } else {
        o_str_range_list_free(&captures);
    }
    o_del(p);
    return matched;
}

osize o_str_pattern_find_all(oobj obj, struct o_str_range_list *out_opt_matches,
                             const char *pattern, const char *str,
                             struct o_str_range_list_list *out_opt_captures_list)
{
    oobj p = OPattern_new(obj, pattern);
    struct o_str_range_list matches;
    struct o_str_range_list_list captures_list;
    osize cnt = OPattern_find_all(p, &matches, &captures_list, str);
    if (out_opt_matches) {
        o_str_range_list_move(&matches, obj);
        *out_opt_matches = matches;
    }
    if (out_opt_captures_list) {
        o_str_range_list_list_move(&captures_list, obj);
        *out_opt_captures_list = captures_list;
    }
    o_del(p);
    return cnt;
}

char *o_str_pattern_replace(oobj obj, const char *pattern, const char *str, const char *replace,
                            osize *out_opt_replaced)
{
    oobj p = OPattern_new(obj, pattern);
    char *res = OPattern_replace(p, str, replace, out_opt_replaced);
    o_mem_move(p, obj, res);
    o_del(p);
    return res;
}
