#ifndef X_XVIEWTEX_H
#define X_XVIEWTEX_H

/**
 * @file XViewTex.h
 *
 * object.
 *
 * AView which shows a zoom and drag able RTex.
 * Both touch gestures and mouse wheel works.
 * the RTex is located with lb at 0,0 and rt at size, so the pointer callback positions are in pixel coordinations.
 *
 * Subclass of the AView object
 */

#include "m/vec/vec2.h"
#include "r/RTex.h"
#include "a/AView.h"


/** object id */
#define XViewTex_ID AView_ID "XViewTex"

typedef void (*XViewTex_pointer_fn)(oobj view, struct a_pointer pointer);


typedef struct {
    AView AView;

    // RObjQuad for rendering and holding the RTex
    oobj ro;


    // WTheme and WBox for gui
    oobj theme;
    oobj gui;
    oobj exit_btn;
    oobj title_text;

    // internal stuff:
    float move_timeout;
    struct a_pointer pointer0_prev;
    vec2 move_start_pos;
    float move_start_dist;
    bool move;

    // vfuncs
    OObj__event_fn v_done;
    XViewTex_pointer_fn v_opt_pointer;
} XViewTex;


/**
 * Initializes the object.
 * Creates an AView that renders a RTex with lb at 0, 0 and rt at size.
 * So the pointer position is on the pixel coordinate.
 * @param obj XViewTex object
 * @param parent to inherit from
 * @param tex to show
 * @param move_tex if true, tex is o_move'd into the internal RObjQuad object ro
 * @param done called if exit button is clicked
 * @param opt_title if given, rendered below the RTex
 * @param opt_pointer if given, called for pointer events
 * @return obj casted as XViewTex
 */
O_EXTERN
XViewTex *XViewTex_init(oobj obj, oobj parent, oobj tex, bool move_tex, OObj__event_fn done,
                        const char *opt_title, XViewTex_pointer_fn opt_pointer);

/**
 * Creates a new the OArray object
 * Creates an XViewTex that renders a RTex with lb at 0, 0 and rt at size.
 * So the pointer position is on the pixel coordinate.
 * @param parent to inherit from
 * @param tex to show
 * @param move_tex if true, tex is o_move'd into the internal RObjQuad object ro
 * @param done called if exit button is clicked
 * @param opt_title if given, rendered below the RTex
 * @param opt_pointer if given, called for pointer events
 * @return The new object
 */
O_INLINE
XViewTex *XViewTex_new(oobj parent, oobj tex, bool move_tex, OObj__event_fn done,
                       const char *opt_title, XViewTex_pointer_fn opt_pointer)
{
    OObj_DECL_IMPL_NEW(XViewTex, parent, tex, move_tex, done, opt_title, opt_pointer);
}


//
// virtual implementations
//

O_EXTERN
void XViewTex__v_setup(oobj view);

O_EXTERN
void XViewTex__v_update(oobj view, oobj tex, float dt);

O_EXTERN
void XViewTex__v_render(oobj view, oobj tex, float dt);


//
// object functions:
//

/**
 * @param obj XViewTex object
 * @return RObjQuad object, used for rendering
 */
OObj_DECL_GET(XViewTex, oobj, ro)

/**
 * @param obj XViewTex object
 * @return rendered RTex object
 */
O_EXTERN
RTex *XViewTex_tex(oobj obj);

/**
 * @param obj XViewTex object
 * @return rendered RTex object
 */
O_EXTERN
RTex *XViewTex_tex_set(oobj obj, oobj tex, bool del_old);

/**
 * @param obj XViewTex object
 * @return WTheme for gui
 */
OObj_DECL_GET(XViewTex, oobj, theme)


/**
 * @param obj XViewTex object
 * @return WBox gui, add w tools like WNum here
 */
OObj_DECL_GET(XViewTex, oobj, gui)

/**
 * @param obj XViewTex object
 * @return pos with center origin of the RTex, instead of lb
 */
O_INLINE
vec2 XViewTex_lb_to_center(oobj obj, vec2 pos)
{
    return vec2_sub_v(pos, vec2_div(RTex_size(XViewTex_tex(obj)), 2));
}
#endif //X_XVIEWTEX_H
