#ifndef W_WDRAG_H
#define W_WDRAG_H

/**
 * @file WDrag.h
 *
 * Object (derives WObj)
 *
 * Widget that renders a dragging bar (like a slider with a touchpad).
 * Children are stacked in the center of the drag bar
 *
 * In horizontal mode (default): left is 0.0, right is 1.0.
 * In vertical mode:             bottom is 0.0, top is 1.0.
 */

#include "WObj.h"

/** object id */
#define WDrag_ID WObj_ID "Drag"

enum WDrag_mode {
    WDrag_X,
    WDrag_Y,
    WDrag_XY,
    WDrag_NUM_MODES
};

typedef struct {
    WObj super;

    vec4 bg_color;
    vec4 progress_color_x;
    vec4 progress_color_y;
    vec4 arrow_color;
    vec4 arrow_pressed_color;
    
    // inits to 0
    vec2 progress;
    vec2 progress_raw;
    
    // inits to X
    enum WDrag_mode mode;

    // inits to -1 = disabled
    vec2 steps;
    bool on_unit;
    
    // inits to 1 (second)
    vec2 drag_time;

    OObj__event_fn change_event;

    bool pressed;
    
    struct a_pointer prev_pointer;
    vec2 prev_pos;
    ou64 prev_time;
    vec2 prev_speed;
} WDrag;


/**
 * Initializes the object
 * @param obj WDrag object
 * @param parent to inherit from
 * @return obj casted as WDrag
 */
O_EXTERN
WDrag *WDrag_init(oobj obj, oobj parent);


/**
 * Creates a new WDrag object
 * @param parent to inherit from
 * @return The new object
 */
O_INLINE
WDrag *WDrag_new(oobj parent)
{
    OObj_DECL_IMPL_NEW(WDrag, parent);
}

//
// virtual implementations:
//


/**
 * Virtual implementation that renders the background color.
 * Calls WObj_update on all children.
 * Uses min_size and passes 0 to its children.
 */
O_EXTERN
vec2 WDrag__v_update(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn);

/**
 * Virtual implementation that sets the colors
 */
O_EXTERN
void WDrag__v_style_apply(oobj obj);

//
// object functions:
//

/**
 * @param obj WDrag object
 * @return color for the background
 */
OObj_DECL_GETSET(WDrag, vec4, bg_color)

/**
 * @param obj WDrag object
 * @return color for the progress bar (R_TRANSPARENT to hide)
 */
OObj_DECL_GETSET(WDrag, vec4, progress_color_x)

/**
 * @param obj WDrag object
 * @return color for the progress bar (R_TRANSPARENT to hide)
 */
OObj_DECL_GETSET(WDrag, vec4, progress_color_y)


/**
 * @param obj WDrag object
 * @return color for the arrows
 */
OObj_DECL_GETSET(WDrag, vec4, arrow_color)

/**
 * @param obj WDrag object
 * @return color for the arrows, if currently pressed
 */
OObj_DECL_GETSET(WDrag, vec4, arrow_pressed_color)

/**
 * @param obj WDrag object
 * @return progress [0:1] stepped
 */
OObj_DECL_GET(WDrag, vec2, progress)

/**
 * @param obj WDrag object
 * @return progress [0:1] unstepped
 */
OObj_DECL_GET(WDrag, vec2, progress_raw)


/**
 * @param obj WDrag object
 * @return progress [0:1] unstepped
 * @note if set each frame, may create weird dragging
 */
O_EXTERN
vec2 WDrag_progress_raw_set(oobj obj, vec2 set);

/**
 * @param obj WDrag object
 * @return mode which axes of progress is used for dragging, inits to WDrag_X
 */
OObj_DECL_GETSET(WDrag, enum WDrag_mode, mode)

/**
 * @param obj WDrag object
 * @return step size for progress if >0 (defaults to -1)
 */
OObj_DECL_GETSET(WDrag, vec2, steps)

/**
 * @param obj WDrag object
 * @return if true, the progress bar is ceiled to a unit (default is true)
 */
OObj_DECL_GETSET(WDrag, bool, on_unit)

/**
 * @param obj WDrag object
 * @return adds a progress of '1.00' if dragged the whole bar in that time. (defaults to 1 (second))
 *         so if dragged 50% of the bar in 2*drag_time seconds, adds '0.25' of progress
 */
OObj_DECL_GETSET(WDrag, vec2, drag_time)

/**
 * @param obj WDrag object
 * @return event that is triggered if progress changed via a pointer
 */
OObj_DECL_SET(WDrag, OObj__event_fn, change_event)

/**
 * @param obj WDrag object
 * @return true if the drag bar is currently pressed
 */
OObj_DECL_GET(WDrag, bool, pressed)



#endif //W_WDRAG_H
