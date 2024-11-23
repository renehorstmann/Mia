#include "u/UImg.h"
#include "o/OObj_builder.h"
#include "o/img.h"
#include "m/vec/ivec2.h"
#include "r/quad.h"
#include "r/RCam.h"
#include "u/color.h"
#include "u/atlas.h"
#include "u/pose.h"

#define O_LOG_LIB "u"

#include "o/log.h"



//
// public
//

UImg *UImg_init(oobj obj, oobj parent, const void *opt_data, int cols, int rows, enum r_format format)
{
    UImg *self = obj;
    o_clear(self, sizeof *self, 1);

    OObj_init(self, parent);
    OObj_id_set(self, UImg_ID);

    self->format = format;
    self->size.x = o_max(1, cols);
    self->size.y = o_max(1, rows);

    self->data = o_alloc(self, r_format_size(format), UImg_num(self));

    if (opt_data && self->data) {
        o_memcpy(self->data, opt_data, 1, UImg_data_size(self));
    }

    // vfuncs
    self->super.v_op_num = UImg__v_op_num;
    self->super.v_op_at = UImg__v_op_at;

    return self;
}


UImg* UImg_new_0(oobj parent, int cols, int rows, enum r_format format)
{
    UImg *img = UImg_new(parent, NULL, cols, rows, format);
    if (img->data) {
        o_clear(img->data, 1, UImg_data_size(img));
    }
    return img;
}


struct oobj_opt UImg_new_file(oobj parent, const char* file)
{
    struct o_img img = o_img_new_file(parent, file);
    if (!img.rgba_data) {
        o_log_warn_s(__func__, "failed to create UImg from image file: %s", file);
        return oobj_opt(NULL);
    }
    UImg *self = UImg_new(parent, NULL, 0, 0, R_FORMAT_RGBA_8);
    o_free(self, self->data);

    self->size = ivec2_(img.cols, img.rows);
    self->data = (obyte *) img.rgba_data;
    o_mem_move(parent, self, self->data);

    return oobj_opt(self);
}

struct oobj_opt UImg_new_tex(oobj parent, oobj tex, enum r_format format)
{
    ivec2 size = RTex_size_int(tex);
    if (size.x * size.y <= 0) {
        o_log_warn_s(__func__, "failed to create UImg from tex");
        return oobj_opt(NULL);
    }
    UImg *img = UImg_new(parent, NULL, m_2(size), format);
    RTex_get_ex(tex, UImg_data(img), format);
    return oobj_opt(img);
}

//
// virtual implementations:
//

osize UImg__v_op_num(oobj obj)
{
    return UImg_num(obj);
}

void *UImg__v_op_at(oobj obj, osize idx)
{
    return UImg_at_idx(obj, idx);
}

//
// object functions
//

RTex *UImg_tex(oobj obj)
{
    OObj_assert(obj, UImg);
    UImg *self = obj;
    return RTex_new_ex(obj, self->data, m_2(self->size), self->format, self->format);
}

bool UImg_write_file(oobj obj, const char *file)
{
    OObj_assert(obj, UImg);
    UImg *cast = obj;
    oobj container = OObj_new(obj);
    if(cast->format != R_FORMAT_RGBA_8) {
        o_log_trace_s(__func__, "casting to RGBA_8 from: %i", cast->format);
        cast = UImg_cast(cast, R_FORMAT_RGBA_8);
        o_move(cast, container);
    }
    struct o_img img = {
            (obyte *) cast->data,
            cast->size.x,
            cast->size.y,
            NULL
    };
    bool valid = o_img_write_file(img, file);
    o_del(container);
    return valid;
}


void UImg_clear(oobj obj, vec4 clear_color)
{
    OObj_assert(obj, UImg);
    UImg *self = obj;
    obyte clear_data[R_FORMAT_MAX_SIZE];
    r_format_value_from_vec4(clear_data, self->format, clear_color);

    for (osize i = 0; i < UImg_num(self); i++) {
        r_format_pixel_copy(UImg_at_idx(self, i), clear_data, self->format);
    }
}

vec4 UImg_min(oobj obj)
{
    OObj_assert(obj, UImg);
    UImg *self = obj;
    vec4 value = vec4_(m_MAX);
    for (osize i = 0; i < UImg_num(self); i++) {
        void *data = UImg_at_idx(self, i);
        vec4 c = r_format_value_as_vec4(data, self->format);
        value = vec4_min_v(value, c);
    }
    return value;
}


