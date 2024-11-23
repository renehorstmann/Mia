/**
 * iui ^= immediate user interface.
 * Main purpose are debugging windows to edit values or colors in runtime.
 * In this example, a white quad is jumping on pointer down (cursor click or touch begin).
 * The jump speed can be set with an iui window.
 * In addition to that, the background color can be set by another iui window.
 */


#include "mia.h"

/**
 * quads physics:
 */
#define GRAVITY -500
#define MIN_POS -60

struct ex_03_context {

    /**
     * These variables can be controlled by an iui window, see the update and render function
     */
    vec4 bg_color;
    float jump_speed;

    /**
     * quads physics state
     */
    float pos;
    float speed;
    
    
    struct a_pointer prev;

    /**
     * RObjText object
     */
    oobj text;

};


O_STATIC
void ex_03_setup(oobj view)
{
    struct ex_03_context *C = o_user_set(view, o_new0(view, *C, 1));

    /**
     * Adds an offset to the start position "left top".
     * Otherwise it would be displayed behind the overlying examples close button.
     */
    x_iui_init_lt_set(vec2_(0, 32));

    C->bg_color = vec4_(0.5, 0.1, 0.1, 1.0);
    C->jump_speed = 200;


    /**
     * Info text.
     * Like new_font35 but adds a shadow border.
     * The shadow color can be changed if passed a vec4 color (pointer) instead of NULL
     */
     C->text = RObjText_new_font35_shadow(view, 128,
                                               "TAP TO JUMP\n\n\n"
                                               "OPEN \"JMP\" WINDOW TO\n"
                                               "ADJUST JUMP SPEED\n\n"
                                               "SLIDE TO CHANGE\n\n"
                                               "PRESS ON THE NUM TO\n"
                                               "OPEN A VIRTUAL KEYBOARD",
                                               NULL);
    RObjText_pose_set(C->text, u_pose_new(-32, 64, 2, 2));
}

O_STATIC
void ex_03_update(oobj view, oobj tex, float dt)
{
    struct ex_03_context *C = o_user(view);
    
    /**
     * Check for pointer down to jump
     */
    struct a_pointer p = a_pointer(0, 0);
    if (a_pointer_down(p, C->prev)) {
        C->speed = C->jump_speed;
    }
    C->prev = p;


    /**
     * This one liner creates an iui window to set the jump speed in runtime between [0:500]
     */
    x_iui_float("JMP", &C->jump_speed, 0, 500);

    /**
     * Apply speed
     */
    C->speed += GRAVITY * dt;
    C->pos += C->speed * dt;
    C->pos = o_max(C->pos, MIN_POS);
}

O_STATIC
void ex_03_render(oobj view, oobj tex, float dt)
{
    struct ex_03_context *C = o_user(view);
    
    /**
     * Another iui window to set the rgb color component of the background color
     * .xyz is the rgb sub vec3 of the rgba/xyzw vec4
     * 
     * So call the x_iui_* stuff either in the update or render call.
     * To dismiss the window, stop calling it.
     *
     * The x_iui_* windows are registered as a new layer in the current AView
     */

    x_iui_rgb("BG", &C->bg_color.xyz);

    /**
     * clear_full will override the full texture with the given color.
     * The default RTex_clear will only paint above the current set viewport.
     */
    RTex_clear_full(tex, C->bg_color);


    /**
     * Render the info text
     */
     RTex_ro(tex, C->text);


    /**
     * Renders the white jumping quad
     */
    oobj white = r_tex_white();
    mat4 pose = u_pose_new(0, C->pos, 20, 20);
    mat4 uv = mat4_eye();
    RTex_blit_ex(tex, white, pose, uv);

}


O_EXTERN
oobj ex_03_main(oobj root)
{
    oobj view = AView_new(root, ex_03_setup, ex_03_update, ex_03_render);
    oobj scene = AScene_new(root, view, true);
    return scene;
}

/**
 * Summary:
 *
 * This example shows how easy it is to add an iui window.
 * With these you can tune colors or fixed value on runtime. 
 */
