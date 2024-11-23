#include "x/XViewKeys.h"
#include "o/OObj_builder.h"
#include "o/str.h"
#include "o/clipboard.h"
#include "o/OStreamArray.h"
#include "o/OJoin.h"
#include "o/OArray.h"
#include "r/RCam.h"
#include "r/RTex.h"
#include "a/ADefer.h"
#include "a/input.h"
#include "a/app.h"
#include "w/WTheme.h"
#include "w/WStyle.h"
#include "w/WGrid.h"
#include "w/WBtn.h"
#include "w/WIcon.h"
#include "w/WText.h"
#include "w/WAlign.h"

#define O_LOG_LIB "x"
#include "o/log.h"

#define HOLDABLE_START_TIME 0.66
#define HOLDABLE_NEXT_TIME 0.1

// 64 height
static const vec2 MIN_UNITS_SIZE = {{120, 64}};

static const vec4 CTRL_COLOR = {{0.7, 0.8, 1.0, 1.0}};


struct btn_context {
    XViewKeys *self;
    char *ascii;
    float holdable_next;
};

// forward pages
O_STATIC
void page_ascii_txt(XViewKeys *self);

O_STATIC
void page_ascii_alt_first(XViewKeys *self);

O_STATIC
void page_ascii_alt_second(XViewKeys *self);



O_STATIC
void deferred_repage_ascii(oobj defer)
{
    XViewKeys *self = o_user(defer);
    page_ascii_txt(self);
}
O_STATIC
void deferred_repage_alt_first(oobj defer)
{
    XViewKeys *self = o_user(defer);
    page_ascii_alt_first(self);
}
O_STATIC
void deferred_repage_alt_second(oobj defer)
{
    XViewKeys *self = o_user(defer);
    page_ascii_alt_second(self);
}


O_STATIC
void key_stream_ctrl(oobj btn)
{
    struct btn_context *C = o_user(btn);
    OStream_print(C->self->stream, C->ascii);
}

O_STATIC
void key_stream_acsii(oobj btn)
{
    key_stream_ctrl(btn);
    struct btn_context *C = o_user(btn);
    bool repage = false;
    if(C->self->ascii_upper && C->self->ascii_single_shift) {
        C->self->ascii_upper = false;
        repage = true;
    }
    C->self->ascii_single_shift = false;
    if(repage) {
        oobj defer = ADefer_new(C->self, deferred_repage_ascii);
        o_user_set(defer, C->self);
    }
}

O_STATIC
void key_stream_paste(oobj btn)
{
    struct btn_context *C = o_user(btn);
    char *paste = o_clipboard_get(btn);
    if(paste) {
        OStream_print(C->self->stream, paste);
        o_free(btn, paste);
    }
}

O_STATIC
void key_shift(oobj btn)
{
    struct btn_context *C = o_user(btn);
    OObj__event_fn defer_fn;
    if(C->self->ascii_txt) {
        defer_fn = deferred_repage_ascii;
        if(!C->self->ascii_upper) {
            C->self->ascii_single_shift = true;
            C->self->ascii_upper = true;
        } else {
            if(!C->self->ascii_single_shift) {
                C->self->ascii_upper = false;
            }
            C->self->ascii_single_shift = false;
        }
    } else {
        C->self->ascii_alt_first_page = !C->self->ascii_alt_first_page;
        if(C->self->ascii_alt_first_page) {
            defer_fn = deferred_repage_alt_first;
        } else {
            defer_fn = deferred_repage_alt_second;
        }
    }

    oobj defer = ADefer_new(C->self, defer_fn);
    o_user_set(defer, C->self);
}


O_STATIC
void key_alt(oobj btn)
{
    struct btn_context *C = o_user(btn);
    OObj__event_fn defer_fn;

    C->self->ascii_txt = !C->self->ascii_txt;
    if(C->self->ascii_txt) {
        defer_fn = deferred_repage_ascii;
    } else {
        if(C->self->ascii_alt_first_page) {
            defer_fn = deferred_repage_alt_first;
        } else {
            defer_fn = deferred_repage_alt_second;
        }
    }

    oobj defer = ADefer_new(C->self, defer_fn);
    o_user_set(defer, C->self);

}

struct add_btn_res {
    oobj btn;
    oobj align;
};


