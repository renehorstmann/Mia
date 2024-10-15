#include "s/SFilterFade.h"
#include "o/OObj_builder.h"
#include "m/vec/vecn.h"
#include "s/STrack.h"


SFilterFade *SFilterFade_init(oobj obj, oobj parent)
{
    SFilterFade *self = obj;
    o_clear(self, sizeof *self, 1);

    SFilter_init(obj, parent);
    OObj_id_set(self, SFilterFade_ID);

    self->time_a = self->time_b = 0;
    self->amp_a = self->amp_b = 0;

    return self;
}

//
// virtual implementations:
//

void SFilterFade__v_apply(oobj obj, oobj track, float *data, int channels, osize len, osize time_ticks)
{
    OObj_assert(obj, SFilterFade);
    SFilterFade *self = obj;
    for(osize i=0; i<len; i++) {
        float *i_data = &data[i * channels];
        osize i_time = time_ticks + i;
        float t = (float) (i_time - self->time_a) / (float) (self->time_b - self->time_a);
        t = m_clamp(t, 0.0f, 1.0f);
        float amp = m_mix(self->amp_a, self->amp_b, t);
        vecn_scale(i_data, i_data, amp, channels);
    }
}

//
// object functions
//

void SFilterFade_set_ex(oobj obj, float amp_a, float amp_b, osize time_a, osize time_b, bool update_end_time)
{
    OObj_assert(obj, SFilterFade);
    SFilterFade *self = obj;
    assert(time_b>=time_a);
    self->amp_a = amp_a;
    self->amp_b = amp_b;
    self->time_a = time_a;
    self->time_b = time_b;
    if(update_end_time) {
        SFilter_end_time_set(self, time_b);
    }
}

void SFilterFade_set(oobj obj, oobj track, float amp_a, float amp_b, double duration_seconds, bool update_end_time)
{
    osize time_a = STrack_time_ticks(track);
    osize time_b = time_a + s_audio_spec_time_from_seconds(STrack_spec(track), duration_seconds);
    SFilterFade_set_ex(obj, amp_a, amp_b, time_a, time_b, update_end_time);
}
