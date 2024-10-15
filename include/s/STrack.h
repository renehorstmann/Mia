#ifndef S_STRACK_H
#define S_STRACK_H

/**
 * @file STrack.h
 *
 * Object (derives OJoin)
 *
 * An abstraction for a reading or writing track pipeline.
 * Each STrack may have a list of registered ready to be played STracks.
 *
 * The tracks are like oneway roads.
 * Once a track is played onto a track and it gets discarded, all tracks must be played again to replay.
 *
 * STrack's are fully thread safe due to the deriving of the OJoin.
 * Because they derive from the special object OJoin, do not directly call o_del.
 * Delete the parent instead!
 *
 */

#include "s/common.h"
#include "o/OJoin.h"

/** object id */
#define STrack_ID OJoin_ID "STrack"


/**
 * Virtual function to retrieve data from the track. The default mixes all played tracks or returns silence else
 * @param obj STrack object
 * @param out_data len*STrack_spec(obj).channels long to fill in data retrieved by this track
 * @param len for the data in ticks
 * @param time_ticks current track time (for the first sample)
 * @return true if end was reached
 */
typedef bool (*STrack__retr_fn)(oobj obj, float *out_data, osize len, osize time_ticks);

/**
 * Virtual getter for an optional duration
 * @param obj STrack object
 * @return duration if available, else osize_MAX
 */
typedef osize (*STrack__duration_fn)(oobj obj);


// protected
struct STrack__played {
    osize handle_idx;
    oobj weak;
    oobj opt_parent;
    osize start_ticks;
    float amp;
};

typedef struct
{
    OJoin super;

    // either user set or when enabling time_ticks_auto (default is off)
    // measured in track pcm ticks (freq)
    // NOT to be confused with o_timer's time ticks (>not< ou64 by design...)
    osize time_ticks;
    bool time_ticks_auto;

    struct s_audio_spec spec;

    // if true (default) the track will get removed from beeing played if it signals an end on retr
    bool endable;

    // OArray of registered tracks to be played (struct STrack__played)
    oobj played;
    osize played_idx;

    // OArray of applied SFilter's
    oobj filter;

    // vfuncs
    STrack__retr_fn v_retr;
    STrack__duration_fn v_duration;

} STrack;


/**
 * Creates a new the STrack object.
 * This function can be used to init an derived OJoin (for thread safe stuff as an example)
 * @param object_size size to allocate (asserts >= sizeof(OJoin)
 * @param parent to inherit from
 * @param opt_spec specification to be used, NULL for system spec
 * @return The new object
 */
O_EXTERN
STrack* STrack_new_super(osize object_size, oobj parent, const struct s_audio_spec *opt_spec);


/**
 * Creates a new the STrack object
 * This function can be used to init an derived OJoin (for thread safe stuff as an example)
 * @param object_size size to allocate (asserts >= sizeof(OJoin)
 * @param parent to inherit from
 * @param opt_spec specification to be used, NULL for system spec
 * @return The new object
 */
O_INLINE
STrack* STrack_new(oobj parent, const struct s_audio_spec *opt_spec)
{
    return STrack_new_super(sizeof(STrack), parent, opt_spec);
}


//
// virtual implementations:
//

/**
 * virtual function
 * simply retrieves and mix's all played tracks, else silence
 * @param obj STrack object
 * @param out_data to write into
 * @param len frequency ticks
 * @param time_ticks current track time (for the first sample)
 * @return true if end was reached
 */
O_EXTERN
bool STrack__v_retr(oobj obj, float *out_data, osize len, osize time_ticks);

/**
 * Virtual getter for an optional duration.
 * @param obj STrack object
 * @return osize_MAX
 */
O_EXTERN
osize STrack__v_duration(oobj obj);

//
// object functions
//


/**
 * @param obj STrack object
 * @return track data specifications like frequency
 */
OObj_DECL_GET(STrack, struct s_audio_spec, spec)

/**
 * @param obj STrack object
 * @return measured in track pcm ticks (freq)
 * @note NOT to be confused with o_timer's time ticks
 */
OObj_DECL_GETSET(STrack, osize, time_ticks)

/**
 * @param obj STrack object
 * @return measured in seconds
 */
O_INLINE
double STrack_time(oobj obj)
{
    return s_audio_spec_time_as_seconds(STrack_spec(obj), STrack_time_ticks(obj));
}

/**
 * @param obj STrack object
 * @return automatically update time_ticks on retr (default is off)
 */
