#include "r/RProgram.h"
#include "o/OObj_builder.h"
#include "o/OMap.h"
#include "o/file.h"
#include "o/str.h"
#include "r/gl.h"

#define O_LOG_LIB "r"
#include "o/log.h"


O_STATIC
GLuint shader_stage_new(char **shader_codes, GLint shader_type, const char *source_tag_of_code)
{
    oobj container = OObj_new(r_root());

    osize sclen = o_list_num(shader_codes);

    if (sclen<=0 || o_strlen(shader_codes[0]) == 0) {
        return 0;
    }

    char *version;
#ifdef MIA_OPTION_GLES
    version = "#version 300 es\n";
#else
    version = "#version 330 core\n";
#endif

    char *type = NULL;
    if (shader_type == GL_VERTEX_SHADER) {
        type = "#define MIA_SHADER_VERTEX\n";
    } else if (shader_type == GL_FRAGMENT_SHADER) {
        type = "#define MIA_SHADER_FRAGMENT\n";
    }
    assert(type && "neither vertex nor fragment shader used");

    char *platform = "";
#ifdef MIA_PLATFORM_MINGW
    platform = "#define MIA_PLATFORM_MINGW\n"
               "#define MIA_PLATFORM_WINDOWS\n"
               "#define MIA_PLATFORM_DESKTOP\n";
#endif
#ifdef MIA_PLATFORM_MSVC
    platform = "#define MIA_PLATFORM_MSVC\n"
               "#define MIA_PLATFORM_WINDOWS\n"
               "#define MIA_PLATFORM_DESKTOP\n";
#endif
#ifdef MIA_PLATFORM_UNIX
    platform = "#define MIA_PLATFORM_UNIX\n"
               "#define MIA_PLATFORM_DESKTOP\n";
#endif
#ifdef MIA_PLATFORM_EMSCRIPTEN
    platform = "#define MIA_PLATFORM_EMSCRIPTEN\n";
#endif
#ifdef MIA_PLATFORM_CXXDROID
    platform = "#define MIA_PLATFORM_CXXDROID\n";
#endif

    char *option_gles = "";
#ifdef MIA_OPTION_GLES
    option_gles = "#define MIA_OPTION_GLES\n";
#endif

    char *precision = "";
    if (shader_type == GL_VERTEX_SHADER) {
        precision = "precision highp float;\n"
                    "precision highp int;\n";
    } else if (shader_type == GL_FRAGMENT_SHADER) {
        precision = "#ifdef GL_FRAGMENT_PRECISION_HIGH\n"
                    "precision highp float;\n"
                    "precision highp int;\n"
                    "#ifdef MIA_PLATFORM_EMSCRIPTEN\n"
                    "// chrome may break with highp, even on GL_FRAGMENT_PRECISION_HIGH\n"
                    "precision mediump sampler2D;\n"
                    "#else //MIA_PLATFORM_EMSCRIPTEN\n"
                    "precision highp sampler2D;\n"
                    "#endif//MIA_PLATFORM_EMSCRIPTEN\n"
                    "#else //GL_FRAGMENT_PRECISION_HIGH\n"
                    "precision mediump float;\n"
                    "precision mediump int;\n"
                    "precision mediump sampler2D;\n"
                    "#endif //GL_FRAGMENT_PRECISION_HIGH\n";
    }

    GLuint res = glCreateShader(shader_type);

    const int add_len = 5;
    char **shader = o_new(container, char *, sclen + add_len);
    shader[0] = version;
    shader[1] = type;
    shader[2] = platform;
    shader[3] = option_gles;
    shader[4] = precision;
    for(osize i=0; i<sclen; i++) {
        shader[add_len+i] = shader_codes[i];
    }
    glShaderSource(res, sclen + add_len, (const GLchar * const *) shader, NULL);
    glCompileShader(res);

    int status;
    glGetShaderiv(res, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        int log_len;
        glGetShaderiv(res, GL_INFO_LOG_LENGTH, &log_len);

        char *buffer = o_new(container, char, log_len + 1);
        glGetShaderInfoLog(res, log_len, NULL, buffer);
        o_log_error_s("RProgram_init", "Shader compile failure in %s shader of \"%s\": %s",
                      shader_type == GL_VERTEX_SHADER? "vertex" : "fragment",
                      source_tag_of_code,
                      buffer);
        glDeleteShader(res);

        if(o_log_level()<=O_LOG_TRACE) {
            char *full_program = o_str_join(container, shader, sclen+add_len, NULL);
            char **splits = o_str_split(container, full_program, "\n");
            char **it = splits;
            int line = 0;
            while(*it) {
                printf("<<%4i>>: %s\n", line++, *it++);
            }
            fflush(stdout);
        }

        res = 0;
    }

    o_del(container);
    return res;
}

