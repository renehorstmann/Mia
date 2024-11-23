#ifndef R_RTEX_H
#define R_RTEX_H

/**
 * @file RTex.h
 *
 * Object
 *
 * Texture buffer allocated on the GPU.
 * Some functions work with a NULL passed as object to work on the back buffer
 */


#include "o/OObj.h"
#include "r/common.h"
#include "r/proj.h"
#include "r/format.h"
#include "r/quad.h"
#include "r/rect.h"

/** object id */
#define RTex_ID OObj_ID "RTex"


enum RTex_wrap_modes
{
    RTex_wrap_CLAMP,
    RTex_wrap_REPEAT,
    RTex_wrap_NUM_MODES
};

enum RTex_filter_modes
{
    RTex_filter_NEAREST,
    RTex_filter_LINEAR,
    RTex_filter_NUM_MODES
};

struct RTex_fbo
{
    ou32 gl_fbo;
    struct r_proj proj;
};

typedef struct
{
    OObj super;

    // may be 0 for an invalid tex (for example cols | rows = 0)
    ou32 gl_tex;

    // const, create a new tex to change
    ivec2 size;

    // as lbwh, defaults to ivec4_(0, 0, size.x, size.y)
    ivec4 viewport;

    // const, create a new tex to change
    enum r_format format;

    enum RTex_wrap_modes wrap_mode;
    enum RTex_filter_modes filter_mode;

    // OPtr of optional RTex attachment for layout=1 (automatically second out vec4...)
    oobj attached_tex_ptr;

    // lazy init
    struct RTex_fbo* fbo;
} RTex;


/**
 * Initializes the object
 * @param obj RTex object
 * @param parent to inherit from
 * @param opt_buffer set the tex pixels, must be the size of RTex_buffer_size(format)
 * @param cols, rows columns and rows to allocate, if <=0 RTex will be invalid (RTex_valid() == false; RTex_tex()==0).
 * @param internal_format for the internal pixels.
 * @param format for the opt_buffer pixels, if not NULL.
 * @return obj casted as RTex
 */
O_EXTERN
RTex* RTex_init_ex(oobj obj, oobj parent, const void* opt_buffer, int cols, int rows,
                   enum r_format internal_format, enum r_format format);

/**
 * Creates a new the RTex object
 * @param parent to inherit from
 * @param opt_buffer set the tex pixels, must be the size of RTex_buffer_size(format)
 * @param cols, rows columns and rows to allocate, if <=0 RTex will be invalid (RTex_valid() == false; RTex_tex()==0).
 * @param internal_format for the internal pixels.
 * @param format for the opt_buffer pixels, if not NULL.
 * @return The new object
 */
O_INLINE
RTex* RTex_new_ex(oobj parent, const void* opt_buffer, int cols, int rows,
                  enum r_format internal_format, enum r_format format)
{
    OObj_DECL_IMPL_NEW_SPECIAL(RTex, ex, parent, opt_buffer, cols, rows, internal_format, format);
}

/**
 * Creates a new the RTex object.
 * @param parent to inherit from
 * @param opt_buffer set the tex pixels, must be the size of RTex_buffer_size(R_FORMAT_RGBA_8)
 * @param cols, rows columns and rows to allocate, if <=0 RTex will be invalid (RTex_valid() == false; RTex_tex()==0).
 * @return The new object.
 * @note R_FORMAT_RGBA_8 is used internally.
 */
O_INLINE
RTex* RTex_new(oobj parent, const void* opt_buffer, int cols, int rows)
{
    return RTex_new_ex(parent, opt_buffer, cols, rows, R_FORMAT_RGBA_8, R_FORMAT_RGBA_8);
}

/**
 * Creates a new the RTex object.
 * Loads an image with o_img for the tex buffer.
 * @param parent to inherit from
 * @param file to load the tex from, either png or jpg, loaded with SDL_image
 * @return The new object.
 * @note R_FORMAT_RGBA_8 is used internally.
 *       May be invalid if file failed to read, test with RTex_valid()
 */
