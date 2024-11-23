#include "x/XViewText.h"
#include "o/OObj_builder.h"
#include "o/clipboard.h"
#include "o/OStream.h"
#include "o/OArray.h"
#include "o/str.h"
#include "r/RTex.h"
#include "u/rect.h"
#include "w/WBox.h"
#include "w/WAlign.h"
#include "w/WFrame.h"
#include "w/WColor.h"
#include "w/WText.h"
#include "w/WTextShadow.h"
#include "w/WIcon.h"
#include "w/WBtn.h"
#include "x/XViewKeys.h"

#define MOVE_TIMEOUT 1.5f
#define WHEEL_ZOOM 1.1f

#define O_LOG_LIB "x"

#include "o/log.h"

#define CURSOR_BLINK_TIME 1.0f


//
// public
//

XViewText *XViewText_init(oobj obj, oobj parent, const char *title, bool show_cancel, int max_chars)
{

    AView *super = obj;
    XViewText *self = obj;
    o_clear(self, sizeof *self, 1);

    AView_init(obj, parent, XViewText__v_setup, XViewText__v_update, XViewText__v_render);
    OObj_id_set(self, XViewText_ID);

    self->state = XViewText_RUNNING;
    self->viewkeys = XViewKeys_new(self);
    self->max_chars = max_chars;
    self->text_array = OArray_new_dyn(self, NULL, sizeof(char), 0, max_chars);

    self->ignore_filter = o_str_clone(self, "");
    self->bg = R_GRAY_X(0.3);

    self->w.theme = WTheme_new_tiny(self);

    oobj align = WAlign_new(self);
    self->w.gui = align;
    WAlign_align_set(align, WAlign_CENTER, WAlign_CENTER);

    self->w.bg = WColor_new(align, self->bg);

    oobj box = WBox_new(align, WBox_LAYOUT_V);
    WBox_spacing_set(box, vec2_(8));

    self->w.custom_box_container = WObj_new(box);
    // clear to init the box
    XViewText_custom_clear(self);

    oobj title_text = WTextShadow_new(box, title);
    WText_char_scale_set(title_text, vec2_(1, 2));

    oobj text_frame = WFrame_new(box);
    oobj text_color = WColor_new(text_frame, R_WHITE);

    self->w.text = WText_new(text_color, "");
    WObj_padding_set(self->w.text, vec4_(4));
    WText_color_set(self->w.text, R_BLACK);

    oobj tool_box = WBox_new(box, WBox_LAYOUT_H);
    WBox_spacing_set(tool_box, vec2_(2));
    WObj_padding_set(tool_box, vec4_(2));

    self->w.clear = WBtn_new(tool_box);
    WBtn_style_set(self->w.clear, WBtn_DEF_ROUND);
    oobj clear_text = WText_new(self->w.clear, "CL");
    WObj_padding_set(clear_text, vec4_(2));

    if (o_clipboard_system_available()) {
        self->w.copy = WBtn_new(tool_box);
        WBtn_style_set(self->w.copy, WBtn_DEF_ROUND);
        oobj copy_text = WText_new(self->w.copy, "COPY");
        WObj_padding_set(copy_text, vec4_(2));

        self->w.paste = WBtn_new(tool_box);
        WBtn_style_set(self->w.paste, WBtn_DEF_ROUND);
        oobj paste_text = WText_new(self->w.paste, "PASTE");
        WObj_padding_set(paste_text, vec4_(2));
    }

    oobj bottom_container = WObj_new(box);
    WObj_min_size_set(bottom_container, vec2_(140, 0));
    WObj_padding_set(bottom_container, vec4_(2, 0));

    if (show_cancel) {
        oobj cancel_align = WAlign_new(bottom_container);
        WAlign_align_set(cancel_align, WAlign_START, WAlign_START);
        self->w.cancel = WBtn_new(cancel_align);
        WBtn_style_set(self->w.cancel, WBtn_BIG_ROUND);
        oobj cancel_icon = WIcon_new(self->w.cancel, WTheme_ICON_CROSS);
        WIcon_color_set(cancel_icon, vec4_(0.8, 0.1, 0.1, 1.0));
        WObj_padding_set(cancel_icon, vec4_(8, 2, 8, 2));
    }

    oobj ok_align = WAlign_new(bottom_container);
    WAlign_align_set(ok_align, WAlign_END, WAlign_START);
    self->w.ok = WBtn_new(ok_align);
    WBtn_style_set(self->w.ok, WBtn_BIG_ROUND);
    oobj ok_icon = WIcon_new(self->w.ok, WTheme_ICON_CHECK);
    WIcon_color_set(ok_icon, vec4_(0.1, 0.8, 0.1, 01.0));
    WObj_padding_set(ok_icon, vec4_(8, 2, 8, 2));

    oobj cursor_align = WAlign_new(self->w.text);
    self->w.cursor = WColor_new(cursor_align, R_BLACK);
    // init right
    self->cursor = 999;

    // vfuncs

    return self;

}


