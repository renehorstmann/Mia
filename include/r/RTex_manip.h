#ifndef MIA_RTEX_MANIP_H
#define MIA_RTEX_MANIP_H

/**
 * @file RTex_manip.h
 *
 * Object extension
 *
 * texture manipulate functions
 */

#include "RTex.h"


//
// functions that generate RTex's
//

/**
 * @param obj RTex object
 * @param into RTex object to render into
 * @return RTex allocated on obj, with each pixel: (obj + add) * scale
 */
O_EXTERN
void RTex_add_scaled_into(oobj obj, vec4 add, vec4 scale, oobj into);

/**
 * @param obj RTex object
 * @param format for the resulting RTex
 * @return RTex allocated on obj, with each pixel: (obj + add) * scale
 */
O_INLINE
RTex *RTex_add_scaled(oobj obj, vec4 add, vec4 scale, enum r_format format)
{
    RTex *res = RTex_new_ex(obj, NULL, m_2(RTex_size(obj)), format, format);
    RTex_add_scaled_into(obj, add, scale, res);
    return res;
}


/**
 * @param obj RTex object
 * @param rgba_matrix channel mixer, col[0] -> dst red
 * @param into RTex object to render into
 * @return RTex allocated on obj, with channels remixed
 * @note for example:
 *       oobj red = RTex_channels(obj, vec4_(1, 0), vec4_(0), vec4_(0), vec4_(0), R_FORMAT_R_8);
 *       oobj inv = RTex_channels(obj, vec4_(-1, 0), vec4_(0, -1), vec4_(0, 0, -1), vec4_(0, 0, 0, 1), R_FORMAT_RGBA_8);
 *       oobj grey = RTex_channels(obj, vec4_(0.33, 0.33, 0.33, 0), ... , vec4_(0, 0, 0, 1), R_FORMAT_RGBA_8);
 */
O_EXTERN
void RTex_mixer_into(oobj obj, mat4 rgba_matrix, oobj into);


/**
 * @param obj RTex object
 * @param rgba_matrix channel mixer, col[0] -> dst red
 * @param format for the resulting RTex
 * @return RTex allocated on obj, with channels remixed
 * @note for example:
 *       oobj red = RTex_channels(obj, vec4_(1, 0), vec4_(0), vec4_(0), vec4_(0), R_FORMAT_R_8);
 *       oobj inv = RTex_channels(obj, vec4_(-1, 0), vec4_(0, -1), vec4_(0, 0, -1), vec4_(0, 0, 0, 1), R_FORMAT_RGBA_8);
 *       oobj grey = RTex_channels(obj, vec4_(0.33, 0.33, 0.33, 0), ... , vec4_(0, 0, 0, 1), R_FORMAT_RGBA_8);
 */
O_INLINE
RTex *RTex_mixer(oobj obj, mat4 rgba_matrix, enum r_format format)
{
    RTex *res = RTex_new_ex(obj, NULL, m_2(RTex_size(obj)), format, format);
    RTex_mixer_into(obj, rgba_matrix, res);
    return res;
}

/**
 * @param obj RTex object
 * @param inv_mask channel mask, 0 to clear, 1 to keep, -1 to inv (or in between for scaled)
 * @param into RTex object to render into
 * @note just calls RTex_mixer with an inv_mask as diagonal values
 */
O_EXTERN
void RTex_inv_into(oobj obj, vec4 inv_mask, oobj into);

/**
 * @param obj RTex object
 * @param inv_mask channel mask, 0 to clear, 1 to keep, -1 to inv (or in between for scaled)
 * @param format for the resulting RTex
 * @return RTex allocated on obj, with channels inverted
 * @note just calls RTex_mixer with an inv_mask as diagonal values
 */
O_INLINE
RTex *RTex_inv(oobj obj, vec4 inv_mask, enum r_format format)
{
    RTex *res = RTex_new_ex(obj, NULL, m_2(RTex_size(obj)), format, format);
    RTex_inv_into(obj, inv_mask, res);
    return res;
}


