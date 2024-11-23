#include "x/file.h"
#include "o/file.h"
#include "o/str.h"
#include "x/viewfiles.h"
#include "x/XViewFiles.h"
#include "a/app.h"
#include "a/AScene.h"

#define O_LOG_LIB "x"

#include "o/log.h"


struct file_context {
    oobj root;

    char *file;
    o_file_event_fn opt_event;
    void *user;
};


O_STATIC
char *on_download_prompt(oobj files, bool ok)
{
    char *file = XViewFiles_file(files);
    if(ok && o_file_is_regular(file)) {
        return o_str_clone(files, "FILE EXISTS ALREADY!\n\nOVERRIDE FILE?");
    }
    return NULL;
}

O_STATIC
void on_download_file_picked(oobj scene)
{
    struct file_context *C = o_user(scene);
    oobj view = AScene_view(scene);
    char *dst = XViewFiles_file(view);
    osize cloned = o_file_clone(dst, C->file);
    o_log_s("x_file_download", "downloaded %"osize_PRI" bytes <%s> -> <%s>", cloned, C->file, dst);
    if(C->opt_event) {
        C->opt_event(C->user, C->file, dst, cloned>0);
    }
    o_del(C->root);
}

O_STATIC
char *on_upload_prompt(oobj files, bool ok)
{
    char *file = XViewFiles_file(files);
    if(ok && !o_file_is_regular(file)) {
        return o_str_clone(files, "FILE NOT FOUND!\n\nLOAD ANYWAY?");
    }
    return NULL;
}

O_STATIC
void on_upload_file_picked(oobj scene)
{
    struct file_context *C = o_user(scene);
    oobj view = AScene_view(scene);
    char *src = XViewFiles_file(view);
    osize cloned = o_file_clone(C->file, src);
    o_log_s("x_file_upload", "uploaded %"osize_PRI" bytes <%s> <- <%s>", cloned, C->file, src);
    if(C->opt_event) {
        C->opt_event(C->user, C->file, src, cloned>0);
    }
    o_del(C->root);
}

O_STATIC
void on_file_op_canceled(oobj scene)
{
    struct file_context *C = o_user(scene);
    o_log_s("x_file_down|up-load", "canceled");
    if(C->opt_event) {
        C->opt_event(C->user, C->file, NULL, false);
    }
    o_del(C->root);
}

O_STATIC
void on_system_download_event(void *user_data, const char *file, const char *opt_user_file, bool success)
{
    struct file_context *C = user_data;
    if(success) {
        if(C->opt_event) {
            C->opt_event(C->user, file, opt_user_file, true);
        }
        o_del(C->root);
        return;
    }

    // fallback
    oobj files = x_viewfiles_scene(C->root, "DOWNLOAD",
                                   on_download_file_picked, on_file_op_canceled);
    o_user_set(files, C);
    oobj view = AScene_view(files);
    osize idx = o_str_find_back_char(file, '/');
    // if not found, idx:=-1; so it would start in the beginning,
    // if idx is the last possible, it results in a "\0" string
    const char *file_name = &file[idx+1];
    XViewFiles_file_name_set(view, file_name);
    XViewFiles_dir_home_set(view, o_file_home(o_file_home_EXTERNAL));
    XViewFiles_prompt_set(view, on_download_prompt);
}

O_STATIC
void on_system_upload_event(void *user_data, const char *file, const char *opt_user_file, bool success)
{
    struct file_context *C = user_data;
    if(success) {
        if(C->opt_event) {
            C->opt_event(C->user, file, opt_user_file, true);
        }
        o_del(C->root);
        return;
    }

    // fallback
    oobj files = x_viewfiles_scene(a_app_root(), "UPLOAD",
                                   on_upload_file_picked, on_file_op_canceled);
    o_user_set(files, C);
    oobj view = AScene_view(files);
    osize idx = o_str_find_back_char(file, '/');
    // if not found, idx:=-1; so it would start in the beginning,
    // if idx is the last possible, it results in a "\0" string
    const char *file_name = &file[idx+1];
    XViewFiles_file_name_set(view, file_name);
    XViewFiles_file_name_set(view, file);
    XViewFiles_dir_home_set(view, o_file_home(o_file_home_EXTERNAL));
    XViewFiles_prompt_set(view, on_upload_prompt);
}

//
// public
//

void x_file_download(const char *file, bool try_system_first, o_file_event_fn opt_event, void *event_user_data)
{
    oobj root = OObj_new(a_app_root());
    struct file_context *C = o_new0(root, *C, 1);
    C->root = root;
    C->file = o_str_clone(root, file);
    C->opt_event = opt_event;
    C->user = event_user_data;

    o_log_s("x_file_download", "file: %s", file);
    if(try_system_first) {
        o_log_debug_s("x_file_download", "trying system first");
        o_file_download(file, on_system_download_event, C);
    } else {
        on_system_download_event(C, file, NULL, false);
    }
}

void x_file_upload(const char *file, bool try_system_first, o_file_event_fn opt_event, void *event_user_data)
{
    oobj root = OObj_new(a_app_root());
    struct file_context *C = o_new0(root, *C, 1);
    C->root = root;
    C->file = o_str_clone(root, file);
    C->opt_event = opt_event;
    C->user = event_user_data;

    o_log_s("x_file_upload", "file: %s", file);
    if(try_system_first) {
        o_log_debug_s("x_file_upload", "trying system first");
        o_file_upload(file, on_system_upload_event, C);
    } else {
        on_system_upload_event(C, file, NULL, false);
    }
}