O_STATIC
struct add_btn_res add_btn(XViewKeys *self, int c, int r, int cspan)
{
    oobj btn = WBtn_new(self->gui);
    WBtn_style_set(btn, WBtn_FLAT_ROUND);
    WGrid_child_cell_set(btn, ivec4_(c, r, cspan, 1));
    WBtn_auto_mode_set(btn, WBtn_AUTO_CLICKED);
    oobj align = WAlign_new(btn);
    WAlign_align_set(align, WAlign_CENTER, WAlign_CENTER);
    return (struct add_btn_res) {btn, align};
}


O_STATIC
void btn_post_event(oobj btn)
{
    struct btn_context *C = o_user(btn);
    if(WBtn_is_pressed(btn)) {
        C->holdable_next -= a_app_dt();
        while(C->holdable_next<0) {
            C->holdable_next += HOLDABLE_NEXT_TIME;

            // calling the auto_event function, which is protected
            WBtn *b = btn;
            b->auto_event(b);
        }
    } else {
        C->holdable_next = HOLDABLE_START_TIME;
    }
}

O_STATIC
void btn_register_holdable(oobj btn)
{
    struct btn_context *C = o_user(btn);
    C->holdable_next = HOLDABLE_START_TIME;
    WObj_update_event_fn_set(btn, btn_post_event);
}

// forwarded...
void page_ascii_txt(XViewKeys *self)
{
    self->ascii_txt = true;

    bool upper = self->ascii_upper;

    o_del(self->gui);
    WTheme_reupdate_set(self->theme, true);
    self->gui = WGrid_new(self, 10, 4, vec2_(-1, -1));
    vecn_set(WGrid_col_weights(self->gui), 1, WGrid_cols(self->gui));
    vecn_set(WGrid_row_weights(self->gui), 1, WGrid_rows(self->gui));
    WGrid_align_set(self->gui, WGrid_align_FIT, WGrid_align_FIT);

    oobj style = WObj_style(self->gui);
    WStyle_text_color_set(style, R_WHITE);

    for (int c = 0; c < 10; c++) {
        struct add_btn_res res = add_btn(self, c, 0, 1);
        char *text = o_new0(res.btn, char, 2);
        text[0] = "qwertzuiop"[c];
        if (upper) {
            o_str_toupper_this(text);
        }
        oobj chr = WText_new(res.align, text);
        struct btn_context *C = o_user_set(res.btn, o_new0(res.btn, *C, 1));
        C->self = self;
        C->ascii = text;
        WBtn_auto_event_set(res.btn, key_stream_acsii);
        btn_register_holdable(res.btn);
    }

    for (int c = 0; c < 10; c++) {
        struct add_btn_res res = add_btn(self, c, 1, 1);
        char *text = o_new0(res.btn, char, 2);
        text[0] = "asdfghjkl_"[c];
        if (upper) {
            o_str_toupper_this(text);
        }
        oobj chr = WText_new(res.align, text);
        struct btn_context *C = o_user_set(res.btn, o_new0(res.btn, *C, 1));
        C->self = self;
        C->ascii = text;
        WBtn_auto_event_set(res.btn, key_stream_acsii);
        btn_register_holdable(res.btn);
    }

    // shift
    {
        struct add_btn_res res = add_btn(self, 0, 2, 1);
        enum WTheme_indices idx = WTheme_ICON_SHIFT;
        if(upper) {
            idx = WTheme_ICON_SHIFT_INV;
            if(self->ascii_single_shift) {
                idx = WTheme_ICON_CAPS;
            }
        }
        oobj icon = WIcon_new(res.align, idx);
        WIcon_color_set(icon, CTRL_COLOR);
        struct btn_context *C = o_user_set(res.btn, o_new0(res.btn, *C, 1));
        C->self = self;
        WBtn_auto_event_set(res.btn, key_shift);
    }

    for (int c = 0; c < 7; c++) {
        struct add_btn_res res = add_btn(self, 1 + c, 2, 1);
        char *text = o_new0(res.btn, char, 2);
        text[0] = "yxcvbnm"[c];
        if (upper) {
            o_str_toupper_this(text);
        }
        oobj chr = WText_new(res.align, text);
        struct btn_context *C = o_user_set(res.btn, o_new0(res.btn, *C, 1));
        C->self = self;
        C->ascii = text;
        WBtn_auto_event_set(res.btn, key_stream_acsii);
        btn_register_holdable(res.btn);
    }

    // backspace
    {
        struct add_btn_res res = add_btn(self, 8, 2, 2);
        oobj icon = WIcon_new(res.align, WTheme_ICON_LEFT);
        WIcon_color_set(icon, CTRL_COLOR);
        char *text = o_new0(res.btn, char, 2);
        text[0] = '\b';
        struct btn_context *C = o_user_set(res.btn, o_new0(res.btn, *C, 1));
        C->self = self;
        C->ascii = text;
        WBtn_auto_event_set(res.btn, key_stream_ctrl);
        btn_register_holdable(res.btn);
    }

    // alt
    {
        struct add_btn_res res = add_btn(self, 0, 3, 2);
        oobj chr = WText_new(res.align, "ALT");
        WText_color_set(chr, CTRL_COLOR);
        struct btn_context *C = o_user_set(res.btn, o_new0(res.btn, *C, 1));
        C->self = self;
        WBtn_auto_event_set(res.btn, key_alt);
    }

    // space
    {
        struct add_btn_res res = add_btn(self, 2, 3, 7);
        char *text = o_new0(res.btn, char, 2);
        text[0] = ' ';
        struct btn_context *C = o_user_set(res.btn, o_new0(res.btn, *C, 1));
        C->self = self;
        C->ascii = text;
        WBtn_auto_event_set(res.btn, key_stream_ctrl);
        btn_register_holdable(res.btn);
    }

    // enter
    {
        struct add_btn_res res = add_btn(self, 9, 3, 1);
        oobj icon = WIcon_new(res.align, WTheme_ICON_ENTER);
        WIcon_color_set(icon, CTRL_COLOR);
        char *text = o_new0(res.btn, char, 2);
        text[0] = '\n';
        struct btn_context *C = o_user_set(res.btn, o_new0(res.btn, *C, 1));
        C->self = self;
        C->ascii = text;
        WBtn_auto_event_set(res.btn, key_stream_ctrl);
        btn_register_holdable(res.btn);
    }
}

