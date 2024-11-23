#include "r/RShaderQuadChannelmerge.h"
#include "o/OObj_builder.h"
#include "r/RProgram.h"
#include "r/program.h"

#define O_LOG_LIB "r"
#include "o/log.h"

//
// public
// public
//


RShaderQuadChannelmerge *RShaderQuadChannelmerge_init(oobj obj, oobj parent,
                                                      oobj r, oobj g, oobj b, oobj a, bool move_texs)
{
    RShader *super_shader = obj;
    RShaderQuadChannelmerge *self = obj;
    o_clear(self, sizeof *self, 1);

    RShaderQuad_init(obj, parent, r, move_texs);
    OObj_id_set(self, RShaderQuadChannelmerge_ID);

    self->g = g;
    self->b = b;
    self->a = a;

    if(move_texs) {
        o_move(g, self);
        o_move(b, self);
        o_move(a, self);
    }

    // overwrite program
    super_shader->program = r_program_QuadChannelmerge();
    
    // update vfunc
    super_shader->v_render = RShaderQuadChannelmerge__v_render;

    return self;
}

//
// virtual implementations:
//


void RShaderQuadChannelmerge__v_render(oobj obj, oobj program, int num, const struct r_proj *proj)
{
    OObj_assert(obj, RShaderQuadChannelmerge);
    RShaderQuadChannelmerge *self = obj;

    RProgram_uniform_tex(program, "u_tex_g", 1, RTex_tex(self->g));
    RProgram_uniform_tex(program, "u_tex_b", 2, RTex_tex(self->b));
    RProgram_uniform_tex(program, "u_tex_a", 3, RTex_tex(self->a));

    RShaderQuad__v_render(obj, program, num, proj);
}
