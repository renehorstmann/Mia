#include "a/billing.h"


#define O_LOG_LIB "a"
#include "o/log.h"

#ifdef MIA_PLATFORM_ANDROID

#include "o/OObjRoot.h"
#include "o/OPtr.h"
#include <SDL2/SDL.h>
#include <jni.h>

static struct {
    oobj root;
    a_billing_purchased_fn callback;
    oobj callback_obj_ptr;
} billing_L;

JNIEXPORT bool JNICALL O_ANDROID_INTERFACE(aBillingPurchased)(JNIEnv* env, jobject thisObject, jint idx) {
    o_log_s("aBillingPurchased", "got a purchase: %i", idx);
    if(!billing_L.callback || !billing_L.callback_obj_ptr) {
        o_log_error_s("aBillingPurchased", "callback invalid!");
        return false;
    }
    struct oobj_opt obj = OPtr_get(billing_L.callback_obj_ptr);
    if(!obj.o) {
        o_log_debug_s("aBillingPurchased", "callback object was deleted");
    } else {
        return billing_L.callback(obj.o, idx);
    }
    return false;
}

//
// public
//

bool a_billing_loaded(void)
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
                                              "aBillingLoaded",
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

void a_billing_init(oobj callback_obj, a_billing_purchased_fn callback)
{
    if(!billing_L.root) {
        billing_L.root = OObjRoot_new_heap();
    }
    o_del(billing_L.callback_obj_ptr);
    billing_L.callback = callback;
    billing_L.callback_obj_ptr = OPtr_new(billing_L.root, callback_obj);
}

void a_billing_buy(int idx)
{
    assert(billing_L.callback && billing_L.callback_obj_ptr && "call a_billing_init first");

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
                                                  "aBillingBuy",
                                                  "(I)V");

        if(!method_id) {
            o_log_error_s(__func__, "method not found");
            goto JNI_CLEAN_UP;
        }

        (*env)->CallVoidMethod(env, activity, method_id, (jint) idx);

        JNI_CLEAN_UP:
        if(env) {
            if(activity) (*env)->DeleteLocalRef(env, activity);
            if(clazz) (*env)->DeleteLocalRef(env, clazz);
        }
    }
}

#else //MIA_PLATFORM_ANDROID

bool a_billing_loaded(void)
{
    return false;
}

// setup at app start or smth. so even on restart it may work
void a_billing_init(oobj callback_obj, a_billing_purchased_fn callback)
{
    // noop
}

// asserts a_billing_init was called before to set the callback
void a_billing_buy(int idx) {
    o_log_warn_s(__func__, "billing is not available for this platform");
}
#endif
