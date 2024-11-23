#ifndef O_COMMON_H
#define O_COMMON_H

/**
 * @file common.h
 *
 * Common used | Basic includes, macros, types and functions
 */

//
// Clib includes
//

#include <assert.h>
#include <float.h>      // MAX ..
#include <inttypes.h>   // PRIu8 ..
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>     // offsetof ..
#include <stdint.h>     // uint8_t ..
#include <stdio.h>
#include <string.h>

// we need this for the custom main function of sdl
#include <SDL2/SDL_main.h>


//
// function declaration modifier
//

/**
 * O_EXTERN marks functions for libraries as external "export" functions, visible for the linker
 */
#if (defined _WIN32 || defined WINCE || defined __CYGWIN__)
#define O_EXTERN __declspec(dllexport)
#elif defined __GNUC__ && __GNUC__ >= 4
#define O_EXTERN __attribute__ ((visibility ("default")))
#else
#define O_EXTERN
#endif

/**
 * O_INLINE marks functions as functions which may inline, not visible to the linker
 */
#define O_INLINE static inline

/**
 * O_STATIC marks functions as not visible to the linker
 */
#define O_STATIC static


//
// Macros
//

/**
 * Or for pointers and objects. (optional stuff)
 * @return a if a!=NULL, else b
 * @example:    foo = o_or(bar, baz);
 */
#define o_or(a, b) ((a)? (a): (b))

/**
 * Will call (*opt = set) if opt!=NULL
 * @return opt (NULL if NULL)
 */
#define o_opt_set(opt, set) ((opt)? (*(opt)=(set)), (opt) : NULL)

/** Typical system maximal alignment, windows does not support max_align_t yet... */
#define O_ALIGN_SYSTEM_MAX 16

/** @return a new size, that may be padded to the alignment */
#define o_align_size(size, alignment) (((size)/(alignment) + ((size) % (alignment) != 0)) * (alignment))

/** @return a new size, that may be padded to the maximum system size */
#define o_align_size_max(size) o_align_size(size, O_ALIGN_SYSTEM_MAX)

/** @return the absolute value */
#define o_abs(x) ((x) < 0 ? -(x) : (x))

/** @return the smaller value */
#define o_min(a, b) ((a)<(b)?(a):(b))

/** @return the bigger value */
#define o_max(a, b) ((a)>(b)?(a):(b))

/** @return x clamped in the range [min : max] (so can be exactly min and exactly max) */
#define o_clamp(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

/** @return x % y as positive version for integers */
#define o_mod(x, y) (((x) % (y) + (y)) % (y))

/** @return x>0: +1; x==0: 0; x<0: -1 (the sign of the value, or 0 on 0) */
#define o_sign(x) ((x) > 0 ? 1 : ((x) < 0 ? -1 : 0))

/** @return x>=edge? 1:0 (true or 1 if x has reached the step) */
#define o_step(edge, x) (((x) < (edge)) ? 0 : 1)

/** Unpacks the first 2 values of an array */
#define o_2(v) (v)[0], (v)[1]

/** Unpacks the first 3 values of an array */
#define o_3(v) (v)[0], (v)[1], (v)[2]

/** Unpacks the first 4 values of an array */
#define o_4(v) (v)[0], (v)[1], (v)[2], (v)[3]

/** Unpacks the first 9 values of an array (3x3 mat) */
#define o_9(v) (v)[0], (v)[1], (v)[2], (v)[3], (v)[4], (v)[5], (v)[6], (v)[7], (v)[8]

/** Unpacks the first 16 values of an array (4x4 mat) */
#define o_16(v) (v)[0], (v)[1], (v)[2], (v)[3], (v)[4], (v)[5], (v)[6], (v)[7], \
(v)[8], (v)[9], (v)[10], (v)[11], (v)[12], (v)[13], (v)[14], (v)[15]


