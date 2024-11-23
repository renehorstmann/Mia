#ifndef W_WICON_H
#define W_WICON_H

/**
 * @file WIcon.h
 *
 * Object (derives WObj)
 *
 * Widget that renders an icon from the scene
 */

#include "WObj.h"
#include "WTheme.h"

/** object id */
#define WIcon_ID WObj_ID "Icon"


typedef struct {
    WObj super;

    enum WTheme_indices icon_idx;
    vec4 color;
} WIcon;


/**
 * Initializes the object
 * @param obj WIcon object
 * @param parent to inherit from
 * @param icon_idx one of the WTheme_ICON_* indices
 * @return obj casted as WIcon
 */
O_EXTERN
WIcon *WIcon_init(oobj obj, oobj parent, enum WTheme_indices icon_idx);


/**
 * Creates a new WIcon object
 * @param parent to inherit from
 * @param icon_idx one of the WTheme_ICON_* indices
 * @return The new object
 */
O_INLINE
WIcon *WIcon_new(oobj parent, enum WTheme_indices icon_idx)
{
    OObj_DECL_IMPL_NEW(WIcon, parent, icon_idx);
}

//
// virtual implementations:
//


/**
 * Virtual implementation that renders the background color.
 * Calls WObj__v_update afterwards.
 */
O_EXTERN
vec2 WIcon__v_update(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn);


//
// object functions:
//

/**
 * @param obj RObjQuad object
 * @return color to manipulate the icon
 */
OObj_DECL_GETSET(WIcon, vec4, color)

/**
 * @param obj RObjQuad object
 * @return one of the WTheme_ICON_* indices
 */
OObj_DECL_GETSET(WIcon, enum WTheme_indices, icon_idx)

#endif //W_WICON_H
