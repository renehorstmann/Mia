#include "r/RShaderRect.h"
#include "o/OObj_builder.h"
#include "r/RProgram.h"
#include "r/program.h"
#include "r/gl.h"


RShaderRect *RShaderRect_init(oobj obj, oobj parent, oobj tex, bool move_tex)
{
    RShaderRect *self = obj;
    o_clear(self, sizeof *self, 1);

    RShader_init(obj, parent, r_program_Rect(), RShaderRect__v_render);
    OObj_id_set(self, RShaderRect_ID);

    self->tex = tex;
    if (tex && move_tex) {
        o_move(tex, self);
    }

    self->s = vec4_(1);

    return self;
}


RShaderRect* RShaderRect_new_color(oobj parent, oobj tex, bool move_tex)
{
    RShaderRect *self = RShaderRect_new(parent, tex, move_tex);
    RShader_program_set(self, r_program_Rect_color());
    return self;
}

//
// virtual implementations:
//

void RShaderRect__v_render(oobj obj, oobj program, int num, const struct r_proj *proj)
{
    OObj_assert(obj, RShaderRect);
    RShaderRect *self = obj;

    // common uniforms
    float camera_viewport_scale_double = proj->scale * 2;
    RProgram_uniform_float(program, "u_c_viewport_scale_double", &camera_viewport_scale_double, 1);
    RProgram_uniform_vec2(program, "u_c_viewport_size_half", &proj->vpsh, 1);
    RProgram_uniform_vec2(program, "u_c_viewport_even_offset", &proj->viewport_even_offset, 1);


    // stuv
    RProgram_uniform_vec4(program, "u_s", &self->s, 1);
    RProgram_uniform_vec4(program, "u_t", &self->t, 1);

    // basic uniforms:
    RProgram_uniform_mat4(program, "u_vp", &proj->cam, 1);

    vec2 tex_scale = RTex_get_tex_scale(self->tex);
    RProgram_uniform_vec2(program, "u_tex_scale", &tex_scale, 1);

    RProgram_uniform_tex(program, "u_tex", 0, RTex_tex(self->tex));

    // draw call
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, num);

    RProgram_uniform_tex_off(program, 0);
}

