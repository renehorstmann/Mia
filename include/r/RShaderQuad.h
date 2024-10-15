#ifndef R_RSHADERQUAD_H
#define R_RSHADERQUAD_H

/**
 * @file RShaderQuad.h
 *
 * Object
 *
 * Shader to render a batch of Quads, see RBufferQuad or RObjQuad
 */

#include "r/RShader.h"
#include "r/RTex.h"


/** object id */
#define RShaderQuad_ID RShader_ID "Quad"

typedef struct
{
    RShader super;

    // u_s|t|u|v
    // init as s=vec4_(1), t=u=v=vec4_(0)
    mat4 stuv;

    // may be moved into this object
    RTex* tex;
} RShaderQuad;


/**
 * Initializes the object
 * @param obj RShaderQuad object
 * @param parent to inherit from
 * @param tex RTex object, NULL safe
 * @param move_tex if true, tex is o_move'd into this object
 * @return obj casted as RShaderQuad
 */
O_EXTERN
RShaderQuad* RShaderQuad_init(oobj obj, oobj parent, oobj tex, bool move_tex);

/**
 * Creates a new RShaderQuad object
 * @param parent to inherit from
 * @param tex RTex object, NULL safe
 * @param move_tex if true, tex is o_move'd into this object
 * @return The new object
 */
O_INLINE
RShaderQuad* RShaderQuad_new(oobj parent, oobj tex, bool move_tex)
{
    OObj_DECL_IMPL_NEW(RShaderQuad, parent, tex, move_tex);
}

/**
 * Creates a new RShaderQuad object.
 * s -> rgba.
 * @param parent to inherit from
 * @param tex RTex object, NULL safe
 * @param move_tex if true, tex is o_move'd into this object
 * @return The new object
 */
O_EXTERN
RShaderQuad* RShaderQuad_new_color(oobj parent, oobj tex, bool move_tex);

//
// virtual implementations:
//

void RShaderQuad__v_render(oobj obj, oobj program, int num, const struct r_proj* proj);


//
// object functions:
//

/**
 * @param obj RShaderQuad object
 * @return reference stuv uniforms
 */
O_INLINE
mat4* RShaderQuad_stuv(oobj obj)
{
    OObj_assert(obj, RShaderQuad);
    RShaderQuad* self = obj;
    return &self->stuv;
}

/**
 * @param obj RShaderQuad object
 * @return The used RTex
 */
OObj_DECL_GET(RShaderQuad, RTex *, tex)

/**
 * @param obj RShaderQuad object
 * @param tex The new RTex
 * @param del_old if true, o_del old used RTex
 * @return == tex
 */
O_INLINE
RTex* RShaderQuad_tex_set(oobj obj, oobj tex, bool del_old)
{
    OObj_assert(obj, RShaderQuad);
    RShaderQuad* self = obj;
    if (del_old) {
        o_del(self->tex);
    }
    self->tex = tex;
    return self->tex;
}


#endif //R_RSHADERQUAD_H
