#ifndef W_WCOLOR_H
#define W_WCOLOR_H

/**
 * @file WColor.h
 *
 * Object (derives WObj)
 *
 * Widget that renders a color in its used size.
 * Useful dor debug mockups, etc.
 */

#include "WObj.h"

/** object id */
#define WColor_ID WObj_ID "Color"


typedef struct {
    WObj super;

    struct r_rect rect;
    vec4 color;
} WColor;


/**
 * Initializes the object
 * @param obj WColor object
 * @param parent to inherit from
 * @param color for the background
 * @return obj casted as WColor
 */
O_EXTERN
WColor *WColor_init(oobj obj, oobj parent, vec4 color);


/**
 * Creates a new WColor object
 * @param parent to inherit from
 * @param color for the background
 * @return The new object
 */
O_INLINE
WColor *WColor_new(oobj parent, vec4 color)
{
    OObj_DECL_IMPL_NEW(WColor, parent, color);
}

//
// virtual implementations:
//


/**
 * Virtual implementation that renders the background color.
 * Calls WObj__v_update afterwards.
 */
O_EXTERN
vec2 WColor__v_update(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn);


//
// object functions:
//

/**
 * @param obj WColor object
 * @return background color
 */
OObj_DECL_GETSET(WColor, vec4, color)

#endif //W_WCOLOR_H
