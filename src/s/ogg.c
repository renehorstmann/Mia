#include "s/ogg.h"
#include "o/OArray.h"
#include "o/file.h"
#include "s/STrackArray.h"
#include <SDL2/SDL_audio.h>

#define STB_VORBIS_HEADER_ONLY
#include "ext_stb_vorbis.c"

#define O_LOG_LIB "s"
#include "o/log.h"

// Function to load and decode an Ogg Vorbis from memory using stb_vorbis.c
// chatgpt version...
O_STATIC
float* loadOggMemory(const void* data, int data_len, Uint32* audioLength, Uint16* channels, Uint32* sampleRate) {
    int error;

    // this function would not run on android, because it uses the plain c file loader, instead of sdl's
    //stb_vorbis* vorbis = stb_vorbis_open_filename(filename, &error, NULL);

    stb_vorbis *vorbis = stb_vorbis_open_memory(data, data_len, &error, NULL);
    if (vorbis == NULL) {
        fprintf(stderr, "Error opening Ogg Vorbis file: %d\n", error);
        return NULL;
    }

    stb_vorbis_info info = stb_vorbis_get_info(vorbis);
    *channels = info.channels;
    *sampleRate = info.sample_rate;

    // Calculate total PCM data size
    int num_samples = stb_vorbis_stream_length_in_samples(vorbis);
    int num_channels = info.channels;
    int num_floats = num_samples * num_channels;

    // Allocate buffer for decoded PCM data (as floats)
    float* pcm = (float*)malloc(num_floats * sizeof(float));
    if (pcm == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        stb_vorbis_close(vorbis);
        return NULL;
    }

    // Decode Ogg Vorbis data into PCM buffer
    int samples = stb_vorbis_get_samples_float_interleaved(vorbis, info.channels, pcm, num_samples * num_channels);
    if (samples != num_samples) {
        fprintf(stderr, "Failed to decode Ogg Vorbis file\n");
        free(pcm);
        stb_vorbis_close(vorbis);
        return NULL;
    }

    // Close the Ogg Vorbis file
    stb_vorbis_close(vorbis);

    *audioLength = num_floats * sizeof(float);
    return pcm;
}

struct oobj_opt s_ogg_load_array(oobj parent, const char *file, const struct s_audio_spec *opt_spec)
{
    struct s_audio_spec spec = opt_spec ? *opt_spec : s_audio_spec_default();

    oobj container = OObj_new(parent);
    oobj dst = NULL;
    float *ogg_data = NULL;

    Uint32 ogg_audio_length;
    Uint16 ogg_channels;
    Uint32 ogg_sample_rate;

    struct oobj_opt memory = o_file_read(container, file, false, 1);
    if(!memory.o) {
        o_log_error_s(__func__, "Failed to load file to memory %s", file);
        goto CLEAN_UP;
    }

    ogg_data = loadOggMemory(o_at(memory.o, 0), (int) o_num(memory.o),
                             &ogg_audio_length, &ogg_channels, &ogg_sample_rate);

    // Build the audio conversion structure
    SDL_AudioCVT cvt;
    if (SDL_BuildAudioCVT(&cvt, AUDIO_F32, ogg_channels, (int) ogg_sample_rate,
                          AUDIO_F32, spec.channels, spec.freq) < 0) {
        o_log_error_s(__func__, "Failed to build audio CVT: %s\n", SDL_GetError());
        goto CLEAN_UP;
    }

    // Allocate buffer for converted audio data
    cvt.len = (int) ogg_audio_length;
    cvt.buf = o_alloc(container, 1, cvt.len * cvt.len_mult);

    // Copy the source audio data into the conversion buffer
    o_memcpy(cvt.buf, ogg_data, 1, ogg_audio_length);

    // Perform the audio conversion
    if (SDL_ConvertAudio(&cvt) < 0) {
        o_log_error_s(__func__, "Failed to convert audio: %s\n", SDL_GetError());
        goto CLEAN_UP;
    }

    // Calculate the length of the converted data
    osize dst_len = cvt.len_cvt / sizeof(float);

    // Create the OArray with the converted data
    dst = OArray_new(parent, cvt.buf, sizeof(float), dst_len);

    CLEAN_UP:
    if (ogg_data) free(ogg_data);
    o_del(container);

    return oobj_opt(dst);
}



struct oobj_opt s_ogg_load_track(oobj parent, const char *file, const struct s_audio_spec *opt_spec)
{
    struct oobj_opt array = s_ogg_load_array(parent, file, opt_spec);
    if(!array.o) {
        return oobj_opt(NULL);
    }
    oobj track = STrackArray_new(parent, o_at(array.o, 0), o_num(array.o), opt_spec);
    o_del(array.o);
    return oobj_opt(track);
}
