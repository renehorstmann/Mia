#include "s/common.h"
#include "o/OObjRoot.h"
#include "o/ODelcallback.h"
#include "o/OArray.h"
#include "o/OWeakjoin.h"
#include "s/STrack.h"
#include "s/STrackArray.h"
#include <SDL2/SDL_audio.h>

#define O_LOG_LIB "s"

#include "o/log.h"

static struct {
    bool init;
    oobj root;
    struct s_audio_spec_ex spec;

    bool audio_active;
    SDL_AudioDeviceID audio_sdl_device;

    bool mic_active;
    SDL_AudioDeviceID mic_sdl_device;
    // OArray of OWeakjoin of STrackArray for capturing
    // this object also serves as lock for its usage
    oobj mic_tracks;

    // STrack of the real hardware
    oobj track;
    osize track_time;

} common_L;


O_STATIC
void audio_callback(void *userdata, Uint8 *stream, int len);

O_STATIC
void mic_callback(void *userdata, Uint8 *stream, int len);


//
// public
//


struct s_audio_spec_ex s_audio_spec_ex_default(void)
{
#ifdef MIA_PLATFORM_EMSCRIPTEN
    return (struct s_audio_spec_ex) {
            {
                22050,
                1
            },
            4096
    };
#else
    return (struct s_audio_spec_ex) {
            {
                    44100,
                    1
            },
            256
    };
#endif
}

void s_init(oobj opt_root, const struct s_audio_spec_ex *opt_spec)
{
    if (common_L.init) {
        o_log_error_s(__func__, "r_init called already!");
        return;
    }
    common_L.init = true;

    if (opt_root) {
        common_L.root = OObj_new(opt_root);
    } else {
        common_L.root = OObjRoot_new_heap();
    }
    OObj_name_set(common_L.root, "s_root");
    ODelcallback_new_assert(common_L.root, "s_root", "deleted!");

    if (opt_spec) {
        common_L.spec = *opt_spec;
    } else {
        common_L.spec = s_audio_spec_ex_default();
    }

    if(common_L.spec.spec.channels > S_CHANNELS_MAX) {
        o_log_warn_s(__func__, "limiting channels from %i to %i", common_L.spec.spec.channels, S_CHANNELS_MAX);
        common_L.spec.spec.channels = S_CHANNELS_MAX;
    } else if(common_L.spec.spec.channels < 1) {
        common_L.spec.spec.channels = 1;
    }
}

oobj s_root(void)
{
    return common_L.root;
}

void s_audio_device_open(void)
{
    if (common_L.audio_active) {
        // noop
        return;
    }

    // Open audio device with desired parameters
    SDL_AudioSpec want = {0};
    want.freq = common_L.spec.spec.freq;
    want.format = AUDIO_F32;
    want.channels = common_L.spec.spec.channels;
    want.samples = common_L.spec.samples;
    want.callback = audio_callback;
    SDL_AudioSpec have;
    common_L.audio_sdl_device = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);

    if (common_L.audio_sdl_device == 0) {
        o_log_error_s(__func__, "Failed to open audio device: %s", SDL_GetError());
        return;
    }
    if (have.freq != want.freq) {
        o_log_error_s(__func__, "Invalid opened frequency!: %i/%i", have.freq, want.freq);
        return;
    }
    if (have.format != want.format) {
        o_log_error_s(__func__, "Invalid opened format!: %i/%i", have.format, want.format);
        return;
    }
    if (have.channels != want.channels) {
        o_log_error_s(__func__, "Invalid number of channels!: %i/%i", have.channels, want.channels);
        return;
    }
    if (have.samples != want.samples) {
        o_log_error_s(__func__, "Invalid number of samples!: %i/%i", have.samples, want.samples);
        return;
    }

    o_log_s(__func__, "Opened audio with: %i Hz; %i channels; %i samples", have.freq, have.channels, have.samples);

    common_L.track = STrack_new(common_L.root, NULL);
    OObj_name_set(common_L.root, "s_audio_track");
    ODelcallback_new_assert(common_L.root, "s_audio_track", "deleted!");
    STrack_time_ticks_auto_set(common_L.track, true);

    common_L.audio_active = true;

    // Start audio playback
    s_audio_play();
}