O_EXTERN
RTex* RTex_new_file(oobj parent, const char* file);

/**
 * Creates a new the RTex object.
 * Blits the current back buffer into the new RTex.
 * @param parent to inherit from
 * @return The new object
 * @note R_FORMAT_RGBA_8 is used internally.
 *       calls RTex_blit_back internally
 */
O_EXTERN
RTex* RTex_new_blit_back(oobj parent);

/**
 * Creates a kernel tex.
 * @param parent to inherit from
 * @param cols, rows to allocate
 * @param set for all values
 * @param plus if true, edges cleared to 0.0
 * @param normalize if >0.0: normalize to given
 * @return The new object
 * @note R_FORMAT_R_32F is used internally
 */
O_EXTERN
RTex *RTex_new_kernel(oobj parent, int cols, int rows, float set, bool plus, float normalize);

/**
 * Creates a kernel tex with a gauss 2d bell kurve.
 * @param parent to inherit from
 * @param cols, rows to allocate
 * @param sigma gauss factor for x and y axis, pass <=0 to auto scale (vec2_(-1))
 * @param normalize, if <=0: center is 1.0
 *                     else: normalized to given value (... 1.0f)
 * @return The new object
 * @note R_FORMAT_R_32F is used internally
 */
O_EXTERN
RTex *RTex_new_kernel_gauss(oobj parent, int cols, int rows, vec2 sigma, float normalize);


//
// virtual implementations:
//

/**
 * Default deletor that deletes the internal tex
 * @param obj RTex object
 */
O_EXTERN
void RTex__v_del(oobj obj);


//
// object functions:
//

/**
 * @param obj RTex object
 * @return the opengl tex handle, if NULL: 0
 */
O_INLINE
ou32 RTex_tex(oobj obj)
{
    if (!obj) {
        return 0;
    }
    OObj_assert(obj, RTex);
    RTex* self = obj;
    return self->gl_tex;
}

/**
 * @param obj RTex object
 * @return true if valid
 */
O_INLINE
bool RTex_valid(oobj obj)
{
    OObj_assert(obj, RTex);
    RTex* self = obj;
    return self->gl_tex!=0;
}


/**
 * @param obj RTex object
 * @return current internal pixel format
 */
OObj_DECL_GET(RTex, enum r_format, format)

/**
 * @param obj RTex object, if NULL: r_back_size is returned
 * @return size of the allocated framebuffer
 */
O_INLINE
ivec2 RTex_size_int(oobj obj)
{
    if (!obj) {
        return r_back_size_int();
    }
    OObj_assert(obj, RTex);
    RTex* self = obj;
    return self->size;
}

/**
 * @param obj RTex object, if NULL: back buffer
 * @return viewport lbwh of the allocated framebuffer
 */
O_EXTERN
ivec4 RTex_viewport(oobj obj);

/**
 * @param obj RTex object, if NULL: back buffer
 * @return viewport lbwh of the allocated framebuffer
 */
O_EXTERN
ivec4 RTex_viewport_set(oobj obj, ivec4 set);

/**
 * @param obj RTex object, if NULL: back buffer
 * @return viewport for the full tex: vec4_(0, 0, m_2(RTex_size_int(obj))
 */
O_INLINE
ivec4 RTex_viewport_set_full(oobj obj)
{
    ivec4 viewport = ivec4_(0);
    viewport.zw = RTex_size_int(obj);
    return RTex_viewport_set(obj, viewport);
}

/**
 * @param obj RTex object, if NULL: back buffer
 * @param pose_aa axis aligned pose in projection space
 * @return viewport lbwh of the allocated framebuffer
 */
O_EXTERN
ivec4 RTex_viewport_set_pose_aa(oobj obj, mat4 pose_aa);

/**
 * @param obj RTex object, if NULL: back buffer
 * @param rect as center_x, center_y, w, h
 * @return viewport lbwh of the allocated framebuffer
 */
O_EXTERN
ivec4 RTex_viewport_set_rect(oobj obj, vec4 rect);

