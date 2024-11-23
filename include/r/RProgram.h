#ifndef R_RPROGRAM_H
#define R_RPROGRAM_H

/**
 * @file RProgram.h
 *
 * Object
 *
 * A shader program for render objects.
 * The program always uses a single .glsl file, containing all shader stages.
 * Currently supports only vertex and fragment shader stages. (WebGL2.0)
 * Each shader stage is within the #ifdef MIA_SHADER_* block:
 *
 * MIA_SHADER_VERTEX
 * MIA_SHADER_FRAGMENT
 *
 * MIA_PLATFORM_* is also set
 *
 * The shader program given should also miss the first "#version ..." line.
 * It will get automatically set to one of (if GLES or not...):
 * #version 300 es
 * #version 330 core
 *
 * some precision defaults are also set (to maximal possible, according to the platfrom
 */

#include "o/OObj.h"
#include "m/types/flt.h"
#include "m/types/int.h"

/** object id */
#define RProgram_ID OObj_ID "RProgram"

typedef struct {
    OObj super;

    ou32 gl_program;
    int num_draw_buffers;
    
    // OMap with string keys of the uniform names to cache the location
    oobj uniforms;

} RProgram;


/**
 * Initializes the object
 * @param obj RProgram object
 * @param parent to inherit from
 * @param shader_codes null terminated list of:
 *                     glsl code string to compile, containing both vertex and fragment shader.
 * @param num_draw_buffers the number of "out vec4 f_rgba" in fragment shader (typically 1)
 * @param source_tag_of_code a tag describing where the shader code comes from (for debugging...)
 * @param assume_valid if true, o_assume is called to validate a working program, else RProgram_program may return 0
 * @return obj casted as RProgram
 */
O_EXTERN
RProgram *RProgram_init(oobj obj, oobj parent, char **shader_codes, int num_draw_buffers,
                        const char *source_tag_of_code, bool assume_valid);

/**
 * Creates a new the RProgram object
 * @param parent to inherit from
 * @param shader_codes null terminated list of:
 *                     glsl code string to compile, containing both vertex and fragment shader.
 * @param num_draw_buffers the number of "out vec4 f_rgba" in fragment shader (typically 1)
 * @param source_tag_of_code a tag describing where the shader code comes from (for debugging...)
 * @param assume_valid if true, o_assume is called to validate a working program, else RProgram_program may return 0
 * @return The new object
 * @note
 */
O_INLINE
RProgram *RProgram_new(oobj parent, char **shader_codes, int num_draw_buffers,
                       const char *source_tag_of_code, bool assume_valid)
{
    OObj_DECL_IMPL_NEW(RProgram, parent, shader_codes, num_draw_buffers,
                       source_tag_of_code, assume_valid);
}

/**
 * Creates a new the RProgram object
 * @param parent to inherit from
 * @param shader_file file of glsl code to compile, containing both vertex and fragment shader.
 * @param opt_shader_codes optional (NULL safe) additional NULL terminated list of glsl code,
 *                         set before the file content
 * @param num_draw_buffers the number of "out vec4 f_rgba" in fragment shader (typically 1)
 * @return The new object
 * @note
 */
O_EXTERN
RProgram *RProgram_new_file(oobj parent, const char *shader_file,
                            char **opt_shader_codes, int num_draw_buffers);


//
// virtual implementations:
//

/**
 * calls dlDeleteProgram on the shader program
 * @param obj RProgram object
 */
O_EXTERN
void RProgram__v_del(oobj obj);


//
// object functions:
//

/**
 * @param obj RProgram object, NULL safe
 * @return The compiled shader program, or 0 if it failed to compile or obj==NULL
 */
O_INLINE
ou32 RProgram_program(oobj obj)
{
    if(!obj) {
        return 0;
    }
    OObj_assert(obj, RProgram);
    RProgram *self = obj;
    return self->gl_program;
}

/**
 * @param obj RProgram object, NULL safe
 * @return The number of "out vec4 f_rgba" in fragment shader (typically 1)
 */
O_INLINE
int RProgram_num_draw_buffers(oobj obj)
{
    if(!obj) {
        return 0;
    }
    OObj_assert(obj, RProgram);
    RProgram *self = obj;
    return self->num_draw_buffers;
}

/**
 * Sets this shader as the current program and activates all associated uniforms
 * @param obj RProgram object, NULL safe -> will set the gl program to 0
 * @return The compiled shader program, or 0 if it failed to compile or obj==NULL
 */
O_EXTERN
ou32 RProgram_use(oobj obj);

