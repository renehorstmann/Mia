#ifdef MIA_OPTION_FETCH
#ifndef O_OFETCH_H
#define O_OFETCH_H

/**
 * @file OFetch.h
 *
 * Object
 *
 * A Fetch creates a http GET or POST request
 *
 * @note deleting the OFetch will abort the fetch.
 * For that an internal new OOBjRoot tree is created and shared with OJoin
 */


#include "OArray.h"


/** object id */
#define OFetch_ID OObj_ID "OFetch"


typedef struct {
    OObj super;

    void *impl;
} OFetch;


/**
 * Initializes the object
 * @param obj OFetch object
 * @param parent to inherit from
 * @param url Full http url (example.com instead of https://example.com may uses the relative path...)
 * @param opt_post_data if NULL:  GET
 *                      if !NULL: POST with that data and opt_post_data_size
 *                                content type is set to text/plain
 * @return obj casted as OFetch
 */
O_EXTERN
OFetch *OFetch_init(oobj obj, oobj parent, const char *url, const void *opt_post_data, osize opt_post_data_size);

/**
 * Creates a new the OFetch object
 * @param parent to inherit from
 * @param fn the future execution function
 * @param url Full http url (example.com instead of https://example.com may uses the relative path...)
 * @param opt_post_data if NULL:  GET
 *                      if !NULL: POST with that data and opt_post_data_size
 *                                content type is set to text/plain
 * @return The new object
 */
O_INLINE
OFetch *OFetch_new(oobj parent, const char *url, const void *opt_post_data, osize opt_post_data_size)
{
    OObj_DECL_IMPL_NEW(OFetch, parent, url, opt_post_data, opt_post_data_size);
}

//
// object functions:
//

/**
 * Checks if the fetch response is available and returns it as OArray object (else NULL)
 * @param obj OFetch object
 * @param opt_error If not NULL, will be set to true in an error occurred
 * @return A new OArray object, containing the full response, or NULL on error, or if not received yet
 */
O_EXTERN
struct oobj_opt OFetch_response(oobj obj, bool *opt_error);

/**
 * Waits for the fetch to be available and returns it as OArray object (else NULL)
 * @param obj OFetch object
 * @param opt_timeout_ms if <=0, waits at max that time
 * @return A new OArray object, containing the full response, or NULL on error
 * @note: just calls OFetch_response and o_thread_sleep(1) in a loop until either data or opt_error is set
 */
O_STATIC
struct oobj_opt OFetch_response_wait(oobj obj, oi32 opt_timeout_ms)
{
    bool error = false;
    struct oobj_opt res = {0};
    if(opt_timeout_ms<=0) {
        opt_timeout_ms = oi32_MAX;
    }
    for(int i = 0; i < opt_timeout_ms; i++) {
        res = OFetch_response(obj, &error);
        if(res.o || error) {
            break;
        }
        o_sleep(1);
    }
    // returns res=NULL if error is true...
    return res;
}


#endif //O_OFETCH_H
#endif //MIA_OPTION_FETCH
