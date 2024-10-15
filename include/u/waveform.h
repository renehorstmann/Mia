#ifndef MIA_WAVEFORM_H
#define MIA_WAVEFORM_H

/**
 * @file gradient.h
 *
 * draws an audio waveform onto an RTex
 */

#include "o/common.h"
#include "m/types/flt.h"
#include "r/rect.h"


/**
 * Creates an OArray of struct r_rect's for each column
 * @param parent to allocate on
 * @param audio_data audio stream start
 * @param channel channel to use
 * @param channels_num number of channels
 * @param ticks number of audio ticks to sample
 * @param rect xywh rectangle to create the points into (dst RTex full rect, for example)
 * @param thickness width of each rendered col, normally just "1"
 * @param init to copy the rects from
 * @return OArray of struct r_rect
 */
O_EXTERN
oobj u_waveform_rects(oobj parent, const float *audio_data, int channel, int channels_num, int ticks,
                      vec4 rect, float thickness, struct r_rect init);


/**
 * Renders the waveform as a colored line onto the texture
 * @param tex RTex object to render onto
 * @param audio_data audio stream start
 * @param channel channel to use
 * @param channels_num number of channels
 * @param ticks number of audio ticks to sample
 * @param rect xywh rectangle to create the points into (dst RTex full rect, for example)
 *             use vec4_(-1) (w||h <= 0) to use the full RTex size
 * @param thickness width of each rendered col, normally just "1"
 * @param color for the line
 * @return number of sampled points
 */
O_EXTERN
void u_waveform_render(oobj tex, const float *audio_data, int channel, int channels_num, int ticks,
                       vec4 rect, float thickness, vec4 color);



#endif //MIA_WAVEFORM_H
