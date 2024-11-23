#ifndef W_WIMG_H
#define W_WIMG_H

/**
 * @file WImg.h
 *
 * Object (derives WObj)
 *
 * Widget that renders a custom image, drawn into the theme texture.
 * WTheme has some custom slots to draw into its tex (WTheme_CUSTOM_*.
 *
 * Makes use of WObj_fixed_size to stretch the image, if >= 0
 */

#include "WObj.h"

/** object id */
#define WImg_ID WObj_ID "Img"


typedef struct {
    WObj super;

    // rect in WTheme Tex coordinates
    vec4 uv_rect;
    vec4 color;
} WImg;


/**
 * Initializes the object
 * @param obj WImg object
 * @param parent to inherit from
 * @param uv_rect in WTheme Tex coordinates, like "u_atlas_rect(WTheme_atlas(...), WTheme_CUSTOM_8);"
 * @return obj casted as WImg
 */
O_EXTERN
WImg *WImg_init(oobj obj, oobj parent, vec4 uv_rect);


/**
 * Creates a new WImg object
 * @param parent to inherit from
 * @param uv_rect in WTheme Tex coordinates
 * @return The new object
 */
O_INLINE
WImg *WImg_new(oobj parent, vec4 uv_rect)
{
    OObj_DECL_IMPL_NEW(WImg, parent, uv_rect);
}

//
// virtual implementations:
//


/**
 * Virtual implementation that renders the background color.
 * Calls WObj__v_update afterwards.
 */
O_EXTERN
vec2 WImg__v_update(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn);


//
// object functions:
//

/**
 * @param obj RObjQuad object
 * @return color to manipulate the image
 */
OObj_DECL_GETSET(WImg, vec4, color)

/**
 * @param obj RObjQuad object
 * @return rect in WTheme's RTex, may be one of WTheme_CUSTOM_*,
 *         like "u_atlas_rect(WTheme_atlas(...), WTheme_CUSTOM_8);"
 */
OObj_DECL_GETSET(WImg, vec4, uv_rect)


/**
 * @param obj WImg object
 * @return either uv_rect.zw (size) but overridden with WObj_fixed_size, if given
 */
O_EXTERN
vec2 WImg_size(oobj obj);

#endif //W_WIMG_H
