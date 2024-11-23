#include "a/ad.h"


#define O_LOG_LIB "a"
#include "o/log.h"

#ifdef MIA_PLATFORM_ANDROID

#include "o/OObjRoot.h"
#include "o/OPtr.h"
#include <SDL2/SDL.h>
#include <jni.h>

static struct {
    oobj root;
    OObj__event_fn callback;
    oobj callback_obj_ptr;
} ad_L;

JNIEXPORT void JNICALL O_ANDROID_INTERFACE(aAdReward)(JNIEnv* env, jobject thisObject)
{
    o_log_s("aAdReward", "got an ad reward");
    if(!ad_L.callback || !ad_L.callback_obj_ptr) {
        o_log_error_s("aAdReward", "callback invalid!");
        return;
    }
    struct oobj_opt obj = OPtr_get(ad_L.callback_obj_ptr);
    if(!obj.o) {
        o_log_debug_s("aAdReward", "callback object was deleted");
    } else {
        ad_L.callback(obj.o);
    }
}

//
// public
//


bool a_ad_ump_loaded(void)
{
    bool loaded = false;

    // JNI CALL
    {
        JNIEnv* env = NULL;
        jobject activity = NULL;
        jclass clazz = NULL;
        jstring jurl = NULL;

        env = (JNIEnv*) SDL_AndroidGetJNIEnv();
        if(!env) {
            o_log_error_s(__func__, "failed to get jni env");
            goto JNI_CLEAN_UP;
        }

        activity = (jobject) SDL_AndroidGetActivity();
        if(!activity) {
            o_log_error_s(__func__, "failed to get activity");
            goto JNI_CLEAN_UP;
        }

        clazz = (*env)->GetObjectClass(env, activity);
        if(!clazz) {
            o_log_error_s(__func__, "failed to get clazz");
            goto JNI_CLEAN_UP;
        }

        jmethodID method_id = (*env)->GetMethodID(env, clazz,
                                              "aAdUmpLoaded",
                                              "()Z");

        if(!method_id) {
            o_log_error_s(__func__, "method not found");
            goto JNI_CLEAN_UP;
        }

        jboolean jloaded = (*env)->CallBooleanMethod(env, activity, method_id);
        loaded = jloaded;

        JNI_CLEAN_UP:
        if(env) {
            if(activity) (*env)->DeleteLocalRef(env, activity);
            if(clazz) (*env)->DeleteLocalRef(env, clazz);
        }
    }

    return loaded;
}



void a_ad_ump_show(void)
{
    // JNI CALL
    {
        JNIEnv* env = NULL;
        jobject activity = NULL;
        jclass clazz = NULL;

        env = (JNIEnv*) SDL_AndroidGetJNIEnv();
        if(!env) {
            o_log_error_s(__func__, "failed to get jni env");
            goto JNI_CLEAN_UP;
        }

        activity = (jobject) SDL_AndroidGetActivity();
        if(!activity) {
            o_log_error_s(__func__, "failed to get activity");
            goto JNI_CLEAN_UP;
        }

        clazz = (*env)->GetObjectClass(env, activity);
        if(!clazz) {
            o_log_error_s(__func__, "failed to get clazz");
            goto JNI_CLEAN_UP;
        }

        jmethodID method_id = (*env)->GetMethodID(env, clazz,
                                                  "aAdUmpShow",
                                                  "()V");

        if(!method_id) {
            o_log_error_s(__func__, "method not found");
            goto JNI_CLEAN_UP;
        }

        (*env)->CallVoidMethod(env, activity, method_id);

        JNI_CLEAN_UP:
        if(env) {
            if(activity) (*env)->DeleteLocalRef(env, activity);
            if(clazz) (*env)->DeleteLocalRef(env, clazz);
        }
    }
}

bool a_ad_loaded(void)
{
    bool loaded = false;

    // JNI CALL
    {
        JNIEnv* env = NULL;
        jobject activity = NULL;
        jclass clazz = NULL;
        jstring jurl = NULL;

        env = (JNIEnv*) SDL_AndroidGetJNIEnv();
        if(!env) {
            o_log_error_s(__func__, "failed to get jni env");
            goto JNI_CLEAN_UP;
        }

        activity = (jobject) SDL_AndroidGetActivity();
        if(!activity) {
            o_log_error_s(__func__, "failed to get activity");
            goto JNI_CLEAN_UP;
        }

        clazz = (*env)->GetObjectClass(env, activity);
        if(!clazz) {
            o_log_error_s(__func__, "failed to get clazz");
            goto JNI_CLEAN_UP;
        }

        jmethodID method_id = (*env)->GetMethodID(env, clazz,
                                              "aAdLoaded",
                                              "()Z");

        if(!method_id) {
            o_log_error_s(__func__, "method not found");
            goto JNI_CLEAN_UP;
        }

        jboolean jloaded = (*env)->CallBooleanMethod(env, activity, method_id);
        loaded = jloaded;

        JNI_CLEAN_UP:
        if(env) {
            if(activity) (*env)->DeleteLocalRef(env, activity);
            if(clazz) (*env)->DeleteLocalRef(env, clazz);
        }
    }

    return loaded;
}

void a_ad_show(oobj callback_obj, OObj__event_fn callback)
{
    if(!ad_L.root) {
        ad_L.root = OObjRoot_new_heap();
    }
    o_del(ad_L.callback_obj_ptr);
    ad_L.callback = callback;
    ad_L.callback_obj_ptr = OPtr_new(ad_L.root, callback_obj);

    // JNI CALL
    {
        JNIEnv* env = NULL;
        jobject activity = NULL;
        jclass clazz = NULL;

        env = (JNIEnv*) SDL_AndroidGetJNIEnv();
        if(!env) {
            o_log_error_s(__func__, "failed to get jni env");
            goto JNI_CLEAN_UP;
        }

        activity = (jobject) SDL_AndroidGetActivity();
        if(!activity) {
            o_log_error_s(__func__, "failed to get activity");
            goto JNI_CLEAN_UP;
        }

        clazz = (*env)->GetObjectClass(env, activity);
        if(!clazz) {
            o_log_error_s(__func__, "failed to get clazz");
            goto JNI_CLEAN_UP;
        }

        jmethodID method_id = (*env)->GetMethodID(env, clazz,
                                                  "aAdShow",
                                                  "()V");

        if(!method_id) {
            o_log_error_s(__func__, "method not found");
            goto JNI_CLEAN_UP;
        }

        (*env)->CallVoidMethod(env, activity, method_id);

        JNI_CLEAN_UP:
        if(env) {
            if(activity) (*env)->DeleteLocalRef(env, activity);
            if(clazz) (*env)->DeleteLocalRef(env, clazz);
        }
    }
}

#else //MIA_PLATFORM_ANDROID

bool a_ad_ump_loaded(void)
{
    return false;
}

void a_ad_ump_show(void)
{
    o_log_warn_s(__func__, "admob not available for this platform");
}

bool a_ad_loaded(void)
{
    return false;
}

void a_ad_show(oobj callback_obj, OObj__event_fn callback)
{
    o_log_warn_s(__func__, "admob not available for this platform");
}
#endif
