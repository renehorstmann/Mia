#ifdef MIA_PLATFORM_DESKTOP

#include "o/file.h"
#include "o/str.h"
#include "o/OObjRoot.h"
#include <stdlib.h>

#define O_LOG_LIB "o"
#include "o/log.h"

//
// sfd.h
//

/**
 * Copyright (c) 2017 rxi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See `sfd.c` for details.
 */

#define SFD_VERSION "0.1.0"

typedef struct {
    const char *title;
    const char *path;
    const char *filter_name;
    const char *filter;
    const char *extension;
} sfd_Options;

O_STATIC
const char* sfd_get_error(void);
O_STATIC
const char* sfd_open_dialog(sfd_Options *opt);
O_STATIC
const char* sfd_save_dialog(sfd_Options *opt);


//
// sfd.c
//
static const char *last_error;


const char* sfd_get_error(void) {
    const char *res = last_error;
    last_error = NULL;
    return res;
}


O_STATIC
int next_filter(char *dst, const char **p) {
    int len;

    *p += strspn(*p, "|");
    if (**p == '\0') {
        return 0;
    }

    len = strcspn(*p, "|");
    memcpy(dst, *p, len);
    dst[len] = '\0';
    *p += len;

    return 1;
}


/******************************************************************************
** Windows
*******************************************************************************/

#ifdef _WIN32

#include <windows.h>
#include <commdlg.h>

typedef struct {
  unsigned long process_id;
  void* handle_root;
  void* handle_first;
} FindMainWindowInfo;


O_STATIC
int find_main_window_callback(HWND handle, LPARAM lParam) {
  FindMainWindowInfo* info = (FindMainWindowInfo*)lParam;
  unsigned long process_id = 0;
  GetWindowThreadProcessId(handle, &process_id);
  if (info->process_id == process_id) {
    info->handle_first = handle;
    if (GetWindow(handle, GW_OWNER) == 0 && IsWindowVisible(handle)) {
      info->handle_root = handle;
      return 0;
    }
  }
  return 1;
}


O_STATIC
HWND find_main_window() {
  FindMainWindowInfo info = {
    .process_id = GetCurrentProcessId()
  };
  EnumWindows(find_main_window_callback, (LPARAM)&info);
  return info.handle_root;
}


O_STATIC
const char* make_filter_str(sfd_Options *opt) {
  O_STATIC
char buf[1024];
  int n;

  buf[0] = '\0';
  n = 0;

  if (opt->filter) {
    const char *p;
    char b[32];
    const char *name = opt->filter_name ? opt->filter_name : opt->filter;
    n += sprintf(buf + n, "%s", name) + 1;

    p = opt->filter;
    while (next_filter(b, &p)) {
      n += sprintf(buf + n, "%s;", b);
    }

    buf[++n] = '\0';
  }

  n += sprintf(buf + n, "All Files") + 1;
  n += sprintf(buf + n, "*.*");
  buf[++n] = '\0';

  return buf;
}


O_STATIC
void init_ofn(OPENFILENAME *ofn, sfd_Options *opt) {
  O_STATIC
char result_buf[2048];
  result_buf[0] = '\0';

  memset(ofn, 0, sizeof(*ofn));
  ofn->hwndOwner        = find_main_window();
  ofn->lStructSize      = sizeof(*ofn);
  ofn->lpstrFilter      = make_filter_str(opt);
  ofn->nFilterIndex     = 1;
  ofn->lpstrFile        = result_buf;
  ofn->Flags            = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
  ofn->nMaxFile         = sizeof(result_buf) - 1;
  ofn->lpstrInitialDir  = opt->path;
  ofn->lpstrTitle       = opt->title;
  ofn->lpstrDefExt      = opt->extension;
}


const char* sfd_open_dialog(sfd_Options *opt) {
  int ok;
  OPENFILENAME ofn;

  char working_directory[1024];
  int wd_len = GetCurrentDirectory(sizeof working_directory, working_directory);
  if(wd_len == 0)
      return NULL;

  last_error = NULL;
  init_ofn(&ofn, opt);
  ok = GetOpenFileName(&ofn);

  SetCurrentDirectory(working_directory);

  return ok ? ofn.lpstrFile : NULL;
}


const char* sfd_save_dialog(sfd_Options *opt) {
  int ok;
  OPENFILENAME ofn;

    char working_directory[1024];
    int wd_len = GetCurrentDirectory(sizeof working_directory, working_directory);
    if(wd_len == 0)
        return NULL;

  last_error = NULL;
  init_ofn(&ofn, opt);
  ok = GetSaveFileName(&ofn);

  SetCurrentDirectory(working_directory);

  return ok ? ofn.lpstrFile : NULL;
}

#endif


/******************************************************************************
** Zenity
*******************************************************************************/

