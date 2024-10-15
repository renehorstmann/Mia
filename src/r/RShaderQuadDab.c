#include "r/RShaderQuadDab.h"
#include "o/OObj_builder.h"
#include "r/RProgram.h"
#include "r/program.h"
#include "r/tex.h"
#include "m/vec/vec2.h"

#define O_LOG_LIB "r"
#include "o/log.h"

//
// public
//


RShaderQuadDab* RShaderQuadDab_init(oobj obj, oobj parent, oobj tex, bool move_tex)
{
    RShader *super_shader = obj;
    RShaderQuadDab *self = obj;
    o_clear(self, sizeof *self, 1);

    RShaderQuad_init(obj, parent, tex, move_tex);
    OObj_id_set(self, RShaderQuadDab_ID);

    self->pattern = r_tex_white();
    self->pattern_scale = vec2_(1);

    // overwrite program
    super_shader->program = r_program_QuadDab();
    
    // update vfunc
    super_shader->v_render = RShaderQuadDab__v_render;

    return self;
}

//
// virtual implementations:
//


void RShaderQuadDab__v_render(oobj obj, oobj program, int num, const struct r_proj *proj)
{
    OObj_assert(obj, RShaderQuadDab);
    RShaderQuadDab *self = obj;

    RProgram_uniform_tex(program, "u_pattern", 1, RTex_tex(self->pattern));

    vec2 size_inv = vec2_div_v(vec2_(1), vec2_scale_v(RTex_size(self->pattern), self->pattern_scale));
    RProgram_uniform_vec2(program, "u_pattern_size_inv", &size_inv, 1);

    RShaderQuad__v_render(obj, program, num, proj);
}