vec4 UImg_max(oobj obj)
{
    OObj_assert(obj, UImg);
    UImg *self = obj;
    vec4 value = vec4_(m_MIN);
    for (osize i = 0; i < UImg_num(self); i++) {
        void *data = UImg_at_idx(self, i);
        vec4 c = r_format_value_as_vec4(data, self->format);
        value = vec4_max_v(value, c);
    }
    return value;
}

void UImg_add(oobj obj, vec4 add)
{
    OObj_assert(obj, UImg);
    UImg *self = obj;
    for (osize i = 0; i < UImg_num(self); i++) {
        void *data = UImg_at_idx(self, i);
        vec4 c = r_format_value_as_vec4(data, self->format);
        c = vec4_add_v(c, add);
        c = vec4_clamp(c, 0, 1);
        r_format_value_from_vec4(data, self->format, c);
    }
}

void UImg_scale(oobj obj, vec4 scale)
{
    OObj_assert(obj, UImg);
    UImg *self = obj;
    for (osize i = 0; i < UImg_num(self); i++) {
        void *data = UImg_at_idx(self, i);
        vec4 c = r_format_value_as_vec4(data, self->format);
        c = vec4_scale_v(c, scale);
        c = vec4_clamp(c, 0, 1);
        r_format_value_from_vec4(data, self->format, c);
    }
}

void UImg_add_scaled(oobj obj, vec4 add, vec4 scale)
{
    OObj_assert(obj, UImg);
    UImg *self = obj;
    for (osize i = 0; i < UImg_num(self); i++) {
        void *data = UImg_at_idx(self, i);
        vec4 c = r_format_value_as_vec4(data, self->format);
        c = vec4_add_v(c, add);
        c = vec4_scale_v(c, scale);
        c = vec4_clamp(c, 0, 1);
        r_format_value_from_vec4(data, self->format, c);
    }
}

void UImg_normalize(oobj obj)
{
    vec4 min = UImg_min(obj);
    vec4 max = UImg_max(obj);
    vec4 range = vec4_sub_v(max, min);
    vec4 add = vec4_neg(min);
    vec4 scale = vec4_div_v(vec4_(1.0f), range);
    UImg_add_scaled(obj, add, scale);
}

void UImg_blit_lb(oobj obj, oobj img, ivec2 offset_lb)
{
    OObj_assert(obj, UImg);
    UImg *self = obj;
    OObj_assert(img, UImg);
    UImg *blit = img;
    UImg *tmp = NULL;

    if (blit->format != self->format) {
        tmp = UImg_cast(blit, self->format);
        o_move(tmp, obj);
        blit = tmp;
    }

    for (int r = 0; r < blit->size.y; r++) {
        int dst_r = r + offset_lb.v1;
        if (dst_r < 0 || dst_r >= self->size.y) {
            continue;
        }
        for (int c = 0; c < blit->size.x; c++) {
            int dst_c = c + offset_lb.v0;
            if (dst_c < 0 || dst_c >= self->size.x) {
                continue;
            }

            r_format_pixel_copy(UImg_at(self, dst_c, dst_r), UImg_at(blit, c, r), self->format);
        }
    }

    o_del(tmp);
}

//
// functions that generate UImg's
//

UImg *UImg_clone(oobj obj)
{
    OObj_assert(obj, UImg);
    UImg *self = obj;
    return UImg_new(obj, self->data, m_2(self->size), self->format);
}

UImg *UImg_cast(oobj obj, enum r_format format)
{
    OObj_assert(obj, UImg);
    UImg *self = obj;
    if (self->format == format) {
        return UImg_clone(obj);
    }
    UImg *res = UImg_new(obj, NULL, m_2(self->size), format);
    for (osize i = 0; i < UImg_num(self); i++) {
        r_format_value_cast(UImg_at_idx(res, i), res->format, UImg_at_idx(self, i), self->format);
    }
    return res;
}

UImg *UImg_channel(oobj obj, int channel)
{
    OObj_assert(obj, UImg);
    UImg *self = obj;
    assert(r_format_channels(self->format) > channel);

    enum r_format format = r_format_element_is_8(self->format) ? R_FORMAT_R_8 : R_FORMAT_R_32F;

    int byte_offset = r_format_size(format) * channel;

    UImg *res = UImg_new(self, NULL, m_2(self->size), format);
    for (osize i = 0; i < UImg_num(self); i++) {
        const obyte *src_data = UImg_at_idx(self, i);
        src_data += byte_offset;
        r_format_pixel_copy(UImg_at_idx(res, i), src_data, format);
    }
    return res;
}


