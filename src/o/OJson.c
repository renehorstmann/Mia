#include "o/OJson.h"
#include "o/OObj_builder.h"
#include "o/OStreamBuffered.h"
#include "o/OArray.h"
#include "o/OStreamArray.h"
#include "o/OStreamMem.h"
#include "o/file.h"
#include "o/str.h"
#include <stdlib.h>

#define O_LOG_LIB "o"
#include "o/log.h"

#define INDENT_LEVEL "  "

#define LOG_PREV 16
#define LOG_NEXT 8

O_STATIC
void log_parse_failed(OStream *stream, const char *msg)
{
    _Static_assert(LOG_PREV < 32 && LOG_NEXT < 32, "invalid buffer");
    char prev[32] = {0};
    char next[32] = {0};
    int prev_num = o_min(LOG_PREV, OStream_seek(stream, 0, OStream_SEEK_CUR));
    OStream_seek(stream, -prev_num, OStream_SEEK_CUR);
    OStream_read(stream, prev, 1, prev_num);
    OStream_read(stream, next, 1, LOG_NEXT);
    OStream_seek(stream, -LOG_NEXT, OStream_SEEK_CUR);

    o_str_replace_char_this(prev, '\n', ' ');
    o_str_replace_char_this(next, '\n', ' ');

    o_log_warn_s("OJson_new_read_stream",
               "Parsing failed: %s at: [..]%s<!!!>%s[..]", msg, prev, next);
}

O_STATIC
void log_stream_failed(void)
{
    o_log_warn_s("OJson_new_read_stream",
               "stream failed to read");
}

O_STATIC
osize write_stream_r(oobj container, OJson *json, OStream *stream, int lvl);

O_STATIC
osize write_stream_newline(OStream *stream, int lvl)
{
    osize written = 0;
    written += OStream_print(stream, "\n");
    for (int i = 0; i < lvl; i++) {
        written += OStream_print(stream, INDENT_LEVEL);
    }
    return written;
}

O_STATIC
osize write_stream_object(oobj container, OJson *json_object, OStream *stream, int lvl)
{
    osize written = 0;
    OJson **list = OJson_list(json_object, NULL);

    // not a single child found
    if (*list == NULL) {
        written += OStream_print(stream, "{}");
        o_free(json_object, list);
        return written;
    }

    written += OStream_print(stream, "{");
    OJson **it = list;
    while (*it) {
        OJson *child = *it++;
        if (!json_object->packed) {
            written += write_stream_newline(stream, lvl + 1);
        }
        written += OStream_printf(stream, "\"%s\": ", o_or(OObj_name(child), OJson_NAME_DEFAULT));
        written += write_stream_r(container, child, stream, lvl + 1);
        if (*it != NULL) {
            written += OStream_print(stream, ",");
        }
    }

    if (!json_object->packed) {
        written += write_stream_newline(stream, lvl);
    }
    written += OStream_print(stream, "}");

    o_free(json_object, list);
    return written;
}

O_STATIC
osize write_stream_array(oobj container, OJson *json_array, OStream *stream, int lvl)
{
    osize written = 0;
    OJson **list = OJson_list(json_array, NULL);

    // not a single child found, replacing with null
    if (*list == NULL) {
        written += OStream_print(stream, "[]");
        o_free(json_array, list);
        return written;
    }

    written += OStream_print(stream, "[");
    OJson **it = list;
    while (*it) {
        OJson *child = *it++;
        if (!json_array->packed) {
            written += write_stream_newline(stream, lvl + 1);
        }
        written += write_stream_r(container, child, stream, lvl + 1);
        if (*it != NULL) {
            written += OStream_print(stream, ",");
        }
    }

    if (!json_array->packed) {
        written += write_stream_newline(stream, lvl);
    }
    written += OStream_print(stream, "]");

    o_free(json_array, list);
    return written;
}

// recursive function, returns the num written chars
O_STATIC
osize write_stream_r(oobj container, OJson *json, OStream *stream, int lvl)
{
    switch (json->type) {
        case OJson_TYPE_NULL:
            return OStream_print(stream, "null");
        case OJson_TYPE_BOOLEAN:
            return OStream_print(stream, json->data.boolean ? "true" : "false");
        case OJson_TYPE_NUMBER:
            return OStream_printf(stream, "%g", json->data.number);
        case OJson_TYPE_STRING:
            return OStream_printf(stream, "\"%s\"", o_str_escape(container, json->data.string));
        case OJson_TYPE_OBJECT:
            return write_stream_object(container, json, stream, lvl);
        case OJson_TYPE_ARRAY:
            return write_stream_array(container, json, stream, lvl);
        default:
            o_log_warn_s("OJson_write_stream", "invalid type: %i", (int) json->type);
    }
    return 0;
}


