#ifndef A_APP_H
#define A_APP_H

/**
 * @file app.h
 *
 * Starts up the app, creating contexts, etc.
 * Handles the scene stack.
 *
 * Each scene may have some options in other modules.
 * Instead of passing a scene object or something,
 * the current scene registers in the app and other
 * modules check grab the current scene in their functions.
 *
 * This file contains functions in either the namespace a_app or simply the library namespace a
 */


#include "s/common.h"
#include "m/types/flt.h"

// GLES and WebGL use GLES3.0
#ifdef MIA_OPTION_GLES
#define A_GL_MAJOR_VERSION 3
#define A_GL_MINOR_VERSION 0

// GL uses GL3.3
#else
#define A_GL_MAJOR_VERSION 3
#define A_GL_MINOR_VERSION 3
#endif


// forwarded from o/img.h
struct o_img;

/**
 * Different screen_modes, not all are available on some platforms
 */
enum a_app_screen_mode {
    a_app_MODE_WINDOW,
    a_app_MODE_MAXIMIZED,
    a_app_MODE_FULLSCREEN,
    a_app_NUM_MODES
};

struct a_app_run_options {
    const char *title;
    int cols, rows;
    enum a_app_screen_mode mode;
    struct s_audio_spec_ex audio_spec_ex;
};

/**
 * main function to create scenes, etc.
 * the app runs until all scenes are deleted, so if no scene is created, the main loop will get stopped.
 * @param root object == a_app_root
 */
typedef void (*a_app_main_fn)(oobj root);


/**
 * Runs the main loop until an exit event is received, or all scenes are deleted (a_app_scene_exit(0))
 * @param main_fn runs with a, r, m, o init called first (deferred in the main loop)
 * @param opt_options custom start options, if not NULL
 */
O_EXTERN
void a_app_run(a_app_main_fn main_fn, const struct a_app_run_options *opt_options);

/**
 * @return default run options
 */
O_EXTERN
struct a_app_run_options a_app_run_options_default(void);


/**
 * @return the main app object root to allocate on.
 *         Use scene objects to allocate for scenes as default...
 * @note Will init a_app on first call, which will be done in AScene_new as an example
 */
O_EXTERN
oobj a_app_root(void);


/**
 * @return the current screen mode like a_app_MODE_FULLSCREEN
 */
O_EXTERN
enum a_app_screen_mode a_app_screen_mode(void);


/**
 * @param set the current screen mode like a_app_MODE_FULLSCREEN
 * @note not all platforms support all modes, the nearest is choosen than, for example:
 *       MIA_PLATFORM_EMSCRIPTEN is always a_app_MODE_FULLSCREEN
 */
O_EXTERN
enum a_app_screen_mode a_app_screen_mode_set(enum a_app_screen_mode set);

/**
 * @return the current app / window title
 */
O_EXTERN
const char *a_app_title(void);

/**
 * @param set the current app / window title
 */
O_EXTERN
void a_app_title_set(const char *set);

/**
 * @param set the current app / window icon as struct o_img ( o/img.h )
 */
O_EXTERN
void a_app_icon_set(const struct o_img *set);

/**
 * @param icon_path an image file to load the icon from
 */
O_EXTERN
void a_app_icon_set_file(const char *icon_path);

/**
 * Opens the hardware audio device for playback.
 * Will call s_audio_device_open on first pointer down event.
 * This way the browser will not block the sound.
 */
O_EXTERN
void a_app_audio_device_open(void);

/**
 * @return current delta time, may differ in update calls
 * @note dt is updated in (and after) update calls.
 *       so event function has the old time
 */
O_EXTERN
double a_app_dt(void);

/**
 * @return current app time, may differ in update calls
 * @note dt is updated in (and after) update calls.
 *       so event function has the old time
 */
O_EXTERN
double a_app_time(void);

/**
 * @return current fps
 * @note smoothed
 */
O_EXTERN
float a_app_fps(void);

/**
 * Load is the amount of time spend in ratio to the full frame time.
 * @return current load (all stuff except for waiting for next frame of course) [%]
 * @note smoothed
 */
O_EXTERN
float a_app_load(void);

/**
 * @return true if focus is lost and the app is in pause mode.
 */
O_EXTERN
bool a_app_paused(void);

/**
 * @return true (default) if the app should skip working during pause
 */
O_EXTERN
bool a_app_suspend_paused(void);

/**
 * @return true (default) if the app should skip working during pause
 */
O_EXTERN
bool a_app_suspend_paused_set(bool suspend_during_pause);