#ifndef _WIN32


O_STATIC
const char* file_dialog(sfd_Options *opt, int save) {
    O_STATIC
    char result_buf[2048];
    char buf[2048];
    char *p;
    const char *title;
    FILE *fp;
    int n, len;

    last_error = NULL;

    fp = popen("zenity --version", "r");
    if (fp == NULL || pclose(fp) != 0) {
        last_error = "could not open zenity";
        return NULL;
    }


    n = sprintf(buf, "zenity --file-selection");

    if (save) {
        n += sprintf(buf + n, " --save --confirm-overwrite");
    }

    if (opt->title) {
        title = opt->title;
    } else {
        title = save ? "Save File" : "Open File";
    }

    n += sprintf(buf + n, " --title=\"%s\"", title);

    if (opt->path && opt->path[0] != '\0') {
        n += sprintf(buf + n, " --filename=\"");
        p = realpath(opt->path, buf + n);
        if (p == NULL) {
            last_error = "call to realpath() failed";
            return NULL;
        }
        n += strlen(buf + n);
        n += sprintf(buf + n, "/\"");
    }

    if (opt->filter) {
        char b[64];
        const char *p;
        n += sprintf(buf + n, " --file-filter=\"");

        if (opt->filter_name) {
            n += sprintf(buf + n, "%s | ", opt->filter_name);
        }

        p = opt->filter;
        while (next_filter(b, &p)) {
            n += sprintf(buf + n, "\"%s\" ", b);
        }

        n += sprintf(buf + n, "\"");
    }

    n += sprintf(buf + n, " --file-filter=\"All Files | *\"");


    fp = popen(buf, "r");
    len = fread(result_buf, 1, sizeof(result_buf) - 1, fp);
    pclose(fp);

    if (len > 0) {
        result_buf[len - 1] = '\0';
        if (save && opt->extension && !strstr(result_buf, opt->extension)) {
            sprintf(&result_buf[len - 1], ".%s", opt->extension);
        }
        return result_buf;
    }

    return NULL;
}


const char* sfd_open_dialog(sfd_Options *opt) {
    return file_dialog(opt, 0);
}


const char* sfd_save_dialog(sfd_Options *opt) {
    return file_dialog(opt, 1);
}

#endif



//
// helper
//

O_STATIC
sfd_Options create_options(const char *file) {
    sfd_Options opt;
    oobj root = OObjRoot_new_heap();
    char *filestring = o_str_clone(root, file);

    o_str_tolower_this(filestring);
    if(o_str_ends(filestring, ".png")) {
        opt = (sfd_Options) {
                .title        = "Image PNG File",
                .filter_name  = "Image PNG File",
                .filter       = "*.png",
                .extension    = ".png"
        };
    } else if(o_str_ends(filestring, ".txt")) {
        opt = (sfd_Options) {
                .title        = "Text File",
                .filter_name  = "Text File",
                .filter       = "*.txt",
                .extension    = ".txt"
        };
    } else if(o_str_ends(filestring, ".gif")) {
        opt = (sfd_Options) {
                .title        = "GIF File",
                .filter_name  = "GIF File",
                .filter       = "*.gif",
                .extension    = ".gif"
        };
    } else if(o_str_ends(filestring, ".json")) {
        opt = (sfd_Options) {
                .title        = "Json File",
                .filter_name  = "Json File",
                .filter       = "*.json",
                .extension    = ".json",
        };
    } else {
        opt = (sfd_Options) {
                .title        = "File"
        };
    }
    o_del(root);
    return opt;
}

//
// public
//


void o_file_download(const char *file, o_file_event_fn opt_event, void *event_user_data)
{
    o_thread_assert_main();

    sfd_Options opt = create_options(file);
    const char *filename = sfd_save_dialog(&opt);
    if (filename) {
        o_file_clone(filename, file);
        o_log_debug_s(__func__, "downloaded");
        if(opt_event) {
            opt_event(event_user_data, file, filename, true);
        }
        return;
    }
    o_log_debug_s(__func__, "download canceled");
    if(opt_event) {
        opt_event(event_user_data, file, NULL, false);
    }
}

void o_file_upload(const char *file, o_file_event_fn opt_event, void *event_user_data)
{
    o_thread_assert_main();

    sfd_Options opt = create_options(file);
    const char *filename = sfd_open_dialog(&opt);
    if (filename) {
        o_file_clone(file, filename);
        o_log_debug_s(__func__, "uploaded");
        if(opt_event) {
            opt_event(event_user_data, file, filename, true);
        }
        return;
    }

    o_log_debug_s(__func__, "upload canceled");
    if(opt_event) {
        opt_event(event_user_data, file, NULL, false);
    }
}



#endif
typedef int avoid_empty_translation_unit;
