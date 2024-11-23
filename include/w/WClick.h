#ifndef W_WCLICK_H
#define W_WCLICK_H

/**
 * @file WClick.h
 *
 * Object (derives WObj)
 *
 * Widget that listens for pointer events in its used size
 */

#include "WObj.h"

/** object id */
#define WClick_ID WObj_ID "Click"


typedef struct {
    WObj super;

    bool down;
    bool pressed;
    bool clicked;   // aka released

    vec2 pos;

    OObj__event_fn pressed_event, clicked_event;

    struct a_pointer pointer, prev;
} WClick;


/**
 * Initializes the object
 * @param obj WClick object
 * @param parent to inherit from
 * @return obj casted as WClick
 */
O_EXTERN
WClick *WClick_init(oobj obj, oobj parent);


/**
 * Creates a new WClick object
 * @param parent to inherit from
 * @return The new object
 */
O_INLINE
WClick *WClick_new(oobj parent)
{
    OObj_DECL_IMPL_NEW(WClick, parent);
}

//
// virtual implementations:
//


/**
 * Virtual implementation that renders the background color.
 * Calls WObj__v_update afterwards.
 */
O_EXTERN
vec2 WClick__v_update(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn);


//
// object functions:
//

/**
 * @param obj WClick object
 * @return true if down
 */
OObj_DECL_GET(WClick, bool, down)

/**
 * @param obj WClick object
 * @return true if down in this frame
 */
OObj_DECL_GET(WClick, bool, pressed)

/**
 * @param obj WClick object
 * @return true if up in this frame and was down
 */
OObj_DECL_GET(WClick, bool, clicked)

/**
 * @param obj WClick object
 * @return pointer pos relative to this WObj
 */
OObj_DECL_GET(WClick, vec2, pos)

/**
 * @param obj WClick object
 * @return a_pointer (0, 0)
 */
OObj_DECL_GET(WClick, struct a_pointer, pointer)

/**
 * @param obj WClick object
 * @return called if this object is pressed
 */
OObj_DECL_SET(WClick, OObj__event_fn, pressed_event)

/**
 * @param obj WClick object
 * @return called if this object is clicked
 */
OObj_DECL_SET(WClick, OObj__event_fn, clicked_event)

#endif //W_WCLICK_H
