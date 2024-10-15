#ifndef R_RSHADERQUADCHANNELMERGE_H
#define R_RSHADERQUADCHANNELMERGE_H

/**
 * @file RShaderQuadChannelmerge.h
 *
 * Object
 *
 * merges rgba from splitted textures (uses .r on all to merge).
 * default tex is red
 */

#include "RShaderQuad.h"

/** object id */
#define RShaderQuadChannelmerge_ID RShaderQuad_ID "Channelmerge"

typedef struct
{
    RShaderQuad super;

    // RTex channels
    oobj g, b, a;

} RShaderQuadChannelmerge;


/**
 * Initializes the object.
 * @param obj RShaderQuadChannelmerge object
 * @param parent to inherit from
 * @param r, g, b, a RTex object, NULL safe
 * @param move_texs if true, rgba is o_move'd into this object
 * @return obj casted as RShaderQuadChannelmerge
 */
O_EXTERN
RShaderQuadChannelmerge* RShaderQuadChannelmerge_init(oobj obj, oobj parent,
                                                      oobj r, oobj g, oobj b, oobj a, bool move_texs);


/**
 * Creates a new RShaderQuadChannelmerge object.
 * @param parent to inherit from
 * @param r, g, b, a RTex object, NULL safe
 * @param move_texs if true, rgba is o_move'd into this object
 * @return The new object
 */
O_INLINE
RShaderQuadChannelmerge* RShaderQuadChannelmerge_new(oobj parent,
                                                     oobj r, oobj g, oobj b, oobj a, bool move_texs)

{
    OObj_DECL_IMPL_NEW(RShaderQuadChannelmerge, parent, r, g, b, a, move_texs);
}


//
// virtual implementations:
//

O_EXTERN
void RShaderQuadChannelmerge__v_render(oobj obj, oobj program, int num, const struct r_proj* proj);


//
// object functions:
//

/**
 * @param obj RTex object
 * @return red RTex
 */
O_INLINE
oobj RShaderQuadChannelmerge_r(oobj obj)
{
    return RShaderQuad_tex(obj);
}
/**
 * @param obj RTex object
 * @return red RTex
 */
O_INLINE
oobj RShaderQuadChannelmerge_r_set(oobj obj, oobj r)
{
    return RShaderQuad_tex_set(obj, r, false);
}

/**
 * @param obj RTex object
 * @return green RTex
 */
OObj_DECL_GETSET(RShaderQuadChannelmerge, oobj, g)

/**
 * @param obj RTex object
 * @return blue RTex
 */
OObj_DECL_GETSET(RShaderQuadChannelmerge, oobj, b)

/**
 * @param obj RTex object
 * @return alpha RTex
 */
OObj_DECL_GETSET(RShaderQuadChannelmerge, oobj, a)


#endif //R_RSHADERQUADCHANNELMERGE_H