//
// virtual implementations
//

void XViewText__v_setup(oobj view)
{
    // to allow smooth zooming
//    RCam_pixelperfect_view_set(a_app_cam(), false);

//    cam_home(view);
}

void XViewText__v_update(oobj view, oobj tex, float dt)
{
    OObj_assert(view, XViewText);
    XViewText *self = view;

    struct r_proj *proj = RTex_proj(tex);
    vec2 lt = vec2_(r_proj_left_int(proj), r_proj_top(proj));
    vec2 size = vec2_cast_int(r_proj_size_int(proj).v);
    size.y *= 0.66f;

    ivec4 viewport = RTex_viewport(tex);
    ivec4 upper_vp = viewport;
    ivec4 keys_vp = viewport;
    keys_vp.v3 /= 3;
    upper_vp.y += keys_vp.v3;
    upper_vp.v3 -= keys_vp.v3;

    AView_update(self->viewkeys, tex, keys_vp);

    oobj stream = XViewKeys_stream(self->viewkeys);
    bool enter_typed = false;
    bool escape = false;
    bool escape_dir = false;
    for (;;) {
        char read;
        osize num = OStream_read_try(stream, &read, sizeof(char), 1);
        if (num == 0) {
            break;
        }
        if (escape_dir) {
            escape = escape_dir = false;
            if (read == 'B' || read == 'D') {
                self->cursor--;
            } else {
                self->cursor++;
            }
            continue;
        }
        if (escape) {
            escape = false;
            if (read == '[') {
                escape_dir = true;
            }
            continue;
        }
        if (read == '\x1B') {
            escape = true;
            continue;
        }
        if (o_str_find_char(self->ignore_filter, read) >= 0) {
            continue;
        }
        if (read == '\b') {
            if (o_num(self->text_array) > 0 && self->cursor > 0 && self->cursor <= o_num(self->text_array)) {
                OArray_pop_at(self->text_array, self->cursor - 1, NULL);
                self->cursor--;
            }
            continue;
        }
        if (read == '\n') {
            enter_typed = true;
            break;
        }
        if (o_num(self->text_array) < self->max_chars && self->cursor >= 0 && self->cursor <= o_num(self->text_array)) {
            OArray_push_at(self->text_array, self->cursor, &read);
            self->cursor++;
        }
    }

    WText_text_set(self->w.text, OArray_data(self->text_array, char));
    WObj_min_size_set(self->w.text, vec2_((self->max_chars+1) * 4, 5));
    WColor_color_set(self->w.bg, self->bg);

    // cursor
    self->cursor_blink_time -= dt;
    while (self->cursor_blink_time < 0) {
        self->cursor_blink_time += CURSOR_BLINK_TIME;
    }
    // WText_char_size is first set after update
    WObj_min_size_set(self->w.cursor, WText_char_size(self->w.text));
    WColor_color_set(self->w.cursor, self->cursor_blink_time < CURSOR_BLINK_TIME / 2.0f ?
                                     R_TRANSPARENT : vec4_(0, 0, 1, 0.5));
    self->cursor = o_clamp(self->cursor, 0, o_num(self->text_array));
    WObj_padding_set(self->w.cursor, vec4_(self->cursor * WText_char_offset(self->w.text).x, 0));

    WTheme_update(self->w.theme, self->w.gui, lt, size, a_pointer);

    // bg
    {
        struct r_rect *rect = WObj_gen_rects(self->w.bg);
        // m_MAX is to large...
        rect->rect = u_rect_new(0, 0, 1E8, 1E8);
    }

    if (WBtn_clicked(self->w.clear)) {
        OArray_resize(self->text_array, 0);
    }
    if (self->w.copy && WBtn_clicked(self->w.copy)) {
        o_clipboard_set(OArray_data(self->text_array, char));
    }
    if (self->w.paste && WBtn_clicked(self->w.paste)) {
        char *paste = o_clipboard_get(self);
        for (char *it = paste; *it; it++) {
            if (o_num(self->text_array) < self->max_chars && self->cursor >= 0 &&
                self->cursor <= o_num(self->text_array)) {
                OArray_push_at(self->text_array, self->cursor, it);
                self->cursor++;
            }
        }
        o_free(self, paste);
    }
    if (self->w.cancel && WBtn_clicked(self->w.cancel)) {
        self->state = XViewText_CANCEL;
        if (self->event) {
            self->event(self);
        }
    }
    if (enter_typed || WBtn_clicked(self->w.ok)) {
        self->state = XViewText_OK;
        if (self->event) {
            self->event(self);
        }
    }

}

