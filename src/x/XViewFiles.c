#include "x/XViewFiles.h"
#include "o/OObj_builder.h"
#include "o/str.h"
#include "o/file.h"
#include "o/OArray.h"
#include "r/RTex.h"
#include "r/RCam.h"
#include "a/AScene.h"
#include "w/WTheme.h"
#include "w/WGrid.h"
#include "w/WBox.h"
#include "w/WColor.h"
#include "w/WBtn.h"
#include "w/WIcon.h"
#include "w/WFrame.h"
#include "w/WText.h"
#include "w/WTextShadow.h"
#include "w/WView.h"
#include "w/WClick.h"
#include "w/WPane.h"
#include "w/WWindowDialog.h"
#include "w/WAlign.h"
#include "x/XViewText.h"
#include "x/viewtext.h"


#define O_LOG_LIB "x"

#include "o/log.h"


#define PAGE_ENTRIES 256


O_STATIC
oobj dir_stack_clone(oobj parent, oobj stack)
{
    oobj clone = OArray_new_dyn(parent, NULL, sizeof(char *), o_num(stack), OArray_capacity(stack));
    for (osize i = 0; i < o_num(stack); i++) {
        char **src = o_at(stack, i);
        char **dst = o_at(clone, i);
        *dst = o_str_clone(clone, *src);
    }
    return clone;
}

O_STATIC
void dir_clicked(oobj btn)
{
    XViewFiles *self = o_user(btn);
    struct oobj_opt text = OObj_find(btn, WText, NULL, oi32_MAX);
    assert(text.o);
    const char *dir = WText_text(text.o);
    XViewFiles_dir_stack_push(self, dir);
}

O_STATIC
void file_clicked(oobj btn)
{
    XViewFiles *self = o_user(btn);
    struct oobj_opt text = OObj_find(btn, WText, NULL, oi32_MAX);
    assert(text.o);
    const char *file = WText_text(text.o);
    WText_text_set(self->w.entry_file, file);
}

O_STATIC
void files_page_prev(oobj btn)
{
    XViewFiles *self = o_user(btn);
    self->w.f.page--;
}

O_STATIC
void files_page_next(oobj btn)
{
    XViewFiles *self = o_user(btn);
    self->w.f.page++;
}


O_STATIC
void append_page_btn(XViewFiles *self, oobj box, int page, int pages, bool prev)
{
    oobj btn = WBtn_new(box);
    WBtn_style_set(btn, WBtn_DEF_ROUND);
    WBtn_color_set(btn, vec4_(0.5, 0.4, 0.4, 1.0));
    WBtn_auto_mode_set(btn, WBtn_AUTO_CLICKED);
    o_user_set(btn, self);
    WBtn_auto_event_set(btn, prev ? files_page_prev : files_page_next);
    oobj h = WBox_new(btn, WBox_LAYOUT_H);
    WBox_spacing_set(h, vec2_(4));
    WIcon_new(h, prev ? WTheme_ICON_UP : WTheme_ICON_DOWN);
    char buf[32];
    o_strf_buf(buf, "PAGE %i / %i", page, pages);
    WText_new(h, buf);
}

O_STATIC
void append_dir(XViewFiles *self, oobj box, const char *name)
{
    oobj btn = WBtn_new(box);
    WBtn_style_set(btn, WBtn_FLAT);
    WBtn_auto_mode_set(btn, WBtn_AUTO_CLICKED);
    o_user_set(btn, self);
    WBtn_auto_event_set(btn, dir_clicked);
    oobj h = WBox_new(btn, WBox_LAYOUT_H);
    WBox_spacing_set(h, vec2_(4));
    WIcon_new(h, WTheme_ICON_LOAD);
    WText_new(h, name);
}

