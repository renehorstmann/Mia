#ifndef W_WALIGN_H
#define W_WALIGN_H

/**
 * @file WAlign.h
 *
 * Object (derives WObj)
 *
 * Widget that tries to align its children (stacked).
 * Uses min_size for the align field.
 * Returns o_max(min_size, children_size) but may render outside it for a frame to calc the offset
 *
 * Each child of WAlign may have its own WAlign_mode.
 * Default, see WAlign_align_h|v.
 * For a specific child, set the child WObj_option to one of the WAlign_VALUES
 * with the key of WAlign_KEY_H|V
 * for example:
 * ```c
 *     WObj_option_set(child, WAlign_KEY_H, WAlign_VALUE_CENTER); 
       WObj_option_set(child, WAlign_KEY_V, WAlign_VALUE_CENTER);   
 * ```
 *
 */

#include "WObj.h"

/** object id */
#define WAlign_ID WObj_ID "Align"

// START -> left and top
// END -> right and bottom
enum WAlign_mode {
    WAlign_START,
    WAlign_CENTER,
    WAlign_END,
    WAlign_FIT,
    WAlign_NUM_MODES
};

#define WAlign_KEY_H "WAlign_h"
#define WAlign_KEY_V "WAlign_v"

static const char *WAlign_VALUES[] = {
    "start",
    "center",
    "end",
    "fit"
};

#define WAlign_VALUE_START   (WAlign_VALUES[0])
#define WAlign_VALUE_CENTER  (WAlign_VALUES[1])
#define WAlign_VALUE_END     (WAlign_VALUES[2])
#define WAlign_VALUE_FIT     (WAlign_VALUES[3])


typedef struct {
    WObj super;
    
    // defaults
    enum WAlign_mode align_h, align_v;

} WAlign;


/**
 * Initializes the object
 * @param obj WAlign object
 * @param parent to inherit from
 * @return obj casted as WAlign
 */
O_EXTERN
WAlign *WAlign_init(oobj obj, oobj parent);


/**
 * Creates a new WAlign object.
 * Alignment default is set to start.
 * @param parent to inherit from
 * @return The new object
 */
O_INLINE
WAlign *WAlign_new(oobj parent)
{
    OObj_DECL_IMPL_NEW(WAlign, parent);
}


/**
 * Creates a new WAlign object.
 * Centered in both horizontal and vertical.
 * @param parent to inherit from
 * @return The new object
 */
O_EXTERN
WAlign *WAlign_new_center(oobj parent);


/**
 * Creates a new WAlign object.
 * Centered horizontal.
 * @param parent to inherit from
 * @return The new object
 */
O_EXTERN
WAlign *WAlign_new_center_h(oobj parent);

/**
 * Creates a new WAlign object.
 * Centered vertical.
 * @param parent to inherit from
 * @return The new object
 */
O_EXTERN
WAlign *WAlign_new_center_v(oobj parent);

//
// virtual implementations:
//


/**
 * Virtual implementation that renders the background color.
 * Calls WObj_update on all children.
 * min_size is used and to the children a min_size of 0 is passed
 */
O_EXTERN
vec2 WAlign__v_update(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn);


//
// object functions:
//

/**
 * @param obj WAlign object
 * @return horizontal default align mode (left : right) (defaults to start)
 */
OObj_DECL_GETSET(WAlign, enum WAlign_mode, align_h)

/**
 * @param obj WAlign object
 * @return vertical default align mode (top : bottom) (defaults to start)
 */
OObj_DECL_GETSET(WAlign, enum WAlign_mode, align_v)

/**
 * @param obj WAlign object
 * @param align_h horizontal default align mode (left : right) (defaults to start)
 * @param align_v vertical default align mode (top : bottom) (defaults to start)
 */
O_INLINE
void WAlign_align_set(oobj obj, enum WAlign_mode align_h, enum WAlign_mode align_v)
{
    WAlign_align_h_set(obj, align_h);
    WAlign_align_v_set(obj, align_v);
}


#endif //W_WALIGN_H
