#include "s/STrackArena.h"
#include "o/OObj_builder.h"
#include "o/OArray.h"


#define O_LOG_LIB "s"
#include "o/log.h"

STrackArena *STrackArena_new_super(osize object_size, oobj parent, int limit, const struct s_audio_spec *opt_spec)
{
    assert(object_size >= (osize) sizeof(STrackArena));
    STrack *super = STrack_new_super(object_size, parent, opt_spec);
    STrackArena *self = (STrackArena*) super;
    OObj_id_set(self, STrackArena_ID);

    super->endable = false;
    super->time_ticks_auto = true;
    self->limit = limit;

    // vfuncs
    super->v_retr = STrackArena__v_retr;

    return self;
}

//
// virtual implementations:
//

bool STrackArena__v_retr(oobj obj, float *out_data, osize len, osize time_ticks)
{
    OObj_assert(obj, STrackArena);
    STrack *super = obj;
    STrackArena *self = obj;
    o_lock(self);

    assert(self->limit>=0);

    // protected
    O_EXTERN
    void STrack__played_del(struct STrack__played *self);

    // keep in limit and remove oldest (at front) until limit is reached
    while(o_num(super->played) > self->limit) {
        struct STrack__played *p = o_at(super->played, 0);
        o_log_trace_s("STrackArena", "popping: %"osize_PRI, p->handle_idx);
        STrack__played_del(p);
        OArray_pop_front(super->played, NULL);
    }

    // call super to mix played on this track tracks
    bool ended = STrack__v_retr(self, out_data, len, time_ticks);

    o_unlock(self);
    return ended;
}

//
// object functions
//

