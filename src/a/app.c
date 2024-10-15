#include "a/app.h"
#include "o/OObjRoot.h"
#include "o/OArray.h"
#include "o/OPtr.h"
#include "o/ODelcallback.h"
#include "o/timer.h"
#include "o/img.h"
#include "m/vec/bvecn.h"
#include "m/vec/ivec2.h"
#include "m/mat/mat4.h"
#include "s/common.h"
#include "r/common.h"
#include "r/RCam.h"
#include "r/RObjQuad.h"
#include "a/AScene.h"
#include "a/ADefer.h"
#include "a/pointer.h"

#include <SDL2/SDL_hints.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_events.h>


#define O_LOG_LIB "a"
#include "o/log.h"

#ifdef MIA_PLATFORM_EMSCRIPTEN
#include <emscripten.h>
#endif

#ifdef MIA_OPTION_GLEW
#include "r/gl.h"
#endif


#define MAX_DELTA_TIME 5.0 // seconds

// only for some platforms, see block_for_fps_limit
#define MAX_FPS 60

#define LOAD_FPS_SMOOTH_ALPHA 0.025


#ifndef MIA_TITLE
#define MIA_TITLE Mia
#endif



static struct {
    bool init;
    SDL_Window *sdl_window;
    SDL_GLContext gl_context;
    bool running;
    bool paused;
    bool suspend_paused;
    bool open_audio;

    enum a_app_screen_mode screen_mode;

    ivec2 back_size_prev;
    float dpi;

    double dt, time;
    double limit_fps_time;

    float fps, load;

    bool is_touch;


    oobj root;

    // OArray od ADefer
    oobj deferred;

    // OArray of struct scene_info
    oobj scenes;

    // for pointers
    mat4 back_inv;

    // -1 for off
    int scene;

    // prevents recursion
    bool scene_exit_active;

    // set by AView
    oobj view;
} app_L;


struct scene_info {
    // if a scene got o_del'ed, it should automatically call exit
    // for that, each scene has an internal join object and the weakjoin accessor
    AScene *scene;

    // >NOT< THREADSAFE, but AScene's should not be used in threads...
    oobj ptr;
    double load_update;
    double load_render;
};


// returns NULL if not available
O_STATIC
struct scene_info *get_scene(int idx)
{
    if(idx<0 || idx >=o_num(app_L.scenes)) {
        return NULL;
    }
    struct scene_info *si = o_at(app_L.scenes, idx);
    if(!OPtr_get(si->ptr).o) {
        return NULL;
    }
    return si;
}


O_STATIC
void init(const struct a_app_run_options *opt_options)
{
    struct a_app_run_options options = {0};
    if(opt_options) {
        options = *opt_options;
    } else {
        options = a_app_run_options_default();
    }


    assert(!app_L.init);
    app_L.init = true;
    o_init();
    app_L.root = OObjRoot_new_pool();
    OObj_name_set(app_L.root, "a_app_root");
    ODelcallback_new_assert(app_L.root, "a_app_root", "deleted!");

    s_init(app_L.root, &options.audio_spec_ex);

    app_L.deferred = OArray_new_dyn(app_L.root, NULL, sizeof(ADefer *), 0, 32);
    app_L.scenes = OArray_new_dyn(app_L.root, NULL, sizeof(struct scene_info), 0, 16);

    app_L.suspend_paused = true;

    // some start values for the smoothing
    app_L.fps = 60;
    app_L.load = 0.5;

#ifdef MIA_OPTION_GAMEPAD
    // disable accelerometer joysticks
    SDL_SetHintWithPriority(SDL_HINT_ACCELEROMETER_AS_JOYSTICK, "0", SDL_HINT_OVERRIDE);
#endif


    // setup OpenGL usage
    o_log_debug_s(__func__,
               "OpenGL minimal version: %d.%d", A_GL_MAJOR_VERSION, A_GL_MINOR_VERSION);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, A_GL_MAJOR_VERSION);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, A_GL_MINOR_VERSION);