/**
 * @param obj RTex object, if NULL: r_back_size is returned
 * @return size of the allocated framebuffer
 * @note integer values
 */
O_INLINE
vec2 RTex_size(oobj obj)
{
    ivec2 size = RTex_size_int(obj);
    return vec2_(size.x, size.y);
}

/**
 * @param obj RTex object, if NULL: r_back_size is returned
 * @return number of pixels (size.x * size.y)
 */
O_INLINE
osize RTex_num(oobj obj)
{
    ivec2 size = RTex_size_int(obj);
    return size.x * size.y;
}

/**
 * @param obj RTex object
 * @return bytes needed to get or set the buffer (RTex_num * r_format_size)
 */
O_INLINE
osize RTex_buffer_size(oobj obj, enum r_format format)
{
    OObj_assert(obj, RTex); // to assert obj not NULL
    RTex* self = obj;
    return RTex_num(obj) * r_format_size(format);
}

/**
 * @param obj RTex object
 * @return the current wrap mode
 */
O_INLINE
enum RTex_wrap_modes RTex_wrap(oobj obj)
{
    OObj_assert(obj, RTex);
    RTex* self = obj;
    return self->wrap_mode;
}

/**
 * Updates the wrap mode, if texture is sampled outside its borders through uv mapping
 * @param obj RTex object
 * @param mode CLAMP to border; REPEAT texture
 */
O_EXTERN
void RTex_wrap_set(oobj obj, enum RTex_wrap_modes mode);

/**
 * @param obj RTex object
 * @return the current filter mode
 */
O_INLINE
enum RTex_filter_modes RTex_filter(oobj obj)
{
    OObj_assert(obj, RTex);
    RTex* self = obj;
    return self->filter_mode;
}

/**
 * Updates the filter mode
 * @param obj RTex object
 * @param mode NEAREST: pixel art, LINEAR: else
 */
O_EXTERN
void RTex_filter_set(oobj obj, enum RTex_filter_modes mode);


/**
 * @param obj RTex object, if NULL r_back_size is used internally
 * @return The left border for uv matrices
 * @note mostly valid for render with, not render on. see r_proj_left, etc.
 */
O_INLINE
float RTex_left(oobj obj)
{
    return -RTex_size(obj).x / 2.0f;
}

/**
 * @param obj RTex object, if NULL r_back_size is used internally
 * @return The right border for uv matrices
 * @note mostly valid for render with, not render on. see r_proj_left, etc.
 */
O_INLINE
float RTex_right(oobj obj)
{
    return +RTex_size(obj).x / 2.0f;
}

/**
 * @param obj RTex object, if NULL r_back_size is used internally
 * @return The bottom border for uv matrices
 * @note mostly valid for render with, not render on. see r_proj_left, etc.
 */
O_INLINE
float RTex_bottom(oobj obj)
{
    return -RTex_size(obj).x / 2.0f;
}

/**
 * @param obj RTex object, if NULL r_back_size is used internally
 * @return The top border for uv matrices
 * @note mostly valid for render with, not render on. see r_proj_left, etc.
 */
O_INLINE
float RTex_top(oobj obj)
{
    return +RTex_size(obj).y / 2.0f;
}


/**
 * @param obj RTex object, if NULL r_back_size is used internally
 * @return left, right, bottom, top
 * @note mostly valid for render with, not render on. see r_proj_left, etc.
 */
O_INLINE
vec4 RTex_bounds(oobj obj)
{
    return vec4_(RTex_left(obj),
                 RTex_right(obj),
                 RTex_bottom(obj),
                 RTex_top(obj));
}


/**
 * Creates a pose containing the full tex for rendering with quads, etc. (also for uvs...)
 * @param obj RTex object, if NULL r_back_size is used internally
 * @param x position
 * @param y position
 * @return pose containing the full tex on position xy, also usable for uv's
 * @note mostly valid for render with, not render on. see r_proj_left, etc.
 */
