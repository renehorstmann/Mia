#ifndef R_ROBJ_H
#define R_ROBJ_H

/**
 * @file RObj.h
 *
 * Object
 *
 * Render object abstract base class.
 */


#include "o/OObj.h"
#include "r/common.h"

/** object id */
#define RObj_ID OObj_ID "RObj"

/**
 * Virtual render function.
 * Renders the object to the bound frame buffer
 * @param obj RObj object
 * @param tex RTex to render in, or NULL to use the back buffer
 * @param proj the camera projection to use (vp)
 */
typedef void (*RObj__render_fn)(oobj obj, oobj tex, const struct r_proj *proj);


typedef struct {
    OObj super;

    //
    // vfuncs
    //
    OObj__event_fn v_update;
    RObj__render_fn v_render;
} RObj;

/**
 * Initializes the object, used by sub classes, so no new...
 * @param obj RObj object
 * @param parent to inherit from
 * @param update_fn virtual update function
 * @param render_fn virtual render function
 * @return obj casted as RObj
 */
O_EXTERN
RObj *RObj_init(oobj obj, oobj parent, OObj__event_fn update_fn, RObj__render_fn render_fn);

//
// object functions
//

/**
 * Calls the virtual update function.
 * Updates the internals like streaming data to the GPU
 * @param obj RObj object
 */
O_INLINE
void RObj_update(oobj obj)
{
    OObj_assert(obj, RObj);
    RObj *self = obj;
    self->v_update(self);
}

/**
 * Calls the virtual render function.
 * Extended version.
 * Renders the object to the bound frame buffer.
 * Uses all shaders in the shader_pipeline one by one.
 * @param obj RObj object
 * @param tex RTex to render in, or NULL to use the back buffer
 * @param opt_proj the camera projection to use (vp), if NULL: RTex_proj(tex) is used instead
 * @param update if true: calls RObj_update first
 */
O_EXTERN
void RObj_render_ex(oobj obj, oobj tex, const struct r_proj *opt_proj, bool update);

/**
 * Calls the virtual render function.
 * Simpler default version.
 * Renders the object to the bound frame buffer
 * @param obj RObj object
 * @param tex RTex to render in, or NULL to use the back buffer
 * @note just calls RObj_render_ex(obj, tex, NULL, true)
 */
O_INLINE
void RObj_render(oobj obj, oobj tex)
{
    RObj_render_ex(obj, tex, NULL, true);
}


#endif //R_ROBJ_H
