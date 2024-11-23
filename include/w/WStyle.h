#ifndef MIA_WSTYLE_H
#define MIA_WSTYLE_H


/**
 * @file WStyle.h
 *
 * Object
 *
 * Default colors and styles for WObj's.
 * In contrast to most w classes, does >NOT< inherit from WObj!
 */


#include "WBtn.h"
#include "WPane.h"


/** object id */
#define WStyle_ID OObj_ID "WStyle"

typedef struct {
    OObj super;
    
    vec4 btn_color;
    enum WBtn_style btn_style;
    
    vec4 drag_bg_color;
    vec4 drag_progress_color_x;
    vec4 drag_progress_color_y;
    vec4 drag_arrow_color;
    vec4 drag_arrow_pressed_color;
    
    vec4 frame_border_color;
    float frame_border_size;
    
    vec4 pane_color;
    enum WPane_style pane_style;
    
    vec4 progress_color;
    
    vec4 slider_bar_color;
    vec4 slider_badge_color;
    vec4 slider_badge_pressed_color;

    vec4 imgpicker_badge_color;
    vec4 imgpicker_badge_pressed_color;
    
    vec4 text_color;
    vec4 textshadow_shadow_color;

    vec4 window_header_color;
    enum WPane_style window_header_style;
    vec4 window_body_color;
    enum WPane_style window_body_style;
    
    vec4 windowdialog_bg;
    vec4 windowdialog_icon_pane;
    
} WStyle;


/**
 * Initializes the object
 * @param obj WStyle object
 * @param parent to inherit from
 * @return obj casted as WStyle
 */
O_EXTERN
WStyle *WStyle_init(oobj obj, oobj parent);

/**
 * Creates a new the WStyle object
 * @param parent to inherit from
 * @return The new object
 */
O_INLINE
WStyle *WStyle_new(oobj parent)
{
    OObj_DECL_IMPL_NEW(WStyle, parent);
}


//
// object functions
//


/**
 * @param obj WStyle object
 * @return color
 */
OObj_DECL_GETSET(WStyle, vec4, btn_color)

/**
 * @param obj WStyle object
 * @return style
 */
OObj_DECL_GETSET(WStyle, enum WBtn_style, btn_style)


/**
 * @param obj WStyle object
 * @return color
 */
OObj_DECL_GETSET(WStyle, vec4, drag_bg_color)

/**
 * @param obj WStyle object
 * @return color
 */
OObj_DECL_GETSET(WStyle, vec4, drag_progress_color_x)

/**
 * @param obj WStyle object
 * @return color
 */
OObj_DECL_GETSET(WStyle, vec4, drag_progress_color_y)

/**
 * @param obj WStyle object
 * @return color
 */
OObj_DECL_GETSET(WStyle, vec4, drag_arrow_color)

/**
 * @param obj WStyle object
 * @return color
 */
OObj_DECL_GETSET(WStyle, vec4, drag_arrow_pressed_color)



/**
 * @param obj WStyle object
 * @return color
 */
OObj_DECL_GETSET(WStyle, vec4, frame_border_color)

/**
 * @param obj WStyle object
 * @return size
 */
OObj_DECL_GETSET(WStyle, float, frame_border_size)


/**
 * @param obj WStyle object
 * @return color
 */
OObj_DECL_GETSET(WStyle, vec4, pane_color)

/**
 * @param obj WStyle object
 * @return style
 */
OObj_DECL_GETSET(WStyle, enum WPane_style, pane_style)


/**
 * @param obj WStyle object
 * @return color
 */
OObj_DECL_GETSET(WStyle, vec4, progress_color)


/**
 * @param obj WStyle object
 * @return color
 */
OObj_DECL_GETSET(WStyle, vec4, slider_bar_color)

/**
 * @param obj WStyle object
 * @return color
 */
OObj_DECL_GETSET(WStyle, vec4, slider_badge_color)

/**
 * @param obj WStyle object
 * @return color
 */
OObj_DECL_GETSET(WStyle, vec4, slider_badge_pressed_color)


/**
 * @param obj WStyle object
 * @return color
 */
OObj_DECL_GETSET(WStyle, vec4, imgpicker_badge_color)

/**
 * @param obj WStyle object
 * @return color
 */
OObj_DECL_GETSET(WStyle, vec4, imgpicker_badge_pressed_color)


/**
 * @param obj WStyle object
 * @return color
 */
OObj_DECL_GETSET(WStyle, vec4, text_color)

/**
 * @param obj WStyle object
 * @return color
 */
OObj_DECL_GETSET(WStyle, vec4, textshadow_shadow_color)



/**
 * @param obj WStyle object
 * @return color
 */
OObj_DECL_GETSET(WStyle, vec4, window_header_color)

/**
 * @param obj WStyle object
 * @return style
 */
OObj_DECL_GETSET(WStyle, enum WPane_style, window_header_style)

/**
 * @param obj WStyle object
 * @return color
 */
OObj_DECL_GETSET(WStyle, vec4, window_body_color)

/**
 * @param obj WStyle object
 * @return style
 */
OObj_DECL_GETSET(WStyle, enum WPane_style, window_body_style)


/**
 * @param obj WStyle object
 * @return color
 */
OObj_DECL_GETSET(WStyle, vec4, windowdialog_bg)

/**
 * @param obj WStyle object
 * @return color
 */
OObj_DECL_GETSET(WStyle, vec4, windowdialog_icon_pane)



#endif //MIA_WSTYLE_H
