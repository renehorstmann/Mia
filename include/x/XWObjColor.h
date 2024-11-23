#ifndef W_XWOBJCOLOR_H
#define W_XWOBJCOLOR_H

/**
 * @file XWObjColor.h
 *
 * Object (derives WObj)
 *
 * Widget that combines x_gradient's to show hsvrgb(a) sliders to set a color
 * Also via hexcode
 */

#include "w/WObj.h"

/** object id */
#define XWObjColor_ID WObj_ID "XWObjColor"


typedef struct {
    WObj super;
    
    vec4 rgba;
    vec4 rgba_prev;
    bool rgba_update;

    // const for the lifetime
    bool alpha_enabled;
    
    vec4 uv_rect;

    // WBox
    oobj box;

    // WGradient's
    // alpha is always created, but if disabled, not part of the WObj hierarchy
    oobj hue, sat, val, red, green, blue, alpha;

    bool gradients_update;
    
    // WColor
    oobj color_prev, color_new;
    
    // WText
    oobj hex;
    
    // RTex to render gradients
    oobj gradient_tex;
    
} XWObjColor;


/**
 * Initializes the object
 * @param obj XWObjColor object
 * @param parent to inherit from
 * @param uv_rect theme (custom) area to write the gradients to. 
                  For example rect of WTheme_CUSTOM_32.
                  Width determines cols of gradient.
                  Height is asserted to min 7.
                  Uses the upper cols.
 * @param rgba_prev previous color so show and select
 * @param enable_alpha true to show an alpha gradient
 * @return obj cased as XWObjColor
 */
O_EXTERN
XWObjColor *XWObjColor_init(oobj obj, oobj parent, vec4 uv_rect, vec4 rgba_prev, bool enable_alpha);


/**
 * Creates a new XWObjColor object
 * @param parent to inherit from
 * @param uv_rect theme (custom) area to write the gradients to. 
                  For example rect of WTheme_CUSTOM_32.
                  Width determines cols of gradient.
                  Height is asserted to min 7.
                  Uses the upper cols.
 * @param rgba_prev previous color so show and select
 * @param enable_alpha true to show an alpha gradient
 * @return The new object
 */
O_INLINE
XWObjColor *XWObjColor_new(oobj parent, vec4 uv_rect, vec4 rgba_prev, bool enable_alpha)
{
    OObj_DECL_IMPL_NEW(XWObjColor, parent, uv_rect, rgba_prev, enable_alpha);
}


//
// virtual implementations:
//


/**
 * Virtual implementation that may update the gradient and checks for hex clicked.
 * Calls WObj_update on all children.
 */
O_EXTERN
vec2 XWObjColor__v_update(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn);

//
// object functions:
//

/**
 * @param obj XWObjColor object
 * @return actual rgba color
 */
OObj_DECL_GET(XWObjColor, vec4, rgba)


/**
 * @param obj XWObjColor object
 * @return previous rgba color
 */
OObj_DECL_GET(XWObjColor, vec4, rgba_prev)

/**
 * @param obj XWObjColor object
 * @return actual rgba color
 */
O_INLINE
vec4 XWOBjColor_rgba_set(oobj obj, vec4 set)
{
    OObj_assert(obj, XWObjColor);
    XWObjColor *self = obj;
    self->rgba = set;
    self->rgba_update = true;
    return set;
}

/**
 * @param obj XWObjColor object
 * @return main WBox
 */
OObj_DECL_GET(XWObjColor, oobj, box)


/**
 * @param obj XWObjColor object
 * @return WGradient
 */
OObj_DECL_GET(XWObjColor, oobj, hue)

/**
 * @param obj XWObjColor object
 * @return WGradient
 */
OObj_DECL_GET(XWObjColor, oobj, sat)

/**
 * @param obj XWObjColor object
 * @return WGradient
 */
OObj_DECL_GET(XWObjColor, oobj, val)
/**
 * @param obj XWObjColor object
 * @return WGradient
 */
OObj_DECL_GET(XWObjColor, oobj, red)

/**
 * @param obj XWObjColor object
 * @return WGradient
 */
OObj_DECL_GET(XWObjColor, oobj, green)

/**
 * @param obj XWObjColor object
 * @return WGradient
 */
OObj_DECL_GET(XWObjColor, oobj, blue)

/**
 * @param obj XWObjColor object
 * @return WGradient
 */
OObj_DECL_GET(XWObjColor, oobj, alpha)





#endif //W_XWOBJCOLOR_H
