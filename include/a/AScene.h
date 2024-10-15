#ifndef A_ASCENE_H
#define A_ASCENE_H

/**
 * @file AScene.h
 *
 * Object
 *
 * Manages one scene on the app scene stack.
 * Creating an AScene automatically registers it in the a_app module.
 *
 * Each scene has its own RConfig internally, so changes to that only effect the current scene.
 */


#include "o/OObj.h"
#include "m/types/int.h"
#include "r/quad.h"
#include "AView.h"

/** object id */
#define AScene_ID OObj_ID "AScene"


typedef struct {
    OObj super;

    AView *view;

    bool opaque;

    // bounds left bottom width height in backbuffer viewport coordinates (r_back_size())
    ivec4 viewport;
    bool full_x_auto, full_y_auto;
    bool full_even_size;

    // optional events
    OObj__event_fn on_pause_event;
    OObj__event_fn on_resume_event;

} AScene;


/**
 * Initializes the object.
 * @param obj AScene object
 * @param parent to inherit from
 * @param view AView object for this scene, see AScene_view_set
 *             (NULL ok, but needs to be set with AScene_view_set immediately)
 * @param move_view if true, view is o_move_res'd into this object
 * @return obj casted as AScene
 * @note inits full_* options to true
 */
O_EXTERN
AScene *AScene_init(oobj obj, oobj parent, oobj view, bool move_view);

/**
 * Creates a new the AScene object
 * @param view AView object for this scene, see AScene_view_set
 *             (NULL ok, but needs to be set with AScene_view_set immediately)
 * @param move_view if true, view is o_move_res'd into this object
 * @return The new object
 */
O_INLINE
AScene *AScene_new(oobj parent, oobj view, bool move_view)
{
    OObj_DECL_IMPL_NEW(AScene, parent, view, move_view);
}

//
// object functions:
//

/**
 * @param obj AScene object
 * @return true (default) if this scene is opaque (false: transparent)
 */
OObj_DECL_GETSET(AScene, bool, opaque)

/**
 * @param obj AScene object
 * @return AView object of this scene
 */
OObj_DECL_GET(AScene, AView *, view)

/**
 * @param obj AScene object
 * @param view AView object for this scene.
 * @param del_old if true, the olf view is o_del'ed
 */
O_EXTERN
AView *AScene_view_set(oobj obj, oobj view, bool del_old);

/**
 * @param obj AScene object
 * @return bounds left bottom width height
 */
OObj_DECL_GETSET(AScene, ivec4, viewport)

/**
 * @param obj AScene object
 * @return if true (default), the bounds left and right are updated to the full back buffer size
 */
OObj_DECL_GETSET(AScene, bool, full_x_auto)

/**
 * @param obj AScene object
 * @return if true (default), the bounds bottom and top are updated to the full back buffer size
 */
OObj_DECL_GETSET(AScene, bool, full_y_auto)

/**
 * @param obj AScene object
 * @return if true (default), the size may be reduced to be even (unit size)
 */
OObj_DECL_GETSET(AScene, bool, full_even_size)

/**
 * @param obj AScene object
 * @return called if the app goes into pause mode (optional)
 */
OObj_DECL_SET(AScene, OObj__event_fn , on_pause_event)

/**
 * @param obj AScene object
 * @return called if the app goes finished pause mode and resumes (optional)
 */
OObj_DECL_SET(AScene, OObj__event_fn , on_resume_event)


/**
 * Returns the stack position index from a_app.
 *      Calls simply a_app_scene_object_index(obj)
 * @param obj AScene object
 * @return the stack position from a_app [0 : a_app_scenes_num())
 */
O_INLINE
int AScene_index(oobj obj)
{
    OObj_assert(obj, AScene);

    O_EXTERN
    int a_app_scene_get_index(oobj scene);
    return a_app_scene_get_index(obj);
}



#endif //A_ASCENE_H
