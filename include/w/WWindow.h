#ifndef W_WWINDOW_H
#define W_WWINDOW_H

/**
 * @file WWindow.h
 *
 * Object (derives WObj)
 *
 * Widget that has a header and a body as WFloat, the body may be hidden.
 * Dragging the header with the pointer shifts the window within its min_size.
 * So using a "fullscreen window" (lt = proj_lt, min_size = proj_size) allows for draggable windows.
 * To create a window label, use the WWindow_header() (WPane) object.
 * To add elements in the window body, use the WWindow_body() (WPane) object.
 *
 * @note WWindow does >NOT< render its own children, use WWindow_header() and WWindow_body() instead!
 */

#include "WObj.h"
#include "u/btn.h"

/** object id */
#define WWindow_ID WObj_ID "Window"

typedef struct {
    WObj super;

    // options
    bool lt_on_unit;
    bool hideable;
    bool draggable;
    vec2 header_min_size;
    vec2 body_offset;

    // state
    bool hidden;
    vec2 lt;

    bool dragging;
    vec2 dragging_pos;
    vec2 dragging_lt;

    // OObj to break the WObj update hierarchy (to allocate header and body)
    oobj container;
    // WPane's
    oobj header, body;
    // WAlign in header
    oobj header_align;

    int btn_hide_rect_idx;
    struct u_btn btn_hide;
    ou64 dbl_tap_hide_timer;
    ou64 hide_block_timer;

    vec2 children_size_prev;
    struct a_pointer pointer_prev;

} WWindow;


/**
 * Initializes the object
 * @param obj WWindow object
 * @param parent to inherit from
 * @param header_min_size header size
 * @return obj casted as WWindow
 */
O_EXTERN
WWindow *WWindow_init(oobj obj, oobj parent, vec2 header_min_size);


/**
 * Creates a new WWindow object
 * @param parent to inherit from
 * @param header_min_size header size
 * @return The new object
 */
O_INLINE
WWindow *WWindow_new(oobj parent, vec2 header_min_size)

{
    OObj_DECL_IMPL_NEW(WWindow, parent, header_min_size);
}


/**
 * Creates a new WWindow object with a WTextShadow as title in the header
 * @param parent to inherit from
 * @param header_min_size header size
 * @param title text for the embedded WTextShadow title (header)
 * @param opt_out_title if not NULL, set to the created WTextShadow
 * @return The new object
 */
O_EXTERN
WWindow *WWindow_new_title(oobj parent, vec2 header_min_size,
        const char *title, oobj *opt_out_title);


//
// virtual implementations:
//


/**
 * Virtual implementation that creates and manages the window
 * Calls WObj_update on all children.
 * min_size used and passed to direct children, body and header get 0
 */
O_EXTERN
vec2 WWindow__v_update(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn);


/**
 * Virtual implementation that returns both the direct children, the header and the body
 */
O_EXTERN
oobj *WWindow__v_list(oobj obj, osize *opt_out_num);

/**
 * Virtual implementation that sets the colors and styles for the internal WPane's
 */
O_EXTERN
void WWindow__v_style_apply(oobj obj);

//
// object functions:
//

/**
 * @param obj WWindow object
 * @return if true, lt is rounded to a unit (default)
 */
OObj_DECL_GETSET(WWindow, bool, lt_on_unit)

/**
 * @param obj WWindow object
 * @return if true (default) the body can be hidden either by a btn or dbl header click
 */
OObj_DECL_GETSET(WWindow, bool, hideable)

/**
 * @param obj WWindow object
 * @return if true (default) the window can be dragged with the header
 */
OObj_DECL_GETSET(WWindow, bool, draggable)


/**
 * @param obj WWindow object
 * @return minimal size for the header
 */
OObj_DECL_GETSET(WWindow, vec2, header_min_size)

/**
 * @param obj WWindow object
 * @return additional offset for the body, defaults to vec2_(0, 1) (so the header shadow is on top of the body)
 */
OObj_DECL_GETSET(WWindow, vec2, body_offset)

/**
 * @param obj WWindow object
 * @return if true, the body is hidden
 */
OObj_DECL_GETSET(WWindow, bool, hidden)

/**
 * @param obj WWindow object
 * @return dragged left top position
 */
OObj_DECL_GETSET(WWindow, vec2, lt)

/**
 * @param obj WWindow object
 * @return true if the header is currently dragged
 */
OObj_DECL_GET(WWindow, bool, dragging)

/**
 * @param obj WWindow object
 * @return WPane header
 * @note Use this WObj to place window elements in the header.
 *       body lt depends on WWindow_min_size.y and NOT the actual generated header size!
 */
OObj_DECL_GET(WWindow, oobj, header)


/**
 * @param obj WWindow object
 * @return WPane body
 * @note Use this WObj to place window elements in the body
 */
OObj_DECL_GET(WWindow, oobj, body)

/**
 * @param obj WWindow object
 * @return WAlign allocated on header
 * @note Use this WObj to place window elements in the header with alignments
 */
OObj_DECL_GET(WWindow, oobj, header_align)




#endif //W_WWINDOW_H
