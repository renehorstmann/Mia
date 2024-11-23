#include "x/viewfiles.h"
#include "x/XViewFiles.h"
#include "a/AScene.h"
#include "a/ADefer.h"

#define VIEWFILES_INJECT "x_viewfiles_context"

struct vfiles_context {
    oobj scene;
    OObj__event_fn ok, cancel;
};


O_STATIC
void viewfiles_scene_done(oobj view)
{
    OObj_assert(view, XViewFiles);
    XViewFiles *self = view;
    struct oobj_opt inject = OObj_find(view, OObj, VIEWFILES_INJECT, 0);
    if(!inject.o || !o_user(inject.o)) {
        return;
    }
    struct vfiles_context *C = o_user(inject.o);

    if(XViewFiles_state(self) == XViewFiles_OK) {
        C->ok(C->scene);
    }
    if(C->cancel && XViewFiles_state(self) == XViewFiles_CANCEL) {
        C->cancel(C->scene);
    }

    ADefer_new_del(self, C->scene);
}

//
// public
//

oobj x_viewfiles_scene(oobj parent, const char *title, OObj__event_fn on_ok_event, OObj__event_fn opt_on_cancel_event)
{
    XViewFiles *view = XViewFiles_new(parent, title);
    AScene *scene = AScene_new(parent, view, true);
    AScene_opaque_set(scene, true);
    o_move(view, scene);

    XViewFiles_event_set(view, viewfiles_scene_done);

    oobj inject = OObj_new(view);
    OObj_name_set(inject, VIEWFILES_INJECT);
    struct vfiles_context *C = o_user_set(inject, o_new0(inject, *C, 1));
    C->scene = scene;
    C->ok = on_ok_event;
    C->cancel = opt_on_cancel_event;

    return scene;
}
