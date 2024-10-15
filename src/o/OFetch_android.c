#if defined(MIA_OPTION_FETCH) && defined(MIA_PLATFORM_ANDROID)

#include "o/OFetch.h"
#include "o/OObj_builder.h"
#include "o/OObjRoot.h"
#include "o/OJoin.h"
#include "o/OThread.h"
#include "o/OArray.h"
#include "o/str.h"
#include <SDL2/SDL.h>
#include <jni.h>

#define O_LOG_LIB "o"
#include "o/log.h"


//
// protected
//

O_EXTERN
void o__fetch_init(void)
{
    // noop
}


//
// private
//

struct Data {
    oobj root;

    // joined are the threads root and the calling OFetch
    // remaining data below is allocated on that OJoin
    oobj join;

    char *url;
    void *opt_post_data;
    osize opt_post_data_size;

    // locked:

    bool error;
    oobj data_response;
};


O_STATIC
void request_thread(oobj thread_obj)
{
    struct Data *data = o_user(thread_obj);


    // keeps beeing NULL on error
    oobj result_array = NULL;

    // JNI CALL
    {
        JNIEnv *env = NULL;
        jobject activity = NULL;
        jclass clazz = NULL;
        jstring url = NULL;
        jbyteArray post_data = NULL;
        jbyteArray result_data = NULL;
        jsize result_length;
        jbyte *result_bytes = NULL;

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

        jmethodID method_id = (*env)->GetStaticMethodID(env, clazz,
                                                  "oFetchBlocking",
                                                  "(Ljava/lang/String;[B)[B");
        if (!method_id) {
            o_log_error_s(__func__, "method not found");
            goto JNI_CLEAN_UP;
        }

        url = (*env)->NewStringUTF(env, data->url);
        if(data->opt_post_data && data->opt_post_data_size>0) {
            post_data = (*env)->NewByteArray(env, (jsize) data->opt_post_data_size);
            if(!post_data) {
                o_log_error_s(__func__, "failed to create post data array");
                goto JNI_CLEAN_UP;
            }
            (*env)->SetByteArrayRegion(env, post_data, 0, (jsize) data->opt_post_data_size,
                                       (const jbyte*) data->opt_post_data);
        }

        result_data = (jbyteArray) (*env)->CallStaticObjectMethod(env, clazz, method_id, url, post_data);
        if(!result_data) {
            goto JNI_CLEAN_UP;
        }

        result_length = (*env)->GetArrayLength(env, result_data);
        result_bytes = (*env)->GetByteArrayElements(env, result_data, NULL);

        // clone result
        result_array = OArray_new(data->join, result_bytes, 1, result_length);


        JNI_CLEAN_UP:
        if (env) {
            if(result_bytes) (*env)->ReleaseByteArrayElements(env, result_data, result_bytes, JNI_ABORT);
            if (result_data) (*env)->DeleteLocalRef(env, result_data);
            if (post_data) (*env)->DeleteLocalRef(env, post_data);
            if (url) (*env)->DeleteLocalRef(env, url);
            if (activity) (*env)->DeleteLocalRef(env, activity);
            if (clazz) (*env)->DeleteLocalRef(env, clazz);
        }
    }

    // set response
    o_lock_block(data->join) {
        data->error = result_array == NULL;
        data->data_response = result_array;
    }

    if(data->error) {
        o_log_trace_s("OFetch", "failed to fetch: <%s>", data->url);
    } else {
        o_log_trace_s("OFetch", "fetched %"osize_PRI" bytes from: <%s>",
                   OArray_num(data->data_response), data->url);
    }

    // deletes one side of the join, if OFetch is also deleted, the OJoin gets deleted
    o_del(data->root);
}


//
// public
//

OFetch *OFetch_init(oobj obj, oobj parent, const char *url, const void *opt_post_data, osize opt_post_data_size)
{
    assert((!opt_post_data || opt_post_data_size>0) && "either NULL to opt_post_data or set a valid positive size");

    OFetch *self = obj;
    o_clear(self, sizeof *self, 1);

    OObj_init(self, parent);
    OObj_id_set(self, OFetch_ID);

    // new root for the detached thread
    oobj root = OObjRoot_new_heap();
    // join together OFetch and the threads root
    oobj join = OJoin_new_heap(self, root);


    struct Data *data = o_new0(join, *data, 1);
    data->root = root;
    data->join = join;

    data->url = o_str_clone(join, url);
    data->opt_post_data = o_str_clone(join, opt_post_data);
    data->opt_post_data_size = opt_post_data_size;

    oobj thread = OThread_new_run(join, request_thread, "OFetch", data);
    OThread_detach(thread);

    // holds the tree, ith it's joined together data fields
    self->impl = data;

    return self;
}


struct oobj_opt OFetch_response(oobj obj, bool *opt_error)
{
    OObj_assert(obj, OFetch);
    OFetch *self = obj;
    struct Data *data = self->impl;

    OArray *res = NULL;
    o_lock_block(data->join) {
        *opt_error = data->error;
        if(!data->error && data->data_response) {
            // clone into OFetchs tree
            res = OArray_new(obj, o_at(data->data_response, 0), 1, o_num(data->data_response));
        }
    }
    return oobj_opt(res);
}

#endif
typedef int avoid_empty_translation_unit;
