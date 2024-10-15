#include "r/RTex.h"
#include "r/RTex_manip.h"
#include "o/OObj_builder.h"
#include "o/img.h"
#include "o/OPtr.h"
#include "m/byte.h"
#include "m/flt.h"
#include "m/int.h"
#include "r/RCam.h"
#include "r/gl.h"
#include "r/quad.h"
#include "r/RBufferQuad.h"
#include "r/RBufferRect.h"
#include "r/RShaderQuad.h"
#include "r/RShaderQuadChannelmerge.h"
#include "r/RShaderQuadKernel.h"
#include "r/RShaderQuadMerge.h"
#include "r/RObj.h"
#include "r/tex.h"

#define O_LOG_LIB "r"
#include "o/log.h"
#include "r/program.h"



static struct {
    oobj bound_tex;
    bool bound_valid;

    ou32 back_fbo;
    ivec4 back_viewport;

    oobj o;
    oobj b_quad;
    oobj b_rect;
    oobj s_quad;
    oobj s_quad_channelmerge;
    oobj s_quad_kernel;
    oobj s_quad_merge;
} RTex_L;

O_STATIC
GLenum format_channels(enum r_format format)
{
    switch(format) {
        case R_FORMAT_R_8:
        case R_FORMAT_R_32F:
            return GL_RED;
        case R_FORMAT_RGBA_8:
        case R_FORMAT_RGBA_32F:
            return GL_RGBA;
        default:
            break;
    }
    assert(0 && "invalid format");
    // should not get here...
    return 0;
}

O_STATIC
GLenum format_size(enum r_format format)
{
    switch(format) {
        case R_FORMAT_R_8:;
        case R_FORMAT_RGBA_8:
            return GL_UNSIGNED_BYTE;
        case R_FORMAT_R_32F:
        case R_FORMAT_RGBA_32F:
            return GL_FLOAT;
        default:
            break;
    }
    assert(0 && "invalid format");
    // should not get here...
    return 0;
}


O_STATIC
GLint format_internal(enum r_format format)
{
    switch(format) {
        case R_FORMAT_R_8:
            return GL_R8;
        case R_FORMAT_R_32F:
            return GL_R32F;
        case R_FORMAT_RGBA_8:
            return GL_RGBA8;
        case R_FORMAT_RGBA_32F:
            return GL_RGBA32F;
        default:
            break;
    }
    assert(0 && "invalid format");
    // should not get here...
    return 0;
}



O_STATIC
void update_fbo(RTex *self, int requested_num_draw_buffers) {
    if(!self->fbo) {
        // create a new fbo and proj
        self->fbo = o_new0(self, struct RTex_fbo, 1);
        self->fbo->proj = r_proj_new(RTex_size_int(self), vec2_(-1), true);
        glGenFramebuffers(1, &self->fbo->gl_fbo);
    }

    ou32 attachment1 = 0;
    RTex *attached_tex = OPtr_get(self->attached_tex_ptr).o;
    if(requested_num_draw_buffers == 2 && OObj_check(attached_tex, RTex)) {
        attachment1 = attached_tex->gl_tex;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, self->fbo->gl_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, self->gl_tex, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, attachment1, 0);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch (status) {
        case GL_FRAMEBUFFER_COMPLETE:
            break;
        case GL_FRAMEBUFFER_UNDEFINED:
            o_log_error_s("RTex fbo", "Framebuffer undefined");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            o_log_error_s("RTex fbo", "Framebuffer incomplete attachment");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            o_log_error_s("RTex fbo", "Framebuffer incomplete missing attachment");
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            o_log_error_s("RTex fbo", "Framebuffer unsupported");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE :
            o_log_error_s("RTex fbo", "Framebuffer incomplete multisample");
            break;
        case 0:
            o_log_error_s("RTex fbo", "Framebuffer failed to get status");
            r_error_check_call(__FILE__, __LINE__, "RTex fbo");
            break;
        default:
            o_log_error_s("RTex fbo", "Unknown framebuffer status");
            break;
    }

    if(OObj_check(OPtr_get(self->attached_tex_ptr).o, RTex)) {
        glDrawBuffers(2, (GLenum[]) {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1});
    } else {
        glDrawBuffers(1, (GLenum[]) {GL_COLOR_ATTACHMENT0});
    }
}


