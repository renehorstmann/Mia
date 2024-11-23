#include "o/OStream.h"
#include "o/OObj_builder.h"
#include "o/str.h"
#include "o/OArray.h"


OStream *OStream_init(oobj obj, oobj parent,
                      OStream__valid_fn valid_fn,
                      OStream__size_fn size_fn,
                      OStream__seek_fn seek_fn,
                      OStream__read_try_fn read_try_fn,
                      OStream__write_fn write_fn,
                      OStream__close_fn close_fn)
{
    OStream *self = obj;
    o_clear(self, sizeof *self, 1);

    OObj_init(self, parent);
    OObj_id_set(self, OStream_ID);

    // vfuncs
    self->v_valid = valid_fn;
    self->v_size = size_fn;
    self->v_seek = seek_fn;
    self->v_read_try = read_try_fn;
    self->v_write = write_fn;
    self->v_close = close_fn;

    self->super.v_del = OStream__v_del;

    return self;
}

void OStream__v_del(oobj obj)
{
    // closed the stream
    OStream_close(obj);

    // call super del
    OObj__v_del(obj);
}


osize OStream_read(oobj obj, void *out_data, osize element_size, osize num)
{
    osize read = 0;
    obyte *it = out_data;
    do {
        osize current_read = OStream_read_try(obj, it, element_size, num - read);
        if (current_read <= 0) {
            return read;
        }
        read += current_read;
        it += (current_read * element_size);
    } while (read < num);
    return read;
}

osize OStream_printf_va(oobj obj, const char *format, va_list args)
{
    char *text = o_strf_va(obj, format, args);
    osize written = OStream_write(obj, text, 1, o_strlen(text));
    o_free(obj, text);
    return written;
}

osize OStream_printf(oobj obj, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    osize written = OStream_printf_va(obj, format, args);
    va_end(args);
    return written;
}



osize OStream_read_until_into(oobj obj,
                              int *out_opt_until_character_found,
                              char *out_opt_data, osize max,
                              const char *until_character_set)
{
    assert(max > 0);
    osize len = 0;
    o_opt_set(out_opt_until_character_found, -1);
    do {
        int r = OStream_getchar(obj);
        if (r < 0) {
            return 0;
        }
        len++;
        if (o_str_find_char(until_character_set, (char) r) >= 0) {
            o_opt_set(out_opt_until_character_found, r);
            break;
        }
        if(out_opt_data) {
            out_opt_data[len-1] = (char) r;
        }
    } while (len < max);
    return len-1;
}

char *OStream_read_until(oobj obj, int *out_opt_until_character_found,
                         osize max, const char *until_character_set)
{
    o_opt_set(out_opt_until_character_found, -1);
    OArray *data = (OArray *) OArray_new_dyn(obj, NULL, 1, 0, 128);
    for (osize len=0; len<max; len++) {
        int r = OStream_getchar(obj);
        if (r < 0) {
            o_del(data);
            return NULL;
        }
        char c = (char) r;
        OArray_push(data, &c);
        if (o_str_find_char(until_character_set, c) >= 0) {
            o_opt_set(out_opt_until_character_found, r);
            break;
        }
    }

    // move data
    char *ret = (char *) data->data;
    o_mem_move(data, obj, ret);
    data->data = NULL;
    o_del(data);
    return ret;
}


osize OStream_read_all_into(oobj obj, int *out_opt_first_character_found,
                            char *out_opt_data, osize max,
                            const char *all_character_set)
{
    assert(max > 0);
    o_opt_set(out_opt_first_character_found, -1);
    osize len = 0;
    do {
        int r = OStream_getchar(obj);
        if (r < 0) {
            return 0;
        }
        len++;
        if (o_str_find_char(all_character_set, (char) r) < 0) {
            o_opt_set(out_opt_first_character_found, r);
            break;
        }
        if(out_opt_data) {
            out_opt_data[len-1] = (char) r;
        }
    } while (len < max);
    return len-1;
}


char *OStream_read_all(oobj obj, int *out_opt_first_character_found, osize max, const char *all_character_set)
{
    o_opt_set(out_opt_first_character_found, -1);
    OArray *data = (OArray *) OArray_new_dyn(obj, NULL, 1, 0, 128);
    for (osize len=0; len<max; len++) {
        int r = OStream_getchar(obj);
        if (r < 0) {
            o_del(data);
            return NULL;
        }
        char c = (char) r;
        if (o_str_find_char(all_character_set, c) < 0) {
            o_opt_set(out_opt_first_character_found, r);
            break;
        }
        OArray_push(data, &c);
    }

    // move data
    char *ret = (char *) data->data;
    o_mem_move(data, obj, ret);
    data->data = NULL;
    o_del(data);
    return ret;
}