O_INLINE
mat4 RTex_pose(oobj obj, float x, float y)
{
    vec2 size = RTex_size(obj);
    return (mat4){
        {
            size.x / 2.0f, 0, 0, 0,
            0, size.y / 2.0f, 0, 0,
            0, 0, 1, 0,
            x, y, 0, 1
        }
    };
}


/**
 * Creates a rect containing the full tex for rendering with quads, etc. (also for uvs...)
 * @param obj RTex object, if NULL r_back_size is used internally
 * @param x position
 * @param y position
 * @return rext containing the full tex on position xy, also usable for uv's
 * @note mostly valid for render with, not render on. see r_proj_left, etc.
 */
O_INLINE
vec4 RTex_rect(oobj obj, float x, float y)
{
    vec2 size = RTex_size(obj);
    return (vec4){{ x, y, size.x, size.y }};
}


/**
 * Create a new RCam (pixelperfect; min_unit_radius = o_min(size.x, size.y)/2.0f
 * @param obj RTex object
 */
O_EXTERN
oobj RTex_cam(oobj obj);


/**
 * Returns the tex scaling used by render objects
 * @param obj RTex object
 * @return 1 / tex_size.
 */
O_INLINE
vec2 RTex_get_tex_scale(oobj obj)
{
    vec2 size = RTex_size(obj);
    return vec2_(1.0f / size.x, 1.0f / size.y);
}


//
// fbo stuff
//

/**
 * Use this RTex as active framebuffer (called by shaders)
 * @param obj RTex object
 * @param requested_draw_buffers from RProgram_num_draw_buffers.
 *                               the number of "out vec4 f_rgba" in fragment shader (typically 1)
 */
O_EXTERN
void RTex_use(oobj obj, int requested_draw_buffers);


/**
 * Deletes the internal fbo, proj, etc. if available.
 * Could be called after editing this texture on setup.
 * @param obj RTex object
 */
O_EXTERN
void RTex_use_done(oobj obj);

/**
 * Retrieves the current buffer.
 * @param obj RTex object
 * @param out_buffer a buffer of (at least) RTex_buffer_size
 * @param format for the out_buffer pixels
 * @note internal and given format may be different
 */
O_EXTERN
void RTex_get_ex(oobj obj, void* out_buffer, enum r_format format);

/**
 * Retrieves the current buffer as R_FORMAT_RGBA_8.
 * @param obj RTex object
 * @param out_buffer a buffer of (at least) RTex_buffer_size
 * @note internal and given format may be different
 */
O_INLINE
void RTex_get(oobj obj, void* out_buffer)
{
    RTex_get_ex(obj, out_buffer, R_FORMAT_RGBA_8);
}

/**
 * Sets the buffer
 * @param obj RTex object
 * @param buffer a buffer of (at least) RTex_buffer_size size
 * @param format for the buffer pixels
 * @note internal and given format may be different
 */
O_EXTERN
void RTex_set_ex(oobj obj, const void* buffer, enum r_format format);

/**
 * Sets the buffer as R_FORMAT_RGBA_8
 * @param obj RTex object
 * @param buffer a buffer of (at least) RTex_buffer_size
 * @note internal and given format may be different
 */
O_INLINE
void RTex_set(oobj obj, const void* buffer)
{
    RTex_set_ex(obj, buffer, R_FORMAT_RGBA_8);
}

/**
 * Save the tex image as .png file (rgba 8bit)
 * @param obj RTex object
 * @param file png file to write into
 * @return false if failed
 * @note uses R_FORMAT_RGBA_8 to save
 */
O_EXTERN
bool RTex_write_file(oobj obj, const char* file);

/**
 * @param obj RTex object
 * @return a reference the default render projection, defaults to a minradius -> min units cam
 */
O_EXTERN
struct r_proj* RTex_proj(oobj obj);


/**
 * @param obj RTex object
 * @return the attached RTex object or NULL
 */
O_EXTERN
struct oobj_opt RTex_attach(oobj obj);