O_STATIC
void page_ascii_alt_special_keys(XViewKeys *self)
{

    // cursor up
    {
        struct add_btn_res res = add_btn(self, 8, 2, 1);
        oobj icon = WIcon_new(res.align, WTheme_ICON_DIR_UP);
        WIcon_color_set(icon, CTRL_COLOR);
        char *text = o_str_clone(res.btn, "\x1B[A");
        struct btn_context *C = o_user_set(res.btn, o_new0(res.btn, *C, 1));
        C->self = self;
        C->ascii = text;
        WBtn_auto_event_set(res.btn, key_stream_ctrl);
        btn_register_holdable(res.btn);
    }

    // backspace
    {
        struct add_btn_res res = add_btn(self, 9, 2, 1);
        oobj icon = WIcon_new(res.align, WTheme_ICON_LEFT);
        WIcon_color_set(icon, CTRL_COLOR);
        char *text = o_new0(res.btn, char, 2);
        text[0] = '\b';
        struct btn_context *C = o_user_set(res.btn, o_new0(res.btn, *C, 1));
        C->self = self;
        C->ascii = text;
        WBtn_auto_event_set(res.btn, key_stream_ctrl);
        btn_register_holdable(res.btn);
    }

    // alt
    {
        struct add_btn_res res = add_btn(self, 0, 3, 2);
        oobj chr = WText_new(res.align, "TXT");
        WText_color_set(chr, CTRL_COLOR);
        struct btn_context *C = o_user_set(res.btn, o_new0(res.btn, *C, 1));
        C->self = self;
        WBtn_auto_event_set(res.btn, key_alt);
        btn_register_holdable(res.btn);
    }

    int paste_cols = 0;
    int space_cols = 4;
    if(o_clipboard_system_available()) {
        paste_cols = 3;
        space_cols -= paste_cols;
        struct add_btn_res res = add_btn(self, 2, 3, paste_cols);
        oobj chr = WText_new(res.align, "PASTE");
        WText_color_set(chr, CTRL_COLOR);
        struct btn_context *C = o_user_set(res.btn, o_new0(res.btn, *C, 1));
        C->self = self;
        WBtn_auto_event_set(res.btn, key_stream_paste);
    }

    // space
    {
        struct add_btn_res res = add_btn(self, 2+paste_cols, 3, space_cols);
        char *text = o_new0(res.btn, char, 2);
        text[0] = ' ';
        struct btn_context *C = o_user_set(res.btn, o_new0(res.btn, *C, 1));
        C->self = self;
        C->ascii = text;
        WBtn_auto_event_set(res.btn, key_stream_ctrl);
        btn_register_holdable(res.btn);
    }

    // enter
    {
        struct add_btn_res res = add_btn(self, 6, 3, 1);
        oobj icon = WIcon_new(res.align, WTheme_ICON_ENTER);
        WIcon_color_set(icon, CTRL_COLOR);
        char *text = o_new0(res.btn, char, 2);
        text[0] = '\n';
        struct btn_context *C = o_user_set(res.btn, o_new0(res.btn, *C, 1));
        C->self = self;
        C->ascii = text;
        WBtn_auto_event_set(res.btn, key_stream_ctrl);
        btn_register_holdable(res.btn);
    }

    // cursor left
    {
        struct add_btn_res res = add_btn(self, 7, 3, 1);
        oobj icon = WIcon_new(res.align, WTheme_ICON_DIR_LEFT);
        WIcon_color_set(icon, CTRL_COLOR);
        char *text = o_str_clone(res.btn, "\x1B[D");
        struct btn_context *C = o_user_set(res.btn, o_new0(res.btn, *C, 1));
        C->self = self;
        C->ascii = text;
        WBtn_auto_event_set(res.btn, key_stream_ctrl);
        btn_register_holdable(res.btn);
    }

    // cursor down
    {
        struct add_btn_res res = add_btn(self, 8, 3, 1);
        oobj icon = WIcon_new(res.align, WTheme_ICON_DIR_DOWN);
        WIcon_color_set(icon, CTRL_COLOR);
        char *text = o_str_clone(res.btn, "\x1B[B");
        struct btn_context *C = o_user_set(res.btn, o_new0(res.btn, *C, 1));
        C->self = self;
        C->ascii = text;
        WBtn_auto_event_set(res.btn, key_stream_ctrl);
        btn_register_holdable(res.btn);
    }

    // cursor right
    {
        struct add_btn_res res = add_btn(self, 9, 3, 1);
        oobj icon = WIcon_new(res.align, WTheme_ICON_DIR_RIGHT);
        WIcon_color_set(icon, CTRL_COLOR);
        char *text = o_str_clone(res.btn, "\x1B[C");
        struct btn_context *C = o_user_set(res.btn, o_new0(res.btn, *C, 1));
        C->self = self;
        C->ascii = text;
        WBtn_auto_event_set(res.btn, key_stream_ctrl);
        btn_register_holdable(res.btn);
    }
}

