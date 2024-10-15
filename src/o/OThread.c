#ifdef MIA_OPTION_THREAD

#include "o/OThread.h"
#include "o/OObj_builder.h"
#include "o/str.h"

#define O_LOG_LIB "o"
#include "o/log.h"

#include <SDL2/SDL_thread.h>
#include <time.h>


O_STATIC
int thread_function(void *user_data)
{
    // seed random
    o_rand_seed(time(NULL));

    OThread *self = user_data;

    // pass parents log settings to the new thread
    o_log_level_set(self->parent_init_log_level);
    o_log_quiet_set(self->parent_init_log_quiet);

    // call the OThread function
    self->fn(self);

    return 0;
}

OThread *OThread_init(oobj obj, oobj parent, OObj__event_fn fn, const char *name)
{
    OThread *self = obj;
    o_clear(self, sizeof *self, 1);

    OObj_init(self, parent);
    OObj_id_set(self, OThread_ID);

    self->state = OThread_INIT;
    self->fn = fn;
    self->name = o_str_clone(self, name);

    // set virtual deletor
    self->super.v_del = OThread__v_del;

    OObj_name_set(self, name);

    return self;
}

OThread *OThread_new_run(oobj parent, OObj__event_fn fn, const char *name, void *user)
{
    OThread *self = OThread_new(parent, fn, name);
    o_user_set(self, user);
    OThread_run(self);
    return self;
}

void OThread__v_del(oobj obj)
{
    OThread_wait(obj);
    OObj__v_del(obj);
}

void OThread_run(oobj obj)
{
    OObj_assert(obj, OThread);
    OThread *self = obj;

    if(self->state != OThread_INIT || self->thread) {
        // noop
        return;
    }
    // copy parents current log settings
    self->parent_init_log_level = o_log_level();
    self->parent_init_log_quiet = o_log_quiet();

    // run the sdl thread
    self->state = OThread_RUNNING;
    self->thread = SDL_CreateThread(thread_function, self->name, self);
    self->id = SDL_GetThreadID(self->thread);
}

void OThread_wait(oobj obj)
{
    OObj_assert(obj, OThread);
    OThread *self = obj;

    assert(self->state != OThread_INIT);
    if(self->state != OThread_RUNNING || !self->thread) {
        // noop
        return;
    }
    int status;
    SDL_WaitThread(self->thread, &status);
    self->thread = NULL;
    self->state = OThread_FINISHED;
}

void OThread_detach(oobj obj)
{
    OObj_assert(obj, OThread);
    OThread *self = obj;

    assert(self->state != OThread_INIT);
    if(self->state != OThread_RUNNING || !self->thread) {
        // noop
        return;
    }

    SDL_DetachThread(self->thread);
    self->thread = NULL;
    self->state = OThread_DETACHED;
}

#endif // MIA_OPTION_THREAD
typedef int avoid_empty_translation_unit;
