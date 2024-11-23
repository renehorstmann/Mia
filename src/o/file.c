#include "o/file.h"
#include "o/str.h"
#include "o/OStreamSdl.h"
#include "o/OObjRoot.h"  // for internal work
#include <SDL2/SDL_rwops.h>
#include <SDL2/SDL_system.h>

// list dir and check file type stuff
#include "file_dirent.h"
#include <sys/stat.h>

// mkdirs
#ifdef MIA_PLATFORM_WINDOWS
#include <direct.h>
#else
#include <unistd.h>
#endif



#define O_LOG_LIB "o"

#include "o/log.h"


struct oobj_opt o_file_open(oobj parent, const char *file, const char *mode) {
    SDL_RWops *rwops = SDL_RWFromFile(file, mode);
    if (!rwops) {
        return oobj_opt(NULL);
    }
    OStreamSdl *stream = OStreamSdl_new(parent, rwops);

    char buf[64];
    o_strf_buf(buf, "Stream:%s", file);
    OObj_name_set(stream, buf);

    return oobj_opt(stream);
}

struct oobj_opt o_file_c(oobj parent, FILE *file, bool auto_close) {
    SDL_RWops *rwops = SDL_RWFromFP(file, auto_close);
    if (!rwops) {
        return oobj_opt(NULL);
    }
    OStreamSdl *stream = OStreamSdl_new(parent, rwops);
    return oobj_opt(stream);
}

struct oobj_opt o_file_read(oobj parent, const char *file, bool ascii, osize element_size) {
    oobj data = NULL;
    struct oobj_opt stream = o_file_open(parent, file, ascii ? "r" : "rb");
    if (!stream.o) {
        o_log_debug_s(__func__, "failed to open file: %s", file);
        goto CLEAN_UP;
    }
    osize size = OStream_size(stream.o);
    if (size <= 0) {
        o_log_debug_s(__func__, "failed to read file size: %s", file);
        goto CLEAN_UP;
    }
    osize num = size / element_size;

    if (size % element_size != 0) {
        o_log_debug_s(__func__,
                      "warning: skipped some bytes: size % element_size = %i",
                      size % element_size);
    }

    data = OArray_new(parent, NULL, element_size, num);
    osize read = OStream_read(stream.o, OArray_data_void(data), element_size, num);
    if (read != num) {
        o_log_debug_s(__func__,
                      "warning: read fewer bytes than expacted: %i/%i",
                      read, num);
        OArray_resize(data, read);
    }


    char buf[64];
    o_strf_buf(buf, "Data:%s", file);
    OObj_name_set(data, buf);

    CLEAN_UP:
    o_del(stream.o);
    return oobj_opt(data);
}

osize o_file_write(const char *file, bool ascii, const void *data, osize element_size, osize num) {
    oobj root = OObjRoot_new_heap();
    osize written = 0;
    struct oobj_opt stream = o_file_open(root, file, ascii ? "w" : "wb");
    if (!stream.o) {
        o_log_debug_s(__func__, "failed to open file: %s", file);
        goto CLEAN_UP;
    }
    written = OStream_write(stream.o, data, element_size, num);

    CLEAN_UP:
    o_del(root);
    return written;
}

osize o_file_append(const char *file, bool ascii, const void *data, osize element_size, osize num) {
    oobj root = OObjRoot_new_heap();
    osize written = 0;
    struct oobj_opt stream = o_file_open(root, file, ascii ? "a" : "ab");
    if (!stream.o) {
        o_log_debug_s(__func__, "failed to open file: %s", file);
        goto CLEAN_UP;
    }
    written = OStream_write(stream.o, data, element_size, num);

    CLEAN_UP:
    o_del(root);
    return written;
}


osize o_file_clone(const char *dst, const char *src) {
    oobj root = OObjRoot_new_heap();
    struct oobj_opt stream_dst = o_file_open(root, dst, "wb");
    struct oobj_opt stream_src = o_file_open(root, src, "rb");

    osize cloned_bytes = 0;

    ou8 buffer[1024];
    while (OStream_valid(stream_dst.o) && OStream_valid(stream_src.o)) {
        osize read = OStream_read_try(stream_src.o, buffer, 1, sizeof buffer);
        OStream_write(stream_dst.o, buffer, 1, read);
        cloned_bytes += read;
    }
    o_del(root);

    return cloned_bytes;
}

bool o_file_is_dir(const char *file) {
    struct stat info;
    if (file && stat(file, &info) == 0) {
        if (S_ISDIR(info.st_mode)) {
            return true;
        }
    }
    return false;
}

bool o_file_is_regular(const char *file) {
    struct stat info;
    if (file && stat(file, &info) == 0) {
        if (S_ISREG(info.st_mode)) {
            return true;
        }
    }
    return false;
}

bool o_file_mkdirs(const char *path) {
#ifdef MIA_PLATFORM_WINDOWS
#define MKDIR(path) _mkdir(path)
#else
#define MKDIR(path) mkdir(path, 0755)
#endif

    char tmp[512];
    o_strf_buf(tmp, "%s", path);
    osize len = o_strlen(tmp);

    // remove ending slash
    if (tmp[len - 1] == '/') {
        tmp[len - 1] = 0;
    }
    for (char *p = tmp + 1; *p; p++) {
        if (*p == '/') {
            *p = 0;
            if (!o_file_is_dir(tmp)) {
                if (MKDIR(tmp) != 0) {
                    return false;
                }
            }
            *p = '/';
        }
    }
    if (!o_file_is_dir(tmp)) {
        if (MKDIR(tmp) != 0) {
            return false;
        }
    }
    return true;

#undef MKDIR
}

