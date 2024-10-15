#include "s/STrack.h"
#include "o/OObj_builder.h"
#include "o/OArray.h"
#include "o/OJoin.h"
#include "o/OWeakjoin.h"

#include "s/wav.h"
#include "s/SFilter.h"

#define O_LOG_LIB "s"
#include "o/log.h"


// protected
O_EXTERN
void STrack__played_del(struct STrack__played *self)
{
    o_del(self->weak);
    o_del(self->opt_parent);
}


STrack* STrack_new_super(osize object_size, oobj parent, const struct s_audio_spec *opt_spec)
{
    assert(object_size >= (osize) sizeof(STrack));
    OJoin *super = OJoin_new_super(object_size, &parent, 1, o_allocator_heap_new());
    STrack *self = (STrack *) super;
    OObj_id_set(self, STrack_ID);


    if(opt_spec) {
        self->spec = *opt_spec;
    } else {
        self->spec = s_audio_spec_default();
    }

    self->endable = true;

    // allocates the played array, inits time, and sets active to true
    STrack_reset(self);
    self->played_idx = 0;

    self->filter = OArray_new_dyn(self, NULL, sizeof(oobj), 0, 4);

    // vfuncs
    self->v_retr = STrack__v_retr;
    self->v_duration = STrack__v_duration;

    return self;
}


//
// virtual implementations:
//


bool STrack__v_retr(oobj obj, float *out_data, osize len, osize time_ticks)
{
    OObj_assert(obj, STrack);
    STrack *self = obj;
    o_lock(self);

    assert(time_ticks>=0);
    if(self->time_ticks_auto) {
        self->time_ticks = time_ticks;
    }

    oobj container = OObj_new(obj);

    // init silence (floats bitmap is in fact 0 for 0.0f), so using o_clea for performance
    o_clear(out_data, 1, s_audio_spec_buffer_size(self->spec, len));

    // keep track of played to remove
    bool *p_del = o_new0(container, bool, o_num(self->played));

    float *mix_buf = o_new(container, float, s_audio_spec_buffer_size(self->spec, len));

    for(osize i=0; i<o_num(self->played); i++) {
        struct STrack__played *p = o_at(self->played, i);

        // needs to be released, if valid!
        struct oobj_opt track = OWeakjoin_acquire(p->weak);
        if(!track.o) {
            p_del[i] = true;
            continue;
        }

        bool retr = true;

        // span before track begins?
        if(time_ticks + len <= p->start_ticks) {
            retr = false;
        }

        if(retr) {

            osize track_time = time_ticks - p->start_ticks;
            osize track_len = len;
            float *out_track_data = out_data;
            if(track_time < 0) {
                // tracks are not able to retr data before their start tick, so shifting to start
                track_len += track_time;
                out_track_data -= track_time*self->spec.channels;
                track_time = 0;
            }

            // retrieve track data in this spec
            bool ended = STrack_retr(track.o, mix_buf, track_len, track_time, &self->spec);
            if(ended && STrack_endable(track.o)) {
                p_del[i] = true;
            }

            // mix into our output
            s_mix_into(out_track_data, mix_buf, p->amp, self->spec.channels, track_len);
        }

        OWeakjoin_release(p->weak);
    }

    // pop from back to front so the idx remains valid
    for(osize i=o_num(self->played)-1; i>=0; i--) {
        if(p_del[i]) {
            struct STrack__played *p = o_at(self->played, i);
            STrack__played_del(p);
            OArray_pop_at(self->played, i, NULL);
        }
    }

    o_unlock(self);
    return o_num(self->played)==0;
}

osize STrack__v_duration(oobj obj)
{
    return osize_MAX;
}

//
// object functions
//


O_STATIC
void track_apply_filters(STrack *self, float *data, osize len, osize time_ticks)
{
    // already locked STrack!

    for(osize i=0; i<o_num(self->filter); i++)
    {
        oobj *filter = o_at(self->filter, i);
        OObj_assert(*filter, SFilter);
        SFilter *f = *filter;
        f->v_apply(f, self, data, self->spec.channels, len, time_ticks);
    }

    // remove and del backwards
    for(osize i=o_num(self->filter)-1; i>=0; i--)
    {
        oobj *filter = o_at(self->filter, i);
        OObj_assert(*filter, SFilter);
        SFilter *f = *filter;
        osize end_time = f->end_time;
        if(time_ticks > end_time) {
            OArray_pop_at(self->filter, i, NULL);
        }
        o_del(f);
    }

}


