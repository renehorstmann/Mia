#ifndef MP_MPVIEW_H
#define MP_MPVIEW_H

/**
 * @file MPView.h
 *
 * Object
 *
 * Subclass of the AView with some extensions like an menu icon, etc.
 */

#include "a/AView.h"
#include "u/btn.h"

/** object id */
#define MPView_ID AView_ID "MP"


typedef struct {
    AView super;

    // AView's fn's are proxied to add icon and stuff
    AView__fn update, render;

    // copy of given name + NULL
    char name[4];

    // RObjQuad to render the icon
    oobj icon;

    struct u_btn icon_btn;
    struct a_pointer prev;
} MPView;


/**
 * Initializes the object.
 * @param obj MPView object
 * @param parent to inherit from
 * @param opt_setup Virtual setup function. Called once on scene startup, or NULL
 * @param update, render Virtual loop function to handle events, update and render stuff.
 * @param name 3 chars
 * @return obj casted as MPView
 */
O_EXTERN
MPView *MPView_init(oobj obj, oobj parent,
                    OObj__event_fn opt_setup,
                    AView__fn update,
                    AView__fn render,
                    const char *name);

/**
 * Creates a new the MPView object.
 * @param parent to inherit from
 * @param opt_setup Virtual setup function. Called once on scene startup, or NULL
 * @param update, render Virtual loop function to handle events, update and render stuff.
 * @param name 3 chars
 * @return The new object
 */
O_INLINE
MPView *MPView_new(oobj parent,
                   OObj__event_fn opt_setup,
                   AView__fn update,
                   AView__fn render,
                   const char *name)
{
    OObj_DECL_IMPL_NEW(MPView, parent, opt_setup, update, render, name);
}


//
// virtual implementations
//

///**
// * Draws the icon on top of the tex after the loop calls.
// * May update the internal tex and calls the virtual loop function.
// * @param obj AView object
// * @param tex RTex object to render on
// * @param viewport for this view in tex viewport coordinates
// */
//O_EXTERN
//void MPView__v_update(oobj obj, oobj tex, ivec4 viewport);


#endif //MP_MPVIEW_H