void XViewText__v_render(oobj view, oobj tex, float dt)
{
    OObj_assert(view, XViewText);
    XViewText *self = view;

    WTheme_render(self->w.theme, tex);

    AView_render(self->viewkeys, tex);
}

//
// object functions:
//

void XViewText_custom_clear(oobj obj)
{
    OObj_assert(obj, XViewText);
    XViewText *self = obj;
    o_del(self->w.custom_box);
    self->w.custom_box = WBox_new(self->w.custom_box_container, WBox_LAYOUT_H);
    WBox_spacing_set(self->w.custom_box, vec2_(4));
    WObj_padding_ref(self->w.custom_box)->x = 2;
}

struct custom_btn_context {
    XViewText *self;
    char *text;
    bool replace;
};

O_STATIC
void on_custom_btn_clicked(oobj obj)
{
    struct custom_btn_context *C = o_user(obj);
    if (C->replace) {
        OArray_clear(C->self->text_array);
        C->self->cursor = 0;
    }
    for (char *it = C->text; *it; it++) {
        if (o_num(C->self->text_array) < C->self->max_chars
            && C->self->cursor >= 0
            && C->self->cursor <= o_num(C->self->text_array)) {
            OArray_push_at(C->self->text_array, C->self->cursor, it);
            C->self->cursor++;
        }
    }
}

void XViewText_custom_replace(oobj obj, const char *text, const vec4 *opt_color)
{
    OObj_assert(obj, XViewText);
    XViewText *self = obj;

    oobj btn = WBtn_new(self->w.custom_box);
    WBtn_style_set(btn, WBtn_DEF);
    oobj btn_text = WText_new(btn, text);
    WObj_padding_set(btn_text, vec4_(2));
    struct custom_btn_context *C = o_user_set(btn, o_new0(btn, *C, 1));
    C->self = self;
    C->text = o_str_clone(btn, text);
    C->replace = true;
    WBtn_auto_mode_set(btn, WBtn_AUTO_CLICKED);
    WBtn_auto_event_set(btn, on_custom_btn_clicked);
    if(opt_color) {
        WBtn_color_set(btn, *opt_color);
    }
}

void XViewText_custom_append(oobj obj, const char *text, const vec4 *opt_color)
{
    OObj_assert(obj, XViewText);
    XViewText *self = obj;

    oobj btn = WBtn_new(self->w.custom_box);
    WBtn_style_set(btn, WBtn_FLAT_ROUND);
    WObj_padding_ref(btn)->y = 1;
    oobj btn_text = WText_new(btn, text);
    WObj_padding_set(btn_text, vec4_(2));
    struct custom_btn_context *C = o_user_set(btn, o_new0(btn, *C, 1));
    C->self = self;
    C->text = o_str_clone(btn, text);
    C->replace = false;
    WBtn_auto_mode_set(btn, WBtn_AUTO_CLICKED);
    WBtn_auto_event_set(btn, on_custom_btn_clicked);
    if(opt_color) {
        WBtn_color_set(btn, *opt_color);
    }
}