/**
 * Sets opt_tex_attachment as second color attachment for rendering (or disable it, if NULL is passed)
 * @param obj RTex object
 * @param opt_tex_attachment RTex object to attach onto obj, or NULL to disable
 */
O_EXTERN
oobj RTex_attach_set(oobj obj, oobj opt_tex_attachment);



//
// render stuff
//


/**
 * @param obj RTex object, if NULL, window buffer is used instead
 * @param clear_color to set as draw color clearing the full texture
 * @note ignores the viewport
 */
O_EXTERN
void RTex_clear_full(oobj obj, vec4 clear_color);


/**
 * @param obj RTex object, if NULL, window buffer is used instead
 * @param clear_color to set as draw color, draws within the viewport
 */
O_EXTERN
void RTex_clear(oobj obj, vec4 clear_color);

/**
 * Calls the virtual render function of the given RShader.
 * @param obj RTex object
 * @param shader RShader object
 * @param buffer RBuffer object
 * @param opt_num_rendered if <=0: RBuffer_num is used, else o_min(num_rendered, 1, RBuffer_num(buffer))
 * @param opt_proj the camera projection to use (vp), if NULL RTex_proj(tex) is used
 * @note calls RShader_render_ex internally
 */
O_EXTERN
void RTex_shader_ex(oobj obj, oobj shader, oobj buffer, int opt_num_rendered, const struct r_proj *opt_proj);

/**
 * Calls the virtual render function of the given RShader.
 * Simpler default version.
 * @param obj RTex object
 * @param shader RShader object
 * @param buffer RBuffer object
 * @note just calls RTex_shader_ex(obj, buffer, tex, 0, NULL)
 */
O_INLINE
void RTex_shader(oobj obj, oobj shader, oobj buffer)
{
    RTex_shader_ex(obj, shader, buffer, 0, NULL);
}

/**
 * Calls the virtual render function of the given RObj.
 * Extended version.
 * Renders the object to the bound frame buffer.
 * Uses all shaders in the shader_pipeline one by one.
 * @param obj RTex object
 * @param ro RObj object
 * @param opt_proj the camera projection to use (vp), if NULL: RTex_proj(tex) is used instead
 * @param update if true: calls RObj_update first
 * @note calls RObj_render_ex internally
 */
O_EXTERN
void RTex_ro_ex(oobj obj, oobj ro, const struct r_proj *opt_proj, bool update);

/**
 * Calls the virtual render function of the given RObj.
 * Simpler default version.
 * Renders the object to the bound frame buffer
 * @param obj RTex object
 * @param ro RObj object
 * @note just calls RTex_ro_ex(obj, ro, NULL, true)
 */
O_INLINE
void RTex_ro(oobj obj, oobj ro)
{
    RTex_ro_ex(obj, ro, NULL, true);
}

/**
 * Renders a bunch of quads on this RTex using the given RShader.
 * @param obj RTex object
 * @param shader RShader object
 */
O_EXTERN
void RTex_quads(oobj obj, oobj shader, const struct r_quad *quads, int n);

/**
 * Renders a bunch of rects on this RTex using the given RShader.
 * @param obj RTex object
 * @param shader RShader object
 */
O_EXTERN
void RTex_rects(oobj obj, oobj shader, const struct r_rect *rects, int n);

/**
 * Overwrites (blend off) the tex onto obj
 * @param obj RTex object
 */
O_EXTERN
void RTex_blit_ex(oobj obj, oobj tex, mat4 pose, mat4 uv);

/**
 * Overwrites (blend off) the tex onto obj
 * @param obj RTex object
 * @note calls RTex_blit_ex internally
 */
O_INLINE
void RTex_blit(oobj obj, oobj tex, float x, float y)
{
    mat4 pose = RTex_pose(tex, x, y);
    mat4 uv = RTex_pose(tex, 0, 0);
    RTex_blit_ex(obj, tex, pose, uv);
}

/**
 * Overwrites (blend off) the colored tex onto obj
 * @param obj RTex object
 * @param rgba * color
 * @param hsva + color
 */