OObj_DECL_GETSET(STrack, bool, time_ticks_auto)

/**
 * @param obj STrack object
 * @return measured in ticks, default is osize_MAX
 */
O_INLINE
osize STrack_duration_ticks(oobj obj)
{
    OObj_assert(obj, STrack);
    STrack *self = obj;
    return self->v_duration(self);
}

/**
 * @param obj STrack object
 * @return measured in seconds
 */
O_INLINE
double STrack_duration(oobj obj)
{
    return s_audio_spec_time_as_seconds(STrack_spec(obj), STrack_duration_ticks(obj));
}

/**
 * @param obj STrack object
 * @return if true (default) the track will get removed while playing, when its retr signals the end
 */
OObj_DECL_GETSET(STrack, bool, endable)

/**
 * Calls the vfunc and will convert the data to the wanted spec, if unequal.
 * Filled with silence (0.0f) if no data is available.
 * After data has been retrieved, it will get filtered with the applied SFilter's (STrack_filters)
 * @param obj STrack object
 * @param out_data retrieved data in the wanted spec
 * @param len number of ticks to be retr'ieved (if multiple channels that data lenght = len * channels)
 * @param time_ticks current time (for the first sample)
 * @param opt_spec the specification which is needed, may convert if unequal, if NULL uses system specs
 * @return true if track has ended
 */
O_EXTERN
bool STrack_retr(oobj obj, float *out_data, osize len, osize time_ticks, const struct s_audio_spec *opt_spec);

/**
 * Calls STrack_retr and returns it as OArray of floats
 * @param obj STrack object
 * @param len number of ticks to be retr'ieved (if multiple channels that data lenght = len * channels)
 *            if <0, uses the remaining up to STrack_duration()
 * @param time_ticks current time (for the first sample)
 * @param opt_spec the specification which is needed, may convert if unequal, if NULL uses system specs
 * @param opt_out_ended if not NULL, will be set to true if retr ended
 * @return OArray of floats that was retr'ieved
 */
O_EXTERN
oobj STrack_retr_span_ex(oobj obj, osize len, osize time_ticks, const struct s_audio_spec *opt_spec, bool *opt_out_ended);

/**
 * Calls STrack_retr and returns it as OArray of floats
 * @param obj STrack object
 * @param seconds to be retr'ieved
 * @param time_ticks current time (for the first sample)
 * @param opt_spec the specification which is needed, may convert if unequal, if NULL uses system specs
 * @param opt_out_ended if not NULL, will be set to true if retr ended
 * @return OArray of floats that was retr'ieved
 */
O_INLINE
oobj STrack_retr_span(oobj obj, double seconds, osize time_ticks,
                      const struct s_audio_spec *opt_spec, bool *opt_out_ended)
{
    if(opt_spec) {
        return STrack_retr_span_ex(obj, (osize) (opt_spec->freq * seconds), time_ticks, opt_spec, opt_out_ended);
    }
    return STrack_retr_span_ex(obj, (osize) (s_audio_spec_default().freq * seconds), time_ticks, NULL, opt_out_ended);
}

/**
 * Calls STrack_retr_span using the remaining len
 * @param obj STrack object
 * @param opt_spec the specification which is needed, may convert if unequal, if NULL uses system specs
 * @return OArray of floats that was retr'ieved
 */
O_INLINE
oobj STrack_retr_all(oobj obj, const struct s_audio_spec *opt_spec)
{
    return STrack_retr_span_ex(obj, -1, 0, opt_spec, NULL);
}

/**
 * Calls STrack_retr_span and deletes the output to just seek or walk through the whole track
 * @param obj STrack object
 * @param len number of ticks to be seek through (if multiple channels that data lenght = len * channels)
 *            if <0, uses the remaining up to STrack_duration()
 * @param time_ticks current time (for the first sample)
 * @param opt_spec the specification which is needed, may convert if unequal, if NULL uses system specs
 * @param opt_out_ended if not NULL, will be set to true if retr ended
 * @note this function is useful if you have an STrackArray with mode WRITE to write to,
 *       but don't need the extra copy of the data from STrack_retr_span
 */
O_INLINE
void STrack_seekthrough_span_ex(oobj obj, osize len, osize time_ticks, const struct s_audio_spec *opt_spec, bool *opt_out_ended)
{
    oobj array = STrack_retr_span_ex(obj, len, time_ticks, opt_spec, opt_out_ended);
    o_del(array);
}