/**
 * @param obj RTex object
 * @param channels vec4_(0, 1, 2, 3) for a copy, vec4_(2, 2, 2, 3) to copy .b to .rgb but keep .a
 * @param into RTex object to render into
 * @note for example to get .r as a single channel image:
 *       oobj red = RTex_channels(obj, vec4_(0), vec4_(0), R_FORMAT_R_8);
 *       just calls RTex_mixer
 */
O_EXTERN
void RTex_channels_into(oobj obj, ivec4 channel, oobj into);


/**
 * @param obj RTex object
 * @param channels vec4_(0, 1, 2, 3) for a copy, vec4_(2, 2, 2, 3) to copy .b to .rgb but keep .a
 * @param format for the resulting RTex
 * @return RTex allocated on obj, with channels remixed / colors reorderd
 * @note for example to get .r as a single channel image:
 *       oobj red = RTex_channels(obj, vec4_(0), vec4_(0), R_FORMAT_R_8);
 *       just calls RTex_mixer
 */
O_INLINE
RTex *RTex_channels(oobj obj, ivec4 channel, enum r_format format)
{
    RTex *res = RTex_new_ex(obj, NULL, m_2(RTex_size(obj)), format, format);
    RTex_channels_into(obj, channel, res);
    return res;
}

/**
 * @param obj RTex object
 * @param out_rgba an array of (at least) 4 oobj to set r g b a into
 * @note calls RTex_channels 4 times in a row...
 */
O_INLINE
void RTex_channels_split_into(oobj obj, oobj *out_rgba_into)
{
    for(int i=0; i<4; i++) {
        RTex_channels_into(obj, ivec4_(i), out_rgba_into[i]);
    }
}

/**
 * @param obj RTex object
 * @param out_rgba an array of (at least) 4 oobj to set r g b a into
 * @param format for the resulting RTex's
 * @note calls RTex_channels 4 times in a row...
 */
O_INLINE
void RTex_channels_split(oobj obj, oobj *out_rgba, enum r_format format)
{
    for(int i=0; i<4; i++) {
        out_rgba[i] = RTex_channels(obj, ivec4_(i), format);
    }
}

/**
 * @param r, g, b, a RTex object
 * @param into RTex object to render into
 * @return RTex allocated on r, with channels merged together
 * @note assumes all RTex's channels share the same size. uses .r on each channel to merge
 */
O_EXTERN
void RTex_channels_merge_into(oobj r, oobj g, oobj b, oobj a, oobj into);

/**
 * @param r, g, b, a RTex object
 * @param format for the resulting RTex
 * @return RTex allocated on r, with channels merged together
 * @note assumes all RTex's channels share the same size. uses .r on each channel to merge
 */
O_INLINE
RTex *RTex_channels_merge(oobj r, oobj g, oobj b, oobj a, enum r_format format)
{
    RTex *res = RTex_new_ex(r, NULL, m_2(RTex_size(r)), format, format);
    RTex_channels_merge_into(r, g, b, a, res);
    return res;
}

/**
 * Colorize the RTex into a new one
 * @param obj RTex object
 * @param rgba * color
 * @param hsva + color
 * @param into RTex object to render into
 */
O_EXTERN
void RTex_color_into(oobj obj, vec4 rgba, vec4 hsva, oobj into);

/**
 * Colorize the RTex into a new one
 * @param obj RTex object
 * @param rgba * color
 * @param hsva + color
 * @param format for the resulting RTex
 * @return RTex allocated on obj
 */
O_INLINE
RTex* RTex_color(oobj obj, vec4 rgba, vec4 hsva, enum r_format format)
{
    RTex *res = RTex_new_ex(obj, NULL, m_2(RTex_size(obj)), format, format);
    RTex_color_into(obj, rgba, hsva, res);
    return res;
}