O_STATIC
void append_file(XViewFiles *self, oobj box, const char *name)
{
    oobj btn = WBtn_new(box);
    WBtn_style_set(btn, WBtn_FLAT);
    WBtn_color_set(btn, vec4_(0.4, 0.4, 0.5, 1.0));
    o_user_set(btn, self);
    WBtn_auto_event_set(btn, file_clicked);
    WBtn_auto_mode_set(btn, WBtn_AUTO_CLICKED);
    oobj h = WBox_new(btn, WBox_LAYOUT_H);
    WBox_spacing_set(h, vec2_(4));

    enum WTheme_indices icon_idx = WTheme_ICON_FILE;

    // choose icon by file extension:
    char *ending = o_str_tolower(self, name);
    if (o_str_ends(ending, ".txt")
        || o_str_ends(ending, ".json")) {
        icon_idx = WTheme_ICON_FILE_TXT;
    } else if (o_str_ends(ending, ".wav")
               || o_str_ends(ending, ".mp3")
               || o_str_ends(ending, ".ogg")) {
        icon_idx = WTheme_ICON_FILE_MUS;
    } else if (o_str_ends(ending, ".jpg")
               || o_str_ends(ending, ".jpeg")
               || o_str_ends(ending, ".bmp")
               || o_str_ends(ending, ".png")) {
        icon_idx = WTheme_ICON_FILE_IMG;
    }
    o_free(self, ending);

    WIcon_new(h, icon_idx);
    WText_new(h, name);
}


O_STATIC
void files_update(oobj view, oobj tex, float dt)
{
    XViewFiles *self = o_user(view);
    char *dir = XViewFiles_dir(self);

    bool dir_changed = !o_str_equals(dir, self->w.f.last_dir);
    bool page_changed = self->w.f.page != self->w.f.last_page;
    self->w.f.last_page = self->w.f.page;

    bool recreate = (!self->w.f.dialog) && (dir_changed || page_changed);

    if (!recreate) {
        // keep gui
        o_free(self, dir);
    } else {
        // recreate gui
        o_free(self, self->w.f.last_dir);
        self->w.f.last_dir = dir;

        o_del(self->w.f.gui);

        if (dir_changed) {
            self->w.f.page = self->w.f.last_page = 0;
        }

        u_scroll_pos_set(&self->w.scroll, vec2_(-1000, 1000));

        oobj gui = WObj_new(self);
        self->w.f.gui = gui;

        oobj box = WBox_new(gui, WBox_LAYOUT_V);
        WBox_spacing_set(box, vec2_(0));

        if (dir_changed) {
            o_del(self->w.cached_dirs);
            o_del(self->w.cached_files);
            self->w.cached_dirs = o_file_list(self, self->w.f.last_dir, o_file_list_DIRS, NULL);
            self->w.cached_files = o_file_list(self, self->w.f.last_dir, o_file_list_FILES, NULL);
        }
        oobj dirs = self->w.cached_dirs;
        oobj files = self->w.cached_files;

        osize entries = o_num(dirs) + o_num(files);
        int pages = (int) (1 + (entries / (PAGE_ENTRIES + 1)));

        self->w.f.page = o_clamp(self->w.f.page, 0, (pages - 1));
        int page = self->w.f.page;

        // to test the dialog:
//        if(self->dir_stack_prev)
//            OArray_clear(dirs);

        // check valid
        if (o_num(dirs) > 0) {
            o_del(self->dir_stack_prev);
            self->dir_stack_prev = dir_stack_clone(self, self->dir_stack);
        } else if (self->dir_stack_prev) {
            o_del(self->dir_stack);
            self->dir_stack = dir_stack_clone(self, self->dir_stack_prev);

            oobj dialog = WWindowDialog_new(gui, "ERROR", false, true);
            self->w.f.dialog = dialog;
            WWindow_draggable_set(dialog, false);
            WWindowDialog_bg_maximize_set(dialog, true);
            WWindow_lt_set(dialog, vec2_(16, 32));
            oobj msg = WText_new(WWindowDialog_body(dialog), "ACCESS DENIED");
            WObj_padding_set(msg, vec4_(4, 8, 4));

        }

        if (page > 0) {
            // add prev page btn
            append_page_btn(self, box, page, pages, true);
        }

        osize start_entry = page * PAGE_ENTRIES;
        osize end_entry = start_entry + PAGE_ENTRIES;
        osize entry = 0;
        for (osize i = 0; i < o_num(dirs); i++, entry++) {
            if (entry >= start_entry && entry < end_entry) {
                char **file = o_at(dirs, i);
                append_dir(self, box, *file);
            }
        }
        for (osize i = 0; i < o_num(files); i++, entry++) {
            if (entry >= start_entry && entry < end_entry) {
                char **file = o_at(files, i);
                append_file(self, box, *file);
            }
        }

        if (page < pages - 1) {
            // add next page btn
            append_page_btn(self, box, page, pages, false);
        }
    }

    vec2 size = WTheme_update(self->w.f.theme, self->w.f.gui, vec2_(0), vec2_(128), a_pointer);

    if (self->w.f.dialog) {
        if (WWindowDialog_state(self->w.f.dialog) != WWindowDialog_RUNNING) {
            o_del(self->w.f.dialog);
        }
    }


    vec4 cam_rect = u_rect_new_bounds(0, size.x, -size.y, 0);
    cam_rect.zw = vec2_add(cam_rect.zw, 16);
    self->w.scroll.cam_limits_rect = cam_rect;
}