#ifdef MIA_OPTION_GLES
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#endif

    // RGB 8 bit per channel
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // create window
    // SDL_WINDOW_ALLOW_HIGHDPI creates render bugs in web version?
    app_L.sdl_window = SDL_CreateWindow(options.title,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                    options.cols, options.rows,
                                    SDL_WINDOW_OPENGL
                                    | SDL_WINDOW_RESIZABLE
                                    //| SDL_WINDOW_ALLOW_HIGHDPI
    );
    if (!app_L.sdl_window) {
        o_exit("failed creating a window: %s", SDL_GetError());
    }
    SDL_SetWindowMinimumSize(app_L.sdl_window, 1, 1);
    a_app_screen_mode_set(options.mode);

    // Not necessary, but recommended to create a gl context:
    app_L.gl_context = SDL_GL_CreateContext(app_L.sdl_window);
    if (!app_L.gl_context) {
        o_exit("failed creating the OpenGL context: %s", SDL_GetError());
    }

#ifdef MIA_OPTION_GLEW
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        o_exit("glewInit failed: %s", glewGetErrorString(err));
    }
    o_log_debug_s(__func__,
               "Using GLEW version: %s", glewGetString(GLEW_VERSION));
#endif

    //
    // r init
    //
    r_init(app_L.root);

    // protected
    O_EXTERN
    void a_pointer__init(void);
    a_pointer__init();

    O_EXTERN
    void a_input__init(void);
    a_input__init();
    
    
#ifdef MIA_OPTION_TESTS
    o_log_trace_s(__func__, "test...");
    void o__test_main(void);
    o__test_main();
    o_log_debug_s(__func__, "test finished successfully");
#endif

}

O_STATIC
void handle_defers(void)
{
    // calling deferred functions
    for (osize i = 0; i < o_num(app_L.deferred); i++) {
        ADefer *defer = *OArray_at(app_L.deferred, i, ADefer*);
        if (!defer || !defer->fn) {
            // unregistered...
            continue;
        }
        defer->fn(defer);
    }
    // delete em
    for (osize i = 0; i < o_num(app_L.deferred); i++) {
        ADefer *defer = *OArray_at(app_L.deferred, i, ADefer*);
        o_del(defer);
    }
    OArray_resize(app_L.deferred, 0);
}

O_STATIC
void handle_window_event(const SDL_Event *event)
{
    if(event->window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
        o_log_s(__func__, "paused");
        app_L.paused = false;
    } else if(event->window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
        o_log_s(__func__, "resumed");
        app_L.paused = true;
    }

    // call events
    for (int s = 0; s < o_num(app_L.scenes); s++) {
        struct scene_info *si = get_scene(s);
        if (!si) {
            continue;
        }
        if(app_L.paused && si->scene->on_pause_event) {
            si->scene->on_pause_event(si->scene);
        }
        if(!app_L.paused && si->scene->on_resume_event) {
            si->scene->on_pause_event(si->scene);
        }
    }
}

O_STATIC
void handle_events(void)
{
    // protected
    O_EXTERN
    void a_pointer__update(void);
    O_EXTERN
    void a_pointer__update_events_handled(void);
    O_EXTERN
    void a_pointer__handle_event(SDL_Event *event);
    O_EXTERN
    void a_input__update(void);
    O_EXTERN
    void a_input__handle_key_event(SDL_Event *event);
    O_EXTERN
    void a_input__handle_wheel_event(SDL_Event *event);

    // resets before the events are handled
    a_pointer__update();
    a_input__update();

    SDL_Event event;
    while (SDL_PollEvent(&event)) {

        if (event.type == SDL_QUIT) {
            app_L.running = false;
            return;
        }

        switch (event.type) {
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEMOTION:
            case SDL_MOUSEBUTTONUP:
            case SDL_FINGERDOWN:
            case SDL_FINGERMOTION:
            case SDL_FINGERUP:
                a_pointer__handle_event(&event);
                break;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                a_input__handle_key_event(&event);
                break;
            case SDL_MOUSEWHEEL:
                a_input__handle_wheel_event(&event);
                break;
            case SDL_WINDOWEVENT:
                handle_window_event(&event);
                break;
        }
    }

    a_pointer__update_events_handled();
}

