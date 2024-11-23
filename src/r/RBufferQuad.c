#include "r/RBufferQuad.h"
#include "r/quad.h"

//
// public
//

RBufferQuad *RBufferQuad_init(oobj obj, oobj parent)
{
    RBufferQuad *self = obj;

    RBuffer_init(self, parent, sizeof (struct r_quad));

    const int in_pose = 0;
    const int in_uv = 4;
    const int in_stuv = 8;

    RBuffer_location_mat4(self, in_pose, offsetof(struct r_quad, pose));
    RBuffer_location_mat4(self, in_uv, offsetof(struct r_quad, uv));
    RBuffer_location_mat4(self, in_stuv, offsetof(struct r_quad, stuv));

    return self;
}
