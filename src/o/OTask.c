#include "o/OTask.h"
#include "o/OObj_builder.h"
#include "o/OArray.h"
#include "o/timer.h"


#define O_LOG_LIB "o"
#include "o/log.h"

O_STATIC
void run_once(OTask *self) {
    o_lock_block(self) {
        if (self->progress < OTask_FINISH) {
            float progress = self->fn(self, self->run_counter++);
            self->progress = progress < 0 ? -1.0f : o_clamp(progress, 0.0f, 1.0f);
        }
    }
}

OTask *OTask_init(oobj obj, oobj parent, OTask__fn fn)
{
    OTask *self = obj;
    o_clear(self, sizeof *self, 1);

    OObj_init(self, parent);
    OObj_id_set(self, OTask_ID);

    self->fn = fn;

    self->run_counter = 0;
    self->progress = -1;
    self->result = NULL;
    self->timeout_ms = OTask_TIMEOUT_MS_DEFAULT;

    return self;
}


//
// object functions
//

float OTask_progress(oobj obj)
{
    OObj_assert(obj, OTask);
    OTask *self = obj;
    float progress;
    o_lock_block(self) {
        progress = self->progress;
    }
    return progress;
}

int OTask_run_counter(oobj obj)
{
    OObj_assert(obj, OTask);
    OTask *self = obj;
    int run_counter;
    o_lock_block(self) {
        run_counter = self->run_counter;
    }
    return run_counter;
}

oobj OTask_result(oobj obj)
{
    OObj_assert(obj, OTask);
    OTask *self = obj;
    oobj result;
    o_lock_block(self) {
        if(self->progress<OTask_FINISH) {
            result = NULL;
        } else {
            result = o_or(self->result, self);
        }
    }
    return result;
}


float OTask_run_once(oobj obj)
{
    OObj_assert(obj, OTask);
    OTask *self = obj;
    if(self->opt_threadpool) {
        return OTask_progress(self);
    }
    // else run once
    run_once(self);
    return self->progress;
}

float OTask_run(oobj obj)
{
    OObj_assert(obj, OTask);
    OTask *self = obj;
    if(self->opt_threadpool) {
        return OTask_progress(self);
    }
    // else run until timeout (no need to lock or smth)
    ou64 timer = o_timer();
    do {
        run_once(self);
    } while(o_timer_elapsed_s(timer) < (self->timeout_ms/1000.0));
    return self->progress;
}



#ifdef MIA_OPTION_THREAD
#include "o/OFuture.h"

O_STATIC
void future_run(oobj future)
{
    OTask *self = o_user(future);
    OTask_run_blocking(self);
}

#endif

void OTask_run_in_threadpool(oobj obj, oobj threadpool)
{
    OObj_assert(obj, OTask);
    OTask *self = obj;
#ifdef MIA_OPTION_THREAD
    if(self->opt_threadpool) {
        o_log_warn_s(__func__, "already runs in a threadpool");
        return;
    }
    // run in parallel
    OFuture_new_run(self, future_run, threadpool, self);
    self->opt_threadpool = threadpool;
#endif
}

void OTask_result_set(oobj obj, oobj result)
{
    OObj_assert(obj, OTask);
    OTask *self = obj;
    o_lock_block(self) {
        self->result = result;
    }
}