O_STATIC
GLuint program_new(const GLuint *shaders, int n, const char *source_tag_of_code)
{

    // check each shader valid
    for (int i = 0; i < n; i++) {
        if (shaders[i] == 0) {
            return 0;
        }
    }

    // Combine shaders into program
    GLuint program = glCreateProgram();
    for (int i = 0; i < n; i++) {
        glAttachShader(program, shaders[i]);
    }
    glLinkProgram(program);

    int status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        int log_len;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_len);

        char *buffer = o_new(r_root(), char, log_len + 1);
        glGetProgramInfoLog(program, log_len, NULL, buffer);
        o_log_error_s("RProgram_init", "Shader linking failure of \"%s\": %s",
                      source_tag_of_code,
                      buffer);
        o_free(r_root(), buffer);

        glDeleteProgram(program);
        program = 0;
    }

    return program;
}

//
// public
//

RProgram *RProgram_init(oobj obj, oobj parent, char **shader_codes, int num_draw_buffers,
                        const char *source_tag_of_code, bool assume_valid)
{
    RProgram *self = obj;
    o_clear(self, sizeof *self, 1);

    OObj_init(self, parent);
    OObj_id_set(self, RProgram_ID);

    GLuint vertex = shader_stage_new(shader_codes, GL_VERTEX_SHADER, source_tag_of_code);
    GLuint fragment = shader_stage_new(shader_codes, GL_FRAGMENT_SHADER, source_tag_of_code);
    o_assume(!assume_valid || (vertex && fragment), "compilation failed");

    self->gl_program = program_new((GLuint[]) {vertex, fragment}, 2, source_tag_of_code);
    o_assume(!assume_valid || (self->gl_program), "linking failed");

    // safe to pass 0
    glDeleteShader(vertex);
    glDeleteShader(fragment);


    self->num_draw_buffers = num_draw_buffers;
    self->uniforms = OMap_new_string_keys(self, sizeof (oi32), 64);

    // deletor
    self->super.v_del = RProgram__v_del;

    return self;
}


RProgram *RProgram_new_file(oobj parent, const char *shader_file,
                            char **opt_shader_codes, int num_draw_buffers)
{
    oobj container = OObj_new(parent);
    struct oobj_opt data = o_file_read(container, shader_file, true, 1);
    char *shader = data.o ? OArray_data_void(data.o) : NULL;
    osize osc_len = o_list_num(opt_shader_codes);
    char **shader_codes = o_new(container, char *, osc_len+3);
    for(osize i=0; i<osc_len; i++) {
        shader_codes[i] = opt_shader_codes[i];
    }
    shader_codes[osc_len] = shader;
    shader_codes[osc_len+1] = o_strf(container,
                                     "// RProgram_init_file(..., \"%s\", ..., %d)\n",
                                     shader_file, num_draw_buffers);
    shader_codes[osc_len+2] = NULL;
    RProgram *res = RProgram_new(parent, shader_codes, num_draw_buffers, shader_file, true);
    o_del(container);
    return res;
}

void RProgram__v_del(oobj obj)
{
    OObj_assert(obj, RProgram);
    RProgram *self = obj;

    // safe to pass 0
    glDeleteProgram(self->gl_program);

    OObj__v_del(self);
}

