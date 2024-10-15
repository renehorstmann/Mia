#include "s/SFilter.h"
#include "o/OObj_builder.h"


SFilter *SFilter_init(oobj obj, oobj parent)
{
    SFilter *self = obj;
    o_clear(self, sizeof *self, 1);

    OObj_init(self, parent);
    OObj_id_set(self, SFilter_ID);

    self->end_time = osize_MAX;

    // vfunc
    self->v_apply = SFilter__v_apply;

    return self;
}



void SFilter__v_apply(oobj obj, oobj track, float *data, int channels, osize len, osize time_ticks)
{
    // noop
}
