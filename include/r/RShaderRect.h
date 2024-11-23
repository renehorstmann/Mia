#ifndef R_RSHADERRECT_H
#define R_RSHADERRECT_H

/**
 * @file RShaderRect.h
 *
 * Object
 *
 * Shader to render a batch of Rects, see RBufferRect or RObjRect
 */

#include "r/RShader.h"
#include "r/RTex.h"


/** object id */
#define RShaderRect_ID RShader_ID "Rect"

typedef struct
{
    RShader super;

    // u_s|t
    // init as s=vec4_(1), t=vec4_(0)
    vec4 s, t;

    // may be moved into this object
    RTex* tex;
} RShaderRect;


/**
 * Initializes the object
 * @param obj RShaderRect object
 * @param parent to inherit from
 * @param tex RTex object, NULL safe
 * @param move_tex if true, tex is o_move'd into this object
 * @return obj casted as RShaderRect
 */
O_EXTERN
RShaderRect* RShaderRect_init(oobj obj, oobj parent, oobj tex, bool move_tex);

/**
 * Creates a new RShaderRect object
 * @param parent to inherit from
 * @param tex RTex object, NULL safe
 * @param move_tex if true, tex is o_move'd into this object
 * @return The new object
 */
O_INLINE
RShaderRect* RShaderRect_new(oobj parent, oobj tex, bool move_tex)
{
    OObj_DECL_IMPL_NEW(RShaderRect, parent, tex, move_tex);
}

/**
 * Creates a new RShaderRect object.
 * a -> rgba.
 * b -> hsva.
 * @param parent to inherit from
 * @param tex RTex object, NULL safe
 * @param move_tex if true, tex is o_move'd into this object
 * @return The new object
 */
O_EXTERN
RShaderRect* RShaderRect_new_color(oobj parent, oobj tex, bool move_tex);

//
// virtual implementations:
//

void RShaderRect__v_render(oobj obj, oobj program, int num, const struct r_proj* proj);


//
// object functions:
//

/**
 * @param obj RShaderRect object
 * @return reference s uniform
 */
O_INLINE
vec4* RShaderRect_s(oobj obj)
{
    OObj_assert(obj, RShaderRect);
    RShaderRect* self = obj;
    return &self->s;
}

/**
 * @param obj RShaderRect object
 * @return reference t uniform
 */
O_INLINE
vec4* RShaderRect_t(oobj obj)
{
    OObj_assert(obj, RShaderRect);
    RShaderRect* self = obj;
    return &self->t;
}

/**
 * @param obj RShaderRect object
 * @return The used RTex
 */
OObj_DECL_GET(RShaderRect, RTex *, tex)

/**
 * @param obj RShaderRect object
 * @param tex The new RTex
 * @param del_old if true, o_del old used RTex
 * @return == tex
 */
O_INLINE
RTex* RShaderRect_tex_set(oobj obj, oobj tex, bool del_old)
{
    OObj_assert(obj, RShaderRect);
    RShaderRect* self = obj;
    if (del_old) {
        o_del(self->tex);
    }
    self->tex = tex;
    return self->tex;
}


#endif //R_RSHADERRECT_H