/**
 * RGBA -> HSVA
 * @param obj RTex object
 * @param into RTex object to render into
 */
O_EXTERN
void RTex_rgba_to_hsva_into(oobj obj, oobj into);

/**
 * RGBA -> HSVA
 * @param obj RTex object
 * @param format for the resulting RTex
 * @return RTex allocated on obj
 */
O_INLINE
RTex* RTex_rgba_to_hsva(oobj obj, enum r_format format)
{
    RTex *res = RTex_new_ex(obj, NULL, m_2(RTex_size(obj)), format, format);
    RTex_rgba_to_hsva_into(obj, res);
    return res;
}

/**
 * HSVA -> RGBA
 * @param obj RTex object
 * @param into RTex object to render into
 */
O_EXTERN
void RTex_hsva_rgba_into(oobj obj, oobj into);

/**
 * HSVA -> RGBA
 * @param obj RTex object
 * @param format for the resulting RTex
 * @return RTex allocated on obj
 */
O_INLINE
RTex* RTex_hsva_rgba(oobj obj, enum r_format format)
{
    RTex *res = RTex_new_ex(obj, NULL, m_2(RTex_size(obj)), format, format);
    RTex_hsva_rgba_into(obj, res);
    return res;
}



/**
 * Runs a convolution with the given kernel on this image and returns a new RTex with the result
 * @param obj RTex object
 * @param kernel RTex object (.r used) for example RTex_new_kernel
 * @param offset for the kernel
 * @param into RTex object to render into
 */
O_EXTERN
void RTex_conv_into(oobj obj, oobj kernel, ivec2 offset, oobj into);


/**
 * Runs a convolution with the given kernel on this image and returns a new RTex with the result
 * @param obj RTex object
 * @param kernel RTex object (.r used) for example RTex_new_kernel
 * @param offset for the kernel
 * @param format for the resulting RTex
 * @return RTex allocated on obj
 */
O_INLINE
RTex* RTex_conv(oobj obj, oobj kernel, ivec2 offset, enum r_format format)
{
    RTex *res = RTex_new_ex(obj, NULL, m_2(RTex_size(obj)), format, format);
    RTex_conv_into(obj, kernel, offset, res);
    return res;
}


/**
 * Runs a convolution with the given kernel on this image and returns a new RTex with the result
 * @param obj RTex object
 * @param size for the kernel, asserts max
 * @param into RTex object to render into
 * @note calls RTex_conv internally
 */
O_EXTERN
void RTex_blur_into(oobj obj, ivec2 size, oobj into);


/**
 * Runs a convolution with the given kernel on this image and returns a new RTex with the result
 * @param obj RTex object
 * @param size for the kernel, asserts max
 * @param format for the resulting RTex
 * @return RTex allocated on obj
 * @note calls RTex_conv internally
 */
O_INLINE
RTex* RTex_blur(oobj obj, ivec2 size, enum r_format format)
{
    RTex *res = RTex_new_ex(obj, NULL, m_2(RTex_size(obj)), format, format);
    RTex_blur_into(obj, size, res);
    return res;
}


/**
 * Runs a convolution with the given kernel on this image and returns a new RTex with the result
 * @param obj RTex object
 * @param size for the kernel, asserts max
 * @param sigma gauss factor for x and y axis, pass <=0 to auto scale (vec2_(-1))
 * @param into RTex object to render into
 * @note calls RTex_conv internally
 */
O_EXTERN
void RTex_gauss_into(oobj obj, ivec2 size, vec2 sigma, oobj into);


/**
 * Runs a convolution with the given kernel on this image and returns a new RTex with the result
 * @param obj RTex object
 * @param size for the kernel, asserts max
 * @param sigma gauss factor for x and y axis, pass <=0 to auto scale (vec2_(-1))
 * @param format for the resulting RTex
 * @return RTex allocated on obj
 * @note calls RTex_conv internally
 */
