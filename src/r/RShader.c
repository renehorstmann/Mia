#include "r/RShader.h"
#include "o/OObj_builder.h"
#include "r/RProgram.h"
#include "r/RBuffer.h"
#include "r/RTex.h"
#include "r/gl.h"


O_STATIC
void blend(bool on)
{
    if(!on) {
        glDisable(GL_BLEND);
        r_error_check("blend off");
        return;
    }

    glEnable(GL_BLEND);
    // adds alpha, can't get more transparency:
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);

    r_error_check("blend");
}


//
// public
//


RShader *RShader_init(oobj obj, oobj parent, oobj program, RShader__render_fn render_fn)
{
    RShader *self = obj;
    o_clear(self, sizeof *self, 1);

    OObj_init(self, parent);
    OObj_id_set(self, RShader_ID);

    self->program = program;
    self->blend = true;
    
    // vfuncs
    self->v_render = render_fn;
    
    return self;
}

void RShader_render_ex(oobj obj, oobj buffer, oobj tex, int opt_num_rendered, const struct r_proj *opt_proj)
{
    OObj_assert(obj, RShader);
    RShader *self = obj;

    int buffer_num = RBuffer_num(buffer);
    if(buffer_num <= 0) {
        return;
    }
    if(opt_num_rendered<=0) {
        opt_num_rendered = buffer_num;
    }

    RProgram_use(self->program);
    RBuffer_use(buffer);
    RTex_use(tex, RProgram_num_draw_buffers(self->program));

    blend(self->blend);
    opt_proj = o_or(opt_proj, RTex_proj(tex));
    
    self->v_render(self, self->program, o_min(opt_num_rendered, RBuffer_num(buffer)), opt_proj);
    
    RBuffer_use(NULL);
    RProgram_use(NULL);
}


