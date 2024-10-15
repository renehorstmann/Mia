#include "x/viewtext.h"
#include "x/XViewText.h"
#include "a/AScene.h"
#include "a/ADefer.h"

#define VIEWTEXT_INJECT "x_viewtext_context"


struct viewtext_context {
    oobj scene;
    OObj__event_fn ok, cancel;
};


O_STATIC
void viewtext_scene_done(oobj view)
{
    OObj_assert(view, XViewText);
    XViewText *self = view;
    struct oobj_opt inject = OObj_find(view, OObj, VIEWTEXT_INJECT, 0);
    if(!inject.o || !o_user(inject.o)) {
        return;
    }
    struct viewtext_context *C = o_user(inject.o);
    
    if(XViewText_state(self) == XViewText_OK) {
        C->ok(C->scene);
    }
    if(C->cancel && XViewText_state(self) == XViewText_CANCEL) {
        C->cancel(C->scene);
    }
    
    ADefer_new_del(self, C->scene);
}

//
// public
//

oobj x_viewtext_scene(oobj parent, const char *title, int max_chars, OObj__event_fn on_ok_event, OObj__event_fn opt_on_cancel_event)
{
    XViewText *view = XViewText_new(parent, title, opt_on_cancel_event!=NULL, max_chars);
    AScene *scene = AScene_new(parent, view, true);
    AScene_opaque_set(scene, true);
    o_move(view, scene);
    
    XViewText_event_set(view, viewtext_scene_done);
    
    oobj inject = OObj_new(view);
    OObj_name_set(inject, VIEWTEXT_INJECT);
    struct viewtext_context *C = o_user_set(inject, o_new0(inject, *C, 1));
    C->scene = scene;
    C->ok = on_ok_event;
    C->cancel = opt_on_cancel_event;
    
    return scene;
}