ou32 RProgram_use(oobj obj)
{
    if (!obj) {
        glUseProgram(0);
        return 0;
    }
    OObj_assert(obj, RProgram);
    RProgram *self = obj;

    glUseProgram(self->gl_program);

    return self->gl_program;
}

#include "o/str.h"
oi32 RProgram_uniform(oobj obj, const char *name)
{
    if (!obj) {
        return -1;
    }
    OObj_assert(obj, RProgram);
    RProgram *self = obj;

    // cached?
    oi32 *cached_loc = OMap_get(self->uniforms, &name, oi32);
    if(cached_loc) {
        return *cached_loc;
    }

    // find location and set in the cache OMap
    oi32 loc = glGetUniformLocation(self->gl_program, name);
    OMap_set(self->uniforms, &name, &loc);

    return loc;
}


//
// uniform wrapper
//

void RProgram_uniform_tex(oobj obj, const char *name, int pos, ou32 tex)
{

    O_EXTERN
    ou32 RTex__bound_gl(void);
    ou32 bound = RTex__bound_gl();
    assert((bound==0 || bound != tex) && "tex already bound as framebuffer!");

    oi32 uniform = RProgram_uniform(obj, name);
    glActiveTexture(GL_TEXTURE0 + pos);
    glUniform1i(uniform, pos);
    glBindTexture(GL_TEXTURE_2D, tex);
}

void RProgram_uniform_tex_off(oobj program, int pos)
{
    glActiveTexture(GL_TEXTURE0 + pos);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void RProgram_uniform_mat4(oobj obj, const char *name, const mat4 *data, int num)
{
    oi32 uniform = RProgram_uniform(obj, name);
    glUniformMatrix4fv(uniform, num, GL_FALSE, (void *) data);
}

void RProgram_uniform_mat3(oobj obj, const char *name, const mat3 *data, int num)
{
    oi32 uniform = RProgram_uniform(obj, name);
    glUniformMatrix3fv(uniform, num, GL_FALSE, (void *) data);
}

void RProgram_uniform_mat2(oobj obj, const char *name, const mat2 *data, int num)
{
    oi32 uniform = RProgram_uniform(obj, name);
    glUniformMatrix2fv(uniform, num, GL_FALSE, (void *) data);
}

void RProgram_uniform_vec4(oobj obj, const char *name, const vec4 *data, int num)
{
    oi32 uniform = RProgram_uniform(obj, name);
    glUniform4fv(uniform, num, (void *) data);
}

void RProgram_uniform_vec3(oobj obj, const char *name, const vec3 *data, int num)
{
    oi32 uniform = RProgram_uniform(obj, name);
    glUniform3fv(uniform, num, (void *) data);
}

void RProgram_uniform_vec2(oobj obj, const char *name, const vec2 *data, int num)
{
    oi32 uniform = RProgram_uniform(obj, name);
    glUniform2fv(uniform, num, (void *) data);
}

void RProgram_uniform_float(oobj obj, const char *name, const float *data, int num)
{
    oi32 uniform = RProgram_uniform(obj, name);
    glUniform1fv(uniform, num, (void *) data);
}

void RProgram_uniform_ivec4(oobj obj, const char *name, const ivec4 *data, int num)
{
    oi32 uniform = RProgram_uniform(obj, name);
    glUniform4iv(uniform, num, (void *) data);
}

void RProgram_uniform_ivec3(oobj obj, const char *name, const ivec3 *data, int num)
{
    oi32 uniform = RProgram_uniform(obj, name);
    glUniform3iv(uniform, num, (void *) data);
}

void RProgram_uniform_ivec2(oobj obj, const char *name, const ivec2 *data, int num)
{
    oi32 uniform = RProgram_uniform(obj, name);
    glUniform2iv(uniform, num, (void *) data);
}

void RProgram_uniform_int(oobj obj, const char *name, const int *data, int num)
{
    oi32 uniform = RProgram_uniform(obj, name);
    glUniform1iv(uniform, num, (void *) data);
}
