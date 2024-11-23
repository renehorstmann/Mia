#include "r/RBuffer.h"
#include "o/OObj_builder.h"
#include "r/gl.h"

#define O_LOG_LIB "r"
#include "o/log.h"


//
// public
//

RBuffer *RBuffer_init(oobj obj, oobj parent, osize element_size)
{
    RBuffer *self = obj;
    o_clear(self, sizeof *self, 1);

    OObj_init(self, parent);
    OObj_id_set(self, RBuffer_ID);

    self->element_size = element_size;
    
    glGenVertexArrays(1, &self->gl_vao);
    glBindVertexArray(self->gl_vao);
    glGenBuffers(1, &self->gl_vbo);
    glBindVertexArray(0);
    r_error_check("create");

    // deletor
    self->super.v_del = RBuffer__v_del;

    return self;
}



void RBuffer__v_del(oobj obj)
{
    OObj_assert(obj, RBuffer);
    RBuffer *self = obj;

    // safe to pass 0
    glDeleteVertexArrays(1, &self->gl_vao);
    glDeleteBuffers(1, &self->gl_vbo);

    OObj__v_del(self);
}

void RBuffer_update(oobj obj, const void *data, osize num)
{
    OObj_assert(obj, RBuffer);
    RBuffer *self = obj;
    
    self->num = num;
    if(num <= 0) {
        return;
    }
    
    glBindVertexArray(self->gl_vao);
    glBindBuffer(GL_ARRAY_BUFFER, self->gl_vbo);
    
    glBufferData(GL_ARRAY_BUFFER, self->element_size * num, data, GL_STREAM_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    r_error_check("update");
}

void RBuffer_use(oobj obj)
{
    if (!obj) {
        glBindVertexArray(0);
        return;
    }
    OObj_assert(obj, RBuffer);
    RBuffer *self = obj;

    glBindVertexArray(self->gl_vao);
}

void RBuffer_location_vec4(oobj obj, int location, osize offset)
{
    OObj_assert(obj, RBuffer);
    RBuffer *self = obj;
    
    glBindVertexArray(self->gl_vao);
    glBindBuffer(GL_ARRAY_BUFFER, self->gl_vbo);

    glEnableVertexAttribArray(location);
    
    glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE,
                          self->element_size, (void *) offset);
    
    glVertexAttribDivisor(location, 1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    r_error_check("location");
}


void RBuffer_location_mat4(oobj obj, int location, osize offset)
{
    for(int c=0; c<4; c++) {
        RBuffer_location_vec4(obj, location + c, offset + c * sizeof(vec4));
    }
}

