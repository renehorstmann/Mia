#ifndef W_WPANE_H
#define W_WPANE_H

/**
 * @file WPane.h
 *
 * Object (derives WObj)
 *
 * Widget that renders its children in a floating frame
 */

#include "WObj.h"

/** object id */
#define WPane_ID WObj_ID "Pane"

enum WPane_style {
    WPane_DEFAULT,
    WPane_ROUND,
    WPane_ROUND_TOP,
    WPane_ROUND_BOTTOM,
    WPane_NUM_STYLES
};

typedef struct {
    WObj super;
    
    enum WPane_style style;
    vec4 color;
    
    vec2 gen_min_inner_size;
    vec2 gen_inner_size;
} WPane;


/**
 * Initializes the object
 * @param obj WPane object
 * @param parent to inherit from
 * @return obj casted as WPane
 */
O_EXTERN
WPane *WPane_init(oobj obj, oobj parent);


/**
 * Creates a new WPane object
 * @param parent to inherit from
 * @return The new object
 */
O_INLINE
WPane *WPane_new(oobj parent)

{
    OObj_DECL_IMPL_NEW(WPane, parent);
}

//
// virtual implementations:
//


/**
 * Virtual implementation that sets the button rects and checks the pointer for clicked.
 * Calls WObj_update on all children.
 */
O_EXTERN
vec2 WPane__v_update(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn);

/**
 * Virtual implementation that sets the color and sryle
 */
O_EXTERN
void WPane__v_style_apply(oobj obj);

//
// object functions:
//

/**
 * @param obj WPane object
 * @return style for the floating pane
 */
OObj_DECL_GETSET(WPane, enum WPane_style, style)



/**
 * @param obj WPane object
 * @return color for the floating pane
 */
OObj_DECL_GETSET(WPane, vec4, color)

/**
 * @param obj WPane object
 * @return min inner size accirding to min_size - frane size
 */
OObj_DECL_GET(WPane, vec2, gen_min_inner_size)

/**
 * @param obj WPane object
 * @return used inner size accirding to min_size - frane size
 */
OObj_DECL_GET(WPane, vec2, gen_inner_size)


#endif //W_WPANE_H