O_STATIC
void files_render(oobj view, oobj tex, float dt)
{
    XViewFiles *self = o_user(view);
    RTex_clear(tex, vec4_(0.7, 0.6, 0.5, 1.0));
    WTheme_render(self->w.f.theme, tex);
}

O_STATIC
void dir_home(oobj click)
{
    XViewFiles *self = o_user(click);
    XViewFiles_dir_stack_clear(self);
}


O_STATIC
void edit_file_ok(oobj scene)
{
    XViewFiles *self = o_user(scene);
    oobj array = XViewText_text_array(AScene_view(scene));
    char *text = OArray_data_void(array);
    if (*text == '\0') {
        // else the WText would have a 0 size and the line would shrink in its height...
        text = " ";
    }
    WText_text_set(self->w.entry_file, text);
}

O_STATIC
void edit_file_cancel(oobj scene)
{
    // noop
}

O_STATIC
void edit_file(oobj click)
{
    XViewFiles *self = o_user(click);
    oobj scene = x_viewtext_scene(self, "FILE", XViewText_DEFAULT_MAX,
                                  edit_file_ok, edit_file_cancel);
    oobj view = AScene_view(scene);
    oobj array = XViewText_text_array(view);
    OArray_append_string(array, XViewFiles_file_name(self));
    o_user_set(scene, self);
}

O_STATIC
void show_prompt(XViewFiles *self, const char *prompt)
{
    oobj dialog = WWindowDialog_new(self->w.f.gui, "CONTINUE?", true, true);
    self->w.f.prompt_dialog = dialog;
    WWindow_draggable_set(dialog, false);
    WWindowDialog_bg_maximize_set(dialog, true);
    WIcon_icon_idx_set(WWindowDialog_icon(dialog), WTheme_ICON_QUESTION);
    WWindow_lt_set(dialog, vec2_(16, 32));
    oobj msg = WText_new(WWindowDialog_body(dialog), prompt);
    WObj_padding_set(msg, vec4_(4, 8, 4));

    // scroll to top to display the dialog right...
    u_scroll_pos_set(&self->w.scroll, vec2_(-1000, 1000));
}

//
// public
//

