#ifndef S_STRACKARRAY_H
#define S_STRACKARRAY_H

/**
 * @file STrackArray.h
 *
 * Object (derives STrack)
 *
 * STrack implementation using OArray as memory to grep and or mix into.
 * Operations on its array should also lock this track
 */

#include "STrack.h"

/** object id */
#define STrackArray_ID STrack_ID "Array"

/**
 * Some different modes for the OArray to work on.
 * READ reads the OArray data at grebbing.
 * WRITE writes into the OArray during the grep what was read from playing and returns only that.
 * READWRITE reads the OArray data at grebbing AND overwrites it.
 * SILENT as if it was a simple STrack.
 */
enum STrackArray_mode {
    STrackArray_READ,
    STrackArray_WRITE,
    STrackArray_READWRITE,
    STrackArray_SILENT,
    STrackArray_NUM_MODES
};


typedef struct {
    STrack super;

    oobj array;
    enum STrackArray_mode mode;
    bool allow_growth;
} STrackArray;


/**
 * Creates a new the STrackArray object
 * This function can be used to init an derived OJoin (for thread safe stuff as an example)
 * @param object_size size to allocate (asserts >= sizeof(OJoin)
 * @param parent to inherit from
 * @param opt_data to be cloned or NULL to clear to 0
 * @param data_len num of floats, asserts "data_len%spec.channels==0"
 * @param opt_spec specification to be used, NULL for system spec
 * @return The new object
 * @note default mode is READ
 */
O_EXTERN
STrackArray *STrackArray_new_super(osize object_size, oobj parent, const float *opt_data, osize data_len,
                                   const struct s_audio_spec *opt_spec);

/**
 * Creates a new the STrackArray object
 * @param parent to inherit from
 * @param opt_data to be cloned or NULL to clear to 0
 * @param data_len num of floats, asserts "data_len%spec.channels==0"
 * @param opt_spec specification to be used, NULL for system spec
 * @return The new object
 * @note default mode is READ
 */
O_INLINE
STrackArray *STrackArray_new(oobj parent, const float *opt_data, osize data_len,
                             const struct s_audio_spec *opt_spec)
{
    return STrackArray_new_super(sizeof(STrackArray), parent, opt_data, data_len, opt_spec);
}

/**
 * Creates a new the STrackArray object to read and write to with the specified seconds time
 * @param parent to inherit from
 * @param len in ticks for the array
 * @param opt_spec specification to be used, NULL for system spec
 * @return The new object
 * @note default mode is WRITE
 */
O_EXTERN
STrackArray *STrackArray_new_ticks(oobj parent, osize len, const struct s_audio_spec *opt_spec);

/**
 * Creates a new the STrackArray object to read and write to with the specified seconds time
 * @param parent to inherit from
 * @param seconds time how long the array at start will be
 * @param opt_spec specification to be used, NULL for system spec
 * @return The new object
 * @note default mode is WRITE
 */
O_EXTERN
STrackArray *STrackArray_new_s(oobj parent, double seconds, const struct s_audio_spec *opt_spec);


//
// virtual implementations:
//

/**
 * virtual function
 * First calls super STrack__v_retr to mix played tracks.
 * Then mix's in the array data at the current time tick.
 * @param obj STrackArray object
 * @param out_data to write into
 * @param len frequency ticks
 * @param time_ticks current track time
 * @return true if end was reached
 */
O_EXTERN
bool STrackArray__v_retr(oobj obj, float *out_data, osize len, osize time_ticks);

/**
 * Virtual getter for an optional duration
 * @param obj STrack object
 * @return duration from array length
 */
O_EXTERN
osize STrackArray__v_duration(oobj obj);

//
// object functions
//


/**
 * @param obj OStreamArray object
 * @return internal array to work on, should be allocated from this object (OJoin base has its own allocator)
 */
OObj_DECL_GETSET(STrackArray, oobj, array)

/**
 * @param obj OStreamArray object
 * @return for example STrackArray_READ to read from the array or _WRITE to write into while retr'ieving
 */
OObj_DECL_GETSET(STrackArray, enum STrackArray_mode, mode)

/**
 * @param obj OStreamArray object
 * @return if true, the OArray may growth on write, if false (default) an ended is generated
 */
OObj_DECL_GETSET(STrackArray, bool, allow_growth)


/**
 * Appends audio data to the end of the array
 * @param obj STrackArray object
 * @param ata audio data to append
 * @param len number of ticks to be pushed (if multiple channels that data lenght = len * channels)
 * @param opt_spec the specification of the pushed data, may convert if unequal, if NULL assumes system spec
 */
O_EXTERN
void STrackArray_feed(oobj obj, const float *data, osize len, const struct s_audio_spec *opt_spec);


/**
 * Writes the buffer into a .wav file
 * @param obj STrackArray object
 * @param file the path to the .wav file
 * @return true on success
 */
O_EXTERN
bool STrackArray_write_wav(oobj obj, const char *file);



#endif //S_STRACKARRAY_H
