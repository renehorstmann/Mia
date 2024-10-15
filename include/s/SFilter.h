#ifndef S_SFILTER_H
#define S_SFILTER_H

/**
 * @file SFilter.h
 *
 * Object
 *
 * Abstract filter to be used on a STrackArray.
 */


#include "o/OObj.h"

/** object id */
#define SFilter_ID OObj_ID "SFilter"


/**
 * Virtual function to filter from the track. The default is a noop, returns true if end time is reached
 * @param obj SFilter object
 * @param track STrack object on which this SFilter is applied
 * @param data channels * len long to fill in data retrieved by this track
 * @param len for the data in ticks
 * @param time_ticks current track time
 */
typedef void (*SFilter__apply_fn)(oobj obj, oobj track, float *data, int channels, osize len, osize time_ticks);


typedef struct {
    OObj super;

    // measured in track pcm ticks (freq)
    // NOT to be confused with o_timer's time ticks (>not< ou64 by design...)
    osize end_time;

    // vfuncs
    SFilter__apply_fn v_apply;
} SFilter;


/**
 * Initializes the object
 * @param obj SFilter object
 * @param parent to inherit from
 * @return obj casted as SFilter
 */
O_EXTERN
SFilter *SFilter_init(oobj obj, oobj parent);

/**
 * Creates a new the SFilter object
 * @param parent to inherit from
 * @return The new object
 */
O_INLINE
SFilter *SFilter_new(oobj parent)
{
    OObj_DECL_IMPL_NEW(SFilter, parent);
}


//
// virtual implementations:
//

/**
 * Virtual function to filter from the track. The default is a noop
 * @param obj SFilter object
 * @param track STrack object on which this SFilter is applied
 * @param data channels * len long to fill in data retrieved by this track
 * @param len for the data in ticks
 * @param time_ticks current track time
 */
O_EXTERN
void SFilter__v_apply(oobj obj, oobj track, float *data, int channels, osize len, osize time_ticks);

//
// object functions
//


/**
 * @param obj SFilter object
 * @return measured in track pcm ticks (freq)
 * @note NOT to be confused with o_timer's time ticks
 */
OObj_DECL_GETSET(SFilter, osize, end_time)

#endif //S_SFILTER_H
