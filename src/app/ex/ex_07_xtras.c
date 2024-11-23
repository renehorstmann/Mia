/**
 * Stuff from module X.
 * What's missing here are the iui features including XWObjColor,
 * which were already shown in ex_03_iui.c
 */


#include "mia.h"

struct ex_07_context {
    oobj theme;
    oobj gui;
    oobj tex_scene_btn;
    oobj files_scene_btn;
    oobj files_selected_text;
};


/**
 * This function opens a scene to view an RTex for debugging, etc.
 */
O_STATIC
void ex_07_viewtex(oobj view)
{

    /**
     * We do some image manipulation here.
     * All stuff will be allocated on the container
     */
    oobj container = OObj_new(view);

    /**
     * Load the candle sprite sheet from ex_02_animations.c.
     * Create a resulting empty texture of 6x size.
     * Clear that with a random color.
     * Blend the candle with 4x size on top of that.
     * Gauss blur the resulting tex.
     */
    oobj candle = RTex_new_file(container, "res/ex/candle.png");
    vec2 size = RTex_size(candle);
    vec2 scale_size = vec2_scale(size, 4);
    vec2 tex_size = vec2_scale(size, 6);
    oobj tex = RTex_new(container, NULL, m_2(tex_size));
    RTex_clear_full(tex, vec4_random());

    mat4 pose = u_pose_new(0, 0, m_2(scale_size));
    mat4 uv = RTex_pose(candle, 0, 0);
    RTex_blend_ex(tex, candle, pose, uv);

    tex = RTex_gauss(tex, ivec2_(9), vec2_(-1), RTex_format(tex));

    /**
     * tex is what we want to keep, so we move it outside the container
     */
    o_move(tex, view);
    o_del(container);

    /**
     * Starting the XViewTex AView in a new scene to show the resulting image
     */
    oobj scene = x_viewtex_scene(view, tex, true, "CANDLE", NULL);

    /**
     * Get XViewTex (AView) from the created AScene
     */
    oobj viewtex = AScene_view(scene);

    /**
     * gui is the WObj based gui to add widgets to
     */
    oobj gui = XViewTex_gui(viewtex);

    const char *info_text;
    if(a_pointer_is_touch()) {
        /**
         * Touch input -> multitouch to zoom and shift the image
         */
        info_text = "PINCH TO ZOOM";
    } else {
        /**
         * default cursor input -> mouse wheel to zoom and hold to shift
         */
        info_text = "WHEEL TO ZOOM";
    }

    /**
     * Injects the info text into the XViewTex
     */
    WText_new(gui, info_text);

    /**
     * Saving the RTex to disk.
     * This function is called before the scene is started!
     * Which will be happen in the next frame...
     */
    RTex_write_file(tex, "debug.ex_07_xtras_candle.png");
}

/**
 * This function is called when the XViewFiles scene finishes
 */
O_STATIC
void ex_07_viewfiles_event(oobj scene)
{
    struct ex_07_context *C = o_user(scene);

    oobj viewfiles = AScene_view(scene);
    if(XViewFiles_state(viewfiles) != XViewFiles_OK) {
        WText_text_set(C->files_selected_text, "CANCELED");
        return;
    }

    /**
     * To get the absolute path, use XViewFiles_file() instead
     */
    const char *file = XViewFiles_file_name(viewfiles);
    WText_text_set(C->files_selected_text, file);
}


/**
 * This function opens a file select scene (like a file dialog)
 */
O_STATIC
void ex_07_viewfiles(oobj view)
{
    oobj scene = x_viewfiles_scene(view, "SELECT A FILE", ex_07_viewfiles_event, ex_07_viewfiles_event);
    o_user_set(scene, o_user(view));
}

//
// view functions
//

O_STATIC
void ex_07_setup(oobj view)
{
    struct ex_07_context *C = o_user_set(view, o_new0(view, *C, 1));
    

    C->theme = WTheme_new_tiny(view);
    C->gui = WBox_new(view, WBox_LAYOUT_V);
    WBox_spacing_set(C->gui, vec2_(8));

    WTextShadow_new(C->gui, "STUFF FROM MODULE X");

    oobj viewtex_frame = WFrame_new(C->gui);
    oobj viewtex_box = WBox_new(viewtex_frame, WBox_LAYOUT_H);
    WObj_padding_set(viewtex_box, vec4_(1));
    C->tex_scene_btn = WBtn_new(viewtex_box);
    oobj tex_text = WText_new(C->tex_scene_btn, "VIEWTEX");
    WObj_padding_set(tex_text, vec4_(4));
    oobj viewtex_info_text = WTextShadow_new(viewtex_box, "VIEW AN IMAGE");
    WObj_padding_set(viewtex_info_text, vec4_(2, 8, 0));

    oobj files_frame = WFrame_new(C->gui);
    oobj files_box = WBox_new(files_frame, WBox_LAYOUT_H);
    WObj_padding_set(files_box, vec4_(1));
    C->files_scene_btn = WBtn_new(files_box);
    oobj files_text = WText_new(C->files_scene_btn, "VIEWFILES");
    WObj_padding_set(files_text, vec4_(4));
    C->files_selected_text = WTextShadow_new(files_box, "SELECT A FILE");
    WObj_padding_set(C->files_selected_text, vec4_(2, 8, 0));

}

O_STATIC
void ex_07_update(oobj view, oobj tex, float dt)
{
    struct ex_07_context *C = o_user(view);

    WTheme_update(C->theme, C->gui, vec2_(-80,32), vec2_(0), a_pointer);

    if(WBtn_clicked(C->tex_scene_btn)) {
        ex_07_viewtex(view);
    }
    if(WBtn_clicked(C->files_scene_btn)) {
        ex_07_viewfiles(view);
    }
}

O_STATIC
void ex_07_render(oobj view, oobj tex, float dt)
{
    struct ex_07_context *C = o_user(view);

    RTex_clear(tex, vec4_(0.4, 0.4, 0.5, 1.0));
    WTheme_render(C->theme, tex);
}


O_EXTERN
oobj ex_07_main(oobj root)
{
    oobj view = AView_new(root, ex_07_setup, ex_07_update, ex_07_render);
    oobj scene = AScene_new(root, view, true);
    return scene;
}

/**
 * Summary:
 *
 * This example shows some features of the X module.
 * - How to show an RTex for debugging.
 * - Open a file dialog scene.
 * The X module also contains the iui features
 * and a color picker called XWObjColor
 * or a virtual keyboard called XViewKeys,
 * which is used in the XViewText line edit.
 */
