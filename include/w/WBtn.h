#ifndef W_WBTN_H
#define W_WBTN_H

/**
 * @file WBtn.h
 *
 * Object (derives WObj)
 *
 * Widget that renders a button around its widgets.
 * @note as with u/btn, only switches mode using the event functions (clicked, toggled, pressed)
 *       or use the auto_ mode|event
 */

#include "WObj.h"

/** object id */
#define WBtn_ID WObj_ID "Btn"

enum WBtn_auto {
    WBtn_AUTO_OFF,
    WBtn_AUTO_CLICKED,
    WBtn_AUTO_PRESSED,
    WBtn_AUTO_TOGGLED
};

enum WBtn_style {
    WBtn_FLAT,
    WBtn_FLAT_ROUND,
    WBtn_DEF,
    WBtn_DEF_ROUND,
    WBtn_BIG,
    WBtn_BIG_ROUND,
    WBtn_RADIO,
    WBtn_CHECK,
    WBtn_SWITCH,
    WBtn_NUM_STYLES
};

typedef struct {
    WObj super;
    
    enum WBtn_style style;
    vec4 color;

    enum WBtn_auto auto_mode;
    OObj__event_fn auto_event;
    bool auto_action;

    // 0 for unpressed, 1 for pressed, >1 for custom
    int mode;

    struct a_pointer pointer_current, pointer_prev;
    
} WBtn;


/**
 * Initializes the object
 * @param obj WBtn object
 * @param parent to inherit from
 * @return obj casted as WBtn
 */
O_EXTERN
WBtn *WBtn_init(oobj obj, oobj parent);


/**
 * Creates a new WBtn object
 * @param parent to inherit from
 * @return The new object
 */
O_INLINE
WBtn *WBtn_new(oobj parent)

{
    OObj_DECL_IMPL_NEW(WBtn, parent);
}

//
// virtual implementations:
//


/**
 * Virtual implementation that sets the button rects and checks the pointer for clicked.
 * Calls WObj_update on all children.
 */
O_EXTERN
vec2 WBtn__v_update(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn);

/**
 * Virtual implementation that sets the color and style
 */
O_EXTERN
void WBtn__v_style_apply(oobj obj);


//
// object functions:
//

/**
 * @param obj WBtn object
 * @return style for the button
 */
OObj_DECL_GETSET(WBtn, enum WBtn_style, style)


/**
 * @param obj WBtn object
 * @return color for the button
 */
OObj_DECL_GETSET(WBtn, vec4, color)


/**
 * @param obj WBtn object
 * @return automatically call clicked, pressed, toggled (disabled by default)
 */
OObj_DECL_GETSET(WBtn, enum WBtn_auto, auto_mode)

/**
 * @param obj WBtn object
 * @return event that is triggered in an auto_mode
 */
OObj_DECL_SET(WBtn, OObj__event_fn, auto_event)


/**
 * @param obj WBtn object
 * @return current btn mode, 0 for unpressed, 1 for pressed, >1 for custom
 */
OObj_DECL_GETSET(WBtn, int, mode)

/**
 * @param obj WBtn object
 * @return true if pressed
 */
O_INLINE
bool WBtn_is_pressed(oobj obj) {
    return WBtn_mode(obj) == 1;
}

/**
 * @param obj WBtn object
 * @param pressed sets mode to 1 if true, else 0
 */
O_INLINE
void WBtn_set(oobj obj, bool pressed) {
    WBtn_mode_set(obj, pressed? 1 : 0);
}

/**
 * @param obj WBtn object
 * @param returns true if button is pressed and released again (click)
 */
O_EXTERN
bool WBtn_clicked(oobj obj);

/**
 * @param obj WBtn object
 * @param returns true if button got pressed
 */
O_EXTERN
bool WBtn_pressed(oobj obj);

/**
 * @param obj WBtn object
 * @param returns true if button got togglet (switched pressed mode)
 */
O_EXTERN
bool WBtn_toggled(oobj obj);

#endif //W_WBTN_H