O_STATIC
const char *file_home_user_path(void) {
#ifdef MIA_PLATFORM_WINDOWS
    static char home_cat[512];
    char *home = getenv("USERPROFILE");
    if(home) {
        o_strf_buf(home_cat, "%s", home);
        home = home_cat;
    } else {
        home = getenv("HOMEDRIVE");
        if (home) {
            char *home_path = getenv("HOMEPATH");
            if (home_path) {
                size_t length = strlen(home) + strlen(home_path) + 1;
                o_strf_buf(home_cat, "%s%s", home, home_path);
                home = home_cat;
            }
        }
    }
    if(home) {
        // use new style and not old...
        o_str_replace_char_this(home, '\\', '/');
    }
    return home;
#else
    return getenv("HOME");
#endif
}

const char *o_file_home(enum o_file_home_loc location) {
    if (location == o_file_home_WORKING) {
        return ".";
    }

#if defined(MIA_PLATFORM_EMSCRIPTEN) || defined(MIA_PLATFORM_CXXDROID)
    return ".";
#endif

#ifdef MIA_PLATFORM_ANDROID
    if (location == o_file_home_DEFAULT || location == o_file_home_INTERNAL) {
        return SDL_AndroidGetInternalStoragePath();
    }
    if (location == o_file_home_EXTERNAL) {
        return SDL_AndroidGetExternalStoragePath();
    }
#endif

    // remaining use home file system
    if (location == o_file_home_DEFAULT) {
        return ".";
    }
    if (location == o_file_home_INTERNAL) {
        static char user_internal[512] = {0};
        if (user_internal[0] == 0) {
            o_strf_buf(user_internal, "%s/." O_MIA_TITLE_STR,
                     file_home_user_path());
            o_file_mkdirs(user_internal);
        }
        return user_internal;
    }
    if (location == o_file_home_EXTERNAL) {
        return file_home_user_path();
    }

    return ".";
}

O_STATIC
int string_compare_fn(const void *a, const void *b) {
    const char **str_a = (const char **) a;
    const char **str_b = (const char **) b;
    return strcmp(*str_a, *str_b);
}


oobj
o_file_list(oobj parent, const char *directory, enum o_file_list_mode mode, char **file_filter) {
    directory = o_or(directory, ".");

    oobj items = OArray_new_dyn(parent, NULL, sizeof(char *), 0, 256);

    DIR *dir;
    struct dirent *entry;
    dir = opendir(directory);
    if (dir == NULL) {
        o_log_debug_s(__func__, "opendir failed for: \"%s\"", directory);
        return items;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (o_str_equals(entry->d_name, ".")) {
            continue;
        }

        bool valid = true;
        if (file_filter) {
            valid = false;
            char *lower = o_str_tolower(items, entry->d_name);
            char **it = file_filter;
            while (*it) {
                assert(o_str_islower(*it) && "file_filter strings must be lower case");
                if (o_str_ends(lower, *it++)) {
                    valid = true;
                    break;
                }
            }
            o_free(items, lower);
        }
        if (!valid) {
            continue;
        }

        char *path = o_strf(parent, "%s/%s", directory, entry->d_name);

        switch (mode) {
            case o_file_list_ALL:
            default:
                valid = o_file_is_dir(path) || o_file_is_regular(path);
                break;
            case o_file_list_DIRS:
                valid = o_file_is_dir(path);
                break;
            case o_file_list_FILES:
                valid = o_file_is_regular(path);
                break;
        }
        o_free(parent, path);
        if (!valid) {
            continue;
        }

        // create a clone of the string and push that pointer to the list of files
        char *push = o_str_clone(items, entry->d_name);
        OArray_push(items, &push);
    }

    closedir(dir);

    // sort list of dir files
    OArray_sort(items, string_compare_fn);

    return items;
}

void o_file_record_name_into(char *out_record_name, const char *filename) {
    assert(out_record_name && filename
           && o_strlen(filename) <= O_FILE_RECORD_MAX_FILE_LENGTH
           && o_str_find_char(filename, '/') == -1);
#ifdef MIA_PLATFORM_EMSCRIPTEN
    snprintf(out_record_name, O_FILE_PATH_MAX, "record/%s", filename);
#else
    snprintf(out_record_name, O_FILE_PATH_MAX, "record.%s", filename);
#endif
}

char *o_file_record_name(const char *filename) {
    static _Thread_local char name[O_FILE_PATH_MAX];
    o_file_record_name_into(name, filename);
    return name;
}


void o_file_record_sync(void) {
#ifdef MIA_PLATFORM_EMSCRIPTEN
    void o_file_record__idbfs_save(void);
    o_file_record__idbfs_save();
#endif // else noop
}

// protected
O_EXTERN
void o_file_record__mount(void) {
#ifdef MIA_PLATFORM_EMSCRIPTEN
    void o_file_record__idbfs_mount(void);
    o_file_record__idbfs_mount();
#endif // else noop
}


#ifdef MIA_PLATFORM_CXXDROID
void o_file_download(const char *file, o_file_event_fn opt_event, void *event_user_data)
{
    // noop
    if(opt_event) {
        opt_event(event_user_data, file, NULL, false);
    }
}

void o_file_upload(const char *file, o_file_event_fn opt_event, void *event_user_data)
{
    // noop
    if(opt_event) {
        opt_event(event_user_data, file, NULL, false);
    }
}
#endif
