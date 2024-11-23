#include "r/RShaderQuadMerge.h"
#include "o/OObj_builder.h"
#include "r/RProgram.h"
#include "r/program.h"
#include "r/tex.h"

#define O_LOG_LIB "r"
#include "o/log.h"

//
// public
//


RShaderQuadMerge* RShaderQuadMerge_init(oobj obj, oobj parent, oobj tex, bool move_tex)
{
    RShader *super_shader = obj;
    RShaderQuadMerge *self = obj;
    o_clear(self, sizeof *self, 1);

    RShaderQuad_init(obj, parent, tex, move_tex);
    OObj_id_set(self, RShaderQuadMerge_ID);

    self->mask_a = self->mask_b = self->tex_old = r_tex_white();

    // overwrite program
    super_shader->program = r_program_QuadMerge();

    // default is not blended
    RShader_blend_set(self, false);
    
    // update vfunc
    super_shader->v_render = RShaderQuadMerge__v_render;

    return self;
}

//
// virtual implementations:
//


void RShaderQuadMerge__v_render(oobj obj, oobj program, int num, const struct r_proj *proj)
{
    OObj_assert(obj, RShaderQuadMerge);
    RShaderQuadMerge *self = obj;

    RProgram_uniform_tex(program, "u_tex_old", 1, RTex_tex(self->tex_old));
    RProgram_uniform_tex(program, "u_mask_a", 2, RTex_tex(self->mask_a));
    RProgram_uniform_tex(program, "u_mask_b", 3, RTex_tex(self->mask_b));

    RShaderQuad__v_render(obj, program, num, proj);
}
