#include "s/STrackArray.h"
#include "o/OObj_builder.h"
#include "o/OArray.h"
#include "s/wav.h"


#define O_LOG_LIB "s"
#include "o/log.h"

STrackArray *STrackArray_new_super(osize object_size, oobj parent, const float *opt_data, osize data_len,
                                   const struct s_audio_spec *opt_spec)
{
    assert(object_size >= (osize) sizeof(STrackArray));
    STrack *super = STrack_new_super(object_size, parent, opt_spec);
    STrackArray *self = (STrackArray*) super;
    OObj_id_set(self, STrackArray_ID);

    struct s_audio_spec spec = opt_spec ? *opt_spec : s_audio_spec_default();
    assert(data_len%spec.channels==0);
    osize ticks = data_len / spec.channels;
    self->array = OArray_new_dyn(self, opt_data, sizeof(float), data_len, data_len);
    self->mode = STrackArray_READ;

    // vfuncs
    super->v_retr = STrackArray__v_retr;
    super->v_duration = STrackArray__v_duration;

    return self;
}


STrackArray *STrackArray_new_ticks(oobj parent, osize len, const struct s_audio_spec *opt_spec)
{
    struct s_audio_spec spec = opt_spec ? *opt_spec : s_audio_spec_default();
    osize array_len = len * spec.channels;
    oobj track = STrackArray_new(parent, NULL, array_len, &spec);
    STrackArray_mode_set(track, STrackArray_WRITE);
    return track;
}


STrackArray *STrackArray_new_s(oobj parent, double seconds, const struct s_audio_spec *opt_spec)
{
    struct s_audio_spec spec = opt_spec ? *opt_spec : s_audio_spec_default();
    osize ticks = (osize) (spec.freq * seconds);
    return STrackArray_new_ticks(parent, ticks, &spec);
}

//
// virtual implementations:
//

bool STrackArray__v_retr(oobj obj, float *out_data, osize len, osize time_ticks)
{
    OObj_assert(obj, STrackArray);
    STrackArray *self = obj;
    o_lock(self);

    // in ticks
    osize start_pos = time_ticks;

    // call super to mix played on this track tracks
    STrack__v_retr(self, out_data, len, time_ticks);

    struct s_audio_spec spec = STrack_spec(self);

    bool ended = false;

    if(self->mode == STrackArray_READ || self->mode == STrackArray_READWRITE) {
        osize array_len = o_num(self->array);
        assert(array_len % spec.channels == 0);
        array_len /= spec.channels;
        osize remaining = array_len - start_pos;
        osize mix_len = o_min(remaining, len);
        if(start_pos>=0 && start_pos<array_len) {
            float *array_data = o_at(self->array, start_pos*spec.channels);
            s_mix_into(out_data, array_data, 1.0f, spec.channels, mix_len);
        }
        if(mix_len < len && self->mode == STrackArray_READ) {
            ended = true;
        }
    }

    if(self->mode == STrackArray_WRITE || self->mode == STrackArray_READWRITE) {
        osize array_len = o_num(self->array);
        assert(array_len % spec.channels == 0);
        array_len /= spec.channels;
        osize remaining = array_len - start_pos;
        if(len>remaining) {
            if(self->allow_growth) {
                osize missing = len - remaining;
                OArray_resize(self->array, (array_len + missing) * spec.channels);
            }else {
                len = remaining;
                ended = true;
            }
        }
        if(len > 0) {
            assert(start_pos>=0 && start_pos<array_len);
            float *array_data = o_at(self->array, start_pos * spec.channels);
            o_memcpy(array_data, out_data, 1, s_audio_spec_buffer_size(spec, len));
        }
    }

    o_unlock(self);
    return ended;
}

osize STrackArray__v_duration(oobj obj)
{
    OObj_assert(obj, STrackArray);
    STrackArray *self = obj;
    return o_num(self->array) / self->super.spec.channels;
}

//
// object functions
//

void STrackArray_feed(oobj obj, const float *data, osize len, const struct s_audio_spec *opt_spec)
{
    if(!data || len <= 0) {
        return;
    }
    OObj_assert(obj, STrackArray);
    STrackArray *self = obj;
    o_lock(self);

    oobj array = self->array;

    struct s_audio_spec given = opt_spec? *opt_spec : s_audio_spec_default();
    struct s_audio_spec needed = STrack_spec(self);
    if(s_audio_spec_equals(needed, given)) {
        // no resample needs to be done, just push
        OArray_append(array, data, s_audio_spec_array_size(needed, len));
        o_unlock(self);
        return;
    }

    // resample needed!
    osize track_len = s_resample_dst_ticks(needed, given, len);
    osize track_array_len = s_audio_spec_array_size(needed, track_len);

    if(track_array_len<=0) {
        // don't know if this may ever happen, just in case...
        return;
    }

    OArray_append(array, NULL, track_array_len);
    float *buf = o_at(array, o_num(array) - track_array_len);

    // resample into the appended array
    s_resample(needed, given, buf, data, len);

    o_unlock(self);
}

bool STrackArray_write_wav(oobj obj, const char *file)
{
    OObj_assert(obj, STrackArray);
    STrackArray *self = obj;
    o_lock(self);

    struct s_audio_spec spec = STrack_spec(self);
    bool ok = s_wav_write(file, o_at(self->array, 0), o_num(self->array), &spec);
    o_unlock(self);
    return ok;
}
