#ifndef S_COMMON_H
#define S_COMMON_H

/**
 * @file common.h
 *
 * Common used | Basic includes, macros, types and functions
 */

#include "o/common.h"


// just some imaginary value to work with, limited by the audio system
#define S_CHANNELS_MAX 32


struct s_audio_spec {
    int freq;
    int channels;
};

struct s_audio_spec_ex {
    struct s_audio_spec spec;
    int samples;
};

/**
 * @return default values for the audio spec's.
 * @note dependend on the platform, for example MIA_PLATFORM_EMSCRIPTEN reduces the frequency and sample size
 */
O_EXTERN
struct s_audio_spec_ex s_audio_spec_ex_default(void);

/**
 * @return default values for the audio spec's.
 * @note dependend on the platform, for example MIA_PLATFORM_EMSCRIPTEN reduces the frequency (and sample size)
 */
O_INLINE
struct s_audio_spec s_audio_spec_default(void)
{
    return s_audio_spec_ex_default().spec;
}


/**
 * @param a, b specs to compare
 * @return true if both specs match and are equal
 */
O_INLINE
bool s_audio_spec_equals(struct s_audio_spec a, struct s_audio_spec b)
{
    return a.freq == b.freq && a.channels == b.channels;
}


/**
 * @param spec audio specification
 * @param len in ticks
 * @return simply len * spec.channels
 */
O_INLINE
osize s_audio_spec_array_size(struct s_audio_spec spec, osize len)
{
    assert(len>=0);
    return len * spec.channels;
}

/**
 * @param spec audio specification
 * @param len in ticks
 * @return s_audio_spec_array_size * sizeof(float)
 */
O_INLINE
osize s_audio_spec_buffer_size(struct s_audio_spec spec, osize len)
{
    return s_audio_spec_array_size(spec, len) * (osize) sizeof(float);
}

/**
 * @param spec audio specification
 * @return time in seconds
 */
O_INLINE
double s_audio_spec_time_as_seconds(struct s_audio_spec spec, osize time_ticks)
{
    return (double) time_ticks / (double) spec.freq;
}

/**
 * @param spec audio specification
 * @return time in ticks
 */
O_INLINE
osize s_audio_spec_time_from_seconds(struct s_audio_spec spec, double time_seconds)
{
    return (osize) (time_seconds * spec.freq);
}



/**
 * @param opt_root a root that will NOT be deleted or NULL to create a heap root internally
 */
O_EXTERN
void s_init(oobj opt_root, const struct s_audio_spec_ex *opt_spec);


/**
 * @return A default allocator for the "s" module.
 */
O_EXTERN
oobj s_root(void);

/**
 * Opens the hardware audio device for playback.
 * Until !s_audio_device_active(), all audio coming into the API is ignored
 * @note s_audio_play is called at the end of the function
 *       MIA_PLATFORM_EMSCRIPTEN should call this after the first pointer down event, else the browser ignores the call!
 *       See a_app_audio_device_open() for a deferred call, which will be executed on the first pointer down event
 */
O_EXTERN
void s_audio_device_open(void);

/**
 * @return true if s_audio_device_open() was called and the audio is audible
 */
O_EXTERN
bool s_audio_device_active(void);

/**
 * Pauses the audio device stream
 */
O_EXTERN
void s_audio_pause(void);

/**
 * Unpauses the audio device stream
 */
O_EXTERN
void s_audio_play(void);

/**
 * @return STrack for the real hardware ( created if s_audio_device_active() == true )
 * @note WARNING: will be NULL if s_audio_device_active() == false!
 */
O_EXTERN
oobj s_audio_track(void);


/**
 * Opens the hardware audio device for record.
 * Until !s_mic_device_active(), audio will not be recorded from this api and the created tracks stay empty
 * @note s_mic_pause is called at the end of the function
 *       MIA_PLATFORM_EMSCRIPTEN should call this after the first pointer down event, else the browser ignores the call!
 */
O_EXTERN
void s_mic_device_open(void);

/**
 * @return true if s_mic_device_open() was called and the audio is recordable
 */
O_EXTERN
bool s_mic_device_active(void);

/**
 * Pauses the mic device stream
 */
O_EXTERN
void s_mic_pause(void);

/**
 * Unpauses the mic device stream
 */
O_EXTERN
void s_mic_record(void);

/**
 * @param parent to allocate on
 * @param opt_spec if NULL, the s_audio_spec_default is used
 * @return a new STrackArray which will push back the current recorded audio
 * @note WARNING: will be NULL if s_audio_device_active() == false!
 */
O_EXTERN
oobj s_mic_track_new(oobj parent, const struct s_audio_spec *opt_spec);

/**
 * Plays a sound/music once on the system track (s_audio_track)
 * @param track STrack object to be played
 * @param time_seconds start in seconds, added to the current time
 * @param amp amplification, 1.0f for simple copy
 * @return handle for the STrack_played_* functions together with s_audio_tack()
 */
O_EXTERN
osize s_play(oobj track, double time_seconds, float amp);

/**
 * Plays a sound/music once on the system track (s_audio_track), keeps the data with a joined parent
 * @param track STrack object to be played
 * @param time_seconds start in seconds, added to the current time
 * @param amp amplification, 1.0f for simple copy
 * @return handle for the STrack_played_* functions together with s_audio_tack()
 */
O_EXTERN
osize s_play_keep(oobj track, double time_seconds, float amp);

/**
 * Mixes two audio buffers into the first
 * @param in_out_data first audio buffer and also the result
 * @param mix_data to add into in_out_data
 * @param mix_amp amplification for the mix data (default is 1.0f)
 * @param channels from the spec
 * @param len number of ticks
 * @note not to be confused with math mix functions. This mix works by adding and clamping
 */
O_EXTERN
void s_mix_into(float * restrict in_out_data, const float * restrict mix_data, float mix_amp, int channels, osize len);

/**
 * @return len for dst if used for resampling
 */
O_INLINE
osize s_resample_dst_ticks(struct s_audio_spec dst_spec, struct s_audio_spec src_spec, osize src_len)
{
    return src_len * dst_spec.freq / src_spec.freq;
}

/**
 * Resamples a track
 */
O_EXTERN
void s_resample(struct s_audio_spec dst_spec, struct s_audio_spec src_spec,
                float *out_dst, const float *src, osize src_len);


#endif //S_COMMON_H
