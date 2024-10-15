#include "u/waveform.h"
#include "o/OArray.h"
#include "r/RTex.h"
#include "r/tex.h"
#include "r/RShaderRect.h"
#include "u/rect.h"

oobj u_waveform_rects(oobj parent, const float *audio_data, int channel, int channels_num, int ticks,
                      vec4 rect, float thickness, struct r_rect init)
{
    int samples = (int) m_ceil(rect.v2 / thickness);
    if(samples <= 0 || ticks <= 0) {
        return OArray_new(parent, NULL, sizeof(struct r_rect), 0);
    }
    oobj array = OArray_new(parent, NULL, sizeof(struct r_rect), samples);
    int prev_tick = 0;
    for(int i=0; i<samples; i++) {
        struct r_rect *r = o_at(array, i);
        int tick = (int) ((osize) (i+1) * ticks / samples);

        float min = m_MAX;
        float max = m_MIN;
        for(int t=prev_tick; t<tick; t++) {
            float sample = audio_data[t*channels_num + channel];
            min = o_min(min, sample);
            max = o_max(max, sample);
        }
        prev_tick = tick;


        // range from min to max is 2.0f
        float h = (max - min) * rect.v3 * 0.5f;
        h = o_max(h, thickness);
        float center = (max+min) * 0.5f * rect.v3 * 0.5f;

        *r = init;
        r->rect = vec4_(
                u_rect_get_left(rect) + thickness * (i+0.5f),
                rect.y + center,
                thickness,
                h
                );
    }
    return array;
}

void u_waveform_render(oobj tex, const float *audio_data, int channel, int channels_num, int ticks,
                       vec4 rect, float thickness, vec4 color)
{
    if(rect.v2<=0) {
        rect.x = 0;
        rect.v2 = RTex_size(tex).x;
    }
    if(rect.v3<=0) {
        rect.y = 0;
        rect.v3 = RTex_size(tex).y;
    }
    oobj container = OObj_new(tex);

    struct r_rect init = r_rect_new(1, 1);
    init.s = color;
    oobj rects = u_waveform_rects(container, audio_data, channel, channels_num, ticks, rect, thickness, init);

    oobj shader = RShaderRect_new_color(container, r_tex_white(), false);
    RTex_rects(tex, shader, o_at(rects, 0), o_num(rects));

    o_del(container);
}
