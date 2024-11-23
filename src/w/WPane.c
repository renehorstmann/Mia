#include "w/WPane.h"
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

WPane *WPane_init(oobj obj, oobj parent)
{
    WObj *super = obj;
    WPane *self = obj;
    o_clear(self, sizeof *self, 1);

    WObj_init(obj, parent);
    OObj_id_set(self, WPane_ID);

    // vfuncs
    super->v_update = WPane__v_update;
    super->v_style_apply = WPane__v_style_apply;
    
    // apply style
    WObj_style_apply(self);

    return self;
}

//
// vfuncs
//

vec2 WPane__v_update(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn)
{
    OObj_assert(obj, WPane);
    WPane *self = obj;


    struct u_atlas atlas = WTheme_atlas(theme);

    int sprite;
    switch(self->style) {
        default:
        case WPane_DEFAULT:
            sprite = WTheme_PANE;
            break;
        case WPane_ROUND:
            sprite = WTheme_PANE_ROUND;
            break;
        case WPane_ROUND_TOP:
            sprite = WTheme_PANE_ROUND_TOP;
            break;
        case WPane_ROUND_BOTTOM:
            sprite = WTheme_PANE_ROUND_BOTTOM;
            break;
    }

    vec2 size_corner_lt = u_atlas_size(atlas, sprite);
    vec2 size_corner_rb = u_atlas_size(atlas, sprite + 8);
    vec2 frame_size = vec2_add_v(size_corner_lt, size_corner_rb);


    vec2 children_lt = vec2_(lt.x + size_corner_lt.x, lt.y - size_corner_lt.y);
    vec2 min_inner_size = vec2_sub_v(min_size, frame_size);

    vec2 inner_size = WObj__update_children_stacked(self, children_lt, min_inner_size, theme, pointer_fn);

    inner_size = vec2_max_v(inner_size, min_inner_size);
    inner_size = vec2_max(inner_size, 0);
    
    
    self->gen_min_inner_size = min_inner_size;
    self->gen_inner_size = inner_size;

    // theme the rects
    struct r_rect *rects = WObj__alloc_rects(self, theme, 9);

    vec2 offsets[9] = {
            {{0, 0}},
            {{size_corner_lt.x, 0}},
            {{size_corner_lt.x + inner_size.x, 0}},
            {{0, size_corner_lt.y}},
            {{size_corner_lt.x, size_corner_lt.y}},
            {{size_corner_lt.x + inner_size.x, size_corner_lt.y}},
            {{0, size_corner_lt.y + inner_size.y}},
            {{size_corner_lt.x, size_corner_lt.y + inner_size.y}},
            {{size_corner_lt.x + inner_size.x, size_corner_lt.y + inner_size.y}},
    };
    vec2 sizes[9] = {
            {{size_corner_lt.x, size_corner_lt.y}},
            {{inner_size.x, size_corner_lt.y}},
            {{size_corner_rb.x, size_corner_lt.y}},
            {{size_corner_lt.x, inner_size.y}},
            {{inner_size.x, inner_size.y}},
            {{size_corner_rb.x, inner_size.y}},
            {{size_corner_lt.x, size_corner_rb.y}},
            {{inner_size.x, size_corner_rb.y}},
            {{size_corner_rb.x, size_corner_rb.y}}
    };

    for (int i = 0; i < 9; i++) {
        struct r_rect *q = &rects[i];
        q->uv_rect = u_atlas_rect(atlas, sprite + i);
        q->rect = u_rect_new_lt(lt.x + offsets[i].x,
                                lt.y - offsets[i].y,
                                m_2(sizes[i]));
        q->s = self->color;
    }

    vec2 full_size = vec2_add_v(inner_size, frame_size);
    return full_size;
}

void WPane__v_style_apply(oobj obj)
{
    WObj__v_style_apply(obj);
    
    OObj_assert(obj, WPane);
    WPane *self = obj;
    oobj style = WObj_style(self);
    
    self->color = WStyle_pane_color(style);
    self->style = WStyle_pane_style(style);    
}