/**
 * Calls STrack_retr_span and deletes the output to just seek or walk through the whole track
 * @param obj STrack object
 * @param seconds to be seek through
 * @param time_ticks current time (for the first sample)
 * @param opt_spec the specification which is needed, may convert if unequal, if NULL uses system specs
 * @param opt_out_ended if not NULL, will be set to true if retr ended
 * @note this function is useful if you have an STrackArray with mode WRITE to write to,
 *       but don't need the extra copy of the data from STrack_retr_span
 */
O_INLINE
void STrack_seekthrough_span(oobj obj, double seconds, osize time_ticks, const struct s_audio_spec *opt_spec, bool *opt_out_ended)
{
    oobj array = STrack_retr_span(obj, seconds, time_ticks, opt_spec, opt_out_ended);
    o_del(array);
}

/**
 * Calls STrack_seekthrough_span using the remaining len
 * @param obj STrack object
 * @param opt_spec the specification which is needed, may convert if unequal, if NULL uses system specs
 * @note this function is useful if you have an STrackArray with mode WRITE to write to,
 *       but don't need the extra copy of the data from STrack_retr_all
 */
O_INLINE
void STrack_seekthrough_all(oobj obj, const struct s_audio_spec *opt_spec)
{
    STrack_seekthrough_span_ex(obj, -1, 0, opt_spec, NULL);
}


/**
 * Reset time and remove all played tracks
 * @param obj STrack object
 */
O_EXTERN
void STrack_reset(oobj obj);


/**
 * Extended play function, using time ticks instead of seconds
 * @param obj STrack object to play on
 * @param track STrack object to be played
 * @param time_ticks start in time ticks (freq), added to the current time
 * @param amp amplification, 1.0f for simple copy
 * @param keep if true, this obj STrack will create an object as new additional parent for the track,
 *             deleted if played to end
 * @return handle for the STrack_played_* functions
 */
O_EXTERN
osize STrack_play_ex(oobj obj, oobj track, osize time_ticks, float amp, bool keep);

/**
 * Plays a sound/music once, will be removed at the end
 * @param obj STrack object to play on
 * @param track STrack object to be played
 * @param time_seconds start in seconds, added to the current time
 * @param amp amplification, 1.0f for simple copy
 * @return handle for the STrack_played_* functions
 */
O_INLINE
osize STrack_play(oobj obj, oobj track, double time_seconds, float amp)
{
    assert(time_seconds>=0);
    return STrack_play_ex(obj, track, (osize) (time_seconds * STrack_spec(obj).freq), amp, false);
}

/**
 * Plays a sound/music once, will be removed at the end, keeps the data with a joined parent
 * @param obj STrack object to play on
 * @param track STrack object to be played
 * @param time_seconds start in seconds, added to the current time
 * @param amp amplification, 1.0f for simple copy
 * @return handle for the STrack_played_* functions
 */
O_INLINE
osize STrack_play_keep(oobj obj, oobj track, double time_seconds, float amp)
{
    assert(time_seconds>=0);
    return STrack_play_ex(obj, track, (osize) (time_seconds * STrack_spec(obj).freq), amp, true);
}


/**
 * @param obj STrack object
 * @param handle from STrack_play* function
 * @return true if the track is available and not removed yet
 */
O_EXTERN
bool STrack_played_available(oobj obj, osize handle);

/**
 * Removes a played track from the list, stopping the audio directly
 * @param obj STrack object
 * @param handle from STrack_play* function
 * @return true if the track was available and is now removed
 */
O_EXTERN
bool STrack_played_remove(oobj obj, osize handle);

/**
 * @param obj STrack object
 * @param handle from STrack_play* function
 * @param amp amplification, 1.0f for simple copy
 * @return true if the track was available
 */
O_EXTERN
bool STrack_played_amp_set(oobj obj, osize handle, float amp);


/**
 * @param obj STrack object
 * @return OArray of SFilter's
 * @note YOU NEED TO LOCK THIS STrack IN ORDER TO USE THIS ARRAY!
 *       Have a look at STrack_filter_add(...) which locks internally
 *       all SFilter that have been added here, may be deleted by this STrack, if end_time is reached
 */
OObj_DECL_GET(STrack, oobj, filter)

/**
 * @param obj STrack object
 * @param filter_sink SFilter to add to the STrack_filter array at the end, will be o_move'd into this STrack
 * @note locks the STrack internally, use STrack_filter directly with caution (locking)
 */
O_EXTERN
void STrack_filter_add(oobj obj, oobj filter_sink);

#endif //S_STRACK_H