O_EXTERN
void RTex_blit_color_ex(oobj obj, oobj tex, mat4 pose, mat4 uv, vec4 rgba, vec4 hsva);

/**
 * Overwrites (blend off) the tex onto obj
 * @param obj RTex object
 * @param rgba * color
 * @param hsva + color
 * @note calls RTex_blit_ex internally
 */
O_INLINE
void RTex_blit_color(oobj obj, oobj tex, float x, float y, vec4 rgba, vec4 hsva)
{
    mat4 pose = RTex_pose(tex, x, y);
    mat4 uv = RTex_pose(tex, 0, 0);
    RTex_blit_color_ex(obj, tex, pose, uv, rgba, hsva);
}

/**
 * Blits the current back buffer into obj.
 * @param obj RTex object
 * @param viewport_obj lbwh in pixels to write to; ivec4_(-1) for full
 * @param viewport_back lbwh in pixels to read from; ivec4_(-1) for full
 */
O_EXTERN
void RTex_blit_back_ex(oobj obj, ivec4 viewport_obj, ivec4 viewport_back, enum RTex_filter_modes filter);

/**
 * Blits the current back buffer into obj.
 * @param obj RTex object
 */
O_INLINE
void RTex_blit_back(oobj obj, enum RTex_filter_modes filter)
{
    RTex_blit_back_ex(obj, ivec4_(-1), ivec4_(-1), filter);
}

/**
 * Blends on tex onto obj
 * @param obj RTex object
 */
O_EXTERN
void RTex_blend_ex(oobj obj, oobj tex, mat4 pose, mat4 uv);

/**
 * Blends on tex onto obj
 * @param obj RTex object
 * @note calls RTex_blend_ex internally
 */
O_INLINE
void RTex_blend(oobj obj, oobj tex, float x, float y)
{
    mat4 pose = RTex_pose(tex, x, y);
    mat4 uv = RTex_pose(tex, 0, 0);
    RTex_blend_ex(obj, tex, pose, uv);
}

/**
 * Blends on tex onto obj
 * @param obj RTex object
 * @param rgba * color
 * @param hsva + color
 */
O_EXTERN
void RTex_blend_color_ex(oobj obj, oobj tex, mat4 pose, mat4 uv, vec4 rgba, vec4 hsva);

/**
 * Blends on tex onto obj
 * @param obj RTex object
 * @param rgba * color
 * @param hsva + color
 * @note calls RTex_blend_ex internally
 */
O_INLINE
void RTex_blend_color(oobj obj, oobj tex, float x, float y, vec4 rgba, vec4 hsva)
{
    mat4 pose = RTex_pose(tex, x, y);
    mat4 uv = RTex_pose(tex, 0, 0);
    RTex_blend_color_ex(obj, tex, pose, uv, rgba, hsva);
}


//
// functions that generate RTex's (more in RTex_manip)
//


/**
 * @param obj RTex object
 * @param format for the resulting RTex
 * @return a new RTex allocated on obj
 * @note calls RTex_blit internally
 */
O_INLINE
RTex* RTex_cast(oobj obj, enum r_format format)
{
    RTex *res = RTex_new_ex(obj, NULL, m_2(RTex_size(obj)), format, format);
    RTex_blit(res, obj, 0, 0);
    return res;
}

/**
 * @param obj RTex object
 * @return a new RTex allocated on obj
 * @note calls RTex_cast internally
 */
O_INLINE
RTex* RTex_clone(oobj obj)
{
    return RTex_cast(obj, RTex_format(obj));
}

/**
 * Creates a new copy of the RTex obj, with the given border around it
 * @param obj RTex object
 * @param lrbt border size in left, right, bottom, top (e. g. ivec4_(8) for all to be 8 pixels wide)
 * @param color border color, for example R_TRANSPARENT, or R_BLACK
 * @param format for the resulting RTex
 * @return RTex allocated on obj
 */
O_EXTERN
RTex* RTex_border(oobj obj, ivec4 lrbt, vec4 color, enum r_format format);


#endif //R_RTEX_H
