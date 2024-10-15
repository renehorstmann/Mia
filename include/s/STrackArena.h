#ifndef S_STRACKARENA_H
#define S_STRACKARENA_H

/**
 * @file STrackArena.h
 *
 * Object (derives STrack)
 *
 * STrack implementation that only mixe's up to "limit" played tracks.
 * If a new track is played and limit is already reached, the oldest track is discarded.
 * Useful to reduce the mixing overhead when playing a lot of tracks in parallel.
 */

#include "STrack.h"

/** object id */
#define STrackArena_ID STrack_ID "Arena"


typedef struct {
    STrack super;

    int limit;
} STrackArena;


/**
 * Creates a new the STrackArena object
 * This function can be used to init an derived OJoin (for thread safe stuff as an example)
 * @param object_size size to allocate (asserts >= sizeof(OJoin)
 * @param parent to inherit from
 * @param limit maximal parallel played tracks
 * @param opt_spec specification to be used, NULL for system spec
 * @return The new object
 * @note inits endable to false and time_ticks_auto to true
 */
O_EXTERN
STrackArena *STrackArena_new_super(osize object_size, oobj parent, int limit, const struct s_audio_spec *opt_spec);

/**
 * Creates a new the STrackArena object
 * @param parent to inherit from
 * @param limit maximal parallel played tracks
 * @param opt_spec specification to be used, NULL for system spec
 * @return The new object
 * @note inits endable to false and time_ticks_auto to true
 */
O_INLINE
STrackArena *STrackArena_new(oobj parent, int limit, const struct s_audio_spec *opt_spec)
{
    return STrackArena_new_super(sizeof(STrackArena), parent, limit, opt_spec);
}

//
// virtual implementations:
//

/**
 * virtual function.
 * First checks if tracks need to be removed to clamp to the limit,
 * then calls STrack__v_retr to mix the remaining.
 * @param obj STrackArena object
 * @param out_data to write into
 * @param len frequency ticks
 * @param time_ticks current track time
 * @return true if end was reached
 */
O_EXTERN
bool STrackArena__v_retr(oobj obj, float *out_data, osize len, osize time_ticks);

//
// object functions
//


/**
 * @param obj OStreamArena object
 * @return maximal parallel played tracks
 */
OObj_DECL_GETSET(STrackArena, int, limit)


#endif //S_STRACKARENA_H