// forwarded...
void page_ascii_alt_first(XViewKeys *self)
{
    self->ascii_txt = false;
    self->ascii_alt_first_page = true;
    self->ascii_single_shift = false;

    o_del(self->gui);
    WTheme_reupdate_set(self->theme, true);
    self->gui = WGrid_new(self, 10, 4, vec2_(-1, -1));
    vecn_set(WGrid_col_weights(self->gui), 1, WGrid_cols(self->gui));
    vecn_set(WGrid_row_weights(self->gui), 1, WGrid_rows(self->gui));
    WGrid_align_set(self->gui, WGrid_align_FIT, WGrid_align_FIT);

    oobj style = WObj_style(self->gui);
    WStyle_text_color_set(style, R_WHITE);

    for (int c = 0; c < 10; c++) {
        struct add_btn_res res = add_btn(self, c, 0, 1);
        char *text = o_new0(res.btn, char, 2);
        text[0] = "1234567890"[c];
        oobj chr = WText_new(res.align, text);
        struct btn_context *C = o_user_set(res.btn, o_new0(res.btn, *C, 1));
        C->self = self;
        C->ascii = text;
        WBtn_auto_event_set(res.btn, key_stream_acsii);
        btn_register_holdable(res.btn);
    }

    for (int c = 0; c < 10; c++) {
        struct add_btn_res res = add_btn(self, c, 1, 1);
        char *text = o_new0(res.btn, char, 2);
        text[0] = "+-*/&={}()"[c];
        oobj chr = WText_new(res.align, text);
        struct btn_context *C = o_user_set(res.btn, o_new0(res.btn, *C, 1));
        C->self = self;
        C->ascii = text;
        WBtn_auto_event_set(res.btn, key_stream_acsii);
        btn_register_holdable(res.btn);
    }

    // shift
    {
        struct add_btn_res res = add_btn(self, 0, 2, 1);
        enum WTheme_indices idx = WTheme_ICON_ROMAN_1;
        oobj icon = WIcon_new(res.align, idx);
        WIcon_color_set(icon, CTRL_COLOR);
        struct btn_context *C = o_user_set(res.btn, o_new0(res.btn, *C, 1));
        C->self = self;
        WBtn_auto_event_set(res.btn, key_shift);
    }

    for (int c = 0; c < 7; c++) {
        struct add_btn_res res = add_btn(self, 1 + c, 2, 1);
        char *text = o_new0(res.btn, char, 2);
        text[0] = "<>[].,;"[c];
        oobj chr = WText_new(res.align, text);
        struct btn_context *C = o_user_set(res.btn, o_new0(res.btn, *C, 1));
        C->self = self;
        C->ascii = text;
        WBtn_auto_event_set(res.btn, key_stream_acsii);
        btn_register_holdable(res.btn);
    }

    page_ascii_alt_special_keys(self);
}

