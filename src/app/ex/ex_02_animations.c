/**
 * Renders an animated candle with a rotating camera.
 * Below the candle, a text is rendered
 */


/**
 * You can also include each header file separately (instead of including a whole module or just mia.h).
 * This will speed up the compile time a little (barely measurable in unity builds (default) ...).
 * Its also more clear what components of mia are used.
 */
#include "o/common.h"
#include "m/vec/vec2.h"
#include "r/RObjText.h"
#include "r/RTex.h"
#include "r/RCam.h"
#include "a/AView.h"
#include "a/AScene.h"
#include "u/atlas.h"
#include "u/pose.h"

/**
 * res/ex/candle.png is a spritesheet with 4 cols and 2 rows
 */
#define CANDLE_COLS 4
#define CANDLE_ROWS 2
#define CANDLE_FPS 6.0f


struct ex_02_context {

    /**
     * Sums up dt for camera rotation [seconds]
     */
    float time;

    /**
     * RTex for rendering the background
     */
    oobj bg;

    /**
     * RTex for the animated candle
     */
    oobj candle;
    /**
     * u_atlas for the spritesheet of the candle texture.
     * Not all stuff in mia is an OObj. classic C structs also exist for simpler stuff.
     */
    struct u_atlas candle_atlas;

    /**
     * animation frame idx [0:3] as integrating float
     */
    float candle_frame;

    /**
     * RObjText to display a text
     */
    oobj text;

} ;


O_STATIC
void ex_02_setup(oobj view)
{
    struct ex_02_context *C = o_user_set(view, o_new0(view, *C, 1));

    /**
     * Manually creating the texture data for a gray scaled chess board background
     * from the math module:
     *      vec4 -> 4x float
     *      bvec4 -> 4x bytes (obyte; ou8; uint8_t)
     * This chess board buffer is passed into the RTex GPU texture (always expects rgba as obyte or bvec4 for a pixel)
     */
    bvec4 buf[4];
    buf[0] = buf[3] = bvec4_(40, 40, 40, 255);
    buf[1] = buf[2] = bvec4_(32, 32, 32, 255);
    C->bg = RTex_new(view, buf, 2, 2);

    /**
     * For the background we make use of uv scaling. See render function.
     * For short: the uv matrix works like a camera on the texture.
     *            width and height are normally set to the texture's size
     *            We set a much bigger size, so that the texture should appear multiple times side by side
     * The default RTex_wrap mode is RTex_wrap_CLAMP.
     * In this mode the last readable texture pixel is used and stretched around the actueal texture.
     * So we use mode REPEAT to actually repeat the chess board
     */
    RTex_wrap_set(C->bg, RTex_wrap_REPEAT);


    /**
     * Loading the candle and setting up the spritesheet atlas
     */
    C->candle = RTex_new_file(view, "res/ex/candle.png");

    /**
     * The atlas needs the size for a single sprite, in our case its the texture_size / COLS (and ROWS)
     * vec2_div would divide the item_size by a single float
     * vec2_*_v uses a vec2 for division, what we want here
     */
    vec2 item_size = RTex_size(C->candle);
    item_size = vec2_div_v(item_size, vec2_(CANDLE_COLS, CANDLE_ROWS));

    /**
     * Creating the atlas.
     * new_fill creates a grid in which the sprites touch each other in a grid
     * @param view is passed to allocate an internal array
     * @param item_size for the size of a single sprite
     * @param u_atlas_START_LT means start is at Left Top.
     * @param true for row_major means first copy the full row before switching to the next, else full columns first
     * @note An atlas contains a simple 1D array and does not track rows, cols or row_major
     */
    C->candle_atlas = u_atlas_new_fill(view, item_size, CANDLE_COLS, CANDLE_ROWS, u_atlas_START_LT, true);


    /**
     * Creating a text render object, which uses an internal RObjQuad to render each font.
     * new_font35 directly loads a texture and an internal atlas like function #
     *            35 -> 3 pixels width; 5 height
     * @param 128 is the maximal number of characters to render
     * @param NULL could be replaced with a string to init the text
     * Pose is the starting point (top left) its size determines the font scaling
     *      (default size is 2x2, so we will get a font with 2xsize)
     */
    C->text = RObjText_new_font35(view, 128, NULL);
    RObjText_pose_set(C->text, u_pose_new(-16, -40, 4, 4));
}

