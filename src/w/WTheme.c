#include "w/WTheme.h"
#include "o/OObj_builder.h"
#include "o/OArray.h"
#include "o/ODelcallback.h"
#include "r/RObjRect.h"
#include "w/WObj.h"
#include "w/WStack.h"

#define O_LOG_LIB "w"

#include "o/log.h"


static struct {
    oobj o;
} L;

O_STATIC
oobj L_o(void)
{
    if (!L.o) {
        L.o = OObj_new(r_root());
        OObj_name_set(L.o, "WTheme");
    }
    return L.o;
}

O_STATIC
vec4 item_lt(oobj tex, float left, float top, float w, float h)
{
    float tex_left = RTex_left(tex);
    float tex_top = RTex_top(tex);

    return vec4_(tex_left + left + w / 2.0f, tex_top - top - h / 2.0f, w, h);
}

O_STATIC
void set_nine_part(struct u_atlas atlas, oobj tex, int sprite, vec2 lt, vec2 lt_size, vec2 center_size, vec2 rb_size)
{
    atlas.rects[sprite + 0] = item_lt(tex, lt.x, lt.y,
                                      lt_size.x, lt_size.y);
    atlas.rects[sprite + 1] = item_lt(tex, lt.x + lt_size.x, lt.y,
                                      center_size.x, lt_size.y);
    atlas.rects[sprite + 2] = item_lt(tex, lt.x + lt_size.x + center_size.x, lt.y,
                                      rb_size.x, lt_size.y);
    atlas.rects[sprite + 3] = item_lt(tex, lt.x, lt.y + lt_size.y,
                                      lt_size.x, center_size.y);
    atlas.rects[sprite + 4] = item_lt(tex, lt.x + lt_size.x, lt.y + lt_size.y,
                                      center_size.x, center_size.y);
    atlas.rects[sprite + 5] = item_lt(tex, lt.x + lt_size.x + center_size.x, lt.y + lt_size.y,
                                      rb_size.x, center_size.y);
    atlas.rects[sprite + 6] = item_lt(tex, lt.x, lt.y + lt_size.y + center_size.y,
                                      lt_size.x, rb_size.y);
    atlas.rects[sprite + 7] = item_lt(tex, lt.x + lt_size.x, lt.y + lt_size.y + center_size.y,
                                      center_size.x, rb_size.y);
    atlas.rects[sprite + 8] = item_lt(tex, lt.x + lt_size.x + center_size.x, lt.y + lt_size.y + center_size.y,
                                      rb_size.x, rb_size.y);
}

//
// public
//

WTheme *WTheme_init(oobj obj, oobj parent, oobj tex, bool move_tex,
                    struct u_atlas atlas, bool move_atlas)
{
    WTheme *self = obj;
    o_clear(self, sizeof *self, 1);

    OObj_init(self, parent);
    OObj_id_set(self, WTheme_ID);

    self->ro = RObjRect_new_color(self, 0, tex, move_tex);

    // setup OArray as dynamic array
    oobj rects = RObjRect_rects(self->ro);
    OArray_dyn_realloc_mode_set(rects, OArray_REALLOC_DOUBLED_FRONT);
    OArray_realloc(rects, 128, 128);

    self->atlas = atlas;
    if (move_atlas) {
        u_atlas_move(&self->atlas, self);
    }
    
    self->reupdate = true;

    // vfuncs
    self->super.v_op_at = WTheme__v_op_at;
    self->super.v_op_num = WTheme__v_op_num;

    return self;
}

