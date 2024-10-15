#ifndef W_WGRADIENT_H
#define W_WGRADIENT_H

/**
 * @file WGradient.h
 *
 * Object (derives WObj)
 *
 * Widget that combines a WImgPicker (mode x) and a WText to display a gradient, which is [min:max] with step instead of [0:1].
 * These widgets are WFrame'd with an otional WText label.
 *
 * WImgPicker's (WGradient_imgpicker) WObj_fixed_size defaults to .x=64, .y=7
 */

#include "WObj.h"

/** object id */
#define WGradient_ID WObj_ID "Gradient"

typedef struct {
    WObj super;

    float num;
    
    float min, max, step;

    oobj imgpicker;
    oobj text_num;
    oobj frame;
    oobj frame_label;

    struct a_pointer pointer_prev;

    OObj__event_fn change_event;
    OObj__event_fn num_pressed_event;
} WGradient;


/**
 * Initializes the object
 * @param obj WGradient object
 * @param parent to inherit from
 * @param uv_rect in WTheme Tex coordinates, like "u_atlas_rect(WTheme_atlas(...), WTheme_CUSTOM_8);"
 * @param min, max minimal and maximal num (clamped)
 * @param step size between a value, will determine the printed precision (asserts >0)
 * @param opt_label if not NULL set as text in the frame label
 * @return obj casted as WGradient
 */
O_EXTERN
WGradient *WGradient_init(oobj obj, oobj parent, vec4 uv_rect, float min, float max, float step, const char *opt_label);


/**
 * Creates a new WGradient object
 * @param parent to inherit from
 * @param uv_rect in WTheme Tex coordinates, like "u_atlas_rect(WTheme_atlas(...), WTheme_CUSTOM_8);"
 * @param min, max minimal and maximal num (clamped)
 * @param step size between a value, will determine the printed precision (asserts >0)
 * @param opt_label if not NULL set as text in the frame label
 * @return The new object
 */
O_INLINE
WGradient *WGradient_new(oobj parent, vec4 uv_rect, float min, float max, float step, const char *opt_label)
{
    OObj_DECL_IMPL_NEW(WGradient, parent, uv_rect, min, max, step, opt_label);
}

//
// virtual implementations:
//


/**
 * Virtual implementation that renders the background color.
 * Calls WObj__v_update afterwards.
 */
O_EXTERN
vec2 WGradient__v_update(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn);


//
// object functions:
//

/**
 * Sets the bundled child colors.
 * @param obj WGradient object
 * @param frame_color for the WFrame and optional WText label
 * @param badge_color, badge_pressed_color for the picker
 * @param num_text_color for the WText besides the WImgPicker, which displays the current value
 */
O_EXTERN
void WGradient_colors_set(oobj obj, vec4 frame_color, 
        vec4 badge_color, vec4 badge_pressed_color,
        vec4 num_text_color);


/**
 * @param obj WGradient object
 * @return current value in [0:1]
 */
O_EXTERN
float WGradient_progress(oobj obj);

/**
 * @param obj WGradient object
 * @return current value in [0:1]
 */
O_EXTERN
float WGradient_progress_set(oobj obj, float set);

/**
 * @param obj WGradient object
 * @return current value, clamped between min and max
 */
OObj_DECL_GET(WGradient, float, num)


/**
 * @param obj WGradient object
 * @param set current value, clamped between min and max and stepped
 * @return self->num which may be != set due to stepping and clamp
 * @note should not be called each frame, else the dragging may feel weird due to stepping
 */
O_EXTERN
float WGradient_num_set(oobj obj, float set);

/**
 * @param obj WGradient object
 * @return minimal value
 */
OObj_DECL_GETSET(WGradient, float, min)

/**
 * @param obj WGradient object
 * @return maximal value
 */
OObj_DECL_GETSET(WGradient, float, max)

/**
 * @param obj WGradient object
 * @return step size, will determine the printed precision (asserts >0)
 */
OObj_DECL_GETSET(WGradient, float, step)


/**
 * @param obj WGradient object
 * @return event that is triggered if num changed via a pointer
 */
OObj_DECL_SET(WGradient, OObj__event_fn, change_event)

/**
 * @param obj WGradient object
 * @return event that is triggered if the num text is pressed by pointer
 */
OObj_DECL_SET(WGradient, OObj__event_fn, num_pressed_event)


/**
 * @param obj WGradient object
 * @return WImgPicker
 */
OObj_DECL_GET(WGradient, oobj, imgpicker)

/**
 * @param obj WGradient object
 * @return WText
 */
OObj_DECL_GET(WGradient, oobj, text_num)

/**
 * @param obj WGradient object
 * @return WFrame
 */
OObj_DECL_GET(WGradient, oobj, frame)

/**
 * @param obj WGradient object
 * @return WText
 */
OObj_DECL_GET(WGradient, oobj, frame_label)

/**
 * @param obj WGradient object
 * @return used decimal places, according to the step
 */
O_EXTERN
int WGradient_decimal_places(oobj obj);

#endif //W_WGRADIENT_H