O_STATIC
void block_for_fps_limit(void) {
    app_L.limit_fps_time += (1.0 / MAX_FPS);
    double diff = app_L.limit_fps_time - app_L.time;
    // <= 0 safe -> will yield than, but that's ok
    o_sleep((osize)(diff * 1000));
}

// float out. fps, load*
O_STATIC
float smooth_out_value(float old_value, float new_value)
{
    if (m_isnan(new_value) || m_isinf(new_value)) {
        return old_value;
    }
    return m_mix(old_value, new_value, LOAD_FPS_SMOOTH_ALPHA);
}

O_STATIC
void main_loop(void)
{
    if(app_L.paused && app_L.suspend_paused) {
        handle_events();
        return;
    }

    // check full frame load time
    ou64 load_timer = o_timer();

    static ou64 timer = 0;
    if (timer == 0) {
        timer = o_timer();
    }

    app_L.scene = -1;

    app_L.dt = o_timer_reset_s(&timer);

    if(app_L.dt < 0 || app_L.dt >= MAX_DELTA_TIME) {
        o_log_trace_s(__func__, "dropped frame: %g sec", app_L.dt);
        return;
    }

    app_L.time += app_L.dt;
    app_L.fps = o_min(240, smooth_out_value(app_L.fps, (float) (1.0 / app_L.dt)));

    // defers should not render...
    handle_defers();

    // check deleted scenes
    for (int i = 0; i < o_num(app_L.scenes); i++) {
        if(!get_scene(i)) {
            a_app_scene_exit_instant(i);
            break;
        }
    }

    if (o_num(app_L.scenes) == 0) {
        app_L.running = false;
        return;
    }

    // check if touch screen is used (affects handle_events() below)
    app_L.is_touch = SDL_GetNumTouchDevices() > 0;

    // sdl events
    handle_events();
    if (!app_L.running) {
        return;
    }

    // open audio on pointer down, else browsers may block the sound (emscripten build)
    if(app_L.open_audio) {
        // we need to use _back, the default a_pointer function needs an active AView!
        struct a_pointer p = a_pointer_back(0, 0);
        if(p.active) {
            s_audio_device_open();
            app_L.open_audio = false;
        }
    }


    // window size (updated by the sdl event system, so handle_events() first...)
    ivec2 window_size;
    SDL_GL_GetDrawableSize(app_L.sdl_window, &window_size.x, &window_size.y);
    bool size_changed = !ivec2_equals_v(window_size, app_L.back_size_prev);
    app_L.back_size_prev = window_size;

    if (size_changed) {
        *r_back_proj() = r_proj_new(window_size, vec2_(180), true);
        app_L.back_inv = mat4_inv(r_back_proj()->cam);
        RTex_viewport_set(NULL, ivec4_(0, 0, m_2(window_size)));

        SDL_GetDisplayDPI(0, &app_L.dpi, NULL, NULL);
        o_log_trace_s("main_loop",
                   "window size changed, "
                   "size: %i:%i; dpi=%f",
                   window_size.x, window_size.y,app_L.dpi);
    }

    // begin a new frame
    r_frame_begin(m_2(window_size));

    RTex_clear_full(NULL, R_BLACK);


    // set viewports and call update
    // backwards, so handled pointer events work
    //     until the first opaque scene
    int scene_start = 0;
    for (int s = (int) o_num(app_L.scenes) - 1; s >= scene_start; s--) {
        struct scene_info *si = get_scene(s);
        if(!si) {
            continue;
        }
        if (AScene_opaque(si->scene)) {
            scene_start = s;
        }


        ou64 scene_timer = o_timer();
        

        if(si->scene->full_x_auto) {
            int l = 0;
            int w = r_back_size_int().x;
            if(si->scene->full_even_size) {
                int off = w % 2;
                w -= off;
            }
            si->scene->viewport.v0 = l;
            si->scene->viewport.v2 = w;
        }
        if(si->scene->full_y_auto) {
            int b = 0;
            int h = r_back_size_int().y;
            if(si->scene->full_even_size) {
                int off = h % 2;
                h -= off;
                b += off;
            }
            si->scene->viewport.v1 = b;
            si->scene->viewport.v3 = h;
        }

        app_L.scene = s;
        AView_update(si->scene->view, NULL, si->scene->viewport);

        si->load_update = m_min(1, smooth_out_value(si->load_update, o_timer_elapsed_s(scene_timer) / app_L.dt));
    }
    app_L.scene = -1;

    // call render
    for (int s = scene_start; s < o_num(app_L.scenes); s++) {
        struct scene_info *si = get_scene(s);
        if(!si) {
            continue;
        }


        ou64 scene_timer = o_timer();

        app_L.scene = s;
        AView_render(si->scene->view, NULL);

        si->load_render = m_min(1, smooth_out_value(si->load_render, o_timer_elapsed_s(scene_timer) / app_L.dt));
    }
    app_L.scene = -1;

    // measure load before swapping window, which will block on some platforms for the next vsync
    double load_time = o_timer_elapsed_s(load_timer);

    
    app_L.load = m_min(1, smooth_out_value(app_L.load, load_time / app_L.dt));

    // end the frame
    // may block until next frame...
    SDL_GL_SwapWindow(app_L.sdl_window);

    r_error_check("frame finished");
}