XViewFiles *XViewFiles_init(oobj obj, oobj parent, const char *title)
{

    AView *super = obj;
    XViewFiles *self = obj;
    o_clear(self, sizeof *self, 1);

    AView_init(obj, parent, XViewFiles__v_setup, XViewFiles__v_update, XViewFiles__v_render);
    OObj_id_set(self, XViewFiles_ID);

    self->state = XViewFiles_RUNNING;

    XViewFiles_dir_home_set(self, o_file_home(o_file_home_DEFAULT));
    self->dir_stack = OArray_new_dyn(self, NULL, sizeof(char *), 0, 8);

    self->w.theme = WTheme_new_tiny(self);
    self->w.f.theme = WTheme_new_tiny(self);


    oobj gui = WObj_new(self);
    self->w.gui = gui;

    oobj box = WBox_new(gui, WBox_LAYOUT_V_WEIGHTS);
    WBox_spacing_set(box, vec2_(4));
    WObj_padding_set(box, vec4_(4));

    oobj title_align = WAlign_new(box);
    WAlign_align_h_set(title_align, WAlign_CENTER);
    oobj title_pane = WPane_new(title_align);
    WPane_style_set(title_pane, WPane_ROUND_BOTTOM);
    WPane_color_set(title_pane, vec4_(0.66, 0.66, 0.66, 1));
    self->w.title = WTextShadow_new(title_pane, title);
    WText_color_set(self->w.title, R_WHITE);
    WObj_padding_set(self->w.title, vec4_(8, 0, 8, 0));

    oobj entry_dir_frame = WFrame_new_title_shadow(box, "DIR", NULL);
    oobj entry_dir_click = WClick_new(entry_dir_frame);
    o_user_set(entry_dir_click, self);
    WClick_clicked_event_set(entry_dir_click, dir_home);
    oobj entry_dir_bg = WColor_new(entry_dir_click, vec4_(0.7, 0.7, 0.8, 1.0));
    oobj entry_dir = WText_new(entry_dir_bg, "./");
    self->w.entry_dir = entry_dir;
    WText_color_set(entry_dir, R_BLACK);
    WObj_padding_set(entry_dir, vec4_(2));

    oobj view_frame = WFrame_new(box);
    WBox_child_weight_set(view_frame, 1);
    WTextShadow_new(WFrame_label(view_frame), "FILES");
    oobj view_view = AView_new(self, NULL, files_update, files_render);
    o_user_set(view_view, self);
    oobj view = WView_new(view_frame, view_view, false);
    self->w.view = view;
    WObj_min_size_set(view, vec2_(64));

    self->w.scroll = u_scroll_new(u_scroll_XY, AView_cam(view_view));
    self->w.scroll.align_y = u_scroll_END;

    oobj entry_file_frame = WFrame_new_title_shadow(box, "FILE", NULL);
    oobj entry_file_click = WClick_new(entry_file_frame);
    o_user_set(entry_file_click, self);
    WClick_clicked_event_set(entry_file_click, edit_file);
    oobj entry_file_bg = WColor_new(entry_file_click, R_GRAY_X(0.9));
    oobj entry_file = WText_new(entry_file_bg, "hello.txt");
    self->w.entry_file = entry_file;
    WText_color_set(entry_file, R_BLACK);
    WObj_padding_set(entry_file, vec4_(2));

    oobj btn_box = WGrid_new(box, 2, 1, vec2_(-1));
    WGrid_align_set(btn_box, WGrid_align_CENTER, WGrid_align_CENTER);
    float *btn_box_weights = WGrid_col_weights(btn_box);
    vecn_set(btn_box_weights, 1, 2);

    oobj btn_cancel = WBtn_new(btn_box);
    self->w.btn_cancel = btn_cancel;
    WGrid_child_cell_set(btn_cancel, ivec4_(0, 0, 1, 1));
    oobj btn_cancel_icon = WIcon_new(btn_cancel, WTheme_ICON_CROSS);
    WIcon_color_set(btn_cancel_icon, vec4_(0.8, 0.1, 0.1, 1.0));
    WObj_padding_set(btn_cancel_icon, vec4_(8, 0, 8, 0));

    oobj btn_ok = WBtn_new(btn_box);
    self->w.btn_ok = btn_ok;
    WGrid_child_cell_set(btn_ok, ivec4_(1, 0, 1, 1));
    oobj btn_ok_icon = WIcon_new(btn_ok, WTheme_ICON_CHECK);
    WIcon_color_set(btn_ok_icon, vec4_(0.1, 0.8, 0.1, 1.0));
    WObj_padding_set(btn_ok_icon, vec4_(8, 0, 8, 0));

    // vfuncs

    return self;

}


//
// virtual implementations
//

void XViewFiles__v_setup(oobj view)
{

}

void XViewFiles__v_update(oobj view, oobj tex, float dt)
{
    OObj_assert(view, XViewFiles);
    XViewFiles *self = view;

    struct r_proj *proj = RTex_proj(tex);
    vec2 lt = vec2_(r_proj_left_int(proj), r_proj_top(proj));
    vec2 size = vec2_cast_int(r_proj_size_int(proj).v);

    char *dir = XViewFiles_dir_small(self);
    WText_text_set(self->w.entry_dir, dir);
    o_free(self, dir);


    WTheme_update(self->w.theme, self->w.gui, lt, size, a_pointer);

    if (WBtn_clicked(self->w.btn_cancel)) {
        self->state = XViewFiles_PROMPT_CANCEL;
        if (self->prompt) {
            char *prompt = self->prompt(self, false);
            if (prompt) {
                show_prompt(self, prompt);
                o_free(self, prompt);
            }
        }
    }
    if (WBtn_clicked(self->w.btn_ok)) {
        self->state = XViewFiles_PROMPT_OK;
        if (self->prompt) {
            char *prompt = self->prompt(self, true);
            if (prompt) {
                show_prompt(self, prompt);
                o_free(self, prompt);
            }
        }
    }

    if (self->w.f.prompt_dialog) {
        enum WWindowDialog_state state = WWindowDialog_state(self->w.f.prompt_dialog);
        if (state == WWindowDialog_CANCEL) {
            self->state = XViewFiles_RUNNING;
        }
        if (state != WWindowDialog_RUNNING) {
            o_del(self->w.f.prompt_dialog);
        }
    }

    if (!self->w.f.prompt_dialog) {
        bool event = false;
        if (self->state == XViewFiles_PROMPT_CANCEL) {
            self->state = XViewFiles_CANCEL;
            event = true;
        } else if (self->state == XViewFiles_PROMPT_OK) {
            self->state = XViewFiles_OK;
            event = true;
        }
        if (event && self->event) {
            self->event(self);
        }
    }
}

