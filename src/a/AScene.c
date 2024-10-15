#include "a/AScene.h"
#include "o/OObj_builder.h"
#include "o/ODelcallback.h"
#include "a/app.h"



//
// public
//


AScene *AScene_init(oobj obj, oobj parent, oobj view, bool move_view)
{
    AScene *self = obj;
    o_clear(self, sizeof *self, 1);

    OObj_init(self, parent);
    OObj_id_set(self, AScene_ID);

    // register in a_app protected function
    void a_app__scene_register(AScene *scene);
    a_app__scene_register(self);

    self->opaque = true;
    self->full_x_auto = self->full_y_auto = true;
    self->full_even_size = true;

    if(view && move_view) {
        o_move(view, self);
    }
    AScene_view_set(self, view, false);

    return self;
}

//
// object functions
//

AView *AScene_view_set(oobj obj, oobj view, bool del_old)
{
    OObj_assert(obj, AScene);
    AScene *self = obj;
    if(del_old) {
        o_del(self->view);
    }
    self->view = view;
    return self->view;
}