// protected
O_EXTERN
void RTex__init(void)
{
    // get the default back framebuffer
    GLint current_fbo;
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &current_fbo);
    RTex_L.back_fbo = (ou32) current_fbo;

    RTex_L.o = OObj_new(r_root());
    OObj_name_set(RTex_L.o, "RTex_render");

    RTex_L.b_quad = RBufferQuad_new(RTex_L.o);
    RTex_L.b_rect = RBufferRect_new(RTex_L.o);

    RTex_L.s_quad = RShaderQuad_new(RTex_L.o, NULL, false);
    RTex_L.s_quad_channelmerge = RShaderQuadChannelmerge_new(RTex_L.o, NULL, NULL, NULL, NULL, false);
    RTex_L.s_quad_kernel = RShaderQuadKernel_new(RTex_L.o, NULL, false, NULL, false);
    RTex_L.s_quad_merge = RShaderQuadMerge_new(RTex_L.o, NULL, false);


    r_error_check("init");
}

// protected
O_EXTERN
ou32 RTex__bound_gl(void)
{
    // NULL safe -> 0
    return RTex_tex(RTex_L.bound_tex);
}

//
// public
//


RTex *RTex_init_ex(oobj obj, oobj parent, const void *opt_buffer, int cols, int rows,
                   enum r_format internal_format, enum r_format format)
{
    // will also internally assert...
    assert(r_format_size(format) > 0);

    RTex *self = obj;
    o_clear(self, sizeof *self, 1);

    OObj_init(self, parent);
    OObj_id_set(self, RTex_ID);

    self->size = ivec2_(cols, rows);
    self->viewport = ivec4_(0, 0, cols, rows);
    self->format = internal_format;

    self->attached_tex_ptr = OPtr_new(self, NULL);

    // deletor
    self->super.v_del = RTex__v_del;

    if(cols <= 0 || rows <= 0) {
        o_log_warn_s(__func__, "invalid");
        self->gl_tex = 0;
        return self;
    }

    r_error_check("tex allocation start...");

    glGenTextures(1, &self->gl_tex);
    glBindTexture(GL_TEXTURE_2D, self->gl_tex);

    // default may be that new rows are not byte aligned, so 1 for byte aligned if format is R_FORMAT_R_8
    glPixelStorei(GL_UNPACK_ALIGNMENT, self->format==R_FORMAT_R_8? 1 : 4);
    // safe to pass NULL as opt_buffer
    glTexImage2D(GL_TEXTURE_2D, 0, format_internal(internal_format),
                 self->size.x, self->size.y,
                 0, format_channels(format), format_size(format), opt_buffer);
    r_error_check("tex allocation");

    glBindTexture(GL_TEXTURE_2D, 0);

    RTex_wrap_set(self, RTex_wrap_CLAMP);
    RTex_filter_set(self, RTex_filter_NEAREST);

    r_error_check("tex");

    return self;
}

RTex* RTex_new_file(oobj parent, const char* file)
{
    struct o_img image = o_img_new_file(parent, file);
    if(!image.rgba_data) {
        o_log_warn_s(__func__, "failed to create RTex from file: %s", file);
        // return invalid RTex and not NULL, because NULL is the back buffer...
        return RTex_new_ex(parent, NULL, 0, 0, R_FORMAT_RGBA_8, R_FORMAT_RGBA_8);
    }
    RTex *self = RTex_new_ex(parent, image.rgba_data, image.cols, image.rows,
                              R_FORMAT_RGBA_8, R_FORMAT_RGBA_8);
    o_img_free(&image);
    return self;
}

RTex* RTex_new_blit_back(oobj parent)
{
    RTex *self = RTex_new(parent, NULL, m_2(r_back_size_int()));
    RTex_blit_back(self, RTex_filter_NEAREST);
    return self;
}

