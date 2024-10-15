/**
 * Load and play sound effects and music.
 * Render a waveform into an RTex to be displayed.
 */


#include "mia.h"

struct ex_06_context {
    /**
     * widget stuff
     */
    oobj theme;
    oobj gui;
    oobj wave_area;
    oobj play_btn;


    /**
     * STrack for the sound to be played
     */
    oobj track;

    /**
     * RTex to render the generated waveform
     */
    oobj waveform;
};


O_STATIC
void ex_06_setup(oobj view)
{
    struct ex_06_context *C = o_user_set(view, o_new0(view, *C, 1));
    

    C->theme = WTheme_new_tiny(view);
    C->gui = WBox_new(view, WBox_LAYOUT_V);
    WBox_spacing_set(C->gui, vec2_(8));

    oobj text = WTextShadow_new(C->gui, "BALLOON SOUND");
    WText_char_scale_set(text, vec2_(2));

    oobj frame = WFrame_new_title_shadow(C->gui, "WAVE", NULL);

    /**
     * Simple WObj which acts as a placeholder to render the RTex waveform (see below) on.
     * The widget's are all rendered in a single draw call with the WTheme texture.
     * It is possible to render the waveform into the texture of WTheme (has custom fields).
     * But this way is more flexible.
     */
    C->wave_area = WObj_new(frame);
    WObj_fixed_size_set(C->wave_area, vec2_(128, 64));

    C->play_btn = WBtn_new(C->gui);
    oobj icon = WIcon_new(C->play_btn, WTheme_ICON_PLAY);
    WIcon_color_set(icon, vec4_(0.1, 0.8, 0.1, 1.0));
    WObj_padding_set(icon, vec4_(4));



    /**
     * We need to open the audio once (multiple calls are fine).
     * s_audio_device_open() also exists. a_app_audio_... exists to add a critical feature.
     * For the web emscripten build:
     * Some browser block sound when the website hasn't received a single user input.
     * This function calls s_audio_device_open after the first pointer down event.
     */
    a_app_audio_device_open();

    /**
     * We change the spec to only use a single channel.
     * The default already uses mono audio (single channel), but we want to be sure here.
     */
    struct s_audio_spec spec = s_audio_spec_default();
    spec.channels = 1;

    /**
     * Loads a sound file (.wav also exists) into an OArray.
     * When only the STrack is needed and not the raw data, s_ogg_load_track is here for you.
     */
    oobj sound = s_ogg_load_array(view, "res/ex/balloon.ogg", &spec).o;
    assert(sound && "failed to load sound");
    float *sound_data = o_at(sound, 0);
    osize sound_ticks = o_num(sound);

    /**
     * Create the STrack from the raw data (s_ogg_load_track also exists...)
     */
    C->track = STrackArray_new(view, sound_data, sound_ticks, &spec);

    /**
     * Creates a texture for the waveform with 128 cols x 64 rows.
     */
    C->waveform = RTex_new(view, NULL, 128, 64);
    RTex_clear_full(C->waveform, R_BLUE);

    /**
     * Render the waveform lines in green onto the texture
     */
    u_waveform_render(C->waveform, sound_data, 0, 1, (int) sound_ticks,
                      RTex_rect(C->waveform, 0, 0), 1, R_GREEN);

    /**
     * We can also write textures to disk with this function.
     */
    RTex_write_file(C->waveform, "debug.ex_06_sound_wave.png");

    /**
     * Free / delete the OArray of the raw sound data. Not needed anymore
     */
    o_del(sound);
}

O_STATIC
void ex_06_update(oobj view, oobj tex, float dt)
{
    struct ex_06_context *C = o_user(view);

    WTheme_update(C->theme, C->gui, vec2_(-64, 64), vec2_(0), a_pointer);

    if(WBtn_clicked(C->play_btn)) {
        o_log("play sound");

        /**
         * Plays the balloon sound on the hardware sound device.
         * In 0.0 seconds (immediatly).
         * With an amp of 1.0 (default).
         *
         * The whole STrack stuff is little bit more advanced than just for playing.
         * Each STrack can have a list of played STrack's.
         * SFilterFade for example can fade in or out the amplitude.
         * STrack's can also be used filter and mix sound on the memory instead in the audio channel.
         */
        s_play(C->track, 0.0f, 1.0f);
    }
}

O_STATIC
void ex_06_render(oobj view, oobj tex, float dt)
{
    struct ex_06_context *C = o_user(view);

    RTex_clear(tex, R_GRAY_X(0.33));
    WTheme_render(C->theme, tex);

    /**
     * Retrieve the generated waveform area and blit the waveform tex onto the resulting view tex.
     */
    mat4 pose = WObj_gen_pose(C->wave_area);
    mat4 uv = RTex_pose(C->waveform, 0, 0);
    RTex_blit_ex(tex, C->waveform, pose, uv);
}


O_EXTERN
oobj ex_06_main(oobj root)
{
    oobj view = AView_new(root, ex_06_setup, ex_06_update, ex_06_render);
    oobj scene = AScene_new(root, view, true);
    return scene;
}

/**
 * Summary:
 *
 * This example shows how simple it is to play and mix audio files.
 * It also showed how to create a waveform and save it and mix widgets with plain render strategies.
 */
