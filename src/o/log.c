#include "o/log.h"
#include "o/terminalcolor.h"
#include "o/OStream.h"
#include "o/str.h"
#include <time.h>

#include <SDL2/SDL_log.h>


#define LOG_MAX_SIZE 4096     // Should be the same as SDL's log max
#define LOG_HEADER_SIZE 256
#define LOG_MSG_SIZE (LOG_MAX_SIZE - LOG_HEADER_SIZE)


// to use or not to use colors...
#ifdef MIA_TERMINALCOLOR_OFF
#define LOG_COLORED false
#else
#define LOG_COLORED true
#endif

// new line for windows limited terminal...
#define LOG_OPT_NEWLINE
// seems not be a problem anymore, just here if it will be a problem in the future...
//#ifdef MIA_PLATFORM_MSVC
//#define LOG_OPT_NEWLINE "\n   ->    "
//#endif


#ifndef MIA_PLATFORM_DESKTOP
#define LOG_VIA_SDL
#endif


static _Thread_local struct {
    enum o_log_level level_used;
    enum o_log_level level_set;
    bool quiet;
    OStream *opt_stream;
} log_L;


static const char *level_color[] = {
        O_TERMINALCOLOR_HIGHINTENSITY_BLUE,
        O_TERMINALCOLOR_CYAN,
        O_TERMINALCOLOR_GREEN,
        O_TERMINALCOLOR_YELLOW,
        O_TERMINALCOLOR_RED,
        O_TERMINALCOLOR_MAGENTA
};

static const char *level_names[] = {
        "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "WTF"
};

O_STATIC
void gen_header_str(char *str, osize size,
                           const char *time_str,
                           const char *tag_str,
                           enum o_log_level level,
                           const char *opt_file, int line,
                           const char *opt_func,
                           bool colored)
{
    if (opt_file && *opt_file != '\0') {
        if (colored) {
            snprintf(str, size,
                     "%s %s%-5s "
                     O_TERMINALCOLOR_RESET
                     "%s"
                     O_TERMINALCOLOR_HIGHINTENSITY_BLACK
                     "%s:%d"
                     O_TERMINALCOLOR_RESET
                     LOG_OPT_NEWLINE
                     O_TERMINALCOLOR_HIGHINTENSITY_BLACK
                     "[%s] "
                     O_TERMINALCOLOR_RESET,
                     time_str, level_color[level], level_names[level], tag_str, opt_file, line,
                     opt_func);
        } else {
            snprintf(str, size,
                     "%s %-5s %s %s:%d"
                     LOG_OPT_NEWLINE
                     "[%s] ",
                     time_str, tag_str, level_names[level], opt_file, line, opt_func);
        }
    } else {
        if (colored) {
            snprintf(str, size,
                     "%s %s%-5s "
                     O_TERMINALCOLOR_RESET
                     "%s"
                     LOG_OPT_NEWLINE
                     O_TERMINALCOLOR_HIGHINTENSITY_BLACK
                     "[%s] "
                     O_TERMINALCOLOR_RESET,
                     time_str, level_color[level], level_names[level], tag_str, opt_func);
        } else {
            snprintf(str, size,
                     "%s %-5s %s"
                     LOG_OPT_NEWLINE
                     "[%s] ",
                     time_str, level_names[level], tag_str, opt_func);
        }
    }
}

enum o_log_level o_log_level(void)
{
    return log_L.level_set;
}

enum o_log_level o_log_level_set(enum o_log_level level)
{
    assert(level >= 0 && level < O_LOG_NUM_LEVELS);
    log_L.level_set = level;
    log_L.level_used = level;
    return level;
}

bool o_log_quiet(void)
{
    return log_L.quiet;
}

void o_log_quiet_set(bool set)
{
    log_L.quiet = set;
    if(set) {
        log_L.level_used = O_LOG_ERROR;
    } else {
        log_L.level_used = log_L.level_set;
    }
}

struct oobj_opt o_log_stream(void)
{
    return oobj_opt(log_L.opt_stream);
}

void o_log_stream_set(oobj opt_stream)
{
    log_L.opt_stream = opt_stream;
}


void o_log_base(enum o_log_level level, const char *opt_lib, const char *opt_file, int line,
                const char *opt_func, const char *format, ...)
{
    if (level < log_L.level_used) {
        return;
    }

#ifdef MIA_LOG_COMPACT
    char *time_str = "";
    opt_file = NULL;
#else
    /* Get current time */
    time_t t = time(NULL);
    struct tm *lt = localtime(&t);
    char time_str[16];
    osize time_size = strftime(time_str, sizeof time_str, "%H:%M:%S", lt);
    time_str[time_size] = '\0';
#endif

    char tag[32];
    if (opt_lib) {
        o_strf_buf(tag, "%s/ ", opt_lib);
    } else {
        tag[0] = '\0';
    }


    char header[LOG_HEADER_SIZE];
    char msg[LOG_MSG_SIZE];

    va_list args;
    va_start(args, format);
    vsnprintf(msg, sizeof msg, format, args);


    gen_header_str(header, sizeof header, time_str, tag, level, opt_file, line, opt_func, LOG_COLORED);

    // print to console
    o_terminalcolor_start();
#ifdef LOG_VIA_SDL
    SDL_Log("%s%s", header, msg);
#else
    fprintf(stderr, "%s%s\n", header, msg);
    fflush(stderr);
#endif
    o_terminalcolor_stop();

    // optional additional stream
    if (log_L.opt_stream) {
        if (LOG_COLORED) {
            gen_header_str(header, sizeof header, time_str, tag, level, opt_file, line, opt_func, false);
        }

        o_lock_block(log_L.opt_stream) {
            OStream_printf(log_L.opt_stream, "%s%s\n", header, msg);
        }
    }
}