O_STATIC
void ex_02_update(oobj view, oobj tex, float dt)
{
    /**
     * Nothing to do here
     */
}

O_STATIC
void ex_02_render(oobj view, oobj tex, float dt)
{
    struct ex_02_context *C = o_user(view);
    /**
     * Count the time up to eight seconds
     */
    const float interval = 8.0f;
    C->time += dt;
    C->time = m_mod(C->time, interval);
    float t = C->time / interval;

    const float radius = 60.0f;
    float rad = t * 2.0f * m_PI;
    float x = radius * m_cos(rad);
    float y = radius * m_sin(rad);

    /**
     * Get a reference to the views camera "RCam""
     */
    oobj cam = AView_cam(view);

    /**
     * Instead of setting the candle pose, we do reset the camera position this time.
     */
    RCam_pos_set(cam, vec2_(x, y), true);
    
    /**
     * Apply the new camera matrices now onto the texture (proj'ection)
     * Else would be done automatically for the next frame
     */
    RCam_apply_proj(cam, tex);
    

    /**
     * For the background, we set a pose of size 1024, so it will hopefully always be rendered using full screen.
     * (Min default unit size is 180x180)
     * the pattern itself has a size of 2x2 pixels.
     * We create a uv scaling that repeats the chessboard with a doubled size, so using 4x4 units.
     */
    const float bg_size = 1024;
    const float bg_uv_scale = 4.0f;

    mat4 bg_pose = u_pose_new(0, 0, bg_size, bg_size);
    mat4 bg_uv = u_pose_new(0, 0, bg_size / bg_uv_scale, bg_size / bg_uv_scale);

    RTex_blit_ex(tex, C->bg, bg_pose, bg_uv);


    /**
     * candle_frame is an integrating float for the sprite frame.
     * The actual frame is floored from that
     */
    C->candle_frame += dt * CANDLE_FPS;
    C->candle_frame = m_mod(C->candle_frame, CANDLE_COLS);
    int candle_frame = (int) m_floor(C->candle_frame);

    /**
     * normal frames:   [0:3]
     * blue frames:     [4:7] (second row of the atlas, which is a linear array)
     */
    int candle_blue = t<0.5? 0 : CANDLE_COLS;
    candle_frame += candle_blue;

    /**
     * Updating the candles uv from the atlas using the current frame
     */
    vec2 candle_size = u_atlas_size(C->candle_atlas, candle_frame);
    candle_size = vec2_scale(candle_size, 4);
    mat4 candle_pose = u_pose_new(0, 0, m_2(candle_size));
    mat4 candle_uv = u_atlas_uv(C->candle_atlas, candle_frame);

    /**
     * This time we blend (not blit), so that candle bg will be blended with the bg (alpha 0)
     */
    RTex_blend_ex(tex, C->candle, candle_pose, candle_uv);


    /**
     * Reset the text to display
     */
    if(t<0.5) {
        RObjText_text_set(C->text, "FIRE");
    } else {
        RObjText_text_set(C->text, "BLUE\nFIRE");
    }

    /**
     * Renders the RObjText (RObj) onto the RTex tex.
     * RObj_render could also be used, just an alias here
     */
    RTex_ro(tex, C->text);
}


O_EXTERN
oobj ex_02_main(oobj root)
{
    oobj view = AView_new(root, ex_02_setup, ex_02_update, ex_02_render);
    oobj scene = AScene_new(root, view, true);
    return scene;
}

/**
 * Summary:
 *
 * In this example we learned how to deal with the texture lookup "uv" matrix.
 * Making use of an u_atlas to animate a candle.
 * And controlling the view's camera instead of the sprite.
 */