O_INLINE
RTex* RTex_gauss(oobj obj, ivec2 size, vec2 sigma, enum r_format format)
{
    RTex *res = RTex_new_ex(obj, NULL, m_2(RTex_size(obj)), format, format);
    RTex_gauss_into(obj, size, sigma, res);
    return res;
}


/**
 * Runs a contouring shader and creates the contour based on the masked rgba values in the given color.
 * @param obj RTex object
 * @param kernel RTex object (.r used) for example RTex_new_kernel
 * @param offset for the kernel
 * @param mask vec4_(0, 0, 0, 1) to mask alpha (default)
 * @param color to render with the kernel
 * @param into RTex object to render into
 */
O_EXTERN
void RTex_dilate_into(oobj obj, oobj kernel, ivec2 offset, vec4 mask, vec4 color, oobj into);


/**
 * Runs a contouring shader and creates the contour based on the masked rgba values in the given color.
 * @param obj RTex object
 * @param kernel RTex object (.r used) for example RTex_new_kernel
 * @param offset for the kernel
 * @param mask vec4_(0, 0, 0, 1) to mask alpha (default)
 * @param color to render with the kernel
 * @param format for the resulting RTex
 * @return RTex allocated on obj
 */
O_INLINE
RTex* RTex_dilate(oobj obj, oobj kernel, ivec2 offset, vec4 mask, vec4 color, enum r_format format)
{
    RTex *res = RTex_new_ex(obj, NULL, m_2(RTex_size(obj)), format, format);
    RTex_dilate_into(obj, kernel, offset, mask, color, res);
    return res;
}

/**
 * Runs a contouring "inv" shader and creates the contour based on the masked rgba values in the given color
 * @param obj RTex object
 * @param kernel RTex object (.r used) for example RTex_new_kernel
 * @param offset for the kernel
 * @param mask vec4_(0, 0, 0, 1) to mask alpha (default)
 * @param color to render with the kernel
 * @param into RTex object to render into
 */
O_EXTERN
void RTex_erode_into(oobj obj, oobj kernel, ivec2 offset, vec4 mask, vec4 color, oobj into);

/**
 * Runs a contouring "inv" shader and creates the contour based on the masked rgba values in the given color
 * @param obj RTex object
 * @param @param kernel RTex object (.r used) for example RTex_new_kernel
 * @param offset for the kernel
 * @param mask vec4_(0, 0, 0, 1) to mask alpha (default)
 * @param color to render with the kernel
 * @param format for the resulting RTex
 * @return RTex allocated on obj
 */
O_INLINE
RTex* RTex_erode(oobj obj, oobj kernel, ivec2 offset, vec4 mask, vec4 color, enum r_format format)
{
    RTex *res = RTex_new_ex(obj, NULL, m_2(RTex_size(obj)), format, format);
    RTex_erode_into(obj, kernel, offset, mask, color, res);
    return res;
}

/**
 * Runs a contouring shader and creates the contour based on the masked rgba values in the given color.
 * Then blends on the obj RTex
 * @param obj RTex object
 * @param kernel RTex object (.r used) for example RTex_new_kernel
 * @param offset for the kernel
 * @param mask vec4_(0, 0, 0, 1) to mask alpha (default)
 * @param color to render with the kernel
 * @param into RTex object to render into
 * @note uses RTex_dilate and blends the obj RTex onto the dilated contour
 */
O_EXTERN
void RTex_contour_into(oobj obj, oobj kernel, ivec2 offset, vec4 mask, vec4 color, oobj into);

/**
 * Runs a contouring shader and creates the contour based on the masked rgba values in the given color.
 * Then blends on the obj RTex
 * @param obj RTex object
 * @param kernel RTex object (.r used) for example RTex_new_kernel
 * @param offset for the kernel
 * @param mask vec4_(0, 0, 0, 1) to mask alpha (default)
 * @param color to render with the kernel
 * @param format for the resulting RTex
 * @return RTex allocated on obj
 * @note uses RTex_dilate and blends the obj RTex onto the dilated contour
 */