// forwarded...
void page_ascii_alt_second(XViewKeys *self)
{
    self->ascii_txt = false;
    self->ascii_alt_first_page = false;
    self->ascii_single_shift = false;

    o_del(self->gui);
    WTheme_reupdate_set(self->theme, true);
    self->gui = WGrid_new(self, 10, 4, vec2_(-1, -1));
    vecn_set(WGrid_col_weights(self->gui), 1, WGrid_cols(self->gui));
    vecn_set(WGrid_row_weights(self->gui), 1, WGrid_rows(self->gui));
    WGrid_align_set(self->gui, WGrid_align_FIT, WGrid_align_FIT);

    oobj style = WObj_style(self->gui);
    WStyle_text_color_set(style, R_WHITE);

    for (int c = 0; c < 10; c++) {
        struct add_btn_res res = add_btn(self, c, 0, 1);
        char *text = o_new0(res.btn, char, 2);
        text[0] = "1234567890"[c];
        oobj chr = WText_new(res.align, text);
        struct btn_context *C = o_user_set(res.btn, o_new0(res.btn, *C, 1));
        C->self = self;
        C->ascii = text;
        WBtn_auto_event_set(res.btn, key_stream_acsii);
        btn_register_holdable(res.btn);
    }

    // tab
    {
        struct add_btn_res res = add_btn(self, 0, 1, 1);
        oobj icon = WIcon_new(res.align, WTheme_ICON_TAB);
        WIcon_color_set(icon, CTRL_COLOR);
        char *text = o_new0(res.btn, char, 2);
        text[0] = '\t';
        struct btn_context *C = o_user_set(res.btn, o_new0(res.btn, *C, 1));
        C->self = self;
        C->ascii = text;
        WBtn_auto_event_set(res.btn, key_stream_ctrl);
        btn_register_holdable(res.btn);
    }

    // ->
    {
        struct add_btn_res res = add_btn(self, 1, 1, 2);
        char *text = o_new0(res.btn, char, 3);
        text[0] = '-';
        text[1] = '>';
        oobj chr = WText_new(res.align, "- >");
        struct btn_context *C = o_user_set(res.btn, o_new0(res.btn, *C, 1));
        C->self = self;
        C->ascii = text;
        WBtn_auto_event_set(res.btn, key_stream_acsii);
        btn_register_holdable(res.btn);
    }

    for (int c = 0; c < 7; c++) {
        struct add_btn_res res = add_btn(self, c+3, 1, 1);
        char *text = o_new0(res.btn, char, 2);
        text[0] = "$@\\^`~|"[c];
        oobj chr = WText_new(res.align, text);
        struct btn_context *C = o_user_set(res.btn, o_new0(res.btn, *C, 1));
        C->self = self;
        C->ascii = text;
        WBtn_auto_event_set(res.btn, key_stream_acsii);
        btn_register_holdable(res.btn);
    }

    // shift
    {
        struct add_btn_res res = add_btn(self, 0, 2, 1);
        enum WTheme_indices idx = WTheme_ICON_ROMAN_2;
        oobj icon = WIcon_new(res.align, idx);
        WIcon_color_set(icon, CTRL_COLOR);
        struct btn_context *C = o_user_set(res.btn, o_new0(res.btn, *C, 1));
        C->self = self;
        WBtn_auto_event_set(res.btn, key_shift);
    }

    for (int c = 0; c < 7; c++) {
        struct add_btn_res res = add_btn(self, 1 + c, 2, 1);
        char *text = o_new0(res.btn, char, 2);
        text[0] = "\"\'#%!?:"[c];
        oobj chr = WText_new(res.align, text);
        struct btn_context *C = o_user_set(res.btn, o_new0(res.btn, *C, 1));
        C->self = self;
        C->ascii = text;
        WBtn_auto_event_set(res.btn, key_stream_acsii);
        btn_register_holdable(res.btn);
    }

    page_ascii_alt_special_keys(self);
}


