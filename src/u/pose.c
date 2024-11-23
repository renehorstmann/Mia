#include "u/pose.h"

bool u_pose_aa_intersects_line(mat4 p, vec2 a, vec2 b)
{
    vec2 wh = u_pose_get_wh(p);
    float size = wh.v0 < wh.v1 ? wh.v0 : wh.v1;

    vec2 diff = {{b.x - a.x, b.y - a.y}};
    float dist = m_sqrt(diff.x * diff.x + diff.y * diff.y);

    int checks = m_ceil(dist / size);
    checks = checks <= 0 ? 1 : checks; // min 1

    for (int i = 0; i < checks; i++) {
        vec2 pos = {{a.x + diff.x / checks, a.y + diff.y / checks}};

        if (u_pose_aa_contains(p, pos))
            return true;
    }
    return false;
}