RTex *RTex_new_kernel(oobj parent, int cols, int rows, float set, bool plus, float normalize)
{
    cols = o_max(cols, 1);
    rows = o_max(rows, 1);

    float *buffer = o_new(parent, float, cols*rows);
    vecn_set(buffer, set, cols*rows);
    if(plus) {
        buffer[0] = 0.0f;
        buffer[cols-1] = 0.0f;
        buffer[(rows-1)*cols] = 0.0f;
        buffer[rows*cols-1] = 0.0f;
    }
    
    if(normalize > 0.0f) {
        float sum = vecn_sum(buffer, cols*rows);
        vecn_scale(buffer, buffer, normalize/sum, cols*rows);
    }
    
    RTex *self = RTex_new_ex(parent, buffer, cols, rows,
            R_FORMAT_R_32F, R_FORMAT_R_32F);
    o_free(parent, buffer);
    return self;
}

RTex *RTex_new_kernel_gauss(oobj parent, int cols, int rows, vec2 sigma, float normalize)
{
    cols = o_max(cols, 1);
    rows = o_max(rows, 1);

    float *buffer = o_new(parent, float, cols*rows);
    
    sigma.x = sigma.x > 0 ? sigma.x : ((cols-1)*0.5 - 1)*0.3 + 0.8;
    sigma.y = sigma.y > 0 ? sigma.y : ((rows-1)*0.5 - 1)*0.3 + 0.8;
    
    float scale_x = -0.5/(sigma.x*sigma.x);
    float scale_y = -0.5/(sigma.y*sigma.y);
    
    int center_r = rows / 2;
    int center_c = cols / 2;
    
    for(int r = 0; r < rows; r++) {
        for(int c = 0; c < cols; c++) {
            float x = c - center_c;
            float y = r - center_r;
            float t_x = m_exp(scale_x*x*x);
            float t_y = m_exp(scale_y*y*y);
            
            float value = t_x * t_y;
           
            buffer[r * cols + c] = m_clamp(value, 0, 1);
        }
    }
    
    if(normalize > 0.0f) {
        float sum = vecn_sum(buffer, cols*rows);
        vecn_scale(buffer, buffer, normalize/sum, cols*rows);
    }
    
    RTex *self = RTex_new_ex(parent, buffer, cols, rows,
            R_FORMAT_R_32F, R_FORMAT_R_32F);
    o_free(parent, buffer);
    return self;
}

//
// virtual implementations:
//

void RTex__v_del(oobj obj)
{
    OObj_assert(obj, RTex);
    RTex *self = obj;
    
    // deletes fbo, if available
    RTex_use_done(self);
    // safe for 0
    glDeleteTextures(1, &self->gl_tex);

    OObj__v_del(obj);
}


//
// object functions:
//

ivec4 RTex_viewport(oobj obj)
{
    if(!obj) {
        return RTex_L.back_viewport;
    }
    OObj_assert(obj, RTex);
    RTex *self = obj;
    return self->viewport;
}

ivec4 RTex_viewport_set(oobj obj, ivec4 set)
{
    RTex_L.bound_valid = false;
    if(!obj) {
        RTex_L.back_viewport = set;
        return RTex_L.back_viewport;
    }
    OObj_assert(obj, RTex);
    RTex *self = obj;
    self->viewport = set;
    return self->viewport;
}

ivec4 RTex_viewport_set_pose_aa(oobj obj, mat4 pose_aa)
{
    // according to u/pose.h
    vec4 lbwh = vec4_(pose_aa.m30 - pose_aa.m00,
                      pose_aa.m31 - pose_aa.m11,
                      pose_aa.m00*2,
                      pose_aa.m11*2);
    return RTex_viewport_set(obj, r_proj_lbwh_to_viewport(RTex_proj(obj), lbwh, NULL));
}

ivec4 RTex_viewport_set_rect(oobj obj, vec4 rect)
{
    // according to u/rect.h
    vec4 lbwh = vec4_(rect.x - rect.v2/2.0,
                      rect.y - rect.v3/2.0,
                      rect.v2,
                      rect.v3);
    return RTex_viewport_set(obj, r_proj_lbwh_to_viewport(RTex_proj(obj), lbwh, NULL));
}

