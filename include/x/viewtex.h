#ifndef X_VIEWTEX_H
#define X_VIEWTEX_H

/**
 * @file viewtex.h
 *
 * Create a scene with a XViewTex as AView
 */

#include "XViewTex.h"



/**
 * Creates a full opaque scene, which shows XViewTex
 * @param parent to inherit from
 * @param tex to show
 * @param move_tex if true, tex is o_move'd into the views ro
 * @param opt_title if given, rendered below the RTex
 * @param opt_pointer if given, called for pointer events
 * @return AScene object
 */
O_EXTERN
oobj x_viewtex_scene(oobj parent, oobj tex, bool move_tex,
                           const char *opt_title, XViewTex_pointer_fn opt_pointer);



#endif //X_VIEWTEX_H
