#ifndef R_ROBJQUAD_H
#define R_ROBJQUAD_H

/**
 * @file RObjQuad.h
 *
 * Object
 *
 * Renders a batch of quads.
 *
 * Operators:
 * o_num -> RObjQuad_num
 * o_at -> RObjQuad_at
 */

#include "o/OArray.h"
#include "RObj.h"
#include "RTex.h"
#include "quad.h"

/** object id */
#define RObjQuad_ID RObj_ID "Quad"

typedef struct {
    RObj super;

    // OArray of "struct r_quad"
    OArray *quads;

    // can be set lower num, o_min(num, num_rendered) is used to render the shaders
    // init with oi32_MAX
    int num_rendered;
    
    // RBufferQuad
    oobj buffer;

    // may be moved into this object
    RTex *tex;
    
    // OArray of RShaderQuad
    oobj shader_pipeline;

} RObjQuad;


/**
 * Initializes the object
 * @param obj RObjQuad object
 * @param parent to inherit from
 * @param num of quads
 * @param tex RTex object, NULL safe
 * @param move_tex if true, tex is o_move'd into this object
 * @return obj casted as RObjQuad
 */
O_EXTERN
RObjQuad *RObjQuad_init(oobj obj, oobj parent, int num, oobj tex, bool move_tex);


/**
 * Creates a new RObjQuad object
 * @param parent to inherit from
 * @param num of quads
 * @param tex RTex object, NULL safe;
 * @param move_tex if true, tex is o_move'd into this object
 * @return The new object
 */
O_INLINE
RObjQuad *RObjQuad_new(oobj parent, int num, oobj tex, bool move_tex)
{
    OObj_DECL_IMPL_NEW(RObjQuad, parent, num, tex, move_tex);
}

/**
 * Initializes the object
 * @param obj RObjQuad object
 * @param parent to inherit from
 * @param num of quads
 * @param tex RTex object, NULL safe
 * @param move_tex if true, tex is o_move'd into this object
 * @param shader RShaderQuad object
 * @param move_shader if true, shader is o_move'd into this object
 * @return obj casted as RObjQuad
 */
O_EXTERN
RObjQuad *RObjQuad_init_shader(oobj obj, oobj parent, int num, oobj tex, bool move_tex, oobj shader, bool move_shader);


/**
 * Creates a new RObjQuad object
 * @param parent to inherit from
 * @param num of quads
 * @param tex RTex object, NULL safe;
 * @param move_tex if true, tex is o_move'd into this object
 * @param shader RShaderQuad object
 * @param move_shader if true, shader is o_move'd into this object
 * @return The new object
 */
O_INLINE
RObjQuad *RObjQuad_new_shader(oobj parent, int num, oobj tex, bool move_tex, oobj shader, bool move_shader)
{
    OObj_DECL_IMPL_NEW_SPECIAL(RObjQuad, shader, parent, num, tex, move_tex, shader, move_shader);
}


/**
 * Creates a new RObjQuad object.
 * s -> rgba.
 * @param parent to inherit from
 * @param num of quads
 * @param tex RTex object, NULL safe;
 * @param move_tex if true, tex is o_move'd into this object
 * @return The new object
 */
O_EXTERN
RObjQuad *RObjQuad_new_color(oobj parent, int num, oobj tex, bool move_tex);




//
// virtual implementations:
//

/**
 * virtual operator function
 * @param obj RObjQuad object
 * @return number of r_quad's
 */
O_EXTERN
osize RObjQuad__v_op_num(oobj obj);

/**
 * virtual operator function
 * @param obj RObjQuad object
 * @return r_quad at given idx
 */
O_EXTERN
void *RObjQuad__v_op_at(oobj obj, osize idx);


O_EXTERN
void RObjQuad__v_update(oobj obj);

O_EXTERN
void RObjQuad__v_render(oobj obj, oobj tex, const struct r_proj *proj);


//
// object functions:
//