void RTex_wrap_set(oobj obj, enum RTex_wrap_modes mode)
{
    OObj_assert(obj, RTex);
    RTex *self = obj;
    self->wrap_mode = mode;
    glBindTexture(GL_TEXTURE_2D, self->gl_tex);
    if(self->wrap_mode == RTex_wrap_CLAMP) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

void RTex_filter_set(oobj obj, enum RTex_filter_modes mode)
{
    OObj_assert(obj, RTex);
    RTex *self = obj;
    self->filter_mode = mode;
    glBindTexture(GL_TEXTURE_2D, self->gl_tex);
    if(self->filter_mode == RTex_filter_LINEAR) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

oobj RTex_camera(oobj obj)
{
    OObj_assert(obj, RTex);
    RTex *self = obj;
    oobj cam = RCam_new(self, true, vec2_cast_int(self->size.v));
    RCam_update_ex(cam, self->size.x, self->size.y);
    return cam;
}

//
// fbo stuff
//


void RTex_use(oobj obj, int requested_draw_buffers)
{
    if(!obj) {
        if(requested_draw_buffers != 1) {
            o_log_warn_s(__func__, "using back buffer, but requested_draw_buffers!=1: %d", requested_draw_buffers);
        }
        if(!RTex_L.bound_valid || RTex_L.bound_tex != NULL) {
            RTex_L.bound_valid = true;
            RTex_L.bound_tex = NULL;
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, RTex_L.back_fbo);
            glDrawBuffers(1, (GLenum[]) {GL_BACK});
            glViewport(m_4(RTex_L.back_viewport));
            r_error_check("use back");
        }
        return;
    }

    OObj_assert(obj, RTex);
    RTex *self = obj;

    if(!RTex_L.bound_valid || RTex_L.bound_tex != obj) {
        RTex_L.bound_valid = true;
        RTex_L.bound_tex = obj;
        update_fbo(self, requested_draw_buffers); // Ensure the framebuffer is created
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, self->fbo->gl_fbo);
        glViewport(m_4(self->viewport));
        r_error_check("use tex");
    }
}


void RTex_use_done(oobj obj)
{
    OObj_assert(obj, RTex);
    RTex *self = obj;
    if(!self->fbo) {
        return;
    }
    glDeleteFramebuffers(1, &self->fbo->gl_fbo);
    o_free(self, self->fbo);
    self->fbo = NULL;
}

void RTex_get_ex(oobj obj, void *out_buffer, enum r_format format)
{
    OObj_assert(obj, RTex);
    RTex *self = obj;
    update_fbo(self, 1); // Ensure the framebuffer is created

    glBindTexture(GL_TEXTURE_2D, self->gl_tex);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, self->fbo->gl_fbo);
    // default may be that new rows are not byte aligned, so 1 for byte aligned if format is R_FORMAT_R_8
    glPixelStorei(GL_PACK_ALIGNMENT, self->format==R_FORMAT_R_8? 1 : 4);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glReadPixels(0, 0, self->size.x, self->size.y,
                 format_channels(format), format_size(format),
                 out_buffer);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void RTex_set_ex(oobj obj, const void *buffer, enum r_format format)
{
    OObj_assert(obj, RTex);
    RTex *self = obj;
    glBindTexture(GL_TEXTURE_2D, self->gl_tex);
    // default may be that new rows are not byte aligned, so 1 for byte aligned if format is R_FORMAT_R_8
    glPixelStorei(GL_UNPACK_ALIGNMENT, self->format==R_FORMAT_R_8? 1 : 4);
    glTexSubImage2D(GL_TEXTURE_2D, 0,
                    0, 0, self->size.x, self->size.y,
                    format_channels(format), format_size(format),
                    buffer);
    glBindTexture(GL_TEXTURE_2D, 0);
}

bool RTex_write_file(oobj obj, const char *file)
{
    ivec2 size = RTex_size_int(obj);
    struct o_img img = o_img_new(obj, m_2(size));
    RTex_get(obj, img.rgba_data);
    bool saved = o_img_write_file(img, file);
    o_img_free(&img);
    return saved;
}