/**
 * @param obj RProgram object, NULL safe -> will return -1
 * @param name of the uniform to locate
 * @return the cached location of the uniform (cached in an OMap with string keys...)
 */
O_EXTERN
oi32 RProgram_uniform(oobj obj, const char *name);


//
// uniform wrapper
//

/**
 * Sets and activates a tex
 * @param obj RProgram object, NULL safe
 * @param name of the uniform to locate
 * @param pos enumeration number of "sampler2D" in the shader [0, 1, ...]
 * @param tex RTex_tex
 */
O_EXTERN
void RProgram_uniform_tex(oobj obj, const char *name, int pos, ou32 tex);

/**
 * unbinds a tex
 * @param obj RProgram object, NULL safe
 */
O_EXTERN
void RProgram_uniform_tex_off(oobj program, int pos);

/**
 * Sets uniform values.
 * @param obj RProgram object, NULL safe
 * @param name of the uniform to locate
 * @param data to be passed into the uniform, must be alive during the render call
 * @param num array length, typically 1
 */
O_EXTERN
void RProgram_uniform_mat4(oobj obj, const char *name, const mat4 *data, int num);

/**
 * Sets uniform values.
 * @param obj RProgram object, NULL safe
 * @param name of the uniform to locate
 * @param data to be passed into the uniform, must be alive during the render call
 * @param num array length, typically 1
 */
O_EXTERN
void RProgram_uniform_mat3(oobj obj, const char *name, const mat3 *data, int num);

/**
 * Sets uniform values.
 * @param obj RProgram object, NULL safe
 * @param name of the uniform to locate
 * @param data to be passed into the uniform, must be alive during the render call
 * @param num array length, typically 1
 */
O_EXTERN
void RProgram_uniform_mat2(oobj obj, const char *name, const mat2 *data, int num);

/**
 * Sets uniform values.
 * @param obj RProgram object, NULL safe
 * @param name of the uniform to locate
 * @param data to be passed into the uniform, must be alive during the render call
 * @param num array length, typically 1
 */
O_EXTERN
void RProgram_uniform_vec4(oobj obj, const char *name, const vec4 *data, int num);

/**
 * Sets uniform values.
 * @param obj RProgram object, NULL safe
 * @param name of the uniform to locate
 * @param data to be passed into the uniform, must be alive during the render call
 * @param num array length, typically 1
 */
O_EXTERN
void RProgram_uniform_vec3(oobj obj, const char *name, const vec3 *data, int num);

/**
 * Sets uniform values.
 * @param obj RProgram object, NULL safe
 * @param name of the uniform to locate
 * @param data to be passed into the uniform, must be alive during the render call
 * @param num array length, typically 1
 */
O_EXTERN
void RProgram_uniform_vec2(oobj obj, const char *name, const vec2 *data, int num);

/**
 * Sets uniform values.
 * @param obj RProgram object, NULL safe
 * @param name of the uniform to locate
 * @param data to be passed into the uniform, must be alive during the render call
 * @param num array length, typically 1
 */
O_EXTERN
void RProgram_uniform_float(oobj obj, const char *name, const float *data, int num);

/**
 * Sets uniform values.
 * @param obj RProgram object, NULL safe
 * @param name of the uniform to locate
 * @param data to be passed into the uniform, must be alive during the render call
 * @param num array length, typically 1
 */
O_EXTERN
void RProgram_uniform_ivec4(oobj obj, const char *name, const ivec4 *data, int num);

/**
 * Sets uniform values.
 * @param obj RProgram object, NULL safe
 * @param name of the uniform to locate
 * @param data to be passed into the uniform, must be alive during the render call
 * @param num array length, typically 1
 */
O_EXTERN
void RProgram_uniform_ivec3(oobj obj, const char *name, const ivec3 *data, int num);

/**
 * Sets uniform values.
 * @param obj RProgram object, NULL safe
 * @param name of the uniform to locate
 * @param data to be passed into the uniform, must be alive during the render call
 * @param num array length, typically 1
 */
O_EXTERN
void RProgram_uniform_ivec2(oobj obj, const char *name, const ivec2 *data, int num);

/**
 * Sets uniform values.
 * @param obj RProgram object, NULL safe
 * @param name of the uniform to locate
 * @param data to be passed into the uniform, must be alive during the render call
 * @param num array length, typically 1
 */
O_EXTERN
void RProgram_uniform_int(oobj obj, const char *name, const int *data, int num);

#endif // R_RPROGRAM_H