bool STrack_retr(oobj obj, float *out_data, osize len, osize time_ticks, const struct s_audio_spec *opt_spec)
{
    OObj_assert(obj, STrack);
    STrack *self = obj;
    o_lock(self);

    struct s_audio_spec wanted = opt_spec? *opt_spec : s_audio_spec_default();
    if(s_audio_spec_equals(self->spec, wanted)) {
        // no resample needs to be done, just calling the vfunc
        bool ended = self->v_retr(obj, out_data, len, time_ticks);
        track_apply_filters(self, out_data, len, time_ticks);
        o_unlock(self);
        return ended;
    }
    // resample needed!
    osize track_len = s_resample_dst_ticks(self->spec, wanted, len);
    osize track_time_ticks = s_resample_dst_ticks(self->spec, wanted, time_ticks);
    float *tmp = o_new(obj, float, s_audio_spec_array_size(self->spec, track_len));
    // retrieve data from this track
    bool ended = self->v_retr(obj, tmp, track_len, track_time_ticks);
    track_apply_filters(self, tmp, track_len, track_time_ticks);
    // resample into the output
    s_resample(wanted, self->spec, out_data, tmp, track_len);
    // done...
    o_free(obj, tmp);

    o_unlock(self);
    return ended;
}

oobj STrack_retr_span_ex(oobj obj, osize len, osize time_ticks, const struct s_audio_spec *opt_spec, bool *opt_out_ended)
{
    o_lock(obj);

    struct s_audio_spec wanted = opt_spec? *opt_spec : s_audio_spec_default();
    if(len < 0) {
        osize duration = STrack_duration_ticks(obj);
        if(duration < osize_MAX && time_ticks < duration) {
            len = duration - time_ticks;
        } else {
            o_log_warn_s(__func__, "invalid duration");
            len = 0;
        }
    }
    oobj array = OArray_new(obj, NULL, sizeof(float),
                            s_audio_spec_array_size(wanted, len));
    bool ended = STrack_retr(obj, OArray_data_void(array), len, time_ticks, &wanted);
    o_opt_set(opt_out_ended, ended);

    o_unlock(obj);
    return array;
}

void STrack_reset(oobj obj)
{
    OObj_assert(obj, STrack);
    STrack *self = obj;
    o_lock(self);

    // struct player stuff is allocated on the player array
    // so all OWeakjoin's are deleted as well
    o_del(self->played);
    self->played = OArray_new_dyn(self, NULL, sizeof(struct STrack__played), 0, 32);

    o_unlock(self);
}

osize STrack_play_ex(oobj obj, oobj track, osize time_ticks, float amp, bool parent_join)
{
    OObj_assert(obj, STrack);
    STrack *self = obj;
    o_lock(self);

    struct STrack__played p = {0};
    p.handle_idx = ++self->played_idx;
    p.weak = OWeakjoin_new(self->played, track);
    if(parent_join) {
        p.opt_parent = OObj_new(self->played);
        OJoin_add(track, p.opt_parent);
    }
    p.start_ticks = self->time_ticks + time_ticks;
    p.amp = amp;

    OArray_push(self->played, &p);

    o_unlock(self);
    return p.handle_idx;
}

// -1 on failure
O_STATIC
osize get_played_array_idx_handle(oobj obj, osize handle)
{
    OObj_assert(obj, STrack);
    STrack *self = obj;
    for(osize i=0; i<o_num(self->played); i++) {
        struct STrack__played *p = o_at(self->played, i);
        if(p->handle_idx == handle) {
            return i;
        }
    }
    return -1;
}

O_STATIC
struct STrack__played *get_played_from_handle(oobj obj, osize handle)
{
    OObj_assert(obj, STrack);
    STrack *self = obj;
    for(osize i=0; i<o_num(self->played); i++) {
        struct STrack__played *p = o_at(self->played, i);
        if(p->handle_idx == handle) {
            return p;
        }
    }
    return NULL;
}


bool STrack_played_available(oobj obj, osize handle)
{
    o_lock(obj);
    bool available = get_played_from_handle(obj, handle) != NULL;
    o_unlock(obj);
    return available;
}

bool STrack_played_remove(oobj obj, osize handle)
{
    OObj_assert(obj, STrack);
    STrack *self = obj;
    o_lock(self);

    osize idx = get_played_array_idx_handle(obj, handle);
    if(idx < 0) {

        o_unlock(self);
        return false;
    }
    struct STrack__played *p = o_at(self->played, idx);
    STrack__played_del(p);
    OArray_pop_at(self->played, idx, NULL);

    o_unlock(self);
    return true;
}

bool STrack_played_amp_set(oobj obj, osize handle, float amp)
{
    o_lock(obj);
    struct STrack__played *p = get_played_from_handle(obj, handle);
    if(p) {
        p->amp = amp;
    }
    o_unlock(obj);
    return p!=NULL;
}


void STrack_filter_add(oobj obj, oobj filter_sink)
{
    o_lock(obj);
    o_move(filter_sink, obj);
    oobj array = STrack_filter(obj);
    OArray_push(array, &filter_sink);
    o_unlock(obj);
}