struct r_proj *RTex_proj(oobj obj)
{
    if(!obj) {
        return r_back_proj();
    }
    OObj_assert(obj, RTex);
    RTex *self = obj;
    update_fbo(self, 1); // Ensure the framebuffer is created
    return &self->fbo->proj;
}

struct oobj_opt RTex_attach(oobj obj)
{
    OObj_assert(obj, RTex);
    RTex *self = obj;
    return OPtr_get(self->attached_tex_ptr);
}

oobj RTex_attach_set(oobj obj, oobj opt_tex_attachment)
{
    OObj_assert(obj, RTex);
    RTex *self = obj;
    OPtr_set(self->attached_tex_ptr, opt_tex_attachment);
    RTex_L.bound_valid = false;
    return opt_tex_attachment;
}

//
// render stuff
//

void RTex_clear_full(oobj obj, vec4 clear_color)
{
    RTex_use(obj, 1);

    glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
    glClear(GL_COLOR_BUFFER_BIT);

    r_error_check("clear");
}

void RTex_clear(oobj obj, vec4 clear_color)
{
    struct r_quad q = {0};
    // to get the full viewport
    q.pose = mat4_inv(RTex_proj(obj)->cam);
    q.uv = mat4_eye();
    q.s = clear_color;

    RShaderQuad_tex_set(RTex_L.s_quad, r_tex_white(), false);
    RShader_blend_set(RTex_L.s_quad, false);
    RShader_program_set(RTex_L.s_quad, r_program_Quad_color());
    RTex_quads(obj, RTex_L.s_quad, &q, 1);
}

void RTex_shader_ex(oobj obj, oobj shader, oobj buffer, int opt_num_rendered, const struct r_proj *opt_proj)
{
    RShader_render_ex(shader, buffer, obj, opt_num_rendered, opt_proj);
}

void RTex_ro_ex(oobj obj, oobj ro, const struct r_proj *opt_proj, bool update)
{
    RObj_render_ex(ro, obj, opt_proj, update);
}

void RTex_quads(oobj obj, oobj shader, const struct r_quad *quads, int n)
{
    RBuffer_update(RTex_L.b_quad, quads, n);
    RTex_shader(obj, shader, RTex_L.b_quad);
}

void RTex_rects(oobj obj, oobj shader, const struct r_rect *rects, int n)
{
    RBuffer_update(RTex_L.b_rect, rects, n);
    RTex_shader(obj, shader, RTex_L.b_rect);
}

void RTex_blit_ex(oobj obj, oobj tex, mat4 pose, mat4 uv)
{
    struct r_quad q = {0};
    q.pose = pose;
    q.uv = uv;

    RShaderQuad_tex_set(RTex_L.s_quad, tex, false);
    RShader_blend_set(RTex_L.s_quad, false);
    RShader_program_set(RTex_L.s_quad, r_program_Quad());
    RTex_quads(obj, RTex_L.s_quad, &q, 1);
}

void RTex_blit_color_ex(oobj obj, oobj tex, mat4 pose, mat4 uv, vec4 rgba, vec4 hsva)
{
    struct r_quad q = {0};
    q.pose = pose;
    q.uv = uv;
    q.s = rgba;
    q.t = hsva;

    RShaderQuad_tex_set(RTex_L.s_quad, tex, false);
    RShader_blend_set(RTex_L.s_quad, false);
    RShader_program_set(RTex_L.s_quad, r_program_Quad_color_hsva());
    RTex_quads(obj, RTex_L.s_quad, &q, 1);
}