O_STATIC
void page_num(XViewKeys *self)
{
    o_del(self->gui);
    WTheme_reupdate_set(self->theme, true);
    self->gui = WGrid_new(self, 5, 4, vec2_(-1, -1));
    vecn_set(WGrid_col_weights(self->gui), 1, WGrid_cols(self->gui));
    vecn_set(WGrid_row_weights(self->gui), 1, WGrid_rows(self->gui));
    WGrid_align_set(self->gui, WGrid_align_FIT, WGrid_align_FIT);

    oobj style = WObj_style(self->gui);
    WStyle_text_color_set(style, R_WHITE);

    for (int c = 0; c < 5; c++) {
        struct add_btn_res res = add_btn(self, c, 0, 1);
        char *text = o_new0(res.btn, char, 2);
        text[0] = "789-*"[c];
        oobj chr = WText_new(res.align, text);
        struct btn_context *C = o_user_set(res.btn, o_new0(res.btn, *C, 1));
        C->self = self;
        C->ascii = text;
        WBtn_auto_event_set(res.btn, key_stream_acsii);
        btn_register_holdable(res.btn);
    }

    for (int c = 0; c < 5; c++) {
        struct add_btn_res res = add_btn(self, c, 1, 1);
        char *text = o_new0(res.btn, char, 2);
        text[0] = "456+/"[c];
        oobj chr = WText_new(res.align, text);
        struct btn_context *C = o_user_set(res.btn, o_new0(res.btn, *C, 1));
        C->self = self;
        C->ascii = text;
        WBtn_auto_event_set(res.btn, key_stream_acsii);
        btn_register_holdable(res.btn);
    }

    for (int c = 0; c < 3; c++) {
        struct add_btn_res res = add_btn(self, c, 2, 1);
        char *text = o_new0(res.btn, char, 2);
        text[0] = "123"[c];
        oobj chr = WText_new(res.align, text);
        struct btn_context *C = o_user_set(res.btn, o_new0(res.btn, *C, 1));
        C->self = self;
        C->ascii = text;
        WBtn_auto_event_set(res.btn, key_stream_acsii);
        btn_register_holdable(res.btn);
    }


    // cursor up
    {
        struct add_btn_res res = add_btn(self, 3, 2, 1);
        oobj icon = WIcon_new(res.align, WTheme_ICON_DIR_UP);
        WIcon_color_set(icon, CTRL_COLOR);
        char *text = o_str_clone(res.btn, "\x1B[A");
        struct btn_context *C = o_user_set(res.btn, o_new0(res.btn, *C, 1));
        C->self = self;
        C->ascii = text;
        WBtn_auto_event_set(res.btn, key_stream_ctrl);
        btn_register_holdable(res.btn);
    }

    // backspace
    {
        struct add_btn_res res = add_btn(self, 4, 2, 1);
        oobj icon = WIcon_new(res.align, WTheme_ICON_LEFT);
        WIcon_color_set(icon, CTRL_COLOR);
        char *text = o_new0(res.btn, char, 2);
        text[0] = '\b';
        struct btn_context *C = o_user_set(res.btn, o_new0(res.btn, *C, 1));
        C->self = self;
        C->ascii = text;
        WBtn_auto_event_set(res.btn, key_stream_ctrl);
        btn_register_holdable(res.btn);
    }

    for (int c = 0; c < 2; c++) {
        struct add_btn_res res = add_btn(self, c, 3, 1);
        char *text = o_new0(res.btn, char, 2);
        text[0] = "0."[c];
        oobj chr = WText_new(res.align, text);
        struct btn_context *C = o_user_set(res.btn, o_new0(res.btn, *C, 1));
        C->self = self;
        C->ascii = text;
        WBtn_auto_event_set(res.btn, key_stream_acsii);
        btn_register_holdable(res.btn);
    }
    // cursor left
    {
        struct add_btn_res res = add_btn(self, 2, 3, 1);
        oobj icon = WIcon_new(res.align, WTheme_ICON_DIR_LEFT);
        WIcon_color_set(icon, CTRL_COLOR);
        char *text = o_str_clone(res.btn, "\x1B[D");
        struct btn_context *C = o_user_set(res.btn, o_new0(res.btn, *C, 1));
        C->self = self;
        C->ascii = text;
        WBtn_auto_event_set(res.btn, key_stream_ctrl);
        btn_register_holdable(res.btn);
    }

    // cursor down
    {
        struct add_btn_res res = add_btn(self, 3, 3, 1);
        oobj icon = WIcon_new(res.align, WTheme_ICON_DIR_DOWN);
        WIcon_color_set(icon, CTRL_COLOR);
        char *text = o_str_clone(res.btn, "\x1B[B");
        struct btn_context *C = o_user_set(res.btn, o_new0(res.btn, *C, 1));
        C->self = self;
        C->ascii = text;
        WBtn_auto_event_set(res.btn, key_stream_ctrl);
        btn_register_holdable(res.btn);
    }

    // cursor right
    {
        struct add_btn_res res = add_btn(self, 4, 3, 1);
        oobj icon = WIcon_new(res.align, WTheme_ICON_DIR_RIGHT);
        WIcon_color_set(icon, CTRL_COLOR);
        char *text = o_str_clone(res.btn, "\x1B[C");
        struct btn_context *C = o_user_set(res.btn, o_new0(res.btn, *C, 1));
        C->self = self;
        C->ascii = text;
        WBtn_auto_event_set(res.btn, key_stream_ctrl);
        btn_register_holdable(res.btn);
    }

}