/**
 * @return the total number of allocated pool blocks
 */
O_EXTERN
int a_app_pool_blocks_num(void);

/**
 * @return the number of used pool blocks
 * @note if this grows each frame, you may have a memory leak
 */
O_EXTERN
int a_app_pool_blocks_used(void);

/**
 * @return true for touch screens
 * @note may be changed during runtime to true, after first usage of the touch screen
 */
O_EXTERN
bool a_app_is_touch(void);

/**
 * Will reset all touchscreen data deferred to the next frame
 */
O_EXTERN
void a_app_reset_touch(void);

/**
 * Screen density to allow for more responsive design.
 * For example a dpi below 200 (not a phone) + large scale -> use larger camera?
 * @return current screen dpi (dots/pixel per inch). >MAY BE 0!<
 * @note value range:
 *       cxxdroid:  560
 *       wsl:       0
 *       msvc:      96
 *       web pc:    96
 *       web phone: 336
 */
O_EXTERN
float a_app_dpi(void);

/**
 * @return The current active AView.
 * @note asserts that a view is currently active
 */
O_EXTERN
oobj a_app_view_try(void);

/**
 * @return The current active AView.
 * @note asserts that a view is currently active
 */
O_INLINE
oobj a_app_view(void)
{
    oobj current_view = a_app_view_try();
    assert(current_view && "no active AView");
    return current_view;
}

/**
 * @return The current active AView's RCam.
 * @note asserts that a view is currently active
 *       Just calls AView_cam(a_app_view());
 */
O_EXTERN
oobj a_app_cam(void);

/**
 * @return Current amount of scenes in the scene tack
 */
O_EXTERN
int a_app_scenes_num(void);

/**
 * Returns the AScene object for the given stack position / index
 * @param index stack position / index (safe to pass outside bounds)
 * @return AScene object or NULL on failure
 */
O_EXTERN
struct oobj_opt a_app_scene_get(int index);

/**
 * Returns the stack position / index in from the given object
 * @param scene AScene object (safe to pass stupid stuff...)
 * @return the index in the stack or -1 on failure
 */
O_EXTERN
int a_app_scene_get_index(oobj scene);


/**
 * @return The current active scene or -1 if none is active
 */
O_EXTERN
int a_app_scene_index_try(void);


/**
 * @return The current active scene.
 * @note asserts that a scene is currently active
 */
O_INLINE
int a_app_scene_index(void)
{
    int current_scene_index = a_app_scene_index_try();
    assert(current_scene_index>=0 && "no active AScene");
    return current_scene_index;
}

/**
 * @return The current active AScene.
 * @note asserts that a scene is currently active
 */
O_INLINE
oobj a_app_scene(void)
{
    return a_app_scene_get(a_app_scene_index()).o;
}


/**
 * Switches the position of two AScenes in the scene stack.
 * Only call this function if you know what you are doing.
 * Mia normally expects a working resource hierarchy and scenes may assume that they have specific parents.
 *      Mixing that could cause errors.
 * A use case of this function is u_splash:
 *      The splash scene is created first and rendered.
 *      In the following frame the actual scene is created and so its position is up the stack.
 *      The splash scene changes their stack positions and runs the actual scene to create it (may block during loadup)
 *      If the splash scene finishes, it deletes itself
 *
 * @param scene_index_a, scene_index_b the scene indices to be changed in the scene stack
 * @note logs an error on failure
 *       asserts main thread
 */
O_EXTERN
void a_app_scene_switch_stack_position(int scene_index_a, int scene_index_b);

/**
 * Deletes the given scene index and all down the stack
 * @param scene_index the index of the scenes (and all up the stack) to be deleted
 * @note logs an error on failure
 *       logs a warning, if the current active scene is >= scene_index.
 *       asserts main thread
 */
O_EXTERN
void a_app_scene_exit_instant(int scene_index);


/**
 * Deletes the given scene index and all down the stack.
 * Defers it to the next frame and calls a_app_scene_exit_instant then
 * @param scene_index the index of the scenes (and all up the stack) to be deleted
 *                    <0 to exit the current scene
 * @note logs an error on failure
 *       asserts main thread
 */
O_EXTERN
void a_app_scene_exit(int scene_index);


/**
 * Deletes the current scene index (a_app_scene_index) and all down the stack.
 * Defers it to the next frame and calls a_app_scene_exit_instant then
 * @note logs an error on failure
 *       asserts main thread
 */
O_INLINE
void a_app_scene_exit_current(void)
{
    a_app_scene_exit(a_app_scene_index());
}


#endif //A_APP_H
