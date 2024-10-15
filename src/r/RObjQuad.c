#include "r/RObjQuad.h"
#include "o/OObj_builder.h"
#include "o/OArray.h"
#include "r/RBufferQuad.h"
#include "r/RShaderQuad.h"
#include "r/RShaderQuadKernel.h"

#define O_LOG_LIB "r"
#include "o/log.h"


// inits all but the shader pipeline
O_STATIC
RObjQuad* quad_base_init(oobj obj, oobj parent, int num, oobj tex, bool move_tex)
{
    RObjQuad* self = obj;
    o_clear(self, sizeof *self, 1);

    RObj_init(obj, parent, RObjQuad__v_update, RObjQuad__v_render);
    OObj_id_set(self, RObjQuad_ID);

    num = o_max(0, num);
    self->num_rendered = oi32_MAX;

    self->quads = OArray_new(self, NULL, sizeof(struct r_quad), num);

    for (int i = 0; i < num; i++) {
        *RObjQuad_at(self, i) = r_quad_new(m_2(RTex_size(tex)));
    }

    self->tex = tex;
    if (tex && move_tex) {
        o_move(tex, self);
    }


    self->buffer = RBufferQuad_new(self);

    // vfuncs
    self->super.super.v_op_num = RObjQuad__v_op_num;
    self->super.super.v_op_at = RObjQuad__v_op_at;

    // update default values to gpu
    RObj_update(self);

    return self;
}

//
// public
//


RObjQuad* RObjQuad_init(oobj obj, oobj parent, int num, oobj tex, bool move_tex)
{
    RObjQuad* self = quad_base_init(obj, parent, num, tex, move_tex);
    oobj shader = RShaderQuad_new(self, tex, false);
    self->shader_pipeline = OArray_new(self, &shader, sizeof(oobj), 1);
    return self;
}

RObjQuad *RObjQuad_init_shader(oobj obj, oobj parent, int num, oobj tex, bool move_tex, oobj shader, bool move_shader)
{
    RObjQuad* self = quad_base_init(obj, parent, num, tex, move_tex);
    if(move_shader) {
        o_move(shader, self);
    }
    self->shader_pipeline = OArray_new(self, &shader, sizeof(oobj), 1);
    return self;
}

RObjQuad *RObjQuad_new_color(oobj parent, int num, oobj tex, bool move_tex)
{
    oobj shader = RShaderQuad_new_color(parent, tex, false);
    return RObjQuad_new_shader(parent, num, tex, move_tex, shader, true);
}

//
// virtual implementations:
//

osize RObjQuad__v_op_num(oobj obj)
{
    return RObjQuad_num(obj);
}

void* RObjQuad__v_op_at(oobj obj, osize idx)
{
    return RObjQuad_at(obj, (int)idx);
}

void RObjQuad__v_update(oobj obj)
{
    OObj_assert(obj, RObjQuad);
    RObjQuad* self = obj;

    RBuffer_update(self->buffer, OArray_data_void(self->quads), OArray_num(self->quads));
}

void RObjQuad__v_render(oobj obj, oobj tex, const struct r_proj* proj)
{
    OObj_assert(obj, RObjQuad);
    RObjQuad* self = obj;

    for (osize i = 0; i < OArray_num(self->shader_pipeline); i++) {
        RShaderQuad* shader = *OArray_at(self->shader_pipeline, i, oobj);
        OObj_assert(shader, RShaderQuad);

        // update tex reference
        shader->tex = self->tex;

        int num_render = o_min(OArray_num(self->quads), self->num_rendered);
        RShader_render_ex(shader, self->buffer, tex, num_render, proj);
    }
}


//
// object functions:
//

oobj RObjQuad_shader(oobj obj, int pipeline_idx)
{
    return *OArray_at(RObjQuad_shader_pipeline(obj), pipeline_idx, oobj);
}

int RObjQuad_shader_set(oobj obj, int pipeline_idx, oobj shader, bool del_old)
{
    OArray *pipeline = RObjQuad_shader_pipeline(obj);
    if(pipeline_idx>=0 && pipeline_idx<o_num(pipeline)) {
        oobj *ref = OArray_at(pipeline, pipeline_idx, oobj);
        if(del_old) {
            oobj old = *ref;
            o_del(old);
        }
        *ref = shader;
        return pipeline_idx;
    }

    OArray_push(pipeline, &shader);
    return o_num(pipeline)-1;
}


oobj RObjQuad_shader_add_bloom(oobj obj, oobj kernel,vec4 color)
{
    OObj_assert(obj, RObjQuad);
    RObjQuad* self = obj;

    oobj shader = RShaderQuadKernel_new(obj, self->tex, false, kernel, false);
    RShaderQuadKernel_color_set(shader, color);
    RShaderQuadKernel_mask_set(shader, vec4_(1, 1, 1, 0.66));
    OArray_push(self->shader_pipeline, &shader);

    return shader;
}

oobj RObjQuad_shader_add_contour(oobj obj, oobj kernel, vec4 color)
{
    OObj_assert(obj, RObjQuad);
    RObjQuad* self = obj;

    oobj shader = RShaderQuadKernel_new(obj, self->tex, false,
            kernel, false);
    RShaderQuadKernel_color_set(shader, color);
    OArray_push_front(self->shader_pipeline, &shader);
    
    return shader;
}
