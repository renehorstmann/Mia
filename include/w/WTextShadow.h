#ifndef W_WTEXTSHADOW_H
#define W_WTEXTSHADOW_H

/**
 * @file WTextShadow.h
 *
 * Object (derives WText)
 *
 * Text with background shadow.
 * Uses 2x rects as compared to WText.
 * Each character has rect 0 as background shadow and rect 1 is the foreground
 */

#include "WText.h"

/** object id */
#define WTextShadow_ID WObj_ID "TextShadow"


typedef struct {
    WText super;

    vec4 shadow_color;
} WTextShadow;


/**
 * Initializes the object
 * @param obj WTextShadow object
 * @param parent to inherit from
 * @param text to render
 * @return obj casted as WTextShadow
 */
O_EXTERN
WTextShadow *WTextShadow_init(oobj obj, oobj parent, const char *text);


/**
 * Creates a new WTextShadow object
 * @param parent to inherit from
 * @param text to render
 * @return The new object
 */
O_INLINE
WTextShadow *WTextShadow_new(oobj parent, const char *text)
{
    OObj_DECL_IMPL_NEW(WTextShadow, parent, text);
}

//
// virtual implementations:
//


/**
 * Virtual implementation that renders the text into the given WTheme object.
 * Calls WObj__v_update afterwards.
 */
O_EXTERN
vec2 WTextShadow__v_update(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn);

/**
 * Virtual implementation that sets the text color
 */
O_EXTERN
void WTextShadow__v_style_apply(oobj obj);
  

//
// object functions:
//


/**
 * @param obj WTextShadow object
 * @return color for each char on an update call
 */
OObj_DECL_GETSET(WTextShadow, vec4, shadow_color)

#endif //W_WTEXTSHADOW_H
