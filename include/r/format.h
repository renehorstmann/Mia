#ifndef R_FORMAT_H
#define R_FORMAT_H

/**
 * @file format.h
 *
 * pixel formats for RTex and co
 */

#include "o/common.h"
#include "m/types/flt.h"
#include "m/types/byte.h"

enum r_format {
    R_FORMAT_R_8,
    R_FORMAT_R_32F,
    R_FORMAT_RGBA_8,
    R_FORMAT_RGBA_32F,
    R_NUM_FORMATS
};

// maximal result of r_format_size()
#define R_FORMAT_MAX_SIZE 16

/**
 * @return true if the format element is of the data type ou8.
 */
O_INLINE
bool r_format_element_is_8(enum r_format format)
{
    return format == R_FORMAT_R_8 || format == R_FORMAT_RGBA_8;
}

/**
 * @return true if the format element is of the data type ou8.
 */
O_INLINE
bool r_format_element_is_32F(enum r_format format)
{
    return format == R_FORMAT_R_32F || format == R_FORMAT_RGBA_32F;
}

/**
 * @return 1 for R_8 || R_32F, else 4
 */
O_INLINE
int r_format_channels(enum r_format format)
{
    return (format == R_FORMAT_R_8 || format == R_FORMAT_R_32F) ? 1 : 4;
}

O_INLINE
void r_format_pixel_copy(void *dst, const void *src, enum r_format format)
{
    switch (format) {
        case R_FORMAT_R_8:
            *((obyte *) dst) = *((obyte *) src);
            return;
        case R_FORMAT_R_32F:
            *((float *) dst) = *((float *) src);
            return;
        case R_FORMAT_RGBA_8:
            *((bvec4 *) dst) = *((bvec4 *) src);
            return;
        case R_FORMAT_RGBA_32F:
            *((vec4 *) dst) = *((vec4 *) src);
            return;
        default:
            break;
    }
    assert(0 && "invalid format");
}

/**
 * @return bytesize of a pixel for the given format
 */
O_INLINE
osize r_format_size(enum r_format format)
{
    switch (format) {
        case R_FORMAT_R_8:
            return 1;
        case R_FORMAT_R_32F:
        case R_FORMAT_RGBA_8:
            return 4;
        case R_FORMAT_RGBA_32F:
            return 16;
        default:
            break;
    }
    assert(0 && "invalid format");
    // should not get here...
    return 0;
}

/**
 * @param format
 * @param src single value of type format
 * @return format casted as RGBA_32F
 * @note RGBA = vec4(R, R, R, 1)
 */
O_INLINE
vec4 r_format_value_as_vec4(const void *src, enum r_format src_format)
{
    switch (src_format) {
        case R_FORMAT_R_8:
            return vec4_(((const ou8 *) src)[0] / 255.0f,
                         ((const ou8 *) src)[0] / 255.0f,
                         ((const ou8 *) src)[0] / 255.0f,
                         1.0f);
        case R_FORMAT_R_32F:
            return vec4_(((const float *) src)[0],
                         ((const float *) src)[0],
                         ((const float *) src)[0],
                         1.0f);
        case R_FORMAT_RGBA_8:
            return vec4_(((const ou8 *) src)[0] / 255.0f,
                         ((const ou8 *) src)[1] / 255.0f,
                         ((const ou8 *) src)[2] / 255.0f,
                         ((const ou8 *) src)[3] / 255.0f);
        case R_FORMAT_RGBA_32F:
            return *((const vec4 *) src);
        default:
            break;
    }
    assert(0 && "invalid format");
    // should not get here...
    return vec4_(NAN);
}

/**
 * @param format
 * @param src single value of type format
 * @return format casted as RGBA_32F
 * @note R = RGBA.r
 *       if dst_format is of type 8, its clamped before casting
 */
O_INLINE
void r_format_value_from_vec4(void *out_dst, enum r_format dst_format, vec4 src)
{
    switch (dst_format) {
        case R_FORMAT_R_8:
            *((ou8 *) out_dst) = (ou8) (o_clamp(src.x, 0.0f, 1.0f) * 255);
            return;
        case R_FORMAT_R_32F:
            *((float *) out_dst) = src.r;
            return;
        case R_FORMAT_RGBA_8:
            *((bvec4 *) out_dst) = bvec4_((ou8) (o_clamp(src.x, 0.0f, 1.0f) * 255),
                                          (ou8) (o_clamp(src.y, 0.0f, 1.0f) * 255),
                                          (ou8) (o_clamp(src.z, 0.0f, 1.0f) * 255),
                                          (ou8) (o_clamp(src.w, 0.0f, 1.0f) * 255));
            return;
        case R_FORMAT_RGBA_32F:
            *((vec4 *) out_dst) = src;
            return;
        default:
            break;
    }
    assert(0 && "invalid format");
}

/**
 * @param format
 * @param src single value of type format
 * @return format casted as RGBA_32F
 * @note R = RGBA.r
 */
O_INLINE
void r_format_value_cast(void *out_dst, enum r_format dst_format, const void *src, enum r_format src_format)
{
    r_format_value_from_vec4(out_dst, dst_format, r_format_value_as_vec4(src, src_format));
}


#endif //R_FORMAT_H
