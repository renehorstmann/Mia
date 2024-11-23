#ifndef R_RSHADERQUADDAB_H
#define R_RSHADERQUADDAB_H

/**
 * @file RShaderQuadDab.h
 *
 * Object
 *
 * Draws a Dab. Using hardness as d.x and a pattern texture.
 * Outputs rgba + mask.r (mask as alpha mask)
 */

#include "RShaderQuad.h"

/** object id */
#define RShaderQuadDab_ID RShaderQuad_ID "Dab"

typedef struct
{
    RShaderQuad super;

    // RTex for the pattern, defaults to r_tex_white
    oobj pattern;

    // in pattern pixel coordinates
    vec2 pattern_offset;

    // defaults to 1
    vec2 pattern_scale;

} RShaderQuadDab;


/**
 * Initializes the object.
 * @param obj RShaderQuadDab object
 * @param parent to inherit from
 * @param tex RTex object, NULL safe
 * @param move_tex if true, tex is o_move'd into this object
 * @return obj casted as RShaderQuadDab
 */
O_EXTERN
RShaderQuadDab* RShaderQuadDab_init(oobj obj, oobj parent, oobj tex, bool move_tex);


/**
 * Creates a new RShaderQuadDab object.
 * @param parent to inherit from
 * @param tex RTex object, NULL safe
 * @param move_tex if true, tex is o_move'd into this object
 * @return The new object
 */
O_INLINE
RShaderQuadDab* RShaderQuadDab_new(oobj parent, oobj tex, bool move_tex)

{
    OObj_DECL_IMPL_NEW(RShaderQuadDab, parent, tex, move_tex);
}


//
// virtual implementations:
//

O_EXTERN
void RShaderQuadDab__v_render(oobj obj, oobj program, int num, const struct r_proj* proj);


//
// object functions:
//

/**
 * @param obj RTex object
 * @return pattern for the dab, defaults to r_tex_white
 * @note remember to set clamping to repeat, for small patterns
 */
OObj_DECL_GETSET(RShaderQuadDab, oobj, pattern)

/**
 * @param obj RTex object
 * @return pixel offset of the pattern
 */
OObj_DECL_GETSET(RShaderQuadDab, vec2, pattern_offset)

/**
 * @param obj RTex object
 * @return texture scaling, defaults to 1
 */
OObj_DECL_GETSET(RShaderQuadDab, vec2, pattern_scale)


#endif //R_RSHADERQUADDAB_H
