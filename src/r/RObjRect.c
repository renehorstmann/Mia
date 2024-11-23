#include "r/RObjRect.h"
#include "o/OObj_builder.h"
#include "o/OArray.h"
#include "r/RBufferRect.h"
#include "r/RShaderRect.h"

#define O_LOG_LIB "r"
#include "o/log.h"


// inits all but the shader pipeline
O_STATIC
RObjRect* rect_base_init(oobj obj, oobj parent, int num, oobj tex, bool move_tex)
{
    RObjRect* self = obj;
    o_clear(self, sizeof *self, 1);

    RObj_init(obj, parent, RObjRect__v_update, RObjRect__v_render);
    OObj_id_set(self, RObjRect_ID);

    num = o_max(0, num);
    self->num_rendered = oi32_MAX;

    self->rects = OArray_new(self, NULL, sizeof(struct r_rect), num);

    for (int i = 0; i < num; i++) {
        *RObjRect_at(self, i) = r_rect_new(m_2(RTex_size(tex)));
    }

    self->tex = tex;
    if (tex && move_tex) {
        o_move(tex, self);
    }


    self->buffer = RBufferRect_new(self);

    // vfuncs
    self->super.super.v_op_num = RObjRect__v_op_num;
    self->super.super.v_op_at = RObjRect__v_op_at;

    // update default values to gpu
    RObj_update(self);

    return self;
}

//
// public
//


RObjRect* RObjRect_init(oobj obj, oobj parent, int num, oobj tex, bool move_tex)
{
    RObjRect* self = rect_base_init(obj, parent, num, tex, move_tex);
    oobj shader = RShaderRect_new(self, tex, false);
    self->shader_pipeline = OArray_new(self, &shader, sizeof(oobj), 1);
    return self;
}

RObjRect *RObjRect_init_shader(oobj obj, oobj parent, int num, oobj tex, bool move_tex, oobj shader, bool move_shader)
{
    RObjRect* self = rect_base_init(obj, parent, num, tex, move_tex);
    if(move_shader) {
        o_move(shader, self);
    }
    self->shader_pipeline = OArray_new(self, &shader, sizeof(oobj), 1);
    return self;
}

RObjRect *RObjRect_new_color(oobj parent, int num, oobj tex, bool move_tex)
{
    oobj shader = RShaderRect_new_color(parent, tex, false);
    return RObjRect_new_shader(parent, num, tex, move_tex, shader, true);
}

//
// virtual implementations:
//

osize RObjRect__v_op_num(oobj obj)
{
    return RObjRect_num(obj);
}

void* RObjRect__v_op_at(oobj obj, osize idx)
{
    return RObjRect_at(obj, (int)idx);
}

void RObjRect__v_update(oobj obj)
{
    OObj_assert(obj, RObjRect);
    RObjRect* self = obj;

    RBuffer_update(self->buffer, OArray_data_void(self->rects), OArray_num(self->rects));
}

void RObjRect__v_render(oobj obj, oobj tex, const struct r_proj* proj)
{
    OObj_assert(obj, RObjRect);
    RObjRect* self = obj;

    for (osize i = 0; i < OArray_num(self->shader_pipeline); i++) {
        RShaderRect* shader = *OArray_at(self->shader_pipeline, i, oobj);
        OObj_assert(shader, RShaderRect);

        // update tex reference
        shader->tex = self->tex;

        int num_render = o_min(OArray_num(self->rects), self->num_rendered);
        RShader_render_ex(shader, self->buffer, tex, num_render, proj);
    }
}


//
// object functions:
//

oobj RObjRect_shader(oobj obj, int pipeline_idx)
{
    return *OArray_at(RObjRect_shader_pipeline(obj), pipeline_idx, oobj);
}

int RObjRect_shader_set(oobj obj, int pipeline_idx, oobj shader, bool del_old)
{
    OArray *pipeline = RObjRect_shader_pipeline(obj);
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