void UImg_channel_set(oobj obj, int obj_channel, oobj set, int set_channel)
{
    OObj_assert(obj, UImg);
    UImg *dst = obj;
    OObj_assert(set, UImg);
    UImg *src = obj;
    assert(r_format_channels(dst->format) > obj_channel);
    assert(r_format_channels(src->format) > set_channel);
    // check if both are 8 bit or 32F
    assert(r_format_element_is_8(dst->format) == r_format_element_is_8(src->format));
    assert(UImg_num(dst) == UImg_num(src));

    enum r_format format = r_format_element_is_8(dst->format) ? R_FORMAT_R_8 : R_FORMAT_R_32F;

    int dst_byte_offset = r_format_size(format) * obj_channel;
    int src_byte_offset = r_format_size(format) * set_channel;

    for (osize i = 0; i < UImg_num(dst); i++) {
        obyte *dst_data = UImg_at_idx(dst, i);
        dst_data += dst_byte_offset;
        const obyte *src_data = UImg_at_idx(src, i);
        src_data += src_byte_offset;
        r_format_pixel_copy(dst_data, src_data, format);
    }
}


UImg *UImg_channel_merge(oobj r, oobj g, oobj b, oobj a)
{
    OObj_assert(r, UImg);
    OObj_assert(g, UImg);
    OObj_assert(b, UImg);
    OObj_assert(a, UImg);
    UImg *red = r;
    UImg *green = g;
    UImg *blue = b;
    UImg *alpha = a;

    assert(UImg_num(red) <= UImg_num(green)
           && UImg_num(red) <= UImg_num(blue)
           && UImg_num(red) <= UImg_num(alpha));
    assert(r_format_element_is_8(red->format) == r_format_element_is_8(green->format)
           && r_format_element_is_8(red->format) == r_format_element_is_8(blue->format)
           && r_format_element_is_8(red->format) == r_format_element_is_8(alpha->format));

    enum r_format format_r = r_format_element_is_8(red->format) ? R_FORMAT_R_8 : R_FORMAT_R_32F;
    enum r_format format_rgba = r_format_element_is_8(red->format) ? R_FORMAT_RGBA_8 : R_FORMAT_RGBA_32F;

    int offset = r_format_size(format_r);

    UImg *res = UImg_new(red, NULL, m_2(red->size), format_rgba);
    for (osize i = 0; i < UImg_num(res); i++) {
        obyte *r_data = UImg_at_idx(res, i);
        obyte *g_data = r_data + offset;
        obyte *b_data = g_data + offset;
        obyte *a_data = b_data + offset;
        r_format_pixel_copy(r_data, UImg_at_idx(red, i), format_r);
        r_format_pixel_copy(g_data, UImg_at_idx(green, i), format_r);
        r_format_pixel_copy(b_data, UImg_at_idx(blue, i), format_r);
        r_format_pixel_copy(a_data, UImg_at_idx(alpha, i), format_r);
    }
    return res;
}


UImg *UImg_border(oobj obj, ivec4 lrbt, vec4 color)
{
    OObj_assert(obj, UImg);
    UImg *self = obj;

    assert(lrbt.v0 >= 0 && lrbt.v1 >= 0 && lrbt.v2 >= 0 && lrbt.v3 >= 0);

    UImg *res = UImg_new(obj, NULL, self->size.x + lrbt.v0 + lrbt.v1, self->size.y + lrbt.v2 + lrbt.v3, self->format);

    obyte border_data[R_FORMAT_MAX_SIZE];
    r_format_value_from_vec4(border_data, self->format, color);

    for (int row = 0; row < res->size.y; row++) {
        int img_row = row - lrbt.v2;
        for (int col = 0; col < res->size.x; col++) {
            int img_col = col - lrbt.v0;

            if (img_row < 0 || img_row >= self->size.y || img_col < 0 || img_col >= self->size.x) {
                r_format_pixel_copy(UImg_at(res, col, row), border_data, self->format);
            } else {
                r_format_pixel_copy(UImg_at(res, col, row), UImg_at(self, img_col, img_row), self->format);
            }
        }
    }
    return res;
}

