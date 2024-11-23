#ifndef S_OGG_H
#define S_OGG_H

/**
 * @file wav.h
 *
 * load .ogg files
 */

#include "common.h"

/**
 * Loads a .ogg (music) file and resamples it to the used frequency (if neccessary)
 * @return OArray of floats for the resulting loaded track or NULL if failed
 */
O_EXTERN
struct oobj_opt s_ogg_load_array(oobj parent, const char *file, const struct s_audio_spec *opt_spec);


/**
 * Loads a .ogg (music) file and resamples it to the used frequency (if neccessary)
 * @return STrackArray of the resulting loaded track or NULL if failed
 */
O_EXTERN
struct oobj_opt s_ogg_load_track(oobj parent, const char *file, const struct s_audio_spec *opt_spec);


#endif //S_OGG_H
