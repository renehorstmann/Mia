#ifndef S_SFILTERFADE_H
#define S_SFILTERFADE_H

/**
 * @file SFilterFade.h
 *
 * Object (derives SFilter)
 *
 * Filter that fades the amplitude of the track data in or out
 */


#include "SFilter.h"

/** object id */
#define SFilterFade_ID SFilter_ID "Fade"


typedef struct {
    SFilter super;

    float amp_a, amp_b;
    osize time_a, time_b;

} SFilterFade;


/**
 * Initializes the object
 * @param obj SFilterFade object
 * @param parent to inherit from
 * @return obj casted as SFilter
 */
O_EXTERN
SFilterFade *SFilterFade_init(oobj obj, oobj parent);

/**
 * Creates a new the SFilterFade object
 * @param parent to inherit from
 * @return The new object
 */
O_INLINE
SFilterFade *SFilterFade_new(oobj parent)
{
    OObj_DECL_IMPL_NEW(SFilterFade, parent);
}


//
// virtual implementations:
//

/**
 * Virtual function to filter from the track.
 * Fades the amplitude in or out.
 * @param obj SFilterFade object
 * @param track STrack object on which this SFilter is applied
 * @param data channels * len long to fill in data retrieved by this track
 * @param len for the data in ticks
 * @param time_ticks current track time
 * @return true if end was reached
 */
O_EXTERN
void SFilterFade__v_apply(oobj obj, oobj track, float *data, int channels, osize len, osize time_ticks);

//
// object functions
//

/**
 * Sets up the fading
 * @param obj SFilterFade object
 * @param update_end_time SFilter_end_time will be set to time_b to auto remove this filter from the STrack
 */
O_EXTERN
void SFilterFade_set_ex(oobj obj, float amp_a, float amp_b, osize time_a, osize time_b, bool update_end_time);

/**
 * Sets up the fading.
 * Uses the current STrack_time and STrack_spec to calc tima_a and time_b
 * @param obj SFilterFade object
 * @param track STrack on which this filter is applied
 * @param update_end_time SFilter_end_time will be set to time_b to auto remove this filter from the STrack
 */
O_EXTERN
void SFilterFade_set(oobj obj, oobj track, float amp_a, float amp_b, double duration_seconds, bool update_end_time);

#endif //S_SFILTERFADE_H
