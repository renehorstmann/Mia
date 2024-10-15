#include "w/WTextShadow.h"
#include "o/OObj_builder.h"
#include "o/str.h"
#include "w/WTheme.h"
#include "w/WStyle.h"
#include "m/vec/vec2.h"
#include "u/rect.h"

#define O_LOG_LIB "w"
#include "o/log.h"

#include <ctype.h>

//
// public
//

WTextShadow *WTextShadow_init(oobj obj, oobj parent, const char *text)
{
    WText *super = obj;
    WTextShadow *self = obj;
    o_clear(self, sizeof *self, 1);

    WText_init(obj, parent, text);

    OObj_id_set(self, WTextShadow_ID);


    // vfuncs
    super->super.v_update = WTextShadow__v_update;
    super->super.v_style_apply = WTextShadow__v_style_apply;
    
    // apply style
    WObj_style_apply(self);

    return self;
}

//
// vfuncs
//

vec2 WTextShadow__v_update(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn)
{
    OObj_assert(obj, WTextShadow);
    WText *super = obj;
    WTextShadow *self = obj;

    //
    // mostly a copy of WText__v_update, but added rects * 4 shadow stuff...
    //


    WText_char_size_update(self, theme);
    vec2 char_size = vec2_scale_v(super->char_size, super->char_scale);
    vec2 char_offset = vec2_scale_v(super->char_offset, super->char_scale);
    vec2 char_shadow_size = vec2_scale_v(vec2_add(super->char_size, 1), super->char_scale);



    vec2 child_size = WObj__v_update(self, lt, min_size, theme, pointer_fn);

    // shadow: *4 for 3 shadow rects
    struct r_rect *rects = WObj__alloc_rects(self, theme, super->num*2);

    int col = 0;
    int row = 0;
    int cols = 0;
    for(int i=0; i<super->num; i++) {
        char c = super->text[i];
        if(super->text_mode == WText_MODE_UPPER) {
            c = (char) toupper(c);
        } else if(super->text_mode == WText_MODE_LOWER) {
            c = (char) tolower(c);
        }

        float left = lt.x + col * char_offset.x;
        float top = lt.y - row * char_offset.y;

        // shadow:
        int base_idx = i*2;

        rects[base_idx+0].uv_rect = u_atlas_rect(WTheme_atlas(theme), WTheme_char_shadow_idx(c));
        rects[base_idx+0].rect = u_rect_new_lt(left, top, m_2(char_shadow_size));
        rects[base_idx+0].s = self->shadow_color;

        rects[base_idx+1].uv_rect = u_atlas_rect(WTheme_atlas(theme), WTheme_char_idx(c));
        rects[base_idx+1].rect = u_rect_new_lt(left, top, m_2(char_size));
        rects[base_idx+1].s = super->color;


        col++;
        if(c=='\n') {
            col = 0;
            row++;
        }
        cols = o_max(cols, col);
    }

    if (cols == 0) {
        super->text_size = vec2_(0);
    } else {
        super->text_size = vec2_(
                (cols - 1) * char_offset.x + char_size.x,
                row * char_offset.y + char_size.y
        );

        // shadow +1
        super->text_size = vec2_add(super->text_size, 1);
    }

    return vec2_max_v(super->text_size, child_size);
}

void WTextShadow__v_style_apply(oobj obj)
{
    WText__v_style_apply(obj);
    
    OObj_assert(obj, WTextShadow);
    WTextShadow *self = obj;
    oobj style = WObj_style(self);
    
    self->shadow_color = WStyle_textshadow_shadow_color(style);
}