O_STATIC
bool parse_null(oobj parent, oobj stream, const char *name)
{
    // n was already read
    char buf[4] = {0};
    OStream_read(stream, buf, 1, 3);
    if (!o_str_equals(buf, "ull")) {
        log_parse_failed(stream, "expected null");
        return false;
    }
    OJson_new_null(parent, name);
    return true;
}

O_STATIC
bool parse_boolean(oobj parent, oobj stream, const char *name, int first_char)
{
    char buf[16] = {0};
    buf[0] = (char) first_char;

    osize read = OStream_read_until_into(stream, NULL, &buf[1], sizeof buf - 2, O_ISSPACE_SET ",}]");
    if (read < 0) {
        log_stream_failed();
        return false;
    }

    // backseek the character to not drop it
    OStream_seek(stream, -1, OStream_SEEK_CUR);

    bool boolean = false;
    if (o_str_equals(buf, "true")) {
        boolean = true;
    } else if (!o_str_equals(buf, "false")) {
        log_parse_failed(stream, "failed to parse the boolean");
        return false;
    }
    OJson_new_boolean(parent, name, boolean);
    return true;
}

O_STATIC
bool parse_number(oobj parent, oobj stream, const char *name, int first_char)
{
    char buf[64] = {0};
    buf[0] = (char) first_char;

    osize read = OStream_read_until_into(stream, NULL, &buf[1], sizeof buf - 2, O_ISSPACE_SET ",}]");
    if (read < 0) {
        log_stream_failed();
        return false;
    }

    // backseek the character to not drop it
    OStream_seek(stream, -1, OStream_SEEK_CUR);

    char *end;
    double number = strtod(buf, &end);
    if (end == buf) {
        log_parse_failed(stream, "failed to parse the number");
        return false;
    }
    OJson_new_number(parent, name, number);
    return true;
}

// needed by parse_string and parse_object
O_STATIC
char *read_string(oobj parent, oobj stream)
{
    // we read in until char '\"', but it could be escaped, so we rerun and put these strings in a list
    oobj list = OArray_new_dyn(parent, NULL, sizeof(char *), 0, 8);
    char *ret = NULL;

    for (;;) {
        char *str = OStream_read_until(stream, NULL, osize_MAX, "\"");
        if (!str) {
            log_stream_failed();
            ret = NULL;
            goto CLEAN_UP;
        }

        // maybe directly closed
        osize len = o_strlen(str);
        if (len <= 1) {
            break;
        }

        // check if not escaped
        if (str[len - 2] != '\\') {
            // remove last '\"'
            str[len - 1] = '\0';

            // add end to list
            OArray_push(list, &str);
            break;
        }

        // add escaped to list
        OArray_push(list, &str);
    }

    char *cat = o_str_join(parent, OArray_data_void(list), OArray_num(list), NULL);
    ret = cat;

    CLEAN_UP:
    // clear list and cat (list is parent)
    o_del(list);
    return ret;
}

O_STATIC
bool parse_string(oobj parent, oobj stream, const char *name)
{
    char *str = read_string(parent, stream);
    if (str) {
        // will be cloned...
        OJson_new_string(parent, name, str);
        o_free(parent, str);
        return true;
    }
    return false;
}


// needed by parse_object, parse_array for recursion
O_STATIC
bool parse_stream_r(oobj parent, oobj stream, const char *name);

O_STATIC
bool parse_object(oobj parent, oobj stream, const char *name)
{

    // create the object
    oobj object_element = OJson_new_object(parent, name);

    for (;;) {
        // ignore whitespace and return the first not isspace character
        int first_char;
        OStream_read_isspace_into(stream, &first_char, NULL, osize_MAX);
        if (first_char == -1) {
            log_stream_failed();
            return false;
        }

        if (first_char == '}') {
            // object closed
            return true;
        }

        // should be '\"' for the name
        if (first_char != '\"') {
            log_parse_failed(stream, "expected beginning of an object name");
            return false;
        }

        char *child_name = read_string(parent, stream);
        if(!child_name) {
            return false;
        }

        // reading up to the colon ":"
        osize read = OStream_read_until_into(stream, NULL, NULL, osize_MAX, ":");
        if (read < 0) {
            log_stream_failed();
            o_free(parent, child_name);
            return false;
        }

        // recursively read the childs data
        bool ok = parse_stream_r(object_element, stream, child_name);
        o_free(parent, child_name);

        if(!ok) {
            return false;
        }

        // ',' or '}' ?
        OStream_read_isspace_into(stream, &first_char, NULL, osize_MAX);
        if (first_char == -1) {
            log_stream_failed();
            return false;
        }

        if(first_char == '}') {
            return true;
        }

        // should be ','
        if(first_char != ',') {
            log_parse_failed(stream, "expected a \',\'");
            return false;
        }
    }
    return false;
}