// helper macros that will get nested below...
#define O__NAME_CONCAT_2(a, b) a ## b
#define O__TO_STRING_2(a) #a
#define O__VA_ARGS_FIRST_2(first, ...) first
#define O__VA_ARGS_COUNT_2(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, N, ...) N

/** @return a and be concatenated */
#define O_NAME_CONCAT(a, b) O__NAME_CONCAT_2(a, b)

/** @return "a" as a string */
#define O_TO_STRING(a) O__TO_STRING_2(a)

/** @return the first parameter of the variadic list */
#define O_VA_ARGS_FIRST(...) O__VA_ARGS_FIRST_2(__VA_ARGS__, 0)

/** @return the number of elements in the variadic list, up to 20, starting by 1 (0->1) */
#define O_VA_ARGS_COUNT(...) O__VA_ARGS_COUNT_2(__VA_ARGS__, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

/** A string set containing the isspace white characters */
#define O_ISSPACE_SET " \t\n\v\f\r"

#define O__TOSTR_2(x) #x
#define O_TOSTR(x) O__TOSTR_2(x)

#ifndef MIA_TITLE
#define MIA_TITLE mia
#endif

#define O_MIA_TITLE_STR O_TOSTR(MIA_TITLE)

//
// Basic primitive types
// NOTE: Using "o" its normal to just use the default C types like char, int, float and double (and void...)
//       These types are here if you want a specific size (serialisation)
//       And below are "osize" and "oobj", which are commonly used in "o" for sizes and objects
//

typedef uint8_t ou8;
typedef int8_t oi8;
typedef uint16_t ou16;
typedef int16_t oi16;
typedef uint32_t ou32;
typedef int32_t oi32;
typedef uint64_t ou64;
typedef int64_t oi64;
typedef float of32;
typedef double of64;
typedef ou8 obyte;

/**
 * Size type to count big stuff like bytes or file size, etc.
 * @note signed, so its less error prone and should be enough for 99.99999% cases
 */
typedef oi64 osize;

/** typeless object type (so it autocasts in C) */
typedef void *oobj;


/**
 * Functions may return this struct, if the created oobj may be NULL.
 * oobj's can always be NULL, but this way the user needs to access ".o"
 * and hopefully remembers to check for NULL.
 * Examples are OJson_new_read_* which may fail and return NULL with this struct.
 */
struct oobj_opt {
    oobj o;
};

/** simple function to create an oobj_opt more easily */
O_INLINE
struct oobj_opt oobj_opt(oobj opt)
{
    return (struct oobj_opt) {opt};
}



// int is always >=32bit in mia
_Static_assert(sizeof(int) >= sizeof(oi32), "int must be (+) 32 bits");

//
// Limits of the primitives
// NOTE: MIN and MAX are always the minimal (biggest negative) and maximal values a type can represent
//       So for float and double that does not match the C standard!
//       of32 is defined as (-MAX) which is the minimal value (biggest negative) to represent.
//           use of32_SMALL if you want the smallest not 0 number (which is MIN)
//

#define ou8_MIN 0
#define ou8_MAX UINT8_MAX
#define oi8_MIN INT8_MIN
#define oi8_MAX INT8_MAX
#define ou16_MIN 0
#define ou16_MAX UINT16_MAX
#define oi16_MIN INT16_MIN
#define oi16_MAX INT16_MAX
#define ou32_MIN 0
#define ou32_MAX UINT32_MAX
#define oi32_MIN INT32_MIN
#define oi32_MAX INT32_MAX
#define ou64_MIN 0
#define ou64_MAX UINT64_MAX
#define oi64_MIN INT64_MIN
#define oi64_MAX INT64_MAX
#define of32_MIN (-MAX)
#define of32_MAX MAX
#define of64_MIN (-DBL_MAX)
#define of64_MAX DBL_MAX
#define obyte_MIN 0
#define obyte_MAX UINT8_MAX
#define osize_MIN INT64_MIN
#define osize_MAX INT64_MAX


//
// printf format specifier fot the primitives
//

