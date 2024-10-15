#include "ex/main.h"
#include "o/img.h"
#include "m/utils/color.h"
#include "r/RTex.h"
#include "r/RCam.h"
#include "a/app.h"
#include "a/AScene.h"
#include "u/scroll.h"
#include "u/splash.h"
#include "w/WTheme.h"
#include "w/WBox.h"
#include "w/WBtn.h"
#include "w/WTextShadow.h"
#include "ex/EXViewClose.h"


#define NUM_EXAMPLES 11

// protected example functions
O_EXTERN oobj ex_00_main(oobj root);
O_EXTERN oobj ex_01_main(oobj root);
O_EXTERN oobj ex_02_main(oobj root);
O_EXTERN oobj ex_03_main(oobj root);
O_EXTERN oobj ex_04_main(oobj root);
O_EXTERN oobj ex_05_main(oobj root);
O_EXTERN oobj ex_06_main(oobj root);
O_EXTERN oobj ex_07_main(oobj root);
O_EXTERN oobj ex_08_main(oobj root);
O_EXTERN oobj ex_09_main(oobj root);
O_EXTERN oobj ex_tea_main(oobj root);

static const char *example_titles[] = {
        "00_hello_world",
        "01_pointers",
        "02_animations",
        "03_iui",
        "04_widgets",
        "05_windows",
        "06_sound",
        "07_xtras",
        "08_fetching",
        "09_upndownload",
        " ~ tea app ~ "
};

// list all functions in an array
static oobj (*example_functions[])(oobj root) = {
        ex_00_main,
        ex_01_main,
        ex_02_main,
        ex_03_main,
        ex_04_main,
        ex_05_main,
        ex_06_main,
        ex_07_main,
        ex_08_main,
        ex_09_main,
        ex_tea_main

};


static struct {
    oobj root;

    oobj theme;
    oobj gui;

    struct u_scroll scroll;
} main_L;


O_STATIC
void main_close_event(oobj close_view)
{
    // the example to close is the scene before the close scene
    a_app_scene_exit(a_app_scene_index()-1);
}

O_STATIC
void main_start_example(oobj btn)
{
    int *idx = o_user(btn);

    // starting the example
    example_functions[*idx](main_L.root);

    // creating a transparent AScene with an exit button at top left
    oobj close_view = EXViewClose_new(main_L.root, main_close_event);
    oobj close_scene = AScene_new(main_L.root, close_view, true);
    AScene_opaque_set(close_scene, false);
}


O_STATIC
void main_setup(oobj view)
{
    main_L.root = OObj_new(view);
    main_L.theme = WTheme_new_tiny(view);

    main_L.gui = WBox_new(view, WBox_LAYOUT_V);
    WBox_spacing_set(main_L.gui, vec2_(4));

    oobj title = WTextShadow_new(main_L.gui, "EXAMPLES:");
    WText_char_scale_set(title, vec2_(2, 3));
    WObj_padding_set(title, vec4_(0, 0, 0, 16));

    float hue = 0.33;
    for(int i=0; i<NUM_EXAMPLES; i++) {
        oobj btn = WBtn_new(main_L.gui);
        int *idx = o_user_set(btn, o_new(btn, int, 1));
        *idx = i;
        WBtn_auto_mode_set(btn, WBtn_AUTO_CLICKED);
        WBtn_auto_event_set(btn, main_start_example);
        oobj example_text = WText_new(btn, example_titles[i]);
        WText_text_mode_set(example_text, WText_MODE_UPPER);

        vec4 hsva = vec4_(hue, 0.3, 0.5, 1.0);
        hue = m_mod(hue + 0.1, 1.0);
        vec4 rgba = vec4_hsv2rgb(hsva);
        WBtn_color_set(btn, rgba);
    }

    main_L.scroll = u_scroll_new(u_scroll_XY, AView_cam(view));
}

O_STATIC
void main_update(oobj view, oobj tex, float dt)
{
    main_L.scroll.view_rect = RCam_rect(AView_cam(view), 0, 0);
    u_scroll_update(&main_L.scroll, dt);

    vec2 lt = vec2_(-80, 0);
    WTheme_update(main_L.theme, main_L.gui, lt, vec2_(160), a_pointer);

    vec2 gui_size = WObj_gen_size(main_L.gui);
    main_L.scroll.cam_limits_rect = u_rect_new_bounds(0, 0, -gui_size.y-32, 32);

}

O_STATIC
void main_render(oobj view, oobj tex, float dt)
{
    RTex_clear_full(tex, vec4_(0.294118,0.164706,0.164706,1));

    WTheme_render(main_L.theme, tex);
}



oobj ex_main(oobj root)
{
    o_clear(&main_L, sizeof main_L, 1);

    oobj view = AView_new(root, main_setup, main_update, main_render);
    oobj splash = u_splash_new_mia(root, view, true,
                                   R_BLACK, R_WHITE, "HORSIMANN", 1.0f, 0.2f);



    // window title + icon
    a_app_title_set("Mia Examples");

    struct o_img img = o_img_new_file(root, "res/ex/icon16.png");
    a_app_icon_set(&img);
    o_img_free(&img);


    return splash;
}

oobj ex_root(void)
{
    return main_L.root;
}