WTheme *WTheme_new_tiny(oobj parent)
{
    oobj tex = RTex_new_file(L_o(), "res/w/theme_tiny.png");

    struct u_atlas atlas = u_atlas_new(parent, WTheme_MAX_IDX);
    //
    // atlas setup
    //

    // fonts
    for (int i = 0; i < WTheme_FONT_NUM; i++) {
        int col = i % 16;
        int row = i / 16;
        atlas.rects[WTheme_FONT + i] = item_lt(tex, 1 + col * 8, 1 + row * 8, 3, 5);
        atlas.rects[WTheme_FONT_SHADOW + i] = item_lt(tex, 1 + col * 8, 65 + row * 8, 4, 6);
    }

    // white (just use center of '+')
    atlas.rects[WTheme_WHITE] = vec4_(m_2(atlas.rects[WTheme_char_idx('+')].xy), 1, 1);

    // icons
    for (int i = 0; i < 16; i++) {
        atlas.rects[WTheme_ICON_CROSS + i] = item_lt(tex, i * 8 + 1, 128 + 1, 7, 7);
    }
    for (int i = 0; i < 16; i++) {
        atlas.rects[WTheme_ICON_RECORD + i] = item_lt(tex, i * 8 + 1, 136 + 1, 7, 7);
    }
    for (int i = 0; i < 7; i++) {
        atlas.rects[WTheme_ICON_TAB + i] = item_lt(tex, i * 8 + 1, 144 + 1, 7, 7);
    }

    // btn
    set_nine_part(atlas, tex, WTheme_BTN_FLAT,
                  vec2_(1, 160 + 1), vec2_(2, 2), vec2_(1), vec2_(2, 3));
    set_nine_part(atlas, tex, WTheme_BTN_FLAT__PRESSED,
                  vec2_(8 + 1, 160 + 1), vec2_(2, 3), vec2_(1), vec2_(2, 2));

    set_nine_part(atlas, tex, WTheme_BTN_FLAT_ROUND,
                  vec2_(16 + 1, 160 + 1), vec2_(2, 2), vec2_(1), vec2_(2, 3));
    set_nine_part(atlas, tex, WTheme_BTN_FLAT_ROUND__PRESSED,
                  vec2_(24 + 1, 160 + 1), vec2_(2, 3), vec2_(1), vec2_(2, 2));

    set_nine_part(atlas, tex, WTheme_BTN_DEF,
                  vec2_(32 + 1, 160), vec2_(3, 2), vec2_(1), vec2_(3, 5));
    set_nine_part(atlas, tex, WTheme_BTN_DEF__PRESSED,
                  vec2_(40 + 1, 160), vec2_(3, 4), vec2_(1), vec2_(3, 3));

    set_nine_part(atlas, tex, WTheme_BTN_DEF_ROUND,
                  vec2_(48 + 1, 160), vec2_(3, 2), vec2_(1), vec2_(3, 5));
    set_nine_part(atlas, tex, WTheme_BTN_DEF_ROUND__PRESSED,
                  vec2_(56 + 1, 160), vec2_(3, 4), vec2_(1), vec2_(3, 3));

    set_nine_part(atlas, tex, WTheme_BTN_BIG,
                  vec2_(64 + 1, 160), vec2_(3, 4), vec2_(1), vec2_(3, 9));
    set_nine_part(atlas, tex, WTheme_BTN_BIG__PRESSED,
                  vec2_(72 + 1, 160), vec2_(3, 7), vec2_(1), vec2_(3, 6));

    set_nine_part(atlas, tex, WTheme_BTN_BIG_ROUND,
                  vec2_(80 + 1, 160), vec2_(3, 4), vec2_(1), vec2_(3, 9));
    set_nine_part(atlas, tex, WTheme_BTN_BIG_ROUND__PRESSED,
                  vec2_(88 + 1, 160), vec2_(3, 7), vec2_(1), vec2_(3, 6));


    set_nine_part(atlas, tex, WTheme_BTN_RADIO,
                  vec2_(0, 168), vec2_(4, 4), vec2_(1), vec2_(3, 3));
    set_nine_part(atlas, tex, WTheme_BTN_RADIO__PRESSED,
                  vec2_(8, 168), vec2_(4, 4), vec2_(1), vec2_(3, 3));

    set_nine_part(atlas, tex, WTheme_BTN_CHECK,
                  vec2_(16, 168), vec2_(4, 4), vec2_(1), vec2_(3, 3));
    set_nine_part(atlas, tex, WTheme_BTN_CHECK__PRESSED,
                  vec2_(24, 168), vec2_(4, 4), vec2_(1), vec2_(3, 3));

    set_nine_part(atlas, tex, WTheme_BTN_SWITCH,
                  vec2_(32 + 1, 168 + 1), vec2_(3, 3), vec2_(1), vec2_(10, 3));
    set_nine_part(atlas, tex, WTheme_BTN_SWITCH__PRESSED,
                  vec2_(48 + 1, 168 + 1), vec2_(10, 3), vec2_(1), vec2_(3, 3));

    // float
    set_nine_part(atlas, tex, WTheme_PANE,
                  vec2_(1, 176 + 1), vec2_(2, 2), vec2_(1), vec2_(3, 3));
    set_nine_part(atlas, tex, WTheme_PANE_ROUND_TOP,
                  vec2_(8 + 1, 176 + 1), vec2_(2, 2), vec2_(1), vec2_(3, 3));
    set_nine_part(atlas, tex, WTheme_PANE_ROUND,
                  vec2_(16 + 1, 176 + 1), vec2_(2, 2), vec2_(1), vec2_(3, 3));
    set_nine_part(atlas, tex, WTheme_PANE_ROUND_BOTTOM,
                  vec2_(24 + 1, 176 + 1), vec2_(2, 2), vec2_(1), vec2_(3, 3));

    // sliders
    atlas.rects[WTheme_SLIDER_H_BAR + 0] = item_lt(tex,
                                                   1, 184, 2, 7);
    atlas.rects[WTheme_SLIDER_H_BAR + 1] = item_lt(tex,
                                                   1 + 2, 184, 1, 7);
    atlas.rects[WTheme_SLIDER_H_BAR + 2] = item_lt(tex,
                                                   1 + 3, 184, 2, 7);
    atlas.rects[WTheme_SLIDER_H_BADGE] = item_lt(tex,
                                                 8, 184, 4, 7);

    atlas.rects[WTheme_SLIDER_V_BAR + 0] = item_lt(tex,
                                                   16, 184 + 1, 7, 2);
    atlas.rects[WTheme_SLIDER_V_BAR + 1] = item_lt(tex,
                                                   16, 184 + 3, 7, 1);
    atlas.rects[WTheme_SLIDER_V_BAR + 2] = item_lt(tex,
                                                   16, 184 + 4, 7, 2);
    atlas.rects[WTheme_SLIDER_V_BADGE] = item_lt(tex,
                                                 24, 184, 7, 4);


    // custom
    for (int i = 0; i < WTheme_CUSTOM_8_NUM; i++) {
        int col = i % 4;
        int row = i / 4;
        atlas.rects[WTheme_CUSTOM_8 + i] = item_lt(tex, 128 + col * 8, 0 + row * 8, 8, 8);
    }
    for (int i = 0; i < WTheme_CUSTOM_16_NUM; i++) {
        int col = i % 2;
        int row = i / 2;
        atlas.rects[WTheme_CUSTOM_16 + i] = item_lt(tex, 128 + col * 16, 16 + row * 16, 16, 16);
    }
    for (int i = 0; i < WTheme_CUSTOM_32_NUM; i++) {
        int col = i % 3;
        int row = i / 3;
        atlas.rects[WTheme_CUSTOM_32 + i] = item_lt(tex, 128 + 32 + col * 32, 0 + row * 32, 32, 32);
    }
    for (int i = 0; i < WTheme_CUSTOM_64_NUM; i++) {
        int col = i % 2;
        int row = i / 2;
        atlas.rects[WTheme_CUSTOM_64 + i] = item_lt(tex, 128 + col * 64, 64 + row * 64, 64, 64);
    }

    //
    // end atlas setup
    //
    return WTheme_new(parent, tex, false, atlas, true);
}

