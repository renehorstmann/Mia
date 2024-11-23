#ifndef R_ROBJRECT_H
#define R_ROBJRECT_H

/**
 * @file RObjRect.h
 *
 * Object
 *
 * Renders a batch of rects.
 *
 * Operators:
 * o_num -> RObjRect_num
 * o_at -> RObjRect_at
 */

#include "o/OArray.h"
#include "RObj.h"
#include "RTex.h"
#include "rect.h"

/** object id */
#define RObjRect_ID RObj_ID "Rect"

typedef struct {
    RObj super;

    // OArray of "struct r_rect"
    OArray *rects;

    // can be set lower num, o_min(num, num_rendered) is used to render the shaders
    // init with oi32_MAX
    int num_rendered;
    
    // RBufferRect
    oobj buffer;

    // may be moved into this object
    RTex *tex;
    
    // OArray of RShaderRect
    oobj shader_pipeline;

} RObjRect;


/**
 * Initializes the object
 * @param obj RObjRect object
 * @param parent to inherit from
 * @param num of rects
 * @param tex RTex object, NULL safe
 * @param move_tex if true, tex is o_move'd into this object
 * @return obj casted as RObjRect
 */
O_EXTERN
RObjRect *RObjRect_init(oobj obj, oobj parent, int num, oobj tex, bool move_tex);


/**
 * Creates a new RObjRect object
 * @param parent to inherit from
 * @param num of rects
 * @param tex RTex object, NULL safe;
 * @param move_tex if true, tex is o_move'd into this object
 * @return The new object
 */
O_INLINE
RObjRect *RObjRect_new(oobj parent, int num, oobj tex, bool move_tex)
{
    OObj_DECL_IMPL_NEW(RObjRect, parent, num, tex, move_tex);
}


/**
 * Initializes the object
 * @param obj RObjRect object
 * @param parent to inherit from
 * @param num of rects
 * @param tex RTex object, NULL safe
 * @param move_tex if true, tex is o_move'd into this object
 * @param shader RShaderRect object
 * @param move_shader if true, shader is o_move'd into this object
 * @return obj casted as RObjRect
 */
O_EXTERN
RObjRect *RObjRect_init_shader(oobj obj, oobj parent, int num, oobj tex, bool move_tex, oobj shader, bool move_shader);


/**
 * Creates a new RObjRect object
 * @param parent to inherit from
 * @param num of rects
 * @param tex RTex object, NULL safe;
 * @param move_tex if true, tex is o_move'd into this object
 * @param shader RShaderRect object
 * @param move_shader if true, shader is o_move'd into this object
 * @return The new object
 */
O_INLINE
RObjRect *RObjRect_new_shader(oobj parent, int num, oobj tex, bool move_tex, oobj shader, bool move_shader)
{
    OObj_DECL_IMPL_NEW_SPECIAL(RObjRect, shader, parent, num, tex, move_tex, shader, move_shader);
}


/**
 * Creates a new RObjRect object.
 * a -> rgba.
 * b -> hsva.
 * @param parent to inherit from
 * @param num of rects
 * @param tex RTex object, NULL safe;
 * @param move_tex if true, tex is o_move'd into this object
 * @return The new object
 */
O_EXTERN
RObjRect *RObjRect_new_color(oobj parent, int num, oobj tex, bool move_tex);




//
// virtual implementations:
//

/**
 * virtual operator function
 * @param obj RObjRect object
 * @return number of r_rect's
 */
O_EXTERN
osize RObjRect__v_op_num(oobj obj);

/**
 * virtual operator function
 * @param obj RObjRect object
 * @return r_rect at given idx
 */
O_EXTERN
void *RObjRect__v_op_at(oobj obj, osize idx);


O_EXTERN
void RObjRect__v_update(oobj obj);

O_EXTERN
void RObjRect__v_render(oobj obj, oobj tex, const struct r_proj *proj);


//
// object functions:
//

/**
 * @param obj RObjRect object
 * @return internal OArray of struct r_rect
 */
OObj_DECL_GET(RObjRect, OArray *, rects)

/**
 * @param obj RObjRect object
 * @param rects The new OArray of struct r_rect
 * @param del_old if true, o_del old used OArray
 * @return == tex
 */
O_INLINE
OArray *RObjRect_rects_set(oobj obj, oobj rects, bool del_old)
{
    OObj_assert(obj, RObjRect);
    RObjRect *self = obj;
    if(del_old) {
        o_del(self->rects);
    }
    self->rects = rects;
    return self->rects;
}

/**
 * @param obj RObjRect object
 * @return number of r_rect's
 */
O_INLINE
osize RObjRect_num(oobj obj)
{
    OObj_assert(obj, RObjRect);
    RObjRect *self = obj;
    return OArray_num(self->rects);
}

/**
 * @param obj RObjRect object
 * @return number of rendered r_rect's (o_min(num, num_rendered))
 */
OObj_DECL_GETSET(RObjRect, int, num_rendered)

/**
 * @param obj RObjRect object
 * @return r_rect at given index
 * @note asserts bounds
 */
O_INLINE
struct r_rect *RObjRect_at(oobj obj, osize idx)
{
    OObj_assert(obj, RObjRect);
    RObjRect *self = obj;
    return OArray_at(self->rects, idx, struct r_rect);
}

/**
 * @param obj RObjRect object
 * @return The used RTex
 */
OObj_DECL_GET(RObjRect, RTex *, tex)

/**
 * @param obj RObjRect object
 * @param tex The new RTex
 * @param del_old if true, o_del old used RTex
 * @return == tex
 */
O_INLINE
RTex *RObjRect_tex_set(oobj obj, oobj tex, bool del_old)
{
    OObj_assert(obj, RObjRect);
    RObjRect *self = obj;
    if(del_old) {
        o_del(self->tex);
    }
    self->tex = tex;
    return self->tex;
}

/**
 * @param obj RObjRect object
 * @return OArray of RShaderRect's, which are rendered one be the other
 */
OObj_DECL_GET(RObjRect, oobj, shader_pipeline)

/**
 * @param obj RObjRect object
 * @param pipeline_idx index in the shader pipeline (pass 0 for the default / first shader)
 * @return RShaderRect at the given index of the pipeline
 */
O_EXTERN
oobj RObjRect_shader(oobj obj, int pipeline_idx);

/**
 * @param obj RObjRect object
 * @param pipeline_idx index in the shader pipeline (pass 0 for the default / first shader).
 *                     if index <0 or >= pipeline_size, the shader is pushed back
 * @param shader RShaderRect object to set
 * @param del_old o_del's the old shader, if true
 * @return created pipeline_idx
 */
O_EXTERN
int RObjRect_shader_set(oobj obj, int pipeline_idx, oobj shader, bool del_old);





#endif //R_ROBJRECT_H
