#ifndef R_RSHADERQUADMERGE_H
#define R_RSHADERQUADMERGE_H

/**
 * @file RShaderQuadMerge.h
 *
 * Object
 *
 * Merges images together. makes only sense for blend=off, which is the default for this shader
 */

#include "RShaderQuad.h"

/** object id */
#define RShaderQuadMerge_ID RShaderQuad_ID "Merge"

typedef struct
{
    RShaderQuad super;

    // RTex previous to blend with
    oobj tex_old;

    // RTex masks
    oobj mask_a, mask_b;

} RShaderQuadMerge;


/**
 * Initializes the object.
 * @param obj RShaderQuadMerge object
 * @param parent to inherit from
 * @param tex RTex object, NULL safe
 * @param move_tex if true, tex is o_move'd into this object
 * @return obj casted as RShaderQuadMerge
 */
O_EXTERN
RShaderQuadMerge* RShaderQuadMerge_init(oobj obj, oobj parent, oobj tex, bool move_tex);


/**
 * Creates a new RShaderQuadMerge object.
 * @param parent to inherit from
 * @param tex RTex object, NULL safe
 * @param move_tex if true, tex is o_move'd into this object
 * @return The new object
 */
O_INLINE
RShaderQuadMerge* RShaderQuadMerge_new(oobj parent, oobj tex, bool move_tex)

{
    OObj_DECL_IMPL_NEW(RShaderQuadMerge, parent, tex, move_tex);
}


//
// virtual implementations:
//

O_EXTERN
void RShaderQuadMerge__v_render(oobj obj, oobj program, int num, const struct r_proj* proj);


//
// object functions:
//

/**
 * @param obj RTex object
 * @return old RTex to blend with
 */
OObj_DECL_GETSET(RShaderQuadMerge, oobj, tex_old)

/**
 * @param obj RTex object
 * @return mask_a RTex
 */
OObj_DECL_GETSET(RShaderQuadMerge, oobj, mask_a)

/**
 * @param obj RTex object
 * @return mask_b RTex
 */
OObj_DECL_GETSET(RShaderQuadMerge, oobj, mask_b)


#endif //R_RSHADERQUADMERGE_H