//
// public
//

XViewKeys *XViewKeys_init(oobj obj, oobj parent)
{

    AView *super = obj;
    XViewKeys *self = obj;
    o_clear(self, sizeof *self, 1);

    AView_init(obj, parent, XViewKeys__v_setup, XViewKeys__v_update, XViewKeys__v_render);
    OObj_id_set(self, XViewKeys_ID);

    // disable auto units, so we have our own minimal height (and width...)
    AView_cam_units_auto_set(super, false);
    RCam_min_units_size_set(AView_cam(super), MIN_UNITS_SIZE);

    self->theme = WTheme_new_tiny(self);

    self->real_key_stream = a_input_key_stream(self, NULL);

    self->stream = OStreamArray_new(self, OArray_new_dyn(self, NULL, sizeof(char), 0, 256),
                                    true, OStreamArray_FIFO);

    self->ascii_alt_first_page = true;
    self->mode = XViewKeys_ASCII;
    // setting to a different invalid mode
    self->prev_mode = XViewKeys_NUM_MODES;

    // vfuncs

    return self;

}


//
// virtual implementations
//

void XViewKeys__v_setup(oobj view)
{

}

void XViewKeys__v_update(oobj view, oobj tex, float dt)
{
    OObj_assert(view, XViewKeys);
    XViewKeys *self = view;

    if(self->mode != self->prev_mode) {
        self->prev_mode = self->mode;
        if(self->mode == XViewKeys_ASCII) {
            page_ascii_txt(self);
        } else {
            page_num(self);
        }
    }

    struct r_proj *proj = RTex_proj(tex);
    vec2 lt = vec2_(r_proj_left_int(proj), r_proj_top(proj));
    vec2 size = vec2_cast_int(r_proj_size_int(proj).v);

    WTheme_update(self->theme, self->gui, lt, size, a_pointer);

    // add real keyboard stream
    bool escape_dir = false;
    bool escape = false;
    for(;;) {
        char read;
        osize num = OStream_read_try(self->real_key_stream, &read, sizeof(char), 1);
        if(num == 0) {
            break;
        }
        if(self->mode == XViewKeys_NUM) {
            if(escape_dir) {
                escape = escape_dir = false;
                OStream_print(self->stream, "\x1B[");
                OStream_write(self->stream, &read, 1, 1);
            }
            if(escape) {
                escape = false;
                if(read == '[') {
                    escape_dir = true;
                }
                continue;
            }
            if(read == '\x1B') {
                escape = true;
                continue;
            }
            if(o_str_find_char("0123456789.+-*\b", read) < 0) {
                continue;
            }
        }
        OStream_write(self->stream, &read, 1, 1);
    }

}

void XViewKeys__v_render(oobj view, oobj tex, float dt)
{
    OObj_assert(view, XViewKeys);
    XViewKeys *self = view;

    RTex_clear(tex, R_GRAY_X(0.2));

    WTheme_render(self->theme, tex);

}

//
// object functions:
//