O_STATIC
bool parse_array(oobj parent, oobj stream, const char *name)
{

    // create the object
    oobj array_element = OJson_new_array(parent, name);

    for (;;) {

        // recursively read the childs data
        bool ok = parse_stream_r(array_element, stream, NULL);
        if(!ok) {
            return false;
        }

        // ',' or ']' ?
        int first_char;
        OStream_read_isspace_into(stream, &first_char, NULL, osize_MAX);
        if (first_char == -1) {
            log_stream_failed();
            return false;
        }

        if(first_char == ']') {
            return true;
        }

        // should be ','
        if(first_char != ',') {
            log_parse_failed(stream, "expected a \',\'");
            return false;
        }
    }
    return false;
}

O_STATIC
bool parse_stream_r(oobj parent, oobj stream, const char *name)
{
    // we start at the value of an element, like the roots opening object "{" or just null, 3.1415, "message", [

    // ignore whitespace and return the first not isspace character
    int first_char;
    OStream_read_isspace_into(stream, &first_char, NULL, osize_MAX);

    if (first_char == 'n') {
        return parse_null(parent, stream, name);
    }
    if (first_char == 't' || first_char == 'f') {
        return parse_boolean(parent, stream, name, first_char);
    }
    if ((first_char >= '0' && first_char <= '9')
        || first_char == '+'
        || first_char == '-'
        || first_char == '.') {
        return parse_number(parent, stream, name, first_char);
    }
    if (first_char == '\"') {
        return parse_string(parent, stream, name);
    }
    if (first_char == '{') {
        return parse_object(parent, stream, name);
    }
    if(first_char == '}') {
        // empty object let the parse_object detect it:
        OStream_seek(stream, -1, OStream_SEEK_CUR);
        return true;
    }
    if (first_char == '[') {
        return parse_array(parent, stream, name);
    }
    if(first_char == ']') {
        // empty object let the parse_array detect it:
        OStream_seek(stream, -1, OStream_SEEK_CUR);
        return true;
    }

    // error
    if (first_char == -1) {
        log_stream_failed();
        return false;
    }

    log_parse_failed(stream, "invalid element start");
    return false;
}


//
// public
//

OJson *OJson_init(oobj obj, oobj parent, const char *name, enum OJson_type type, union OJson_data data)
{
    OJson *self = obj;
    o_clear(self, sizeof *self, 1);

    OObj_init(self, parent);
    OObj_id_set(self, OJson_ID);

    if (!name) {
        name = OJson_NAME_DEFAULT;
    }

    if (type == OJson_TYPE_STRING) {
        // clone string (NULL safe)
        data.string = o_str_clone(self, data.string);
    } else if (type != OJson_TYPE_BOOLEAN && type != OJson_TYPE_NUMBER) {
        // data not used, so clearing it
        o_clear(&data, sizeof data, 1);
    }

    self->type = type;
    self->data = data;
    self->packed = false;

    if (OObj_check(parent, OJson)) {
        OJson *parent_json = parent;
        self->packed = parent_json->packed;
    }

    OObj_name_set(self, name);


//    const char *pname = o_or(OObj_name(parent), "");
//
//    switch (type) {
//        case OJson_TYPE_NULL:
//            o_log("OJSON null: %s (%s)", name, pname);
//            break;
//        case OJson_TYPE_BOOLEAN:
//            o_log("OJSON boolean {%i}: %s (%s)", self->data.boolean, name, pname);
//            break;
//        case OJson_TYPE_NUMBER:
//            o_log("OJSON number {%g}: %s (%s)", self->data.number, name, pname);
//            break;
//        case OJson_TYPE_STRING:
//            o_log("OJSON string {%s}: %s (%s)", self->data.string, name, pname);
//            break;
//        case OJson_TYPE_OBJECT:
//            o_log("OJSON object: %s (%s)", name, pname);
//            break;
//        case OJson_TYPE_ARRAY:
//            o_log("OJSON array: %s (%s)", name, pname);
//            break;
//        default:
//            break;
//    }

    // vfuncs
    self->super.v_op_num = OJson__v_op_num;
    self->super.v_op_at = OJson__v_op_at;

    return self;
}

struct oobj_opt OJson_new_read_stream(oobj parent, const char *name, oobj stream)
{

    // parse_stream_r creates the base object into the given "parent".
    // but our parent may already have OJson objects, so its hard to detect and return
    // that's why we create the move_container, pop out the tree in that.
    // The move_container should than have exactly one OJson object (if valid)
    // that is moved into the parent and returned
    oobj move_container = OObj_new(parent);

    // the stream may be not seekable, so we create a buffered one
    stream = OStreamBuffered_new(move_container, stream, 32, false);

