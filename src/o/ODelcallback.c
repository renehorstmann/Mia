#include "o/ODelcallback.h"
#include "o/OObj_builder.h"
#include "o/str.h"

#define O_LOG_LIB "o"
#include "o/log.h"


O_STATIC
void log_trace_callback(oobj obj) {
    const char *name = o_or(OObj_name(obj), "");
    const char *msg = o_or(o_user(obj), "");
    o_log_trace_s("ODelcallback_new_log_trace", "Deleted: \"%s\" %s", name, msg);
}

O_STATIC
void assert_callback(oobj obj) {
    const char *name = o_or(OObj_name(obj), "");
    const char *msg = o_or(o_user(obj), "");
    o_log_trace_s("ODelcallback_new_assert", "Deleted: \"%s\" %s", name, msg);
}

//
// public
//

ODelcallback *ODelcallback_init(oobj obj, oobj parent, OObj__event_fn fn)
{
    ODelcallback *self = obj;
    o_clear(self, sizeof *self, 1);

    OObj_init(self, parent);
    OObj_id_set(self, ODelcallback_ID);

    self->fn = fn;

    // v del override
    self->super.v_del = ODelcallback__v_del;

    return self;
}

ODelcallback *ODelcallback_new_log_trace(oobj parent, const char *name, const char *msg)
{
    oobj obj = ODelcallback_new(parent, log_trace_callback);
    OObj_name_set(obj, name);
    o_user_set(obj, o_str_clone(obj, msg));
    return obj;
}

ODelcallback *ODelcallback_new_assert(oobj parent, const char *name, const char *msg)
{
    oobj obj = ODelcallback_new(parent, assert_callback);
    OObj_name_set(obj, name);
    o_user_set(obj, o_str_clone(obj, msg));
    return obj;
}

//
// virtual implementations:
//

void ODelcallback__v_del(oobj obj)
{
    OObj_assert(obj, ODelcallback);
    ODelcallback *self = obj;
    if(self->fn) {
        self->fn(self);
    }
    OObj__v_del(self);
}
