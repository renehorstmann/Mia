#include "w/WProgress.h"
#include "o/OObj_builder.h"
#include "w/WTheme.h"
#include "w/WStyle.h"
#include "m/vec/vec2.h"
#include "u/rect.h"

#define O_LOG_LIB "w"
#include "o/log.h"

//
// public
//

WProgress *WProgress_init(oobj obj, oobj parent)
{
    WObj *super = obj;
    WProgress *self = obj;
    o_clear(self, sizeof *self, 1);

    WObj_init(obj, parent);
    OObj_id_set(self, WProgress_ID);

    self->on_unit = true;

    // vfuncs
    super->v_update = WProgress__v_update;
    super->v_style_apply = WProgress__v_style_apply;
    
    // apply style
    WObj_style_apply(self);


    return self;
}

//
// vfuncs
//

vec2 WProgress__v_update(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn)
{
    OObj_assert(obj, WProgress);
    WProgress *self = obj;


    vec2 child_size = WObj__v_update(self, lt, min_size, theme, pointer_fn);

    child_size = vec2_max_v(child_size, min_size);

    vec2 fixed_size = WObj_fixed_size(self);
    if(fixed_size.x >= 0) {
        child_size.x = fixed_size.x;
    }
    if(fixed_size.y >= 0) {
        child_size.y = fixed_size.y;
    }

    vec2 progress_lt = lt;
    vec2 progress_size = child_size;


    if(self->vertical) {
        progress_size.y *= self->progress;
    } else {
        progress_size.x *= self->progress;
    }

    if(self->on_unit) {
        progress_size = vec2_ceil(progress_size);
    }

    if(self->vertical) {
        progress_lt.y -= (child_size.y - progress_size.y);
    }

    progress_size = vec2_clamp_v(progress_size, vec2_(0.0), child_size);

    struct r_rect *rects = WObj__alloc_rects(self, theme, 1);
    rects->uv_rect = u_atlas_rect(WTheme_atlas(theme), WTheme_WHITE);
    rects->rect = u_rect_new_lt(m_2(progress_lt), m_2(progress_size));
    rects->s = self->color;
    
    return child_size;
}

void WProgress__v_style_apply(oobj obj) 
{
    WObj__v_style_apply(obj);
    
    OObj_assert(obj, WProgress);
    WProgress *self = obj;
    oobj style = WObj_style(self);
    
    self->color = WStyle_progress_color(style);
}