    bool valid = parse_stream_r(move_container, stream, name);
    if(!valid) {
        o_del(move_container);
        return oobj_opt(NULL);
    }
    OJson **list = OJson_list(move_container, NULL);
    if(list[0] == NULL) {
        o_log_wtf_s("OJson_new_read_stream",
                   "valid, but got no OJson objects?");
        o_del(move_container);
        return oobj_opt(NULL);
    }
    if(list[1] != NULL) {
        o_log_warn_s("OJson_new_read_stream",
                   "valid, but got multiple OJson objects?");
    }

    OJson *ret = list[0];
    o_move(ret, parent);
    o_del(move_container);
    return oobj_opt(ret);
}

struct oobj_opt OJson_new_read_file(oobj parent, const char *name, const char *file)
{
    struct oobj_opt data = o_file_read(parent, file, true, 1);
    if(!data.o) {
        o_log_warn_s("OJson_new_read_file",
                   "failed to open the file: %s", file);
        return oobj_opt(NULL);
    }
    oobj stream = OStreamArray_new(data.o, data.o, false, OStreamArray_SEEKABLE);
    struct oobj_opt res = OJson_new_read_stream(parent, name, stream);
    o_del(data.o);
    return res;
}

struct oobj_opt OJson_new_read_string(oobj parent, const char *name, const char *string)
{
    oobj container = OObj_new(parent);
    oobj stream = OStreamMem_new(container, (void *) string, -1);
    if(!OStream_valid(stream)) {
        o_log_warn_s("OJson_new_read_file",
                     "empty string");
        o_del(container);
        return oobj_opt(NULL);
    }
    struct oobj_opt res = OJson_new_read_stream(parent, name, stream);
    o_del(container);
    return res;
}

//
// virtual implementations:
//

osize OJson__v_op_num(oobj obj)
{
    return OJson_num(obj);
}

void *OJson__v_op_at(oobj obj, osize idx)
{
    return OJson_at(obj, idx).o;
}

//
// object functions:
//


void OJson_string_set(oobj obj, const char *string)
{
    OObj_assert(obj, OJson);
    OJson *self = obj;
    if (self->type != OJson_TYPE_STRING) {
        o_log_warn_s(__func__, "wrong type: %i", self->type);
        return;
    }
    self->data.string = o_str_clone_realloc(self, self->data.string, string);
}

osize OJson_num(oobj obj)
{
    osize num;
    OJson **list = NULL;
    enum OJson_type type = OJson_type(obj);
    if(type != OJson_TYPE_ARRAY && type != OJson_TYPE_OBJECT) {
        return 0;
    }
    list = OJson_list(obj, &num);
    o_free(obj, list);
    return num;
}

struct oobj_opt OJson_at(oobj obj, osize idx)
{
    osize num;
    OJson **list = NULL;
    oobj res = NULL;
    enum OJson_type type = OJson_type(obj);
    if(type != OJson_TYPE_ARRAY && type != OJson_TYPE_OBJECT) {
        goto CLEAN_UP;
    }
    list = OJson_list(obj, &num);
    if(idx < 0 || idx>=num) {
        goto CLEAN_UP;
    }
    res = list[idx];

    CLEAN_UP:
    o_free(obj, list);
    return oobj_opt(res);
}

struct oobj_opt OJson_get(oobj obj, const char *name)
{
    assert(name);
    OJson **list = OJson_list(obj, NULL);
    OJson *ret = NULL;
    for(OJson **it = list; *it; it++) {
        OJson *child = *it;
        if (o_str_equals(OObj_name(child), name)) {
            ret = child;
        }
    }
    o_free(obj, list);
    return oobj_opt(ret);
}

osize OJson_write_stream(oobj obj, oobj stream)
{
    OObj_assert(obj, OJson);
    OObj_assert(stream, OStream);
    oobj container = OObj_new(obj);
    osize written = write_stream_r(container, obj, stream, 0);
    o_del(container);
    return written;
}

bool OJson_write_file(oobj obj, const char *file)
{
    osize written;
    char *dump = OJson_dump(obj, &written);
    if(!dump) {
        o_log_error_s(__func__, "failed to dump json for file: %s", file);
        return false;
    }

    osize file_written = o_file_write(file, true, dump, 1, written);
    bool ok = file_written == written;
    if (ok) {
        o_log_error_s(__func__, "failed to write to file: %s", file);
    }

    o_free(obj, dump);
    return ok;
}

char *OJson_dump(oobj obj, osize *opt_out_len)
{
    oobj container = OObj_new(obj);
    oobj data = OArray_new_dyn(container, NULL, 1, 0, 1024);
    oobj stream = OStreamArray_new(container, data, false, OStreamArray_SEEKABLE);
    osize written = OJson_write_stream(obj, stream);
    char *result = NULL;
    if (written > 0) {
        result = OArray_data_void(data);
        o_mem_move(container, obj, result);
    }
    o_del(container);

    o_opt_set(opt_out_len, written);
    return result;
}