void RTex_blit_back_ex(oobj obj, ivec4 viewport_obj, ivec4 viewport_back, enum RTex_filter_modes filter)
{
    OObj_assert(obj, RTex);
    RTex *self = obj;

    RTex_use(self, 1);

    if(viewport_obj.x < 0) {
        viewport_obj = self->viewport;
    }
    if(viewport_back.x < 0) {
        viewport_back = RTex_L.back_viewport;
    }

    glBindFramebuffer(GL_READ_FRAMEBUFFER, RTex_L.back_fbo);

    glBlitFramebuffer(viewport_back.x, viewport_back.y,
                      viewport_back.x + viewport_back.v2, viewport_back.y + viewport_back.v3,
                      viewport_obj.x, viewport_obj.y,
                      viewport_obj.x + viewport_obj.v2, viewport_obj.y + viewport_obj.v3,
                      GL_COLOR_BUFFER_BIT,
                      filter == RTex_filter_LINEAR ? GL_LINEAR : GL_NEAREST);

    r_error_check("RTex_blit_fbo_ex");
}

void RTex_blend_ex(oobj obj, oobj tex, mat4 pose, mat4 uv)
{
    struct r_quad q = {0};
    q.pose = pose;
    q.uv = uv;

    RShaderQuad_tex_set(RTex_L.s_quad, tex, false);
    RShader_blend_set(RTex_L.s_quad, true);
    RShader_program_set(RTex_L.s_quad, r_program_Quad());
    RTex_quads(obj, RTex_L.s_quad, &q, 1);
}

void RTex_blend_color_ex(oobj obj, oobj tex, mat4 pose, mat4 uv, vec4 rgba, vec4 hsva)
{
    struct r_quad q = {0};
    q.pose = pose;
    q.uv = uv;
    q.s = rgba;
    q.t = hsva;

    RShaderQuad_tex_set(RTex_L.s_quad, tex, false);
    RShader_blend_set(RTex_L.s_quad, true);
    RShader_program_set(RTex_L.s_quad, r_program_Quad_color_hsva());
    RTex_quads(obj, RTex_L.s_quad, &q, 1);
}

//
// functions that generate RTex's
//


RTex* RTex_border(oobj obj, ivec4 lrbt, vec4 color, enum r_format format)
{
    vec2 size = RTex_size(obj);
    size.x += lrbt.v0 + lrbt.v1;
    size.y += lrbt.v2 + lrbt.v3;

    RTex *res = RTex_new_ex(obj, NULL, m_2(size), format, format);
    RTex_clear_full(res, color);
    RTex_blit(res, obj, 0, 0);
    return res;
}

void RTex_add_scaled_into(oobj obj, vec4 add, vec4 scale, oobj into)
{
    struct r_quad q = r_quad_new(m_2(RTex_size(obj)));
    q.s = add;
    q.t = scale;

    RShaderQuad_tex_set(RTex_L.s_quad, obj, false);
    RShader_blend_set(RTex_L.s_quad, false);
    RShader_program_set(RTex_L.s_quad, r_program_Quad_addscaled());
    RTex_quads(into, RTex_L.s_quad, &q, 1);
}

void RTex_mixer_into(oobj obj, mat4 rgba_matrix, oobj into)
{
    struct r_quad q = r_quad_new(m_2(RTex_size(obj)));
    q.stuv = rgba_matrix;

    RShaderQuad_tex_set(RTex_L.s_quad, obj, false);
    RShader_blend_set(RTex_L.s_quad, false);
    RShader_program_set(RTex_L.s_quad, r_program_Quad_mixer());
    RTex_quads(into, RTex_L.s_quad, &q, 1);
}

void RTex_inv_into(oobj obj, vec4 inv_mask, oobj into)
{
    mat4 mix;
    mix.col[0] = vec4_(inv_mask.r, 0, 0, 0);
    mix.col[1] = vec4_(0, inv_mask.g, 0, 0);
    mix.col[2] = vec4_(0, 0, inv_mask.b, 0);
    mix.col[3] = vec4_(0, 0, 0, inv_mask.a);
    RTex_mixer_into(obj, mix, into);;
}

void RTex_channels_into(oobj obj, ivec4 channel, oobj into)
{
    mat4 mix;
    for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++) {
            mix.col[i].v[j] = (float) (channel.v[i] == j);
        }
    }
    RTex_mixer_into(obj, mix, into);
}

