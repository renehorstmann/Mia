#ifndef W_WWINDOWDIALOG_H
#define W_WWINDOWDIALOG_H

/**
 * @file WWindowDialog.h
 *
 * Object (derives WWindow)
 *
 * Creates the window with additional dialog buttons, etc.
 */

#include "WWindow.h"

/** object id */
#define WWindowDialog_ID WWindow_ID "Dialog"


enum WWindowDialog_state {
    WWindowDialog_RUNNING,
    WWindowDialog_CANCEL,
    WWindowDialog_OK,
    WWindowDialog_NUM_STATES
};

typedef struct {
    WWindow super;
    
    bool bg_maximize;
    bool bg_handles_pointer;

    oobj icon;
    oobj icon_pane;
    oobj body;
    oobj bg;
    oobj btn_cancel, btn_ok;
    
    enum WWindowDialog_state state;

} WWindowDialog;


/**
 * Initializes the object
 * @param obj WWindowDialog object
 * @param parent to inherit from
 * @return obj casted as WWindowDialog
 */
O_EXTERN
WWindowDialog *WWindowDialog_init(oobj obj, oobj parent, const char *title, bool show_cancel, bool show_ok);


/**
 * Creates a new WWindowDialog object
 * @param parent to inherit from
 * @return The new object
 */
O_INLINE
WWindowDialog *WWindowDialog_new(oobj parent, const char *title, bool show_cancel, bool show_ok)
{
    OObj_DECL_IMPL_NEW(WWindowDialog, parent, title, show_cancel, show_ok);
}

//
// virtual implementations:
//


/**
 * Virtual implementation to update bg
 * Calls WObj_update on all children.
 */
O_EXTERN
vec2 WWindowDialog__v_update(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn);

/**
 * Virtual implementation that sets the colors and styles for the internal WPane's
 */
O_EXTERN
void WWindowDialog__v_style_apply(oobj obj);


//
// object functions:
//


/**
 * @param obj WWindowDialog object
 * @return if true, the WColor bg rect is enlarged to a very big near MAX sized rect.
 *         ignoring min_size and lt
 */
OObj_DECL_GETSET(WWindowDialog, bool, bg_maximize)

/**
 * @param obj WWindowDialog object
 * @return if true, pointers (0, 0) are handled if within the bg rect
 */
OObj_DECL_GETSET(WWindowDialog, bool, bg_handles_pointer)


/**
 * @param obj WWindowDialog object
 * @return WIcon icon
 */
OObj_DECL_GET(WWindowDialog, oobj, icon)

/**
 * @param obj WWindowDialog object
 * @return WPane which holds the icon
 */
OObj_DECL_GET(WWindowDialog, oobj, icon_pane)

/**
 * @param obj WWindowDialog object
 * @return WObj body
 * @note Use this WObj to place dialog elements in the body
 */
OObj_DECL_GET(WWindowDialog, oobj, body)

/**
 * @param obj WWindowDialog object
 * @return WObj body
 * @note WColor background, allocated on obj, using its min_size
 */
OObj_DECL_GET(WWindowDialog, oobj, bg)

/**
 * @param obj WWindowDialog object
 * @return state if running, canceled or ok is pressed
 */
OObj_DECL_GET(WWindowDialog, enum WWindowDialog_state, state)

#endif //W_WWINDOWDIALOG_H