/**
 * @param obj RObjQuad object
 * @return internal OArray of struct r_quad
 */
OObj_DECL_GET(RObjQuad, OArray *, quads)

/**
 * @param obj RObjQuad object
 * @param quads The new OArray of struct r_quad
 * @param del_old if true, o_del old used OArray
 * @return == tex
 */
O_INLINE
OArray *RObjQuad_quads_set(oobj obj, oobj quads, bool del_old)
{
    OObj_assert(obj, RObjQuad);
    RObjQuad *self = obj;
    if(del_old) {
        o_del(self->quads);
    }
    self->quads = quads;
    return self->quads;
}

/**
 * @param obj RObjQuad object
 * @return number of r_quad's
 */
O_INLINE
osize RObjQuad_num(oobj obj)
{
    OObj_assert(obj, RObjQuad);
    RObjQuad *self = obj;
    return OArray_num(self->quads);
}

/**
 * @param obj RObjQuad object
 * @return number of rendered r_quad's (o_min(num, num_rendered))
 */
OObj_DECL_GETSET(RObjQuad, int, num_rendered)

/**
 * @param obj RObjQuad object
 * @return r_quad at given index
 * @note asserts bounds
 */
O_INLINE
struct r_quad *RObjQuad_at(oobj obj, osize idx)
{
    OObj_assert(obj, RObjQuad);
    RObjQuad *self = obj;
    return OArray_at(self->quads, idx, struct r_quad);
}

/**
 * @param obj RObjQuad object
 * @return The used RTex
 */
OObj_DECL_GET(RObjQuad, RTex *, tex)

/**
 * @param obj RObjQuad object
 * @param tex The new RTex
 * @param del_old if true, o_del old used RTex
 * @return == tex
 */
O_INLINE
RTex *RObjQuad_tex_set(oobj obj, oobj tex, bool del_old)
{
    OObj_assert(obj, RObjQuad);
    RObjQuad *self = obj;
    if(del_old) {
        o_del(self->tex);
    }
    self->tex = tex;
    return self->tex;
}

/**
 * @param obj RObjQuad object
 * @return OArray of RShaderQuad's, which are rendered one be the other
 */
OObj_DECL_GET(RObjQuad, oobj, shader_pipeline)

/**
 * @param obj RObjQuad object
 * @param pipeline_idx index in the shader pipeline (pass 0 for the default / first shader)
 * @return RShaderQuad at the given index of the pipeline
 */
O_EXTERN
oobj RObjQuad_shader(oobj obj, int pipeline_idx);

/**
 * @param obj RObjQuad object
 * @param pipeline_idx index in the shader pipeline (pass 0 for the default / first shader).
 *                     if index <0 or >= pipeline_size, the shader is pushed back
 * @param shader RShaderQuad object to set
 * @param del_old o_del's the old shader, if true
 * @return created pipeline_idx
 */
O_EXTERN
int RObjQuad_shader_set(oobj obj, int pipeline_idx, oobj shader, bool del_old);

/**
 * Adds a RShaderQuadBloom at the end of the shader pipeline.
 * Kernel set as an gauss 9x9
 * @param obj RObjQuad object
 * @param kernel RTex object for the kernel (.r used), NULL safe
 * @param color to render with the kernel
 * @return created RShaderQuadBloom object
 * @note mask set to vec4_(1, 1, 1, 0.66)
 *       .a is threshold (so 0.66)
 */
O_EXTERN
oobj RObjQuad_shader_add_bloom(oobj obj, oobj kernel,vec4 color);



/**
 * Adds a RShaderQuadContour at the front of the shader pipeline.
 * @param obj RObjQuad object
 * @param kernel RTex object for the kernel (.r used), NULL safe
 * @param color to render with the kernel
 * @return created RShaderQuadContour object
 */
O_EXTERN
oobj RObjQuad_shader_add_contour(oobj obj, oobj kernel, vec4 color);




#endif //R_ROBJQUAD_H
