#include "w/WText.h"
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

WText *WText_init(oobj obj, oobj parent, const char *text)
{
    WObj *super = obj;
    WText *self = obj;
    o_clear(self, sizeof *self, 1);

    text = o_or(text, "");

    WObj_init(obj, parent);
    OObj_id_set(self, WText_ID);

    self->text_mode = WText_MODE_DEFAULT;
    self->char_size = vec2_(-1);
    self->char_offset = vec2_(-1);
    self->char_scale = vec2_(1.0);

    WText_text_set(self, text);

    // vfuncs
    super->v_update = WText__v_update;
    super->v_style_apply = WText__v_style_apply;
    
    // apply style
    WObj_style_apply(self);

    return self;
}

//
// vfuncs
//

vec2 WText__v_update(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn)
{
    OObj_assert(obj, WText);
    WText *self = obj;

    WText_char_size_update(self, theme);
    vec2 char_size = vec2_scale_v(self->char_size, self->char_scale);
    vec2 char_offset = vec2_scale_v(self->char_offset, self->char_scale);

    vec2 child_size = WObj__v_update(self, lt, min_size, theme, pointer_fn);

    struct r_rect *rects = WObj__alloc_rects(self, theme, self->num);

    int col = 0;
    int row = 0;
    int cols = 0;
    for(int i=0; i<self->num; i++) {
        char c = self->text[i];
        if(self->text_mode == WText_MODE_UPPER) {
            c = (char) toupper(c);
        } else if(self->text_mode == WText_MODE_LOWER) {
            c = (char) tolower(c);
        }

        float left = lt.x + col * char_offset.x;
        float top = lt.y - row * char_offset.y;
        rects[i].uv_rect = u_atlas_rect(WTheme_atlas(theme), WTheme_char_idx(c));
        rects[i].rect = u_rect_new_lt(left, top, m_2(char_size));
        rects[i].s = self->color;

        col++;
        if(c=='\n') {
            col = 0;
            row++;
        }
        cols = o_max(cols, col);
    }

    if (cols == 0) {
        self->text_size = vec2_(0);
    } else {
        self->text_size = vec2_(
                (cols - 1) * char_offset.x + char_size.x,
                row * char_offset.y + char_size.y
        );
    }

    return vec2_max_v(self->text_size, child_size);
}

void WText__v_style_apply(oobj obj)
{
    WObj__v_style_apply(obj);
    
    OObj_assert(obj, WText);
    WText *self = obj;
    oobj style = WObj_style(self);
    
    self->color = WStyle_text_color(style);
}

void WText_char_size_update(oobj obj, oobj theme)
{
    OObj_assert(obj, WText);
    WText *self = obj;

    // init char offset + size
    if(self->char_size.x < 0) {
        self->char_size = u_atlas_size(WTheme_atlas(theme), WTheme_char_idx('0'));
        self->char_offset = vec2_add(self->char_size, 1);
    }

}

const char *WText_text_set(oobj obj, const char *text)
{
    OObj_assert(obj, WText);
    WText *self = obj;
    o_free(self, self->text);
    self->text = o_str_clone(self, text);
    self->num = (int) o_strlen(text);
    return self->text;
}