O_INLINE
RTex* RTex_contour(oobj obj, oobj kernel, ivec2 offset, vec4 mask, vec4 color, enum r_format format)
{
    RTex *res = RTex_new_ex(obj, NULL, m_2(RTex_size(obj)), format, format);
    RTex_contour_into(obj, kernel, offset, mask, color, res);
    return res;
}

/**
 * Runs a outlining contour shader and creates the contour based on the masked rgba values in the given color.
 * Creates a plus kernel and runs RTex_contour_into with it.
 * Then blends on the obj RTex
 * @param obj RTex object
 * @param size of the kernel
 * @param offset for the kernel
 * @param mask vec4_(0, 0, 0, 1) to mask alpha (default)
 * @param color to render with the kernel
 * @param into RTex object to render into
 * @note uses RTex_contour_into
 */
O_EXTERN
void RTex_outline_into(oobj obj, ivec2 size, ivec2 offset, vec4 mask, vec4 color, oobj into);

/**
 * Runs a outlining contour shader and creates the contour based on the masked rgba values in the given color.
 * Creates a plus kernel and runs RTex_contour_into with it.
 * Then blends on the obj RTex
 * @param obj RTex object
 * @param size of the kernel
 * @param offset for the kernel
 * @param mask vec4_(0, 0, 0, 1) to mask alpha (default)
 * @param color to render with the kernel
 * @param format for the resulting RTex
 * @return RTex allocated on obj
 * @note uses RTex_contour_into
 */
O_INLINE
RTex* RTex_outline(oobj obj, ivec2 size, ivec2 offset, vec4 mask, vec4 color, enum r_format format)
{
    RTex *res = RTex_new_ex(obj, NULL, m_2(RTex_size(obj)), format, format);
    RTex_outline_into(obj, size, offset, mask, color, res);
    return res;
}


/**
 * Creates a collage of all src RTex's
 * @param srcs array of RTex objects
 * @param n size of the srcs array; asserts(n>=1)
 * @param cols columns, row major order, starting from top left
 * @param spacing additional border around an RTex sub image
 * @param bg_color background and border color
 * @param into RTex object to render into
 * @return RTex allocated on srcs[0]
 * @note the grid size is formed by the maximal RTex width and height of all srcs + margin*2
 *       "into" version does not make much sense...
 */
O_EXTERN
RTex *RTex_collage(const oobj *srcs, int n, int cols, ivec2 margin, vec4 bg_color, enum r_format format);

/**
 * Merges tex onto obj, blended (alpha can just gain more opacity).
 * @param obj RTex object
 * @param tex RTex object to merge onto obj
 * @param opt_mask_a, opt_mask_b masks which use .r to apply as alpha, NULL -> r_tex_white
 * @param rgba additional color effect for tex
 * @param format for the resulting RTex
 */
O_EXTERN
void RTex_merge_into(oobj obj, oobj tex, oobj opt_mask_a, oobj opt_mask_b, vec4 rgba, oobj into);

/**
 * Merges tex onto obj, blended (alpha can just gain more opacity).
 * @param obj RTex object
 * @param tex RTex object to merge onto obj
 * @param opt_mask_a, opt_mask_b masks which use .r to apply as alpha, NULL -> r_tex_white
 * @param rgba additional color effect for tex
 * @param format for the resulting RTex
 * @return RTex allocated on obj
 */
O_INLINE
RTex *RTex_merge(oobj obj, oobj tex, oobj opt_mask_a, oobj opt_mask_b, vec4 rgba, enum r_format format)
{
    RTex *res = RTex_new_ex(obj, NULL, m_2(RTex_size(obj)), format, format);
    RTex_merge_into(obj, tex, opt_mask_a, opt_mask_b, rgba, res);
    return res;
}

#endif //MIA_RTEX_MANIP_H
