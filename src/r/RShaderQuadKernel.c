#include "r/RShaderQuadKernel.h"
#include "o/OObj_builder.h"
#include "r/RProgram.h"
#include "r/program.h"

#define O_LOG_LIB "r"
#include "o/log.h"


//
// public
//


RShaderQuadKernel *RShaderQuadKernel_init(oobj obj, oobj parent, oobj tex, bool move_tex, 
        oobj kernel, bool move_kernel)
{
    RShader *super_shader = obj;
    RShaderQuadKernel *self = obj;
    o_clear(self, sizeof *self, 1);

    RShaderQuad_init(obj, parent, tex, move_tex);
    OObj_id_set(self, RShaderQuadKernel_ID);

    self->kernel = kernel;
    if(move_kernel) {
        o_move(kernel, self);
    }
    
    self->mask = vec4_(0, 0, 0, 1);
    self->color = R_WHITE;
    
    // update vfunc
    super_shader->v_render = RShaderQuadKernel__v_render;

    return self;
}

//
// virtual implementations:
//


void RShaderQuadKernel__v_render(oobj obj, oobj program, int num, const struct r_proj *proj)
{
    OObj_assert(obj, RShaderQuadKernel);
    RShaderQuadKernel *self = obj;

    
    RProgram_uniform_ivec2(program, "u_kernel_offset", &self->offset, 1);
    RProgram_uniform_vec4(program, "u_kernel_color", &self->color, 1);
    RProgram_uniform_vec4(program, "u_kernel_mask", &self->mask, 1);
    
    RProgram_uniform_tex(program, "u_kernel", 1, RTex_tex(self->kernel));
    
    
    RShaderQuad__v_render(obj, program, num, proj);
}

//
// object functions
//

oobj RShaderQuadKernel_kernel_set(oobj obj, oobj set, bool del_old) {
    OObj_assert(obj, RShaderQuadKernel);
    RShaderQuadKernel *self = obj;
    if(del_old) {
        o_del(self->kernel);
    }
    self->kernel = set;
    return self->kernel;
}