bool s_audio_device_audio_active(void)
{
    return common_L.audio_active;
}


void s_audio_pause(void)
{
    if (!common_L.audio_active) {
        return;
    }
    SDL_PauseAudioDevice(common_L.audio_sdl_device, 1);
}

void s_audio_play(void)
{
    if (!common_L.audio_active) {
        return;
    }
    SDL_PauseAudioDevice(common_L.audio_sdl_device, 0);
}

oobj s_audio_track(void)
{
    return common_L.track;
}


void s_mic_device_open(void)
{

    if (common_L.mic_active) {
        // noop
        return;
    }

    // Open audio device with desired parameters
    SDL_AudioSpec want = {0};
    want.freq = common_L.spec.spec.freq;
    want.format = AUDIO_F32;
    want.channels = common_L.spec.spec.channels;
    want.samples = common_L.spec.samples;
    want.callback = mic_callback;
    SDL_AudioSpec have;
    common_L.mic_sdl_device = SDL_OpenAudioDevice(NULL, 1, &want, &have, 0);

    if (common_L.mic_sdl_device == 0) {
        o_log_error_s(__func__, "Failed to open mic device: %s", SDL_GetError());
        return;
    }
    if (have.freq != want.freq) {
        o_log_error_s(__func__, "Invalid opened frequency!: %i/%i", have.freq, want.freq);
        return;
    }
    if (have.format != want.format) {
        o_log_error_s(__func__, "Invalid opened format!: %i/%i", have.format, want.format);
        return;
    }
    if (have.channels != want.channels) {
        o_log_error_s(__func__, "Invalid number of channels!: %i/%i", have.channels, want.channels);
        return;
    }
    if (have.samples != want.samples) {
        o_log_error_s(__func__, "Invalid number of samples!: %i/%i", have.samples, want.samples);
        return;
    }

    o_log_s(__func__, "Opened mic with: %i Hz; %i channels; %i samples", have.freq, have.channels, have.samples);

    common_L.mic_tracks = OArray_new_dyn(common_L.root, NULL, sizeof(oobj), 0, 4);

    common_L.mic_active = true;

    // Pauses audio recording
    s_mic_pause();
}

bool s_mic_device_active(void)
{
    return common_L.mic_active;
}

void s_mic_pause(void)
{
    if (!common_L.mic_active) {
        return;
    }
    SDL_PauseAudioDevice(common_L.mic_sdl_device, 1);
}

void s_mic_record(void)
{
    if (!common_L.mic_active) {
        return;
    }
    SDL_PauseAudioDevice(common_L.mic_sdl_device, 0);
}

oobj s_mic_track_new(oobj parent, const struct s_audio_spec *opt_spec)
{
    if (!common_L.mic_active) {
        o_log_error_s(__func__, "s_mic_device_audio_active() not active!, mic track will be NULL!");
        return NULL;
    }
    o_lock(common_L.mic_tracks);
    oobj track = STrackArray_new(parent, NULL, 0, opt_spec);
    oobj weak = OWeakjoin_new(common_L.mic_tracks, track);
    OArray_push(common_L.mic_tracks, &weak);
    o_unlock(common_L.mic_tracks);
    return track;
}


osize s_play(oobj track, double time_seconds, float amp)
{
    if (!common_L.audio_active) {
        return 0;
    }
    return STrack_play(common_L.track, track, time_seconds, amp);
}

osize s_play_keep(oobj track, double time_seconds, float amp)
{
    if (!common_L.audio_active) {
        return 0;
    }
    return STrack_play_keep(common_L.track, track, time_seconds, amp);
}

