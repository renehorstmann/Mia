#ifdef MIA_PLATFORM_EMSCRIPTEN

#include "o/OFetch.h"
#include "o/OObj_builder.h"
#include "o/OObjRoot.h"
#include "o/OArray.h"
#include "o/OJoin.h"
#include "o/str.h"
#include <emscripten/fetch.h>

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
    osize opt_pose_data_size;

    // locked:

    bool error;
    oobj data_response;
};


O_STATIC
void ems_fetch_success(emscripten_fetch_t *fetch)
{
    struct Data *data = fetch->userData;
    data->error = false;
    data->data_response = OArray_new(data->join, fetch->data, 1, fetch->numBytes);
    o_log_trace_s("OFetch", "fetched %"osize_PRI" bytes from: <%s>",
               OArray_num(data->data_response), data->url);
    emscripten_fetch_close(fetch);

    // deletes one side of the join, if OFetch is also deleted, the OJoin gets deleted
    o_del(data->root);
}

O_STATIC
void ems_fetch_error(emscripten_fetch_t *fetch)
{
    struct Data *data = fetch->userData;
    data->data_response = NULL;
    data->error = true;
    o_log_warn_s("OFetch", "failed to fetch: <%s>, error code: %i", data->url, fetch->status);
    emscripten_fetch_close(fetch);

    // deletes one side of the join, if OFetch is also deleted, the OJoin gets deleted
    o_del(data->root);
}

//
// public
//

OFetch *OFetch_init(oobj obj, oobj parent, const char *url, const void *opt_post_data, osize opt_post_data_size)
{
    assert((!opt_post_data || opt_post_data_size > 0) && "either NULL to opt_post_data or set a valid positive size");

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
    data->opt_pose_data_size = opt_post_data_size;


    emscripten_fetch_attr_t attr;

    emscripten_fetch_attr_init(&attr);
    attr.userData = data;
    attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
    attr.onsuccess = ems_fetch_success;
    attr.onerror = ems_fetch_error;

    if(opt_post_data) {
        strcpy(attr.requestMethod, "POST");
        attr.requestData = data->opt_post_data;
        attr.requestDataSize = data->opt_pose_data_size;
        attr.requestHeaders = (const char *[]) {"Content-Type", "application/octet-stream", NULL};
    } else {
        strcpy(attr.requestMethod, "GET");
    }

    emscripten_fetch_t *fetch = emscripten_fetch(&attr, url);
    assert(fetch->userData == data);

    // holds the tree, which gets killed either by the detached thread or the objs tree
    self->impl = data;

    return self;
}



struct oobj_opt OFetch_response(oobj obj, bool *opt_error)
{
    OObj_assert(obj, OFetch);
    OFetch *self = obj;
    struct Data *data = self->impl;

    OArray *res = NULL;
//    o_lock_block(data->join) {
    *opt_error = data->error;
    if (!data->error && data->data_response) {
        // clone into OFetchs tree
            res = OArray_new(obj, o_at(data->data_response, 0), 1, o_num(data->data_response));
    }
//    }
    return oobj_opt(res);
}

#endif
typedef int avoid_empty_translation_unit;
