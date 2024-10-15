#ifndef R_RSHADER_H
#define R_RSHADER_H

/**
 * @file RShader.h
 *
 * Object
 *
 * A shader is an abstract class that sets uniforms, binds vertex buffers and renders a program
 */

#include "o/OObj.h"
#include "r/common.h"


/** object id */
#define RShader_ID OObj_ID "RShader"

/**
 * Virtual render function.
 * Renders the shader object to the bound frame buffer
 * @param obj RShader object
 * @param program RProgram, already used
 * @param proj the camera projection to use (vp)
 */
typedef void (*RShader__render_fn)(oobj obj, oobj program, int num, const struct r_proj* proj);

typedef struct
{
    OObj super;

    // RProgram
    oobj program;

    bool blend;

    // v funcs
    RShader__render_fn v_render;
} RShader;


/**
 * Initializes the object
 * @param obj RShader object
 * @param parent to inherit from
 * @param program RProgram object to use, NULL safe (glsl shader program)
 * @param render_fn virtual render function
 * @return obj casted as RShader
 */
O_EXTERN
RShader* RShader_init(oobj obj, oobj parent, oobj program, RShader__render_fn render_fn);

/**
 * Creates a new the RShader object
 * @param parent to inherit from
 * @param program RProgram object to use, NULL safe (glsl shader program)
 * @param render_fn virtual render function
 * @return The new object
 */
O_INLINE
RShader* RShader_new(oobj parent, oobj program, RShader__render_fn render_fn)
{
    OObj_DECL_IMPL_NEW(RShader, parent, program, render_fn);
}


//
// object functions:
//

/**
 * Calls the virtual render function.
 * @param obj RShader object
 * @param buffer RBuffer object
 * @param tex RTex to render in, or NULL to use the back buffer
 * @param opt_num_rendered if <=0: RBuffer_num is used, else o_min(num_rendered, 1, RBuffer_num(buffer))
 * @param opt_proj the camera projection to use (vp), if NULL RTex_proj(tex) is used
 */
O_EXTERN
void RShader_render_ex(oobj obj, oobj buffer, oobj tex, int opt_num_rendered, const struct r_proj* opt_proj);

/**
 * Calls the virtual render function.
 * Simpler default version.
 * @param obj RShader object
 * @param buffer RBuffer object
 * @param tex RTex to render in, or NULL to use the back buffer
 * @note just calls RShader_render_ex(obj, buffer, tex, 0, NULL)
 */
O_INLINE
void RShader_render(oobj obj, oobj buffer, oobj tex)
{
    RShader_render_ex(obj, buffer, tex, 0, NULL);
}

/**
 * @param obj RShader object
 * @return The RProgram object
 */
OObj_DECL_GETSET(RShader, oobj, program)

/**
 * @param obj RShader object
 * @return true if blending is active
 */
OObj_DECL_GETSET(RShader, bool, blend)


#endif //R_RSHADER_H
