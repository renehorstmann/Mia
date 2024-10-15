#ifndef S_WAV_H
#define S_WAV_H

/**
 * @file wav.h
 *
 * load and write .wav files
 */

#include "common.h"

/**
 * Loads a .wav (sound effect) file and resamples it to the used frequency (if neccessary)
 * @return OArray of floats for the resulting loaded track or NULL if failed
 */
O_EXTERN
struct oobj_opt s_wav_load_array(oobj parent, const char *file, const struct s_audio_spec *opt_spec);

/**
 * Loads a .wav (sound effect) file and resamples it to the used frequency (if neccessary)
 * @return STrackArray of the resulting loaded track or NULL if failed
 */
O_EXTERN
struct oobj_opt s_wav_load_track(oobj parent, const char *file, const struct s_audio_spec *opt_spec);

/**
 * Writes pcm data into a .wav file
 * @param file to create (.wav ending)
 * @param data pcm data
 * @param data_samples number of floats in data >NOT ticks as with len!!!<
 * @param spec pcm data specification, if NULL: system spec is expected
 * @return true if succeeded
 */
O_EXTERN
bool s_wav_write(const char *file, const float *data, osize data_samples, const struct s_audio_spec *opt_spec);

#endif //S_WAV_H
