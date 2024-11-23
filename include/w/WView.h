#ifndef W_WVIEW_H
#define W_WVIEW_H

/**
 * @file WView.h
 *
 * Object (derives WObj)
 *
 * Widget that bundles an AView.
 *
 * >DOES NOT< call update or render on the AView automatically!
 * Either use WView_render(..) or call AView_update and AView_render yourself (WView_viewport...)
 */

#include "WObj.h"
#include "a/AView.h"

/** object id */
#define WView_ID WObj_ID "View"

typedef struct {
    WObj super;

    bool hidden;
    AView *view;

    vec4 lbwh;
} WView;


/**
 * Initializes the object
 * @param obj WView object
 * @param parent to inherit from
 * @param view AView object to bundle in
 * @param move_view if true, view is o_move'd into this object
 * @return obj casted as WView
 */
O_EXTERN
WView *WView_init(oobj obj, oobj parent, oobj view, bool move_view);


/**
 * Creates a new WView object
 * @param parent to inherit from
 * @param view AView object to bundle in
 * @param move_view if true, view is o_move'd into this object
 * @return The new object
 */
O_INLINE
WView *WView_new(oobj parent, oobj view, bool move_view)
{
    OObj_DECL_IMPL_NEW(WView, parent, view, move_view);
}

//
// virtual implementations:
//


/**
 * Virtual implementation that renders the background color.
 * Calls WObj__v_update afterwards.
 */
O_EXTERN
vec2 WView__v_update(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn);


//
// object functions:
//


/**
 * @param obj WView object
 * @return hidden flag, set by wobj's update, reset by WView_render (which only renders if set)
 */
OObj_DECL_GETSET(WView, bool, hidden)

/**
 * @param obj WView object
 * @return in projection space lbwh (!=viewport...)
 */
OObj_DECL_GETSET(WView, vec4, lbwh)

/**
 * @param obj WView object
 * @return AView object to bundle
 */
OObj_DECL_GET(WView, AView *, view)

/**
 * @param obj WView object
 * @param view AView object to bundle
 * @param del_old if true, the olf view is o_del'ed
 */
O_EXTERN
AView *WView_view_set(oobj obj, oobj view, bool del_old);

/**
 * Calls update and render on the bundled AView
 * @param obj WView object
 * @param tex RTex used to determine the viewport (needs the projection and RTex_viewport ...)
 * @return the RTex style viewport lbwh
 */
O_EXTERN
ivec4 WView_viewport(oobj obj, oobj tex);

/**
 * Calls update and render on the bundled AView
 * @param obj WView object
 * @param tex RTex to render onto
 */
O_EXTERN
void WView_render(oobj obj, oobj tex);

#endif //W_WVIEW_H