//
// public
//

// runs main deferred in the main loop
O_STATIC
void main_deferred(oobj obj)
{
    a_app_main_fn main_fn = *((a_app_main_fn *) o_user(obj));
    main_fn(a_app_root());
}

void a_app_run(a_app_main_fn main_fn, const struct a_app_run_options *opt_options)
{
    init(opt_options);

    //
    // add main_fn as ADefer to run it in the main loop first
    //
    oobj main_defer = ADefer_new(a_app_root(), main_deferred);
    // clone into a new alloacted buffer, because a function pointer != void * (sizeof...)
    o_user_set(main_defer, o_new_clone(main_defer, &main_fn, a_app_main_fn, 1));

    //
    // start main_loop
    //

    o_log_s(__func__, "run");

    app_L.running = true;

#ifdef MIA_PLATFORM_EMSCRIPTEN
    emscripten_set_main_loop(main_loop, 0, true);
#else
    while (app_L.running) {
        main_loop();
#if defined(MIA_PLATFORM_UNIX) || defined(MIA_PLATFORM_CXXDROID)
        block_for_fps_limit();
#endif
}
#endif

    //
    // app finished when this code is reached!
    //

    void o__sanitizer_leak_check(const char *why, bool full);
    o__sanitizer_leak_check("App finished", true);

    SDL_DestroyWindow(app_L.sdl_window);

    // todo quit sdl and ports somewhere?
    /*
#ifdef OPTION_TTF
    TTF_Quit();
#endif
    IMG_Quit();
    SDL_Quit();
     */
}



struct a_app_run_options a_app_run_options_default(void)
{
    struct a_app_run_options options = {0};
    options.title = O_TO_STRING(MIA_TITLE);
    options.cols = 2 * 180;
    options.rows = 2 * 180 * 16 / 9;
    
#ifdef MIA_INIT_FULLSCREEN
    options.mode = a_app_MODE_FULLSCREEN;
#else
    options.mode = a_app_MODE_WINDOW;
#endif

    options.audio_spec_ex = s_audio_spec_ex_default();
    return options;
}


oobj a_app_root(void)
{
    assert(app_L.root && "call a_app_init first");
    return app_L.root;
}

enum a_app_screen_mode a_app_screen_mode(void)
{
    return app_L.screen_mode;
}

