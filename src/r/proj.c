#include "r/proj.h"
#include "m/vec/vec2.h"
#include "m/vec/ivec2.h"
#include "m/utils/camera.h"

struct r_proj r_proj_new(ivec2 viewport_size, vec2 min_units_size, bool pixelperfect)
{
    struct r_proj p = {0};

    p.viewport_size = viewport_size;
    p.vpsh = vec2_(viewport_size.x/2.0f, viewport_size.y/2.0f);
    p.viewport_even_offset = vec2_div(vec2_(viewport_size.x % 2, viewport_size.y % 2), 2.0f);

    if(min_units_size.x<=0 && min_units_size.y<=0) {
        p.size = vec2_(m_2(viewport_size));
        p.scale = 1.0f;
    } else {
        if(min_units_size.x <= 0) {
            p.scale = viewport_size.y / min_units_size.y;
        } else if(min_units_size.y <= 0) {
            p.scale = viewport_size.x / min_units_size.x;
        } else {
            float width_scale = viewport_size.x / min_units_size.x;
            float height_scale = viewport_size.y / min_units_size.y;
            p.scale = o_min(width_scale, height_scale);
        }

        if (pixelperfect && p.scale > 1) {
            p.scale = m_floor(p.scale);
        }
        p.size = vec2_div(vec2_cast_int(viewport_size.v), p.scale);
    }
    
    vec4 bounds = vec4_(
        -p.size.x/2.0f,
        +p.size.x/2.0f,
        -p.size.y/2.0f,
        +p.size.y/2.0f
    );


    p.cam = mat4_camera_ortho(m_4(bounds), -1, 1);

    return p;
}
