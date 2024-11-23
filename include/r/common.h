#ifndef R_COMMON_H
#define R_COMMON_H

/**
 * @file common.h
 *
 * Common used | Basic includes, macros, types and functions
 */


#include "o/common.h"
#include "m/types/byte.h"
#include "m/types/int.h"
#include "m/types/flt.h"

// GLES and WebGL use GLES3.0
#ifdef MIA_OPTION_GLES
#define R_GL_VERSION_MAJOR 3
#define R_GL_VERSION_MINOR 0

// GL uses GL3.3
#else
#define R_GL_VERSION_MAJOR 3
#define R_GL_VERSION_MINOR 3
#endif

//
// some useful default colors
//

static const vec4 R_TRANSPARENT = {{0, 0, 0, 0}};
static const vec4 R_BLACK = {{0, 0, 0, 1}};
static const vec4 R_WHITE = {{1, 1, 1, 1}};
static const vec4 R_RED = {{1, 0, 0, 1}};
static const vec4 R_GREEN = {{0, 1, 0, 1}};
static const vec4 R_BLUE = {{0, 0, 1, 1}};
static const vec4 R_YELLOW = {{1, 1, 0, 1}};
static const vec4 R_CYAN = {{0, 1, 1, 1}};
static const vec4 R_MAGENTA = {{1, 0, 1, 1}};
static const vec4 R_GRAY = {{0.5, 0.5, 0.5, 1}};

#define R_GRAY_X(val) vec4_((val),(val),(val),1)

#define R_HUE_RED 0.0
#define R_HUE_GREEN (1.0/3.0)
#define R_HUE_BLUE (1.0/3.0)
#define R_HUE_YELLOW (0.5/3.0)
#define R_HUE_CYAN (1.5/3.0)
#define R_HUE_MAGENTA (2.5/3.0)



/**
 * Should be called with an active OpenGL context
 * @param opt_root a root that will NOT be deleted or NULL to create a heap root internally
 */
O_EXTERN
void r_init(oobj opt_root);


/**
 * @return A default allocator for the "r" module.
 * Allocates stuff like default RShader's in RObj's.
 */
O_EXTERN
oobj r_root(void);


/**
 * @return The size of the current back output buffer
 */
O_EXTERN
ivec2 r_back_size_int(void);

/**
 * @return The size of the current back output buffer
 * @note integer values
 */
O_INLINE
vec2 r_back_size(void)
{
    ivec2 size = r_back_size_int();
    return vec2_(size.x, size.y);
}

/**
 * @return the default back projection to edit
 */
O_EXTERN
struct r_proj *r_back_proj(void);

/**
 * @return maximal possible size for a texture (RTex) to use (device specific...)
 * @note cols==rows
 */
O_EXTERN
ivec2 r_max_tex_size(void);

//
// lazy loaded default textures
//


//
// less needed stuff
//


/**
 * Starts a frame and updates the back size
 * @note The viewport to draw into the output frame buffer will be set to the next even size.
 * @note RConfig cached
 */
O_EXTERN
void r_frame_begin(int back_cols, int window_rows);


/**
 * Checks for OpenGL errors
 */
O_EXTERN
bool r_error_check_call(const char *file, int line, const char *tag);

/**
 * Checks for OpenGL errors, if debug mode and MIA_OPTION_GL_CHECK
 */
#if !defined(NDEBUG) && defined(MIA_OPTION_GL_CHECK)
#define r_error_check(tag) r_error_check_call(__FILE__, __LINE__, (tag))
#else
#define r_error_check(tag) ((void)0)
#endif

#endif //R_COMMON_H
