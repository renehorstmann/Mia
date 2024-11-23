#include "r/RBufferRect.h"
#include "r/rect.h"

//
// public
//

RBufferRect *RBufferRect_init(oobj obj, oobj parent)
{
    RBufferRect *self = obj;

    RBuffer_init(self, parent, sizeof (struct r_rect));

    const int in_rect = 0;
    const int in_uv_rect = 1;
    const int in_s = 2;
    const int in_t = 3;

    RBuffer_location_vec4(self, in_rect, offsetof(struct r_rect, rect));
    RBuffer_location_vec4(self, in_uv_rect, offsetof(struct r_rect, uv_rect));
    RBuffer_location_vec4(self, in_s, offsetof(struct r_rect, s));
    RBuffer_location_vec4(self, in_t, offsetof(struct r_rect, t));

    return self;
}