#define ou8_PRI PRIu8
#define oi8_PRI PRId8
#define ou16_PRI PRIu16
#define oi16_PRI PRId16
#define ou32_PRI PRIu32
#define oi32_PRI PRId32
#define ou64_PRI PRIu64
#define oi64_PRI PRId64
#define of32_PRI "g"
#define of64_PRI "g"
#define obyte_PRI PRIu8
#define osize_PRI PRId64


//
// additional constants for floating point numbers
//

/**
 * smallest possible value near to 0, that is not 0
 * @note unlike the C standard, of32_MIN is defined as (-MAX) to be the minimal possible value (biggest negative)
 */
#define of32_SMALL MIN

/** smallest positive value change */
#define of32_EPS FLT_EPSILON

/**
 * smallest possible value near to 0, that is not 0
 * @note unlike the C standard, of64_MIN is defined as (-DBL_MAX) to be the minimal possible value (biggest negative)
 */
#define of64_SMALL DBL_MIN

/** smallest positive value change */
#define of64_EPS DBL_EPSILON

//
// windows, android n other stuff
//

#ifdef MIA_PLATFORM_MSVC
#define _Thread_local __declspec(thread)
#endif

#ifdef __cplusplus
#define restrict __restrict
#endif

#ifdef MIA_PLATFORM_ANDROID
#define O__ANDROID_CONCAT2(prefix, class, function)       Java_ ## prefix ## _ ## class ## _ ## function
#define O__ANDROID_CONCAT1(prefix, class, function)       O__ANDROID_CONCAT2(prefix, class, function)
#define O_ANDROID_INTERFACE(function)                     O__ANDROID_CONCAT1(MIA_ANDROID_PACKAGE, Main, function)
#endif


//
// some general purpose functions
//

/**
 * @param list null terminated pointer list ((void) **), NULL safe
 * @return the number of elements of the null terminated list
 */
O_INLINE
osize o_list_num_void(void **list)
{
    osize num = 0;
    while (list && *list++) {
        num++;
    }
    return num;
}

/**
 * @param list null terminated pointer list ((void) **), NULL safe
 * @return the number of elements the null terminated list
 */
#define o_list_num(list) o_list_num_void((void **) list)

/**
 * @return a null terminated list of type as compound literal
 */
#define o_list_compound(type, ...) ((type[]) {__VA_ARGS__, NULL})

/**
 * Returns the length of the string like strlen, but NULL checked
 * @param string null terminated, or NULL (returns 0)
 * @return length of the string or 0 if string is NULL
 */
O_INLINE
osize o_strlen(const char *string)
{
    if (!string) {
        return 0;
    }
    return (osize) strlen(string);
}

/**
 * Simple wrapper for the standard o_memcpy.
 * A lot of stupid coding mistakes come from using
 *   o_memcpy and using n as number of elements instead of the byte size.
 * Use this wrapper to avoid that.
 * dst and src are nit allowed to alias
 * @param dst (restrict) to copy into
 * @param src (restrict) to copy from
 * @param element_size for a single element
 * @param count no. of elements
 * @return dst
 * @threadsafe
 * @note want to move aliasing buffers? use o_memmove instead
 */
O_INLINE
void *o_memcpy(void * restrict dst, const void * restrict src, osize element_size, osize count)
{
    osize n = element_size * count;
    assert(n>=0 && "o_memcpy failed, overflow?");
    return memcpy(dst, src, n);
}
  
      
/**
 * Simple wrapper for the standard o_memmove.
 * A lot of stupid coding mistakes come from using
 *   o_memcpy and using n as number of elements instead of the byte size.
 * Use this wrapper to avoid that.
 * @param dst (restrict) to copy into
 * @param src (restrict) to copy from
 * @param element_size for a single element
 * @param count no. of elements
 * @return dst
 * @threadsafe
 */
O_INLINE
void *o_memmove(void *dst, const void *src, osize element_size, osize count)
{
    osize n = element_size * count;
    assert(n>=0 && "o_memmove failed, overflow?");
    return memmove(dst, src, n);
}
  
    