void RTex_channels_merge_into(oobj r, oobj g, oobj b, oobj a, oobj into)
{
    struct r_quad q = r_quad_new(m_2(RTex_size(r)));

    RShaderQuadChannelmerge_r_set(RTex_L.s_quad_channelmerge, r);
    RShaderQuadChannelmerge_g_set(RTex_L.s_quad_channelmerge, g);
    RShaderQuadChannelmerge_b_set(RTex_L.s_quad_channelmerge, b);
    RShaderQuadChannelmerge_a_set(RTex_L.s_quad_channelmerge, a);
    RShader_blend_set(RTex_L.s_quad_channelmerge, false);
    RTex_quads(into, RTex_L.s_quad_channelmerge, &q, 1);
}

void RTex_color_into(oobj obj, vec4 rgba, vec4 hsva, oobj into)
{
    struct r_quad q = r_quad_new(m_2(RTex_size(obj)));
    q.s = rgba;
    q.t = hsva;

    RShaderQuad_tex_set(RTex_L.s_quad, obj, false);
    RShader_blend_set(RTex_L.s_quad, false);
    RShader_program_set(RTex_L.s_quad, r_program_Quad_color_hsva());
    RTex_quads(into, RTex_L.s_quad, &q, 1);
}

void RTex_rgba_to_hsva_into(oobj obj, oobj into)
{
    struct r_quad q = r_quad_new(m_2(RTex_size(obj)));

    RShaderQuad_tex_set(RTex_L.s_quad, obj, false);
    RShader_blend_set(RTex_L.s_quad, false);
    RShader_program_set(RTex_L.s_quad, r_program_Quad_hsva_from_rgba());
    RTex_quads(into, RTex_L.s_quad, &q, 1);
}

void RTex_hsva_rgba_into(oobj obj, oobj into)
{
    struct r_quad q = r_quad_new(m_2(RTex_size(obj)));

    RShaderQuad_tex_set(RTex_L.s_quad, obj, false);
    RShader_blend_set(RTex_L.s_quad, false);
    RShader_program_set(RTex_L.s_quad, r_program_Quad_hsva_into_rgba());
    RTex_quads(into, RTex_L.s_quad, &q, 1);
}

void RTex_conv_into(oobj obj, oobj kernel, ivec2 offset, oobj into)
{
    struct r_quad q = r_quad_new(m_2(RTex_size(obj)));
    
    RShaderQuad_tex_set(RTex_L.s_quad_kernel, obj, false);
    RShader_blend_set(RTex_L.s_quad_kernel, false);
    RShaderQuadKernel_kernel_set(RTex_L.s_quad_kernel, kernel, false);
    RShaderQuadKernel_offset_set(RTex_L.s_quad_kernel, offset);
    RShaderQuadKernel_color_set(RTex_L.s_quad_kernel, R_WHITE);
    RShader_program_set(RTex_L.s_quad_kernel, r_program_QuadKernel_conv());
    RTex_quads(into, RTex_L.s_quad_kernel, &q, 1);
}

void RTex_blur_into(oobj obj, ivec2 size, oobj into)
{
    oobj kernel = RTex_new_kernel(obj, m_2(size), 1.0f, false, 1.0f);
    RTex_conv_into(obj, kernel, ivec2_(0), into);
    o_del(kernel);
}

void RTex_gauss_into(oobj obj, ivec2 size, vec2 sigma, oobj into)
{
    oobj kernel = RTex_new_kernel_gauss(obj, m_2(size), sigma, 1.0f);
    RTex_conv_into(obj, kernel, ivec2_(0), into);
    o_del(kernel);
}

void RTex_dilate_into(oobj obj, oobj kernel, ivec2 offset, vec4 mask, vec4 color, oobj into)
{
    struct r_quad q = r_quad_new(m_2(RTex_size(obj)));

    RShaderQuad_tex_set(RTex_L.s_quad_kernel, obj, false);
    RShader_blend_set(RTex_L.s_quad_kernel, false);
    RShaderQuadKernel_kernel_set(RTex_L.s_quad_kernel, kernel, false);
    RShaderQuadKernel_offset_set(RTex_L.s_quad_kernel, offset);
    RShaderQuadKernel_color_set(RTex_L.s_quad_kernel, color);
    RShaderQuadKernel_mask_set(RTex_L.s_quad_kernel, mask);
    RShader_program_set(RTex_L.s_quad_kernel, r_program_QuadKernel_contour());
    RTex_quads(into, RTex_L.s_quad_kernel, &q, 1);
}

