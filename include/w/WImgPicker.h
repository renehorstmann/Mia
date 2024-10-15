#ifndef W_WIMGPICKER_H
#define W_WIMGPICKER_H

/**
 * @file WImgPicker.h
 *
 * Object (derives WImg)
 *
 * 1D or 2D picker on top of a WImg
 */

#include "WImg.h"

/** object id */
#define WImgPicker_ID WImg_ID "Picker"

enum WImgPicker_mode {
    WImgPicker_X,
    WImgPicker_Y,
    WImgPicker_XY,
    WImgPicker_NUM_MODES
};

typedef struct {
    WImg super;

    vec4 badge_color;
    vec4 badge_pressed_color;

    enum WImgPicker_mode mode;

    // aka picked position [0-1]
    vec2 progress;

    bool on_unit;

    OObj__event_fn change_event;

    bool pressed;

    struct a_pointer prev_pointer;

} WImgPicker;


/**
 * Initializes the object
 * @param obj WImgPicker object
 * @param parent to inherit from
 * @param uv_rect in WTheme Tex coordinates, like "u_atlas_rect(WTheme_atlas(...), WTheme_CUSTOM_8);"
 * @return obj casted as WImgPicker
 */
O_EXTERN
WImgPicker *WImgPicker_init(oobj obj, oobj parent, vec4 uv_rect);


/**
 * Creates a new WImgPicker object
 * @param parent to inherit from
 * @param uv_rect in WTheme Tex coordinates
 * @return The new object
 */
O_INLINE
WImgPicker *WImgPicker_new(oobj parent, vec4 uv_rect)
{
    OObj_DECL_IMPL_NEW(WImgPicker, parent, uv_rect);
}

//
// virtual implementations:
//


/**
 * Virtual implementation that renders the background color.
 * Calls WObj__v_update afterwards.
 */
O_EXTERN
vec2 WImgPicker__v_update(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn);

/**
 * Virtual implementation that sets the colors
 */
O_EXTERN
void WImgPicker__v_style_apply(oobj obj);

//
// object functions:
//


/**
 * @param obj WImgPicker object
 * @return color for the badge
 */
OObj_DECL_GETSET(WImgPicker, vec4, badge_color)

/**
 * @param obj WImgPicker object
 * @return color for the badge, if currently pressed (defaults to badge_color.xyz * 0.5)
 */
OObj_DECL_GETSET(WImgPicker, vec4, badge_pressed_color)

/**
 * @param obj WImgPicker object
 * @return mode which axes of progress is used for picking, inits to WImgPicker_XY
 */
OObj_DECL_GETSET(WImgPicker, enum WImgPicker_mode, mode)


/**
 * @param obj WImgPicker object
 * @return picker progress in [0:1]
 */
OObj_DECL_GETSET(WImgPicker, vec2, progress)

/**
 * @param obj WImgPicker object
 * @return picker progress in [0:WImg_size)
 */
O_EXTERN
ivec2 WImgPicker_progress_img(oobj obj);


/**
 * @param obj WImgPicker object
 * @return if true, the progress badge is ceiled to a unit (default is true)
 */
OObj_DECL_GETSET(WImgPicker, bool, on_unit)


/**
 * @param obj WImgPicker object
 * @return event that is triggered if progress changed via a pointer
 */
OObj_DECL_SET(WImgPicker, OObj__event_fn, change_event)

/**
 * @param obj WImgPicker object
 * @return true if the slider badge is currently pressed
 */
OObj_DECL_GET(WImgPicker, bool, pressed)

#endif //W_WIMGPICKER_H