enum a_app_screen_mode a_app_screen_mode_set(enum a_app_screen_mode set)
{
#ifdef MIA_PLATFORM_EMSCRIPTEN
    set = a_app_MODE_WINDOW;
#endif
#if defined(MIA_PLATFORM_ANDROID) || defined(MIA_PLATFORM_CXXDROID)
    if(set == a_app_MODE_MAXIMIZED) {
        set = a_app_MODE_WINDOW;
    }
#endif
    Uint32 sdl_mode;
    if(set == a_app_MODE_MAXIMIZED) {
        sdl_mode = SDL_WINDOW_FULLSCREEN_DESKTOP;
    } else if(set == a_app_MODE_FULLSCREEN) {
        sdl_mode = SDL_WINDOW_FULLSCREEN;
    } else {
        sdl_mode = 0;
        set = a_app_MODE_WINDOW;
    }
    o_log_debug_s(__func__, "mode = %i", set);
    SDL_SetWindowFullscreen(app_L.sdl_window, sdl_mode);
    app_L.screen_mode = set;
    return app_L.screen_mode;
}

const char *a_app_title(void)
{
    return SDL_GetWindowTitle(app_L.sdl_window);
}

void a_app_title_set(const char *set)
{
    o_log_info_s(__func__, "title = %s", set);
    SDL_SetWindowTitle(app_L.sdl_window, set);
}

void a_app_icon_set(const struct o_img *set)
{
    struct o_img tmp = o_img_new(app_L.root, set->cols, set->rows);
    o_img_flip_rows(tmp, *set);
    SDL_Surface *surface = o_img_as_sdl_surface(tmp);
    SDL_SetWindowIcon(app_L.sdl_window, surface);
    SDL_FreeSurface(surface);
    o_img_free(&tmp);
}

void a_app_icon_set_file(const char *icon_path)
{
    struct o_img img = o_img_new_file(app_L.root, icon_path);
    a_app_icon_set(&img);
    o_img_free(&img);
}

void a_app_audio_device_open(void)
{
    app_L.open_audio = true;
}

double a_app_dt(void)
{
    return app_L.dt;
}

double a_app_time(void)
{
    return app_L.time;
}

float a_app_fps(void)
{
    return app_L.fps;
}

float a_app_load(void)
{
    return app_L.load;
}

bool a_app_paused(void)
{
    return app_L.paused;
}
bool a_app_suspend_paused(void)
{
    return app_L.suspend_paused;
}

O_EXTERN
bool a_app_suspend_paused_set(bool suspend_during_pause)
{
    app_L.suspend_paused = suspend_during_pause;
    return suspend_during_pause;
}

int a_app_pool_blocks_num(void)
{
    return o_allocator_pool_blocks_num(
            OObj_allocator(app_L.root));
}


int a_app_pool_blocks_used(void)
{
    return o_allocator_pool_blocks_used(
            OObj_allocator(app_L.root));
}

bool a_app_is_touch(void)
{
    return app_L.is_touch;
}

O_STATIC
void defer_reset_touch(oobj defer)
{
    // protected
    O_EXTERN
    void a_pointer__reset_touch_instantly(void);
    a_pointer__reset_touch_instantly();
}

void a_app_reset_touch(void)
{
    oobj defer = ADefer_new(a_app_root(), defer_reset_touch);
}

float a_app_dpi(void)
{
    return app_L.dpi;
}

// protected
O_EXTERN
void a_app__view_set(oobj opt_view)
{
    app_L.view = opt_view;
}

oobj a_app_view_try(void)
{
    return app_L.view;
}

oobj a_app_cam(void)
{
    return AView_cam(a_app_view());
}

// protected
O_EXTERN
void a_app__defer_register(ADefer *defer)
{
    OArray_push(app_L.deferred, &defer);
}

// protected
O_EXTERN
void a_app__defer_unregister(ADefer *defer)
{
    for (osize i = 0; i < o_num(app_L.deferred); i++) {
        ADefer **it = OArray_at(app_L.deferred, i, ADefer *);
        if (*it == defer) {
            *it = NULL;
            return;
        }
    }
    o_log_error_s("ADefer_del", "failed to unregister");
}


O_STATIC
void defer_add_scene(oobj defer)
{
    AScene *scene = o_user(defer);
    struct scene_info si = {0};
    si.scene = scene;
    si.load_update = si.load_render = 0.2;
    si.ptr = OPtr_new(a_app_root(), scene);
    OArray_push(app_L.scenes, &si);
}