//
// vfuncs
//

osize WTheme__v_op_num(oobj obj)
{
    return WTheme_num(obj);
}

void *WTheme__v_op_at(oobj obj, osize idx)
{
    return WTheme_at(obj, (int) idx);
}


//
// object functions
//

void WTheme_clear(oobj obj)
{
    oobj rects = RObjRect_rects(WTheme_ro(obj));
    OArray_clear(rects);
}

int WTheme_alloc(oobj obj, int num)
{
    OObj_assert(obj, WTheme);
    WTheme *self = obj;
    oobj rects = RObjRect_rects(self->ro);
    osize back_idx = OArray_num(rects) + num;
    OArray_append_front(rects, NULL, num);
    for (int i = 0; i < num; i++) {
        struct r_rect *q = o_at(rects, i);
        *q = r_rect_new(m_2(RTex_size(RObjRect_tex(self->ro))));
    }
    return (int) back_idx;
}

struct r_rect *WTheme_at(oobj obj, int back_idx)
{
    oobj rects = RObjRect_rects(WTheme_ro(obj));
    osize num = OArray_num(rects);
    return OArray_at_void(rects, num - back_idx);
}

int WTheme_num(oobj obj)
{
    oobj rects = RObjRect_rects(WTheme_ro(obj));
    return (int) OArray_num(rects);
}

oobj WTheme_tex(oobj obj)
{
    return RObjRect_tex(WTheme_ro(obj));
}

vec2 WTheme_update(oobj obj, oobj wobj, vec2 lt, vec2 min_size, a_pointer__fn pointer_fn)
{
    OObj_assert(obj, WTheme);
    WTheme *self = obj;
    vec2 size;
    WTheme_clear(self);
    size = WObj_update(wobj, lt, min_size, self, pointer_fn);
    if(self->reupdate) {
        WTheme_clear(self);
    size = WObj_update(wobj, lt, min_size, self, pointer_fn);
    }
    self->reupdate = false;
    return size;
}

vec2 WTheme_update_stack(oobj obj, oobj stack, oobj tex, vec2 *opt_out_lt, a_pointer__fn pointer_fn)
{
    WTheme_clear(obj);
    return WStack_update(stack, tex, obj, opt_out_lt, pointer_fn);
}

void WTheme_render_ex(oobj obj, oobj tex, const struct r_proj *opt_proj, bool update)
{
    OObj_assert(obj, WTheme);
    WTheme *self = obj;
    RObj_render_ex(self->ro, tex, opt_proj, update);
}
