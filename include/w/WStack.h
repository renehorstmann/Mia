#ifndef W_WSTACK_H
#define W_WSTACK_H

/**
 * @file WStack.h
 *
 * Object (derives WObj)
 *
 * Widget that renders its children in a stack, but each child can set a custom order.
 * The higher that order, the later the child will get rendered / resorted to back.
 * Useful for multiple WWindows, also see window_auto_mode for that.
 */

#include "WObj.h"

/** object id */
#define WStack_ID WObj_ID "Stack"


#define WStack_order_KEY "WStack_order"

/**
 * Helper function to get the order option.
 * Calls for example WObj_option_set(obj_child, WStack_order, "10");
 * @param obj_child
 * @param order the higher: the later rendered / resorted to back
 * @return
 */
O_EXTERN
int WStack_child_order(oobj obj_child);

/**
 * Helper function to set the order option.
 * Calls for example WObj_option_set(obj_child, WStack_order_KEY, "10");
 * @param obj_child
 * @param order the higher: the later rendered / resorted to back
 * @return
 */
O_EXTERN
int WStack_child_order_set(oobj obj_child, int order);


typedef struct {
    WObj super;

    bool window_auto_mode;
} WStack;


/**
 * Initializes the object
 * @param obj WStack object
 * @param parent to inherit from
 * @return obj casted as WStack
 */
O_EXTERN
WStack *WStack_init(oobj obj, oobj parent);


/**
 * Creates a new WStack object
 * @param parent to inherit from
 * @return The new object
 */
O_INLINE
WStack *WStack_new(oobj parent)
{
    OObj_DECL_IMPL_NEW(WStack, parent);
}

//
// virtual implementations:
//


/**
 * Virtual implementation that updates its children according to WObj_list (which is overridden)
 */
O_EXTERN
vec2 WStack__v_update(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn);

/**
 * Virtual implementation that returns a resorted list of children, according to the children order
 */
O_EXTERN
oobj *WStack__v_list(oobj obj, osize *opt_out_num);


//
// object functions:
//

/**
 * @param obj WStack object
 * @return if true and a WWindow child is currently dragged:
 *         That WWindow's order is set to 1, all other WWindow's to 0
 */
OObj_DECL_GETSET(WStack, bool, window_auto_mode)


/**
 * Special update function that uses the tex projection to set lt and window_auto_size
 * @param obj WStack object
 * @param tex RTex to read r_proj from
 * @param theme WTheme to batch
 * @param opt_out_lt if not NULL, set to gained lt from proj
 * @param pointer_fn pointer function to use, may get overridden with self->pointer_fn
 *                   in most cases "a_pointer" (or "a_pointer_p" if rendered onto the p matrix of the cam)
 *                   asserts not NULL
 * @return used size
 */
O_EXTERN
vec2 WStack_update(oobj obj, oobj tex, oobj theme, vec2 *opt_out_lt, a_pointer__fn pointer_fn);


#endif //W_WSTACK_H