UImg *UImg_collage(const oobj *srcs, int n,
                   enum UImg_start start, int block, bool row_major,
                   ivec2 margin, vec4 bg_color)
{
    assert(n > 0);
    assert(margin.x >= 0 && margin.y >= 0);
    ivec2 size = ivec2_(0);
    for (int i = 0; i < n; i++) {
        ivec2 s = UImg_size_int(srcs[i]);
        size.x = o_max(size.x, s.x);
        size.y = o_max(size.y, s.y);
    }
    size = ivec2_add_v(size, ivec2_scale(margin, 2));


    int cols, rows;
    if (row_major) {
        cols = block;
        rows = (int) m_ceil((float) n / (float) block);
    } else {
        rows = block;
        cols = (int) m_ceil((float) n / (float) block);
    }

    ivec2 full_size = ivec2_(size.x * cols, size.y * rows);

    vec2 offset = vec2_cast_int(size.v);
    if (start == UImg_START_LT || start == UImg_START_RT) {
        offset.y *= -1;
    }
    if (start == UImg_START_RB || start == UImg_START_RT) {
        offset.x *= -1;
    }
    vec2 center_start;
    switch (start) {
        default:
        case UImg_START_LB:
            center_start = vec2_(-cols / 2.0f * size.x / 2.0f, -rows / 2.0f * size.y / 2.0f);
            break;
        case UImg_START_RB:
            center_start = vec2_(+cols / 2.0f * size.x / 2.0f, -rows / 2.0f * size.y / 2.0f);
            break;
        case UImg_START_LT:
            center_start = vec2_(-cols / 2.0f * size.x / 2.0f, +rows / 2.0f * size.y / 2.0f);
            break;
        case UImg_START_RT:
            center_start = vec2_(+cols / 2.0f * size.x / 2.0f, +rows / 2.0f * size.y / 2.0f);
            break;
    }

    UImg *res = UImg_new(srcs[0], NULL, m_2(full_size), UImg_format(srcs[0]));
    UImg_clear(res, bg_color);
    for (int i = 0; i < n; i++) {
        int c, r;
        if (row_major) {
            c = i % block;
            r = i / block;
        } else {
            r = i % block;
            c = i / block;
        }
        vec2 center = vec2_add_scaled_v(center_start, offset, vec2_(c, r));
        UImg_blit(res, srcs[i], center);
    }

    return res;
}


UImg *UImg_distance_transform(oobj obj, bool full)
{
    UImg *res = UImg_cast(obj, R_FORMAT_R_8);

    if (!full) {
        // first pass, left bottom -> right top
        for (int r = 0; r < res->size.y; r++) {
            for (int c = 0; c < res->size.x; c++) {
                obyte down = r == 0 ? 0 : *(obyte *) UImg_at(res, c, r - 1);
                obyte left = c == 0 ? 0 : *(obyte *) UImg_at(res, c - 1, r);
                obyte *set = (obyte *) UImg_at(res, c, r);
                obyte value = o_min(down, left) + 1;
                *set = o_min(*set, value);
            }
        }

        // second pass, right top -> left bottom
        for (int r = res->size.y - 1; r >= 0; r--) {
            for (int c = res->size.x - 1; c >= 0; c--) {
                obyte up = r == res->size.y - 1 ? 0 : *(obyte *) UImg_at(res, c, r + 1);
                obyte right = c == res->size.x - 1 ? 0 : *(obyte *) UImg_at(res, c + 1, r);
                obyte *set = (obyte *) UImg_at(res, c, r);
                obyte value = o_min(up, right) + 1;
                *set = o_min(*set, value);
            }
        }
    } else {
        // full:

        // first pass, left bottom -> right top
        for (int r = 0; r < res->size.y; r++) {
            for (int c = 0; c < res->size.x; c++) {
                obyte down = r == 0 ? 0 : *(obyte *) UImg_at(res, c, r - 1);
                obyte left = c == 0 ? 0 : *(obyte *) UImg_at(res, c - 1, r);
                obyte down_left = r == 0 ? 0 : (c == 0 ? 0 : *(obyte *) UImg_at(res, c - 1, r - 1));
                obyte down_right = r == 0 ? 0 : (c == res->size.x - 1 ? 0 : *(obyte *) UImg_at(res, c + 1, r - 1));
                obyte *set = (obyte *) UImg_at(res, c, r);
                obyte value = o_min(o_min(o_min(down, left), down_left), down_right) + 1;
                *set = o_min(*set, value);
            }
        }

        // second pass, right top -> left bottom
        for (int r = res->size.y - 1; r >= 0; r--) {
            for (int c = res->size.x - 1; c >= 0; c--) {
                obyte up = r == res->size.y - 1 ? 0 : *(obyte *) UImg_at(res, c, r + 1);
                obyte right = c == res->size.x - 1 ? 0 : *(obyte *) UImg_at(res, c + 1, r);
                obyte up_left = r == res->size.y - 1 ? 0 : (c == 0 ? 0 : *(obyte *) UImg_at(res, c - 1, r + 1));
                obyte up_right =
                        r == res->size.y - 1 ? 0 : (c == res->size.x - 1 ? 0 : *(obyte *) UImg_at(res, c + 1, r + 1));
                obyte *set = (obyte *) UImg_at(res, c, r);
                obyte value = o_min(o_min(o_min(up, right), up_left), up_right) + 1;
                *set = o_min(*set, value);
            }
        }
    }

    return res;
}


