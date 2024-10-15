#ifndef W_WSLIDER_H
#define W_WSLIDER_H

/**
 * @file WSlider.h
 *
 * Object (derives WObj)
 *
 * Widget that renders a slider.
 * Children are stacked right next to the slider.
 *
 * In horizontal mode (default): left is 0.0, right is 1.0.
 * In vertical mode:             bottom is 0.0, top is 1.0.
 */

#include "WObj.h"

/** object id */
#define WSlider_ID WObj_ID "Slider"

typedef struct {
    WObj super;
    
    vec4 bar_color;
    vec4 badge_color;
    vec4 badge_pressed_color;

    float progress;

    bool vertical;
    bool on_unit;

    OObj__event_fn change_event;

    bool pressed;

    struct a_pointer prev_pointer;
    
} WSlider;


/**
 * Initializes the object
 * @param obj WSlider object
 * @param parent to inherit from;
 * @return obj casted as WSlider
 */
O_EXTERN
WSlider *WSlider_init(oobj obj, oobj parent);


/**
 * Creates a new WSlider object
 * @param parent to inherit from
 * @return The new object
 */
O_INLINE
WSlider *WSlider_new(oobj parent)

{
    OObj_DECL_IMPL_NEW(WSlider, parent);
}

//
// virtual implementations:
//


/**
 * Virtual implementation that sets the button rects and checks the pointer for clicked.
 * Calls WObj_update on all children.
 * Uses min_size for the slider field, children placed right next to it and passed 0
 */
O_EXTERN
vec2 WSlider__v_update(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn);

/**
 * Virtual implementation that sets the colors
 */
O_EXTERN
void WSlider__v_style_apply(oobj obj);

//
// object functions:
//

/**
 * @param obj WSlider object
 * @return color for the bar
 */
OObj_DECL_GETSET(WSlider, vec4, bar_color)


/**
 * @param obj WSlider object
 * @return color for the badge
 */
OObj_DECL_GETSET(WSlider, vec4, badge_color)

/**
 * @param obj WSlider object
 * @return color for the badge, if currently pressed (defaults to badge_color.xyz * 0.5)
 */
OObj_DECL_GETSET(WSlider, vec4, badge_pressed_color)


/**
 * @param obj WSlider object
 * @return badge progress in [0:1]
 */
OObj_DECL_GETSET(WSlider, float, progress)

/**
 * @param obj WSlider object
 * @return if true, the slider is rendered vertically, else horizontally (default is false)
 */
OObj_DECL_GETSET(WSlider, bool, vertical)

/**
 * @param obj WSlider object
 * @return if true, the progress badge is ceiled to a unit (default is true)
 */
OObj_DECL_GETSET(WSlider, bool, on_unit)


/**
 * @param obj WSlider object
 * @return event that is triggered if progress changed via a pointer
 */
OObj_DECL_SET(WSlider, OObj__event_fn, change_event)

/**
 * @param obj WSlider object
 * @return true if the slider badge is currently pressed
 */
OObj_DECL_GET(WSlider, bool, pressed)


#endif //W_WSLIDER_H