void XViewFiles__v_render(oobj view, oobj tex, float dt)
{
    OObj_assert(view, XViewFiles);
    XViewFiles *self = view;

    RTex_clear(tex, vec4_(0.6, 0.5, 0.4, 1.0));

    WTheme_render(self->w.theme, tex);


    self->w.scroll.view_rect = WObj_gen_rect(self->w.view);
    u_scroll_update(&self->w.scroll, dt);

    WView_render(self->w.view, tex);

}

//
// object functions:
//

void XViewFiles_dir_home_set(oobj obj, const char *home)
{
    OObj_assert(obj, XViewFiles);
    XViewFiles *self = obj;
    o_strf_buf(self->dir_home, "%s", home);
}

void XViewFiles_dir_stack_pop(oobj obj)
{
    oobj stack = XViewFiles_dir_stack(obj);
    assert(o_num(stack) > 0);
    char *pop;
    OArray_pop(stack, &pop);
    o_free(stack, pop);
}

void XViewFiles_dir_stack_clear(oobj obj)
{
    oobj stack = XViewFiles_dir_stack(obj);
    while (o_num(stack) > 0) {
        XViewFiles_dir_stack_pop(obj);
    }
}

void XViewFiles_dir_stack_push(oobj obj, const char *dir_file)
{
    oobj stack = XViewFiles_dir_stack(obj);
    if (o_str_equals(dir_file, ".")) {
        //noop
        return;
    }
    if (o_str_equals(dir_file, "..")) {
        // either remove last dir from the stack or append an ".." if its the last or empty
        bool remove = true;
        if (o_num(stack) == 0) {
            remove = false;
        } else {
            char **last = o_at(stack, o_num(stack) - 1);
            if (o_str_equals(*last, "..")) {
                remove = false;
            }
        }
        if (remove) {
            XViewFiles_dir_stack_pop(obj);
            return;
        }
    }

    // either a dir or not removed ".."
    char *clone = o_str_clone(stack, dir_file);
    OArray_push(stack, &clone);
}

char *XViewFiles_dir_small(oobj obj)
{
    oobj stack = XViewFiles_dir_stack(obj);
    if (o_num(stack) == 0) {
        return o_str_clone(obj, ".");
    }
    return o_str_join(obj, OArray_data(stack, char*), o_num(stack), "/");
}

char *XViewFiles_dir(oobj obj)
{
    char *dir_small = XViewFiles_dir_small(obj);
    char *dir;
    if (o_str_equals(dir_small, ".")) {
        // just clone dir_home, else it would result in a "/./" path
        dir = o_str_clone(obj, XViewFiles_dir_home(obj));
    } else {
        dir = o_str_join(obj, (char *[]) {(char *) XViewFiles_dir_home(obj), dir_small}, 2, "/");
    }
    o_free(obj, dir_small);
    return dir;
}

const char *XViewFiles_file_name(oobj obj)
{
    OObj_assert(obj, XViewFiles);
    XViewFiles *self = obj;
    return WText_text(self->w.entry_file);
}

const char *XViewFiles_file_name_set(oobj obj, const char *name)
{
    OObj_assert(obj, XViewFiles);
    XViewFiles *self = obj;
    WText_text_set(self->w.entry_file, name);
    return name;
}

const char *XViewFiles_title(oobj obj)
{
    OObj_assert(obj, XViewFiles);
    XViewFiles *self = obj;
    return WText_text(self->w.title);
}

const char *XViewFiles_title_set(oobj obj, const char *title)
{
    OObj_assert(obj, XViewFiles);
    XViewFiles *self = obj;
    WText_text_set(self->w.title, title);
    return title;
}


char *XViewFiles_file(oobj obj)
{
    char *dir = XViewFiles_dir(obj);
    const char *file_name = XViewFiles_file_name(obj);
    char *joined = o_str_join(obj, (char *[]) {dir, (char *) file_name}, 2, "/");
    o_free(obj, dir);
    return joined;
}
