/**
 * A simple thunder button to play one of a few thunder sounds.
 * This app was part of a helloween costune.
 * My wife was dresses as a lightning with a few led's. And I dressed as thunder, using this app and a bluetooth speaker for the sound effects ;D
 */


#include "app/ex/thunder.h"
#include "mia.h"

/** for the visual flash effect */

/** number of sound files */
#define LIGHTNING_TIME 0.5
#define THUNDER_NUM 5


struct thunder_context {

    oobj theme;
    oobj gui;
    
    /** WNum (x_num) */
    oobj volume;
    
    /** WBtn for the sound effect */
    oobj btn;
    
    /** flash time and color */
    float lightning;
    vec3 lightning_color;
    
    /** STrack's of loaded ogg's */
    oobj thunders[THUNDER_NUM];
    int prev_thunder_idx;
};


/**
 * Renders the lightning image into the themes texture.
 * So the whole gui needs only the single draw call
 */
O_STATIC
void thunder_blit(oobj theme)
{
    oobj tex = WTheme_tex(theme);
    oobj thunder = RTex_new_file(theme, "res/ex/thunder.png");
    
    RTex_blit(tex, thunder, m_2(u_atlas_pos(WTheme_atlas(theme), WTheme_CUSTOM_64)));
}


O_STATIC
void thunder_setup(oobj view)
{
    struct thunder_context *C = o_user_set(view, o_new0(view, *C, 1));
    
    oobj cam = AView_cam(view);
    RCam_min_units_size_set(cam, vec2_(96, 128));
    RCam_update(cam);
    AView_cam_units_auto_set(view, false);


    C->theme = WTheme_new_tiny(view);
    C->gui = WBox_new(view, WBox_LAYOUT_V);
    WBox_spacing_set(C->gui, vec2_(8));
    
    /** Change WNum's progress color to yellow, for the volume drag */
    oobj style = WObj_style(C->gui);
    WStyle_drag_progress_color_x_set(style, vec4_(0.8, 0.7, 0.1, 0.66));
    
    
    oobj title = WTextShadow_new(WAlign_new_center(C->gui), "THUNDER");
    WText_char_scale_set(title, vec2_(2));
    
    
    C->volume = x_num(WAlign_new_center(C->gui),
            0.0, 5.0, 0.05, "VOLUME");
    WObj_min_size_set(C->volume, vec2_(48, 0));
    WNum_num_set(C->volume, 0.50);
    
    C->btn = WBtn_new(WAlign_new_center(C->gui));
    WBtn_style_set(C->btn, WBtn_BIG_ROUND);
    
    /** Lighning image using a custom area of the themes used texture */
    oobj img = WImg_new(C->btn, u_atlas_rect(WTheme_atlas(C->theme), WTheme_CUSTOM_64));
    thunder_blit(C->theme);
    
    
    a_app_audio_device_open();
    
    /** Load all thunder sounds */
    for(int i=0; i<THUNDER_NUM; i++) {
        char src[128];
        o_strf_buf(src, "res/ex/thunder_%i.ogg", i+1);
        o_log("load: %s", src);
        struct oobj_opt track = s_ogg_load_track(view, src, NULL);
        
        o_assume(track.o, "failed to load %s", src);
        C->thunders[i] = track.o;
    }
}

O_STATIC
void thunder_update(oobj view, oobj tex, float dt)
{
    struct thunder_context *C = o_user(view);

    /**
     * Update the gui
     */
    vec2 min_size = {{96, 128}};
    vec2 lt = {{-min_size.x/2, +min_size.y/2}};
    vec2 used_size = WTheme_update(C->theme, C->gui, lt, min_size, a_pointer);
    // o_log("used: %f, %f", m_2(used_size));

    C->lightning = o_max(0, C->lightning-dt);

    if(WBtn_clicked(C->btn)) {
        o_log("THUNDER");
        C->lightning = LIGHTNING_TIME;
        vec3 hsv;
        hsv.v0 = m_random();
        hsv.v1 = m_random_range(0, 0.2);
        hsv.v2 = m_random_range(0.66, 1.0);
        C->lightning_color = vec3_hsv2rgb(hsv);
        
        /** Play a random thunder track, but not the previous one */
        int track_idx = o_rand() % (THUNDER_NUM-1);
        track_idx = (C->prev_thunder_idx + 1 + track_idx) % THUNDER_NUM;
        C->prev_thunder_idx = track_idx;
        
        float vol = WNum_num(C->volume);
        o_log("playing thunder: %i, vol: %.2f", track_idx, vol);
        
        s_play(C->thunders[track_idx], 0, vol);
    }
}


O_STATIC
void thunder_render(oobj view, oobj tex, float dt)
{
    struct thunder_context *C = o_user(view);

    /** flashing lightning background */
    float ligthning_val = C->lightning / LIGHTNING_TIME;    
    vec4 bg;
    bg.xyz = vec3_scale(C->lightning_color, ligthning_val);
    bg.a = 1;
    RTex_clear_full(tex, bg);
    
    WTheme_render(C->theme, tex);
}


/**
 * Starting point of the thunder app
 * @param root parent to allocate on
 * @return AScene created of the thunder app
 */
oobj ex_thunder_main(oobj root)
{
    oobj view = AView_new(root, thunder_setup, thunder_update, thunder_render);
    oobj scene = AScene_new(root, view, true);
    return scene;
}

/**
 * Another Starting point of the thunder app. This time the scene is created during a splash screen
 * @param root parent to allocate on
 * @return AScene of the splash scene that creates the mia app
 */
oobj ex_thunder_main_splashed(oobj root)
{
    oobj view = AView_new(root, thunder_setup, thunder_update, thunder_render);
    oobj splash = u_splash_new_mia(root, view, true,
                                   R_BLACK, R_WHITE, "HORSIMANN", 1.0f, 0.2f);
    return splash;
}