/**
 * Simple wrapper for the standard o_clea with 0.
 * A lot of stupid coding mistakes come from using
 *   o_memcpy and using n as number of elements instead of the byte size.
 * Use this wrapper to avoid that.
 * @param dst (restrict) to be cleared with \0
 * @param element_size for a single element
 * @param count no. of elements
 * @return dst
 * @threadsafe
 */
O_INLINE
void *o_clear(void * restrict dst, osize element_size, osize count)
{
    osize n = element_size * count;
    assert(n>=0 && "o_clea failed, overflow?");
    return memset(dst, 0, n);
}

//
// init, exit and assumptions
//



/**
 * Must be called at the startup of the app.
 * May be called from a_init, in the "a" module
 * @note calls SDL_init
 */
O_EXTERN
void o_init(void);

/** @return true if o_init was called */
O_EXTERN
bool o_init_called(void);

/** asserts that o_init was called before */
#define o_init_assert() assert(o_init_called())

/**
 * Exits the app on failure with a formatted text message
 * calls exit(EXIT_FAILURE)
 */
#define o_exit(...) o__exit_impl (__FILE__, __LINE__, __VA_ARGS__)

O_EXTERN
void o__exit_impl(const char *file, int line, const char *reason_format, ...);

/**
 * Assumptions are runtime asserts.
 * On failure, o_exit is called
 */
#define o_assume(EX, ...) \
(void)((EX) || (o__assume_impl (#EX, __FILE__, __LINE__, __VA_ARGS__),0))

O_EXTERN
void o__assume_impl(const char *expression, const char *file, int line, const char *reason_format, ...);


//
// Thread id stuff
//

/**
 * @return the id of the current thread
 * @note may return 0 if threads are not supported
 */
O_EXTERN
ou64 o_thread_id(void);

/**
 * @return the id of the main thread
 * @note may return 0 if threads are not supported
 */
O_EXTERN
ou64 o_thread_main_id(void);


/**
 * Asserts the the current thread is the main thread.
 * Useful for library functions, etc. that must be called from the main thread
 * */
#define o_thread_assert_main() assert(o_thread_id() == o_thread_main_id())


/** priorities for o_thread_set_priority */
enum OThread_priority {
    OThread_PRIORITY_LOW,
    OThread_PRIORITY_NORMAL,
    OThread_PRIORITY_HIGH,
    OThread_PRIORITY_TIME_CRITICAL
};

/**
 * Sets the priority of the current thread
 *
 * @note that some platforms will not let you alter the priority (or at least,
 * promote the thread to a higher priority) at all, and some require you to be
 * an administrator account. Be prepared for this to fail.
 *
 * @param priority the new priority for the current thread
 * @return false if failed
 */
O_EXTERN
bool o_thread_set_priority(enum OThread_priority priority);

//
// timings
//

/**
 * Sleeps the current thread
 * @param time_ms time to sleep in millis, <= 0 safe -> yields with SDL_Delay(0)
 * @note Calls SDL_Delay(time_ms)
 */
O_EXTERN
void o_sleep(osize millis);


/**
 * Yields the execution to other threads
 * @note Just calls o_sleep(0)
 */
O_INLINE
void o_yield(void)
{
    o_sleep(0);
}

//
// (predictable) random number generator
//

/* o_rand will NOT include this value [0 : o_rand_MAX) */
#define o_rand_MAX 0xefffffff

/**
 * Pseudo random number generator with explicit implementation (not rand from libc)
 * @return a value in the range [0 : o_rand_MAX)
 * @note each thread has its own state
 * @threadsafe
 */
O_EXTERN
ou32 o_rand(void);

/**
 * Seed the pseudo random number generator o_rand
 * @note each thread has its own state
 *       called with current time on o_init and in a new thread instance of OThread to seed it
 * @threadsafe
 */
O_EXTERN
void o_rand_seed(ou32 seed);



#endif //O_COMMON_H
