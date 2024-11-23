#ifdef MIA_PLATFORM_EMSCRIPTEN

#include "o/file.h"
#include "o/common.h"
#include "o/str.h"
#include "o/timer.h"
#include "emscripten.h"

#define O_LOG_LIB "o"
#include "o/log.h"

#define SYNC_TIMEOUT 0.2

static struct {
    bool mounted;
    bool synced;
    ou64 synced_timer;

    bool upload_active;
    char upload_user_file_name[O_FILE_PATH_MAX];
    o_file_event_fn upload_opt_event;
    void *upload_event_user_data;
} fe_L;

O_EXTERN
void o_file_record__idbfs_mount(void)
{
    o_log_debug_s(__func__, "record mount");

    if (fe_L.mounted) {
        return;
    }
    EM_ASM(
        FS.mkdir('/record');
        FS.mount(IDBFS, {}, '/record');
        o__record_idbfs_synced = false;
    );
    fe_L.mounted = true;
    o_log_trace_s(__func__, "record mounted");

    fe_L.synced = false;

    // true = load file system from idbfs
    EM_ASM(
        o__record_idbfs_synced = false;
        FS.syncfs(true, function(err)
        {
            assert(!err);
            o__record_idbfs_synced = true;
        });
    );

    // sleep a ms until synced to get a synchronous call
    while (!fe_L.synced) {
        EM_ASM(
            if (o__record_idbfs_synced) { ccall('o_file_record__idbfs_synced', 'v'); }
        );
        o_sleep(1);
    }
    o_log_trace_s(__func__, "synced");

    fe_L.synced_timer = o_timer();
}

O_EXTERN
void o_file_record__idbfs_synced(void)
{
    o_log_trace_s(__func__, "records synced");
    fe_L.synced = true;
}

O_EXTERN
void o_file_record__idbfs_save(void)
{
    if (o_timer_elapsed_s(fe_L.synced_timer) <= SYNC_TIMEOUT) {
        o_log_debug_s(__func__, "ignoring e_io save sync, timeout");
        return;
    }
    fe_L.synced_timer = o_timer();

    o_log_trace_s(__func__, "save...");

    // false = save file system from idbfs
    EM_ASM(
        o__record_idbfs_synced = false;
        FS.syncfs(false, function(err)
        {
            assert(!err);
        });
    );
}


//
// download
//

void o_file_download(const char *file, o_file_event_fn opt_event, void *event_user_data)
{
    char script[256];
    if (o_strlen(file) >= sizeof script) {
        o_log_error_s(__func__, "file length to long");
        if(opt_event) {
            opt_event(event_user_data, file, NULL, false);
        }
        return;
    }
    o_strf_buf(script, "o__file_download(\'%s\');", file);
    emscripten_run_script(script);
    if(opt_event) {
        opt_event(event_user_data, file, NULL, true);
    }
}


//
// upload
//

O_EXTERN
void o_file_upload__done(const char *file, const char *user_file_name, bool success)
{
    if(!fe_L.upload_active) {
        o_log_warn_s("o_file_upload", "invalid callback call");
        return;
    }
    o_log_debug_s(__func__, "file upload %s for: %s # %s", success? "done" : "failed",
                  file, user_file_name);
    if(fe_L.upload_opt_event) {
        fe_L.upload_opt_event(fe_L.upload_event_user_data, file, user_file_name, success);
    }
    fe_L.upload_active = false;
}


void o_file_upload(const char *file, o_file_event_fn opt_event, void *event_user_data)
{
    fe_L.upload_active = true;
    fe_L.upload_user_file_name[0] = '\0';
    fe_L.upload_opt_event = opt_event;
    fe_L.upload_event_user_data = event_user_data;
    char script[512];
    o_strf_buf(script, "o__file_upload(\'%s\');", file);
    emscripten_run_script(script);
}

#endif
typedef int avoid_empty_translation_unit;