void s_mix_into(float *restrict in_out_data, const float *restrict mix_data, float mix_amp, int channels, osize len)
{
    for (osize i = 0; i < channels * len; i++) {
        float a = in_out_data[i];
        float b = mix_data[i];
        float mixed = a + b * mix_amp;
        mixed = o_clamp(mixed, -1.0f, +1.0f);
        in_out_data[i] = mixed;
    }
}

void s_resample(struct s_audio_spec dst_spec, struct s_audio_spec src_spec,
                float *out_dst, const float *src, osize src_len)
{
    if (s_audio_spec_equals(dst_spec, src_spec)) {
        // no need to resample
        o_memcpy(out_dst, src, sizeof(float), s_audio_spec_array_size(src_spec, src_len));
        return;
    }

    // Check if conversion is needed
    SDL_AudioCVT cvt;
    int build_res;
    build_res = SDL_BuildAudioCVT(&cvt, AUDIO_F32, src_spec.channels, src_spec.freq,
                                  AUDIO_F32, dst_spec.channels, dst_spec.freq);

    if (build_res < 0) {
        o_log_error_s(__func__, "Failed to build audio CVT: %s", SDL_GetError());
        return;
    }

    // Allocate buffer for conversion
    cvt.len = src_len * src_spec.channels * (osize) sizeof(float);
    cvt.buf = o_alloc(common_L.root, 1, cvt.len * cvt.len_mult);

    // Copy the source audio data into the conversion buffer
    o_memcpy(cvt.buf, src, 1, cvt.len);

    // Perform the audio conversion
    if (SDL_ConvertAudio(&cvt) < 0) {
        o_log_error_s(__func__, "Failed to convert audio: %s", SDL_GetError());
        goto CLEAN_UP;
    }

    // Calculate destination length and copy the converted audio data to the destination buffer
    osize dst_len = s_resample_dst_ticks(dst_spec, src_spec, src_len);
    o_memcpy(out_dst, cvt.buf, sizeof(float), dst_len * dst_spec.channels);

    CLEAN_UP:
    // Free the conversion buffer
    o_free(common_L.root, cvt.buf);
}


// forwarded
void audio_callback(void *userdata, Uint8 *stream, int stream_bytes)
{
    float *data = (float *) stream;
    osize len = (osize) stream_bytes / (osize) (common_L.spec.spec.channels * sizeof(float));
    STrack_retr(common_L.track, data, len, common_L.track_time, NULL);
    common_L.track_time += len;
}


O_STATIC
void mic_callback_track_push(oobj track, const float *data, osize len)
{
    o_lock(track);
    STrackArray_feed(track, data, len, NULL);
    o_unlock(track);
}

// forwarded
void mic_callback(void *userdata, Uint8 *stream, int stream_bytes)
{
    const float *data = (float *) stream;
    osize len = (osize) stream_bytes / (osize) (common_L.spec.spec.channels * sizeof(float));

    o_lock(common_L.mic_tracks);

    osize mt_num = o_num(common_L.mic_tracks);
    bool *mt_del = o_new0(common_L.mic_tracks, bool, mt_num);
    for (osize i = 0; i < mt_num; i++) {
        oobj *weak = o_at(common_L.mic_tracks, i);
        struct oobj_opt track = OWeakjoin_acquire(*weak);
        if (track.o) {
            mic_callback_track_push(track.o, data, len);
        } else {
            mt_del[i] = true;
        }
        OWeakjoin_release(*weak);
    }

    // back to front, so the indices are not invalidated
    for (osize i = mt_num - 1; i >= 0; i--) {
        if (mt_del[i]) {
            oobj weak;
            OArray_pop_at(common_L.mic_tracks, i, &weak);
            o_del(weak);
        }
    }
    o_free(common_L.mic_tracks, mt_del);


    o_unlock(common_L.mic_tracks);
}

