#ifndef W_WNUM_H
#define W_WNUM_H

/**
 * @file WNum.h
 *
 * Object (derives WObj)
 *
 * Widget that combines a WDrag and a WText to display the num, which is [min:max] with step instead of [0:1].
 * These widgets are WFrame'd with an otional WText label.
 *
 * WDrag's (WNum_drag) WObj_min_size defaults to .x=32
 */

#include "WObj.h"

/** object id */
#define WNum_ID WObj_ID "Num"


typedef struct {
    WObj super;

    float num;
    
    float min, max, step;

    oobj drag;
    oobj text_num;
    oobj frame;
    oobj frame_label;

    struct a_pointer pointer_prev;

    OObj__event_fn change_event;
    OObj__event_fn num_pressed_event;
} WNum;


/**
 * Initializes the object
 * @param obj WNum object
 * @param parent to inherit from
 * @param min, max minimal and maximal num (clamped)
 * @param step size between a value, will determine the printed precision (asserts >0)
 * @param opt_label if not NULL set as text in the frame label
 * @return obj casted as WNum
 */
O_EXTERN
WNum *WNum_init(oobj obj, oobj parent, float min, float max, float step, const char *opt_label);


/**
 * Creates a new WNum object
 * @param parent to inherit from
 * @param min, max minimal and maximal num (clamped)
 * @param step size between a value, will determine the printed precision (asserts >0)
 * @param opt_label if not NULL set as text in the frame label
 * @return The new object
 */
O_INLINE
WNum *WNum_new(oobj parent, float min, float max, float step, const char *opt_label)
{
    OObj_DECL_IMPL_NEW(WNum, parent, min, max, step, opt_label);
}

//
// virtual implementations:
//


/**
 * Virtual implementation that renders the background color.
 * Calls WObj__v_update afterwards.
 */
O_EXTERN
vec2 WNum__v_update(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn);


//
// object functions:
//

/**
 * Sets the bundled child colors.
 * @param obj WNum object
 * @param frame_color for the WFrame and optional WText label
 * @param drag_bg_color, drag_progress_color, drag_arrow_color background, progress, arrow colors of the WDrag
 * @param num_text_color for the WText besides the WDrag, which displays the current value
 */
O_EXTERN
void WNum_colors_set(oobj obj, vec4 frame_color,
                     vec4 drag_bg_color, vec4 drag_progress_color, vec4 drag_arrow_color,
                     vec4 num_text_color);

/**
 * @param obj WNum object
 * @return current value, clamped between min and max
 */
OObj_DECL_GET(WNum, float, num)


/**
 * @param obj WNum object
 * @param set current value, clamped between min and max and stepped
 * @return self->num which may be != set due to stepping and clamp
 * @note should not be called each frame, else the dragging may feel weird due to stepping
 */
O_EXTERN
float WNum_num_set(oobj obj, float set);

/**
 * @param obj WNum object
 * @return minimal value
 */
OObj_DECL_GETSET(WNum, float, min)

/**
 * @param obj WNum object
 * @return maximal value
 */
OObj_DECL_GETSET(WNum, float, max)

/**
 * @param obj WNum object
 * @return step size, will determine the printed precision (asserts >0)
 */
OObj_DECL_GETSET(WNum, float, step)


/**
 * @param obj WNum object
 * @return event that is triggered if num changed via a pointer
 */
OObj_DECL_SET(WNum, OObj__event_fn, change_event)

/**
 * @param obj WNum object
 * @return event that is triggered if the num text is pressed by pointer
 */
OObj_DECL_SET(WNum, OObj__event_fn, num_pressed_event)


/**
 * @param obj WNum object
 * @return WDrag
 */
OObj_DECL_GET(WNum, oobj, drag)

/**
 * @param obj WNum object
 * @return WText
 */
OObj_DECL_GET(WNum, oobj, text_num)

/**
 * @param obj WNum object
 * @return WFrame
 */
OObj_DECL_GET(WNum, oobj, frame)

/**
 * @param obj WNum object
 * @return WText
 */
OObj_DECL_GET(WNum, oobj, frame_label)

/**
 * @param obj WNum object
 * @return used decimal places, according to the step
 */
O_EXTERN
int WNum_decimal_places(oobj obj);

#endif //W_WNUM_H
