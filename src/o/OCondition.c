#ifdef MIA_OPTION_THREAD

#include "o/OCondition.h"
#include "o/OObj_builder.h"
#include <SDL2/SDL_mutex.h>

OCondition *OCondition_init(oobj obj, oobj parent)
{
    OCondition *self = obj;
    o_clear(self, sizeof *self, 1);

    OObj_init(self, parent);
    OObj_id_set(self, OCondition_ID);

    self->sdl_cond = SDL_CreateCond();
    o_assume(self->sdl_cond, "SDL_CreateCond failed");

    // mutex deletor that calls closed SDL_DestroyMutex
    self->super.v_del = OCondition__v_del;

    return self;
}


void OCondition__v_del(oobj obj)
{
    OObj_assert(obj, OCondition);
    OCondition *self = obj;
    SDL_DestroyCond(self->sdl_cond);

    // call super del
    OObj__v_del(obj);
}


void OCondition_broadcast(oobj obj)
{
    OObj_assert(obj, OCondition);
    OCondition *self = obj;
    int ret = SDL_CondBroadcast(self->sdl_cond);
    o_assume(ret == 0, "SDL_CondBroadcast failed");
}

void OCondition_signal(oobj obj)
{
    OObj_assert(obj, OCondition);
    OCondition *self = obj;
    int ret = SDL_CondSignal(self->sdl_cond);
    o_assume(ret == 0, "SDL_CondSignal failed");
}

void OCondition_wait(oobj obj, oobj mutex)
{
    OObj_assert(obj, OCondition);
    OObj_assert(mutex, OObj);
    OCondition *self = obj;
    OObj *self_mutex = mutex;
    int ret = SDL_CondWait(self->sdl_cond, self_mutex->mutex);
    o_assume(ret == 0, "SDL_CondWait failed");
}

bool OCondition_wait_timeout(oobj obj, oobj mutex, int timeout_ms)
{
    OObj_assert(obj, OCondition);
    OObj_assert(mutex, OObj);
    OCondition *self = obj;
    OObj *self_mutex = mutex;
    int ret = SDL_CondWaitTimeout(self->sdl_cond, self_mutex->mutex, timeout_ms);
    o_assume(ret != 0 && ret != SDL_MUTEX_TIMEDOUT, "SDL_CondWait failed");
    return ret == 0;
}


#endif // MIA_OPTION_THREAD
typedef int avoid_empty_translation_unit;
