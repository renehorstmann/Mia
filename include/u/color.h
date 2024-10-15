#ifndef U_COLOR_H
#define U_COLOR_H

/**
 * @file color.h
 *
 * (pixel) color based on m bvec4 (ou8)
 *
 */
 
#include "m/byte.h"
#include "m/flt.h"
#include "m/utils/color.h"

//
// some useful default colors
//

static const bvec4 U_TRANSPARENT = {{0, 0, 0, 0}};
static const bvec4 U_BLACK = {{0, 0, 0, 255}};
static const bvec4 U_WHITE = {{255, 255, 255, 255}};
static const bvec4 U_RED = {{255, 0, 0, 255}};
static const bvec4 U_GREEN = {{0, 255, 0, 255}};
static const bvec4 U_BLUE = {{0, 0, 255, 255}};
static const bvec4 U_YELLOW = {{255, 255, 0, 255}};
static const bvec4 U_CYAN = {{0, 255, 255, 255}};
static const bvec4 U_MAGENTA = {{255, 0, 255, 255}};

#define U_HUE_RED 0
#define U_HUE_GREEN 85
#define U_HUE_BLUE 170
#define U_HUE_YELLOW 42
#define U_HUE_CYAN 127
#define U_HUE_MAGENTA 212

//
// additional color functions
//

/**
 * @return a==b
 */
O_INLINE
bool u_color_equals(bvec4 a, bvec4 b) {
    return bvec4_equals_v(a, b);
}

/**
 * Default blend like in gl's: glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
 * @return src blended onto dst
 */
O_INLINE
bvec4 u_color_blend(bvec4 dst, bvec4 src) {
    vec4 dst_f = vec4_cast_byte_1(dst.v);
    vec4 src_f = vec4_cast_byte_1(src.v);
    vec4 res_f = vec4_mix(dst_f, src_f, src_f.a);
    return bvec4_cast_float_1(res_f.v);
}

/**
 * @return casted as a vec4 in the range [0:1]
 */
O_INLINE
vec4 u_color_to_vec4(bvec4 color) {
    return vec4_cast_byte_1(color.v);
}

/**
 * @return casted from a vec4 of the range [0:1]
 */
O_INLINE
bvec4 u_color_from_vec4(vec4 color) {
    return bvec4_cast_float_1(color.v);
}


/**
 * @return bvec4 color as hsv
 */
O_INLINE
bvec4 u_color_to_hsv(bvec4 rgba) {
    vec4 rgba_f = vec4_cast_byte_1(rgba.v);
    vec4 hsva_f = vec4_rgb2hsv(rgba_f);
    return bvec4_cast_float_1(hsva_f.v);
}


/**
 * @return bvec4 color as hsv
 */
O_INLINE
bvec4 u_color_from_hsv(bvec4 hsva) {
    vec4 hsva_f = vec4_cast_byte_1(hsva.v);
    vec4 rgba_f = vec4_hsv2rgb(hsva_f);
    return bvec4_cast_float_1(rgba_f.v);
}


#endif //U_COLOR_H
