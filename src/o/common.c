#include "o/common.h"

#define O_LOG_LIB "o"
#include "o/log.h"

// must include all, because SDL_init is located here...
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifdef MIA_OPTION_TTF
#include <SDL2/SDL_ttf.h>
#endif

#ifdef MIA_OPTION_SOCKET
#include <SDL2/SDL_net.h>
#endif

#include <time.h>


#ifdef MIA_PLATFORM_EMSCRIPTEN
#include <emscripten.h>
#endif


static const ou32 RAND_A = 1103515245;
static const ou32 RAND_C = 12345;


_Static_assert(sizeof(ou64) >= sizeof(SDL_threadID), "invalid size?");

_Static_assert(sizeof(int) >= sizeof(oi32), "Using \"o\" in a u-controller or what?, "
                                            "should work fine..."
                                            "I used osize for most, but never tested it :D,"
                                            "just remove this _Static_assert...");

static struct {
    bool init;
    ou64 main_thread_id;
} common_L;

// each thread has its own o_rand state
static _Thread_local ou32 L_rand_x;


//
// protected
//

void o__sanitizer_leak_check(const char *why, bool full)
{
#ifdef MIA_OPTION_SANITIZER
    void __lsan_do_leak_check(void);
    int __lsan_do_recoverable_leak_check(void);

    // checks for memory leaks
    o_log_debug_s(why, "sanitizer leak check...");
    int leaks = __lsan_do_recoverable_leak_check();

    // this call also checks for leaks, but than it doesnt check at program end
    //      some SDL (or ports) may have some memory leaks
    //      so we call it before shutting them down to see our own mistakes
    //      may fail and exit the app
    if(full) {
        __lsan_do_leak_check();
    }

    if (leaks) {
        o_log_error_s(why, "sanitizer leak check done, got %i leaks!", leaks);
    } else {
        o_log_s(why, "sanitizer leak check done, without any leaks");
    }
#endif
}


//
// public
//

void o_init(void)
{
    if(common_L.init) {
        o_log_error_s(__func__, "o_init called already!");
        return;
    }
    common_L.init = true;

    Uint32 flags = SDL_INIT_EVERYTHING;

#ifdef MIA_PLATFORM_EMSCRIPTEN
    flags = SDL_INIT_VIDEO | SDL_INIT_AUDIO;
#endif

#ifdef MIA_OPTION_GAMEPAD
    flags |= SDL_INIT_GAMECONTROLLER;
#endif

    if (SDL_Init(flags) != 0) {
        o_exit("SDL_Init failed");
    }


    int img_flags = IMG_INIT_PNG | IMG_INIT_JPG;
    if(!(IMG_Init(img_flags) & img_flags)) {
        o_exit("IMG_Init failed");
    }

    // SDL_Mixer should be init after first pointer event
    //           not part of the framework itself yet, up to the user

#ifdef MIA_OPTION_TTF
    if (TTF_Init() != 0) {
        o_exit("TTF_Init failed");
    }
#endif

#ifdef MIA_OPTION_SOCKET
    if (SDLNet_Init() != 0) {
        o_exit("SDLNet_Init failed");
    }
#endif


    common_L.main_thread_id = o_thread_id();

    // seed random
    o_rand_seed(time(NULL));

#ifdef MIA_OPTION_FETCH
    // init OFetch
    O_EXTERN
    void o__fetch_init(void);
    o__fetch_init();
#endif

    // sync records (emscripten...)
    O_EXTERN
    void o_file_record__mount(void);
    o_file_record__mount();

    o_log_s(__func__, "start");
}

bool o_init_called(void)
{
    return common_L.init;
}


#define MSG_SIZE 4096

void o__exit_impl(const char *file, int line, const char *reason_format, ...)
{
    va_list args;
    va_start(args, reason_format);
    char *msg = SDL_malloc(MSG_SIZE);
    vsnprintf(msg, MSG_SIZE, reason_format, args);
    va_end(args);


#ifdef NDEBUG
    fprintf(stderr, "-> Program exit, reason: %s\n", msg);
#else
    fprintf(stderr, "-> Program exit at %s:%d %s\n", file, line, msg);
#endif

#ifdef MIA_PLATFORM_EMSCRIPTEN
    {
        // exit emscriptens main loop and call js error handler
        int script_size = 2*MSG_SIZE;
        char *script = SDL_malloc(script_size);
        snprintf(script, script_size, "o__exit(\'%s\');", msg);
        emscripten_cancel_main_loop();
        emscripten_run_script(script);
    }
#endif

#ifdef MIA_PLATFORM_ANDROID
    {
        // show a small toast dialog in the app and then exit
        int toast_size = 2 * MSG_SIZE;
        char *toast = SDL_malloc(toast_size);
        snprintf(toast, toast_size, "Program exit, reason: %s", msg);
        SDL_AndroidShowToast(toast, 1, -1, 0, 0);
        SDL_Delay(8000);
    }
#endif

    o__sanitizer_leak_check("o_exit", true);

    exit(EXIT_FAILURE);

    // should not get here...
    for (;;) {
        SDL_Delay(1);
    }
}

void o__assume_impl(const char *expression, const char *file, int line, const char *reason_format, ...)
{
    va_list args;
    va_start(args, reason_format);
    char *msg = SDL_malloc(MSG_SIZE);
    vsnprintf(msg, MSG_SIZE, reason_format, args);
    va_end(args);

    char *full_msg = SDL_malloc(MSG_SIZE);
#ifdef NDEBUG
    snprintf(full_msg, MSG_SIZE, "Assumption failed: %s", msg);
#else
    snprintf(full_msg, MSG_SIZE, "Assumption failed: (%s) %s", expression, msg);
#endif

    o__exit_impl(file, line, full_msg);
}


ou64 o_thread_id(void)
{
    return SDL_ThreadID();
}

ou64 o_thread_main_id(void)
{
    return common_L.main_thread_id;
}

bool o_thread_set_priority(enum OThread_priority priority)
{
    // enums should match, just in case
    int ret = SDL_SetThreadPriority(
    (SDL_ThreadPriority[]) {SDL_THREAD_PRIORITY_LOW,
                            SDL_THREAD_PRIORITY_NORMAL,
                            SDL_THREAD_PRIORITY_HIGH,
                            SDL_THREAD_PRIORITY_TIME_CRITICAL}[priority]);
    return ret == 0;
}

void o_sleep(osize millis)
{
    millis = o_max(0, millis);

#ifdef MIA_PLATFORM_EMSCRIPTEN
    emscripten_sleep((unsigned int) millis);
#else
    SDL_Delay((Uint32) millis);
#endif
}


ou32 o_rand(void)
{
    L_rand_x = (RAND_A * L_rand_x + RAND_C) % o_rand_MAX;
    return L_rand_x;
}

void o_rand_seed(ou32 seed)
{
    L_rand_x = seed % o_rand_MAX;
}
