// curl is used on desktop versions and in CxxDroid
#if defined(MIA_OPTION_FETCH) && (defined(MIA_PLATFORM_DESKTOP) || defined(MIA_PLATFORM_CXXDROID))

#include "o/OFetch.h"
#include "o/OObj_builder.h"
#include "o/OObjRoot.h"
#include "o/OJoin.h"
#include "o/OThread.h"
#include "o/OArray.h"
#include "o/str.h"
#include <curl/curl.h>

#define O_LOG_LIB "o"
#include "o/log.h"


//
// protected
//

O_EXTERN
void o__fetch_init(void)
{
    curl_global_init(CURL_GLOBAL_ALL);
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

    oobj data_collecting;

    // locked:

    bool error;
    oobj data_response;
};


O_STATIC
size_t response_writer(char *ptr, size_t size, size_t nmemb, void *userdata) {
    struct Data *data = userdata;
    osize full_size = (osize) (size * nmemb);
    OArray_append(data->data_collecting, ptr, full_size);
    return full_size;
}

O_STATIC
void request_thread(oobj thread_obj)
{
    struct Data *data = o_user(thread_obj);

    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, data->url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, response_writer);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, data);

    // don't use signals in multi threads
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

    if(data->opt_post_data) {
        // POST
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/octet-stream");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data->opt_post_data);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data->opt_post_data_size);

    }

    data->data_collecting = OArray_new_dyn(data->join, NULL, 1, 0, 1024);

    CURLcode perform_res = curl_easy_perform(curl);

    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

    curl_easy_cleanup(curl);

    bool error = perform_res != CURLE_OK || http_code != 200;

    if(error) {
        o_del(data->data_collecting);
    }

    // set response
    o_lock_block(data->join) {
        data->error = error;

        // NULL on error (o_del)
        data->data_response = data->data_collecting;
    }

    if(error) {
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
