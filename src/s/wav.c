#include "s/wav.h"
#include "o/OArray.h"
#include "o/OStreamArray.h"
#include "o/file.h"
#include "o/endian.h"
#include "s/STrackArray.h"
#include <SDL2/SDL_audio.h>

#define O_LOG_LIB "s"
#include "o/log.h"


struct oobj_opt s_wav_load_array(oobj parent, const char *file, const struct s_audio_spec *opt_spec)
{
    struct s_audio_spec spec = opt_spec ? *opt_spec : s_audio_spec_default();

    oobj dst = NULL;
    Uint8 *src_buffer = NULL;
    oobj container = OObj_new(parent);

    SDL_AudioSpec src_spec;
    Uint32 src_length;
    if (SDL_LoadWAV(file, &src_spec, &src_buffer, &src_length) == NULL) {
        o_log_error_s(__func__, "Failed to load WAV file: %s\n", SDL_GetError());
        goto CLEAN_UP;
    }

    // Build the audio conversion structure
    SDL_AudioCVT cvt;
    if (SDL_BuildAudioCVT(&cvt, src_spec.format, src_spec.channels, src_spec.freq,
                          AUDIO_F32, spec.channels, spec.freq) < 0) {
        o_log_error_s(__func__, "Failed to build audio CVT: %s\n", SDL_GetError());
        goto CLEAN_UP;
    }

    // Allocate buffer for converted audio data
    cvt.len = (int) src_length;
    cvt.buf = o_alloc(container, 1, cvt.len * cvt.len_mult);

    // Copy the source audio data into the conversion buffer
    o_memcpy(cvt.buf, src_buffer, 1, src_length);

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
    if (src_buffer) SDL_FreeWAV(src_buffer);
    o_del(container);

    return oobj_opt(dst);
}


struct oobj_opt s_wav_load_track(oobj parent, const char *file, const struct s_audio_spec *opt_spec)
{
    struct oobj_opt array = s_wav_load_array(parent, file, opt_spec);
    if(!array.o) {
        return oobj_opt(NULL);
    }
    oobj track = STrackArray_new(parent, o_at(array.o, 0), o_num(array.o), opt_spec);
    o_del(array.o);
    return oobj_opt(track);
}


//
// write
//

O_STATIC
void write_le_ou32(oobj s, ou32 v)
{
    v = ou32_endian_to_lil(v);
    OStream_write(s, &v, sizeof v, 1);
}
O_STATIC
void write_le_ou16(oobj s, ou16 v)
{
    v = ou16_endian_to_lil(v);
    OStream_write(s, &v, sizeof v, 1);
}

bool s_wav_write(const char *file, const float *data, osize data_samples, const struct s_audio_spec *opt_spec)
{
    struct s_audio_spec spec = opt_spec ? *opt_spec : s_audio_spec_default();

    bool ok = false;
    oobj container = OObj_new(s_root());

    assert(data_samples % spec.channels == 0);
    osize len = data_samples / spec.channels;

    //
    // convert to 16 bit little endian pcm max stereo
    //
    int channels = o_min(spec.channels, 2);
    ou16 *pcm_data = NULL;

    // Build the audio conversion structure
    SDL_AudioCVT cvt;
    if (SDL_BuildAudioCVT(&cvt, AUDIO_F32, spec.channels, spec.freq,
                          AUDIO_S16LSB, channels, spec.freq) < 0) {
        o_log_error_s(__func__, "Failed to build audio CVT: %s\n", SDL_GetError());
        goto CLEAN_UP;
    }

    // Allocate buffer for converted audio data
    cvt.len = (int) (len * spec.channels * sizeof(float));
    cvt.buf = o_alloc(container, 1, cvt.len * cvt.len_mult);

    // Copy the source audio data into the conversion buffer
    o_memcpy(cvt.buf, data, 1, cvt.len);

    // Perform the audio conversion
    if (SDL_ConvertAudio(&cvt) < 0) {
        o_log_error_s(__func__, "Failed to convert audio: %s\n", SDL_GetError());
        goto CLEAN_UP;
    }
    pcm_data = (ou16*) cvt.buf;

    //
    // create wav file:
    //

    oobj array = OArray_new_dyn(container, NULL, 1, 0, 1024*32);
    oobj s = OStreamArray_new(container, array, false, OStreamArray_SEEKABLE);

    // wav header
    OStream_print(s, "RIFF");

    // file size -8 (header size is 36, + 8 data header + data)
    ou32 data_size = len * channels * 2;
    ou32 file_size = 36 + data_size;
    write_le_ou32(s, file_size);

    // includes a space at end...
    OStream_print(s, "WAVEfmt ");

    // remaining format header length (32-16=16)
    write_le_ou32(s, 16);

    // format tag 1 for pcm
    write_le_ou16(s, 1);

    // channels
    write_le_ou16(s, channels);

    // freq
    write_le_ou32(s, spec.freq);

    // byte rate
    ou32 byte_rate = spec.freq * channels * 2;
    write_le_ou32(s, byte_rate);

    // block align
    ou16 block_align = channels * 2;
    write_le_ou16(s, block_align);

    // bits each sample
    write_le_ou16(s, 16);

    // begin of data section
    OStream_print(s, "data");

    // data length
    write_le_ou32(s, data_size);

    // actual data
    OStream_write(s, pcm_data, 1, data_size);




    // flush to file
    osize written = o_num(array);
    osize file_written = o_file_write(file, false, OArray_data_void(array), 1, written);
    ok = file_written == written;
    if (!ok) {
        o_log_error_s(__func__, "failed to write to file: %s", file);
    }

    CLEAN_UP:
    o_del(container);
    return ok;
}
