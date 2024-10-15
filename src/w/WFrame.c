#include "w/WFrame.h"
#include "o/OObj_builder.h"
#include "w/WTheme.h"
#include "w/WStyle.h"
#include "w/WText.h"
#include "w/WTextShadow.h"
#include "m/vec/vec2.h"
#include "u/rect.h"

#define O_LOG_LIB "w"
#include "o/log.h"

//
// public
//

WFrame *WFrame_init(oobj obj, oobj parent)
{
    WObj *super = obj;
    WFrame *self = obj;
    o_clear(self, sizeof *self, 1);

    WObj_init(obj, parent);
    OObj_id_set(self, WFrame_ID);

    self->container = OObj_new(self);
    self->label = WObj_new(self->container);

    // vfuncs
    super->v_update = WFrame__v_update;
    super->v_list = WFrame__v_list;
    super->v_style_apply = WFrame__v_style_apply;
    
    // apply style
    WObj_style_apply(self);

    return self;
}

WFrame *WFrame_new_title(oobj parent, const char *title, oobj *opt_out_title)
{
    oobj frame = WFrame_new(parent);
    oobj txt = WText_new(WFrame_label(frame), title);

    o_opt_set(opt_out_title, txt);
    return frame;
}

WFrame *WFrame_new_title_shadow(oobj parent, const char *title, oobj *opt_out_title)
{
    oobj frame = WFrame_new(parent);
    oobj txt = WTextShadow_new(WFrame_label(frame), title);

    o_opt_set(opt_out_title, txt);
    return frame;
}

//
// vfuncs
//

vec2 WFrame__v_update(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn)
{
    OObj_assert(obj, WFrame);
    WFrame *self = obj;
    
    vec2 label_size = WObj_update(self->label, lt, vec2_(0), theme, pointer_fn);

    vec2 inner_lt = lt;
    inner_lt.x += self->border_size;
    inner_lt.y -= o_max(label_size.y, self->border_size);

    vec2 inner_min_size = vec2_(
            o_max(label_size.x, min_size.x) - 2 * self->border_size,

            min_size.y - o_max(label_size.y, self->border_size) - self->border_size
    );

    vec2 inner_size = WObj__update_children_stacked(self, inner_lt, inner_min_size, theme, pointer_fn);

    // theme the rects
    struct r_rect *rects = WObj__alloc_rects(self, theme, 4);

    for(int i=0;i<4;i++) {
        rects[i].uv_rect = u_atlas_rect(WTheme_atlas(theme), WTheme_WHITE);
        rects[i].s = self->border_color;
    }
    
    vec2 label_diff = vec2_(0);
    if(label_size.x>0) {
        label_diff = vec2_(label_size.x+1, 1);
    }

    rects[0].rect = u_rect_new_lb(
            inner_lt.x + label_diff.x,
            inner_lt.y,
            inner_size.x - label_diff.x,
            self->border_size
    );

    rects[1].rect = u_rect_new_lt(
            inner_lt.x + inner_size.x,
            inner_lt.y,
            self->border_size,
            inner_size.y
    );

    rects[2].rect = u_rect_new_lt(
            inner_lt.x,
            inner_lt.y - inner_size.y,
            inner_size.x,
            self->border_size
    );

    rects[3].rect = u_rect_new_rt(
            inner_lt.x,
            inner_lt.y - label_diff.y,
            self->border_size,
            inner_size.y - label_diff.y
    );


    vec2 full_size = vec2_(
        o_max(label_size.x, self->border_size + inner_size.x) + self->border_size,
        
        o_max(label_size.y, self->border_size) + inner_size.y + self->border_size
    );
    
    return full_size;
}

oobj *WFrame__v_list(oobj obj, osize *opt_out_num)
{
    OObj_assert(obj, WFrame);
    WFrame *self = obj;

    osize direct_size;
    WObj **direct = WObj_list_direct(self, &direct_size);

    // directs + label + NULL
    direct = o_realloc(self, direct, sizeof(WObj *), direct_size+2);
    direct[direct_size] = self->label;
    direct[direct_size+1] = NULL;
    o_opt_set(opt_out_num, direct_size+1);
    return (oobj *) direct;
}

void WFrame__v_style_apply(oobj obj)
{
    WObj__v_style_apply(obj);
    
    OObj_assert(obj, WFrame);
    WFrame *self = obj;
    oobj style = WObj_style(self);
    
    self->border_color = WStyle_frame_border_color(style);
    self->border_size = WStyle_frame_border_size(style);
}

