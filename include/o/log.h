#ifndef O_LOG_H
#define O_LOG_H

/**
 * @file log.h
 *
 * Logging system.
 * Settings here are local per thread.
 * Level and quiet option are copied on thread creation in an OThread
 *      opt_stream is NOT copied in to a new thread!
 */

#include "common.h"

//
// options:
//

/** define to not print the time stamp and name: */
// MIA_LOG_COMPACT


/**
 * optional library name like "o"
 */
#ifndef O_LOG_LIB
#define O_LOG_LIB NULL
#endif


/** log levels to be used in o_log_base */
enum o_log_level {
    O_LOG_TRACE, O_LOG_DEBUG, O_LOG_INFO, O_LOG_WARN, O_LOG_ERROR, O_LOG_WTF, O_LOG_NUM_LEVELS
};

/** trace log the formatted text @threadsafe */
#define o_log_trace(...) o_log_base(O_LOG_TRACE, O_LOG_LIB, __FILE__, __LINE__, __func__, __VA_ARGS__)

/** debug log the formatted text @threadsafe */
#define o_log_debug(...) o_log_base(O_LOG_DEBUG, O_LOG_LIB, __FILE__, __LINE__, __func__, __VA_ARGS__)

/** info log the formatted text @threadsafe */
#define o_log_info(...)  o_log_base(O_LOG_INFO, O_LOG_LIB, __FILE__, __LINE__, __func__, __VA_ARGS__)

/** info log the formatted text, same as o_log_info @threadsafe */
#define o_log(...)  o_log_base(O_LOG_INFO, O_LOG_LIB, __FILE__, __LINE__, __func__, __VA_ARGS__)

/** warn log the formatted text @threadsafe */
#define o_log_warn(...)  o_log_base(O_LOG_WARN, O_LOG_LIB, __FILE__, __LINE__, __func__, __VA_ARGS__)

/** error log the formatted text @threadsafe */
#define o_log_error(...) o_log_base(O_LOG_ERROR, O_LOG_LIB, __FILE__, __LINE__, __func__, __VA_ARGS__)

/** wtf log the formatted text @threadsafe */
#define o_log_wtf(...)   o_log_base(O_LOG_WTF, O_LOG_LIB, __FILE__, __LINE__, __func__, __VA_ARGS__)

//
// 's'horter versions with specific func name
//

/** trace log shortened the formatted text @threadsafe */
#define o_log_trace_s(func, ...) o_log_base(O_LOG_TRACE, O_LOG_LIB, NULL, 0, func, __VA_ARGS__)

/** debug log shortened the formatted text @threadsafe */
#define o_log_debug_s(func, ...) o_log_base(O_LOG_DEBUG, O_LOG_LIB, NULL, 0, func, __VA_ARGS__)

/** info log shortened the formatted text @threadsafe */
#define o_log_info_s(func, ...)  o_log_base(O_LOG_INFO, O_LOG_LIB, NULL, 0, func, __VA_ARGS__)

/** info log shortened the formatted text, same as o_log_info @threadsafe */
#define o_log_s(func, ...)  o_log_base(O_LOG_INFO, O_LOG_LIB, NULL, 0, func, __VA_ARGS__)

/** warn log shortened the formatted text @threadsafe */
#define o_log_warn_s(func, ...)  o_log_base(O_LOG_WARN, O_LOG_LIB, NULL, 0, func, __VA_ARGS__)

/** error log shortened the formatted text @threadsafe */
#define o_log_error_s(func, ...) o_log_base(O_LOG_ERROR, O_LOG_LIB, NULL, 0, func, __VA_ARGS__)

/** wtf log shortened the formatted text @threadsafe */
#define o_log_wtf_s(func, ...)   o_log_base(O_LOG_WTF, O_LOG_LIB, NULL, 0, func, __VA_ARGS__)


/**
 * @return the current log level
 */
O_EXTERN
enum o_log_level o_log_level(void);

/**
 * @param level sets the minimal logged level
 */
O_EXTERN
enum o_log_level o_log_level_set(enum o_log_level level);

/**
 * @return true if quiet and logging is stopped (of logs below O_LOG_ERROR)
 */
O_EXTERN
bool o_log_quiet(void);

/**
 * @param set true to stop logging (of logs below O_LOG_ERROR)
 */
O_EXTERN
void o_log_quiet_set(bool set);

/**
 * Create a block in which the logging is set to quiet (o_log_set_quiet)
 * Use continue to leave the block.
 * @threadsafe
 * @note May be nested, but needs to be in another line (uses __LINE__ internally).
 *       DO NOT:
 *          return in the block!
 *          break in the block! (this block in a for loop...)
 */
#define o_log_quiet_block \
for(bool O_NAME_CONCAT(o_log_quiet_block__run_, __LINE__) = (o_log_quiet_set(true), true); \
O_NAME_CONCAT(o_log_quiet_block__run_, __LINE__); \
O_NAME_CONCAT(o_log_quiet_block__run_, __LINE__) = (o_log_quiet_set(false), false))

/**
 * @return OStream object or NULL to additionally log into
 */
O_EXTERN
struct oobj_opt o_log_stream(void);

/**
 * @param opt_stream OStream object or NULL to additionally log into
 */
O_EXTERN
void o_log_stream_set(oobj opt_stream);

/**
 * Logging base function, may be called by library functions to make use of opt_lib
 * @param level: The logging level_used
 * @param opt_lib: if true, the tag for the library
 * @param opt_file: if given, the name and line will be printed (__FILE__)
 * @param line: if opt_file!=NULL, printed after the name (__LINE__)
 * @param func: function name (__func__)
 * @param format: formatted text
 * @threadsafe
 */
O_EXTERN
void o_log_base(enum o_log_level level, const char *opt_lib, const char *opt_file, int line,
                const char *func, const char *format, ...);

#endif //O_LOG_H