// protected
O_EXTERN
void a_app__scene_register(AScene *scene)
{
    // register deffered in the next frame
    // Using scene as parent, so if scene is deleted (instantly in the same frame...), the defer gets unregistered
    oobj defer = ADefer_new(scene, defer_add_scene);
    o_user_set(defer, scene);
}

int a_app_scenes_num(void)
{
    return (int) o_num(app_L.scenes);
}

struct oobj_opt a_app_scene_get(int index)
{
    if (index < 0 || index >= o_num(app_L.scenes)) {
        return oobj_opt(NULL);
    }
    return oobj_opt(OArray_at(app_L.scenes, index, struct scene_info)->scene);
}

int a_app_scene_get_index(oobj scene)
{
    for (int i = 0; i < o_num(app_L.scenes); i++) {
        if (OArray_at(app_L.scenes, i, struct scene_info)->scene == scene) {
            return i;
        }
    }
    return -1;
}

int a_app_scene_index_try(void)
{
    return app_L.scene;
}

void a_app_scene_switch_stack_position(int scene_index_a, int scene_index_b)
{
    o_thread_assert_main();

    o_log_debug_s(__func__, "switching scene stack position of %i <> %i", scene_index_a, scene_index_b);

    osize num = o_num(app_L.scenes);
    if(scene_index_a<0 || scene_index_a>=num) {
        o_log_error_s(__func__, "index a is out of bounds");
        return;
    }
    if(scene_index_b<0 || scene_index_b>=num) {
        o_log_error_s(__func__, "index b is out of bounds");
        return;
    }
    if(scene_index_a == scene_index_b) {
        o_log_warn_s(__func__, "index a == b, ignoring");
        return;
    }

    struct scene_info *a = o_at(app_L.scenes, scene_index_a);
    struct scene_info *b = o_at(app_L.scenes, scene_index_b);
    struct scene_info tmp = *a;
    *a = *b;
    *b = tmp;
}

void a_app_scene_exit_instant(int scene_index)
{
    o_thread_assert_main();

    // prevent a recursion (needed?)
    if (app_L.scene_exit_active) {
        return;
    }
    app_L.scene_exit_active = true;

    if (app_L.scene >= scene_index) {
        o_log_warn_s(__func__,
                     "deleted in active scene?: %i>=%i", app_L.scene, scene_index);
    }

    if (scene_index < 0 || scene_index >= o_num(app_L.scenes)) {
        o_log_debug_s(__func__, "invalid scene index: %i", scene_index);
        return;
    }

    // pop all back until scene_index (incl.)
    // deletor will call this function, that's why the flag .scene_exit_active is set to prevent recursion
    // if the deletor of AScene is called instead of this function, the call to a_app__scene_exit_ignore
    //      sets the ignore index (+1)
    for (int i = (int) o_num(app_L.scenes) - 1; i >= scene_index; i--) {
        struct scene_info *si = OArray_at(app_L.scenes, i, struct scene_info);
        if (OPtr_get(si->ptr).o) {
            o_del(si->scene);
        }
        o_del(si->ptr);
    }
    OArray_resize(app_L.scenes, scene_index);

    app_L.scene = o_min(app_L.scene, o_num(app_L.scenes) - 1);

    app_L.scene_exit_active = false;

    // reset touch on scene exit...
    // protected
    void a_pointer__reset_touch_instantly(void);
    a_pointer__reset_touch_instantly();
}

O_STATIC
void defer_exit_scene(oobj defer)
{
    int *scene = o_user(defer);
    a_app_scene_exit_instant(*scene);
}

void a_app_scene_exit(int scene_index)
{
    o_thread_assert_main();

    if(scene_index<0) {
        scene_index = a_app_scene_index();
    }
    oobj defer = ADefer_new(a_app_root(), defer_exit_scene);
    int *scene = o_user_set(defer, o_new(defer, int, 1));
    *scene = scene_index;
}
