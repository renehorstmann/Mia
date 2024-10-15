#include "r/common.h"
#include "r/proj.h"
#include "r/gl.h"
#include "o/OObjRoot.h"
#include "o/ODelcallback.h"
#include "m/vec/ivec2.h"

#define O_LOG_LIB "r"
#include "o/log.h"


static struct {
    bool init;
    oobj root;
    ivec2 back_size;
    struct r_proj back_proj;
    ivec2 max_tex_size;
} common_L;


O_STATIC
void collect_gl_info(void)
{
    const char* version = (const char*) glGetString(GL_VERSION);
    o_log_trace_s("r_init", "OpenGL Version: %s", version);
    const char* vendor = (const char*) glGetString(GL_VENDOR);
    o_log_trace_s("r_init", "GPU Vendor: %s", vendor);

    GLint param;
    
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &param);
    o_log_trace_s("r_init", "Maximum texture size: %d,%d", param/1000, param%1000);
    common_L.max_tex_size = ivec2_(param);
    
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &param);
    o_log_trace_s("r_init", "Maximum color attachments: %d", param);
    o_assume(param >= 2, "Mia needs at least two color attachments");

    /*
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &param);
    o_log_trace_s("r_init", "Maximum vertex uniforms: %d", param);
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &param);
    o_log_trace_s("r_init", "Maximum fragment uniforms: %d", param);
    */
    
}

//
// public
//

void r_init(oobj opt_root)
{
    if (common_L.init) {
        o_log_error_s(__func__, "r_init called already!");
        return;
    }
    common_L.init = true;

    collect_gl_info();

    if(opt_root) {
        common_L.root = OObj_new(opt_root);
    } else {
        common_L.root = OObjRoot_new_heap();
    }
    OObj_name_set(common_L.root, "r_root");
    ODelcallback_new_assert(common_L.root, "r_root", "deleted!");

    O_EXTERN
    void RTex__init(void);
    RTex__init();

    r_error_check("init");
}

oobj r_root(void)
{
    return common_L.root;
}

ivec2 r_back_size_int(void)
{
    return common_L.back_size;
}

struct r_proj *r_back_proj(void)
{
    return &common_L.back_proj;
}

ivec2 r_max_tex_size(void)
{
    return common_L.max_tex_size;
}



void r_frame_begin(int back_cols, int back_rows)
{
    common_L.back_size = ivec2_(back_cols, back_rows);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_SCISSOR_TEST);
    
    r_error_check("frame begin");
}

bool r_error_check_call(const char *file, int line, const char *tag) {
    static GLenum errs[32];
    int errs_size = 0;
    GLenum err;
    bool unexpected_error = false;
    while ((err = glGetError()) != GL_NO_ERROR) {
        for (int i = 0; i < errs_size; i++) {
            if (err == errs[i])
                continue;
        }

        const char *name;
        switch (err) {
            case 0x500:
                name = "GL_INVALID_ENUM";
                break;
            case 0x501:
                name = "GL_INVALID_VALUE";
                break;
            case 0x502:
                name = "GL_INVALID_OPERATION";
                break;
            case 0x503:
                name = "GL_STACK_OVERFLOW";
                break;
            case 0x504:
                name = "GL_STACK_UNDERFLOW";
                break;
            case 0x505:
                name = "GL_OUT_OF_MEMORY";
                break;
            case 0x506:
                name = "GL_INVALID_FRAMEBUFFER_OPERATION";
                break;
            case 0x507:
                name = "GL_CONTEXT_LOST";
                break;
            case 0x8031:
                name = "GL_TABLE_TOO_LARGE";
                break;
            default:
                name = "UNKNOWN_ERROR?";
                break;
        }

        o_log_base(O_LOG_ERROR, "r", file, line, tag, "OpenGl error: 0x%04x %s", err, name);

        if (errs_size < 32)
            errs[errs_size++] = err;

        unexpected_error = true;
    }

    return unexpected_error;
}
