#ifdef MIA_PLATFORM_ANDROID

#include "o/file.h"
#include "o/str.h"
#include "o/OObjRoot.h"
#include <jni.h>
#include <SDL2/SDL.h>

#define O_LOG_LIB "o"

#include "o/log.h"


static struct {
    oobj root;
    oobj lock;
    bool running;
    bool valid;
    char *client_user_file_name;
} file_android_L;


JNIEXPORT void JNICALL O_ANDROID_INTERFACE(oFileDownloadDone)(JNIEnv *env, jobject thisObject,
                                                              jboolean success,
                                                              jstring client_user_file_name) {
    o_log_s("o_file_download", "file downloaded successfully: %i", success);

    o_lock_block(file_android_L.lock) {
        file_android_L.running = false;
        file_android_L.valid = success;
        o_free(file_android_L.root, file_android_L.client_user_file_name);
        file_android_L.client_user_file_name = NULL;
        if(success) {
            const char *cstr = (*env)->GetStringUTFChars(env, client_user_file_name, NULL);
            if(o_strlen(cstr) < O_FILE_PATH_MAX) {
                file_android_L.client_user_file_name = o_str_clone(file_android_L.root, cstr);
            }
            (*env)->ReleaseStringUTFChars(env, client_user_file_name, cstr);
        }
    }
}


JNIEXPORT void JNICALL O_ANDROID_INTERFACE(oFileUploadDone)(JNIEnv *env, jobject thisObject,
                                                            jboolean success,
                                                            jstring client_user_file_name) {
    if(!client_user_file_name) {
        success = false;
    }
    o_log_s("o_file_upload", "file upload successfully: %i", success);

    o_lock_block(file_android_L.lock) {
        file_android_L.running = false;
        file_android_L.valid = success;
        o_free(file_android_L.root, file_android_L.client_user_file_name);
        file_android_L.client_user_file_name = NULL;
        if(success) {
            const char *cstr = (*env)->GetStringUTFChars(env, client_user_file_name, NULL);
            if(o_strlen(cstr) < O_FILE_PATH_MAX) {
                file_android_L.client_user_file_name = o_str_clone(file_android_L.root, cstr);
            }
            (*env)->ReleaseStringUTFChars(env, client_user_file_name, cstr);
        }
    }
}



//
// public
//


void o_file_download(const char *file, o_file_event_fn opt_event, void *event_user_data)
{
    o_thread_assert_main();
    assert(!file_android_L.running && "already running a download/upload?");

    if (!file_android_L.root) {
        // lazy init
        file_android_L.root = OObjRoot_new_heap();
        file_android_L.lock = OObj_new(file_android_L.root);
    }
    o_log_s(__func__, "download file: %s", file);


    // JNI CALL
    {
        JNIEnv *env = NULL;
        jobject activity = NULL;
        jclass clazz = NULL;
        jstring jfile = NULL;

        env = (JNIEnv *) SDL_AndroidGetJNIEnv();
        if (!env) {
            o_log_error_s(__func__, "failed to get jni env");
            goto JNI_CLEAN_UP;
        }

        activity = (jobject) SDL_AndroidGetActivity();
        if (!activity) {
            o_log_error_s(__func__, "failed to get activity");
            goto JNI_CLEAN_UP;
        }

        clazz = (*env)->GetObjectClass(env, activity);
        if (!clazz) {
            o_log_error_s(__func__, "failed to get clazz");
            goto JNI_CLEAN_UP;
        }

        jmethodID method_id = (*env)->GetMethodID(env, clazz,
                                                  "oFileDownload",
                                                  "(Ljava/lang/String;)V");
        if (!method_id) {
            o_log_error_s(__func__, "method not found");
            goto JNI_CLEAN_UP;
        }

        file_android_L.running = true;

        jfile = (*env)->NewStringUTF(env, file);
        (*env)->CallVoidMethod(env, activity, method_id, jfile);

        JNI_CLEAN_UP:
        if (env) {
            if (jfile) (*env)->DeleteLocalRef(env, jfile);
            if (activity) (*env)->DeleteLocalRef(env, activity);
            if (clazz) (*env)->DeleteLocalRef(env, clazz);
        }
    }

    o_log_trace_s(__func__, "entering spin lock");
    for (;;) {
        bool running;
        bool valid;
        o_lock_block(file_android_L.lock) {
            running = file_android_L.running;
            valid = file_android_L.valid;
        }
        if (running) {
            o_sleep(1);
        } else {
            o_log_trace_s(__func__, "spin lock finished");
            if(valid) {
                if(opt_event) {
                    opt_event(event_user_data, file, file_android_L.client_user_file_name, true);
                }
            } else {
                if(opt_event) {
                    opt_event(event_user_data, file, NULL, false);
                }
            }
            return;
        }
    }

    if(opt_event) {
        opt_event(event_user_data, file, NULL, false);
    }
}

void o_file_upload(const char *file, o_file_event_fn opt_event, void *event_user_data)
{
    o_thread_assert_main();
    assert(!file_android_L.running && "already running a download/upload?");

    if (!file_android_L.root) {
        // lazy init
        file_android_L.root = OObjRoot_new_heap();
        file_android_L.lock = OObj_new(file_android_L.root);
    }
    o_log_s(__func__, "upload file: %s", file);


    // JNI CALL
    {
        JNIEnv *env = NULL;
        jobject activity = NULL;
        jclass clazz = NULL;
        jstring jfile = NULL;

        env = (JNIEnv *) SDL_AndroidGetJNIEnv();
        if (!env) {
            o_log_error_s(__func__, "failed to get jni env");
            goto JNI_CLEAN_UP;
        }

        activity = (jobject) SDL_AndroidGetActivity();
        if (!activity) {
            o_log_error_s(__func__, "failed to get activity");
            goto JNI_CLEAN_UP;
        }

        clazz = (*env)->GetObjectClass(env, activity);
        if (!clazz) {
            o_log_error_s(__func__, "failed to get clazz");
            goto JNI_CLEAN_UP;
        }

        jmethodID method_id = (*env)->GetMethodID(env, clazz,
                                                  "oFileUpload",
                                                  "(Ljava/lang/String;)V");
        if (!method_id) {
            o_log_error_s(__func__, "method not found");
            goto JNI_CLEAN_UP;
        }

        file_android_L.running = true;

        jfile = (*env)->NewStringUTF(env, file);
        (*env)->CallVoidMethod(env, activity, method_id, jfile);

        JNI_CLEAN_UP:
        if (env) {
            if (jfile) (*env)->DeleteLocalRef(env, jfile);
            if (activity) (*env)->DeleteLocalRef(env, activity);
            if (clazz) (*env)->DeleteLocalRef(env, clazz);
        }
    }

    o_log_trace_s(__func__, "entering spin lock");
    for (;;) {
        bool running;
        bool valid;
        o_lock_block(file_android_L.lock) {
            running = file_android_L.running;
            valid = file_android_L.valid;
        }
        if (running) {
            o_sleep(1);
        } else {
            o_log_trace_s(__func__, "spin lock finished");

            if(valid) {
                if(opt_event) {
                    opt_event(event_user_data, file, file_android_L.client_user_file_name, true);
                }
            } else {
                if(opt_event) {
                    opt_event(event_user_data, file, NULL, false);
                }
            }
            return;
        }
    }

    if(opt_event) {
        opt_event(event_user_data, file, NULL, false);
    }
}


#endif
typedef int avoid_empty_translation_unit;
