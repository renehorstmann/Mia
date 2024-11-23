#include "x/viewtex.h"
#include "a/AScene.h"
#include "a/ADefer.h"



#define VIEWTEX_INJECT "x_viewtex_scene"


O_STATIC
void viewtex_scene_done(oobj view)
{
    OObj_assert(view, XViewTex);
    XViewTex *self = view;
    struct oobj_opt inject = OObj_find(view, OObj, VIEWTEX_INJECT, 0);
    if(inject.o && OObj_check(o_user(inject.o), AScene)) {
        ADefer_new_del(self, o_user(inject.o));
    }
}

//
// public
//

oobj x_viewtex_scene(oobj parent, oobj tex, bool move_tex,
                        const char *opt_title, XViewTex_pointer_fn opt_pointer)
{
    XViewTex *view = XViewTex_new(parent, tex, move_tex, viewtex_scene_done, opt_title, opt_pointer);
    AScene *scene = AScene_new(parent, view, true);
    AScene_opaque_set(scene, true);
    o_move(view, scene);
    
    oobj inject = OObj_new(view);
    OObj_name_set(inject, VIEWTEX_INJECT);
    o_user_set(inject, scene);
    
    return scene;
}