void RTex_erode_into(oobj obj, oobj kernel, ivec2 offset, vec4 mask, vec4 color, oobj into)
{
    struct r_quad q = r_quad_new(m_2(RTex_size(obj)));

    RShaderQuad_tex_set(RTex_L.s_quad_kernel, obj, false);
    RShader_blend_set(RTex_L.s_quad_kernel, false);
    RShaderQuadKernel_kernel_set(RTex_L.s_quad_kernel, kernel, false);
    RShaderQuadKernel_offset_set(RTex_L.s_quad_kernel, offset);
    RShaderQuadKernel_color_set(RTex_L.s_quad_kernel, color);
    RShaderQuadKernel_mask_set(RTex_L.s_quad_kernel, mask);
    RShader_program_set(RTex_L.s_quad_kernel, r_program_QuadKernel_contour_inv());
    RTex_quads(into, RTex_L.s_quad_kernel, &q, 1);
}

void RTex_contour_into(oobj obj, oobj kernel, ivec2 offset, vec4 mask, vec4 color, oobj into)
{
    RTex_dilate_into(obj, kernel, offset, mask, color, into);
    RTex_blend(into, obj, 0, 0);
}

void RTex_outline_into(oobj obj, ivec2 size, ivec2 offset, vec4 mask, vec4 color, oobj into)
{
    oobj kernel = RTex_new_kernel(obj, m_2(size), 1.0f, true, -1);
    RTex_contour_into(obj, kernel, offset, mask, color, into);
    o_del(kernel);
}

RTex *RTex_collage(const oobj *srcs, int n, int cols, ivec2 margin, vec4 bg_color, enum r_format format)
{
    assert(n>=1);
    assert(margin.x>=0 && margin.y>=0);

    ivec2 grid_size = ivec2_(0);
    for(int i=0; i<n; i++) {
        ivec2 size = RTex_size_int(srcs[i]);
        grid_size = ivec2_max_v(grid_size, size);
    }
    grid_size = ivec2_add_v(grid_size, ivec2_scale(margin, 2));

    int rows = (int) m_ceil((float) n / (float)  cols);

    ivec2 full_size = ivec2_scale_v(grid_size, ivec2_(cols, rows));

    RTex *res = RTex_new_ex(srcs[0], NULL, m_2(full_size), format, format);
    RTex_clear_full(res, bg_color);

    for(int i=0; i<n; i++) {
        int c = i%cols;
        int r = i/cols;

        vec2 pos = vec2_((grid_size.x-full_size.x)/2.0f, (full_size.y-grid_size.y)/2.0f);
        pos.x += c * grid_size.x;
        pos.y -= r * grid_size.y;

        RTex_blend(res, srcs[i], m_2(pos));
    }

    return res;
}

void RTex_merge_into(oobj obj, oobj tex, oobj opt_mask_a, oobj opt_mask_b, vec4 rgba, oobj into)
{
    struct r_quad q = r_quad_new(m_2(RTex_size(obj)));
    q.s = rgba;

    RShaderQuad_tex_set(RTex_L.s_quad_merge, tex, false);
    RShaderQuadMerge_tex_old_set(RTex_L.s_quad_merge, obj);
    RShaderQuadMerge_mask_a_set(RTex_L.s_quad_merge, o_or(opt_mask_a, r_tex_white()));
    RShaderQuadMerge_mask_b_set(RTex_L.s_quad_merge, o_or(opt_mask_b, r_tex_white()));
    RShader_program_set(RTex_L.s_quad_merge, r_program_QuadMerge());
    RShader_blend_set(RTex_L.s_quad_merge, false);
    RTex_quads(into, RTex_L.s_quad_merge, &q, 1);
}
