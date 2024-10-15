#include "w/WView.h"
#include "o/OObj_builder.h"
#include "r/RTex.h"
#include "m/vec/vec2.h"
#include "m/io/ivec4.h"
#include "u/rect.h"

#define O_LOG_LIB "w"
#include "o/log.h"



//
// public
//

WView *WView_init(oobj obj, oobj parent, oobj view, bool move_view)
{
    WObj *super = obj;
    WView *self = obj;
    o_clear(self, sizeof *self, 1);

    WObj_init(obj, parent);
    OObj_id_set(self, WView_ID);

    self->view = view;
    if(move_view) {
        o_move(view, self);
    }

    // vfuncs
    super->v_update = WView__v_update;

    return self;
}

//
// vfuncs
//

vec2 WView__v_update(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn)
{
    OObj_assert(obj, WView);
    WView *self = obj;

    vec2 child_size = WObj__v_update(self, lt, min_size, theme, pointer_fn);
    vec2 size = vec2_max_v(child_size, min_size);

    self->hidden = false;

    self->lbwh = vec4_(lt.x, lt.y - size.y, size.x, size.y);

    return size;
}

AView *WView_view_set(oobj obj, oobj view, bool del_old)
{
    OObj_assert(obj, WView);
    WView *self = obj;
    if(del_old) {
        o_del(self->view);
    }
    self->view = view;
    return self->view;
}

ivec4 WView_viewport(oobj obj, oobj tex)
{
    ivec2 off = RTex_viewport(tex).xy;
    ivec4 vp = r_proj_lbwh_to_viewport(RTex_proj(tex), WView_lbwh(obj), &off);
    return vp;
}

void WView_render(oobj obj, oobj tex)
{
    OObj_assert(obj, WView);
    WView *self = obj;

    if(!self->hidden) {
        AView_update(self->view, tex, WView_viewport(self, tex));
        AView_render(self->view, tex);
    }
    self->hidden = true;
}
