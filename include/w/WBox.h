#ifndef W_WBOX_H
#define W_WBOX_H

/**
 * @file WBox.h
 *
 * Object (derives WObj)
 *
 * Widget that renders children in a boxed layout aligned next to each other.
 */

#include "WObj.h"

/** object id */
#define WBox_ID WObj_ID "Box"

/**
 * Different layout modes for the box.
 * H: simply align all children right to each other (horizontal); min_size.y is passed
 * V: as H, but down each other (vertical); min_size.x is passed
 * H_V: align children right to each other until min_size is reached, then add a new vertical line
 * V_H: as H_V but vertical first
 * H_WEIGTHS: makes use of min_size and weights to align the children, passes the new cell size
 * V_WEIGTHS: as H_WEIGHTS but vertical
 */
enum WBox_layout {
    WBox_LAYOUT_H,
    WBox_LAYOUT_V,
    WBox_LAYOUT_H_V,
    WBox_LAYOUT_V_H,
    WBox_LAYOUT_H_WEIGHTS,
    WBox_LAYOUT_V_WEIGHTS,
    WBox_LAYOUT_NUM_MODES,
};

#define WBox_weight_KEY "WGrid_weight"

/**
 * Helper function to get the child weight option.
 * Calls for example WObj_option_set(obj_child, WBox_weight_KEY, "2.0");
 * @param obj_child WObj child of WBox
 * @return child weight, defaults to -1.0
 */
O_EXTERN
float WBox_child_weight(oobj obj_child);

/**
 * Helper function to set the cell key option.
 * Calls for example WObj_option_set(obj_child, WBox_weight_KEY, "2.0");
 * @param obj_child WObj child of WBox
 * @return child weight, defaults to -1.0
 */
O_EXTERN
float WBox_child_weight_set(oobj obj_child, float weight);


typedef struct {
    WObj super;

    enum WBox_layout layout;
    vec2 spacing;
} WBox;


/**
 * Initializes the object.
 * @param obj WBox object
 * @param parent to inherit from
 * @param layout order to place the children
 * @return obj casted as WBox
 */
O_EXTERN
WBox *WBox_init(oobj obj, oobj parent, enum WBox_layout layout);


/**
 * Creates a new WBox object
 * @param parent to inherit from
 * @param layout order to place the children
 * @return The new object
 */
O_INLINE
WBox *WBox_new(oobj parent, enum WBox_layout layout)
{
    OObj_DECL_IMPL_NEW(WBox, parent, layout);
}

//
// virtual implementations:
//

/**
 * Virtual implementation that updates the positions of the children.
 * Calls WObj_update on all children.
 * Uses min_size for H_V or V_H and passes 0 as min size to the children
 */
O_EXTERN
vec2 WBox__v_update(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn);

//
// object functions:
//

/**
 * @param obj WBox object
 * @return current layout mode to order the children placement
 */
OObj_DECL_GETSET(WBox, enum WBox_layout, layout)

/**
 * @param obj WBox object
 * @return current spacing between each child
 */
OObj_DECL_GETSET(WBox, vec2, spacing)


#endif //W_WBOX_H
