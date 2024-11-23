#include "u/atlas.h"
#include "o/OObj.h"
#include "m/vec/vec2.h"
#include "r/RTex.h"


struct u_atlas u_atlas_new(oobj obj, int num_sprites)
{
    struct u_atlas self = {0};
    self.rects = o_new0(obj, *self.rects, num_sprites);
    self.num = num_sprites;
    self.wrap_mode = u_atlas_WRAP_ASSERT;
    self.parent = obj;
    return self;
}

struct u_atlas u_atlas_new_grid(oobj obj, vec2 size, int num_sprites,
                                vec2 pos_0, vec2 offset, bool row_major, int block)
{
    struct u_atlas self = u_atlas_new(obj, num_sprites);
    u_atlas_set_size(self, size);
    u_atlas_set_grid(self, pos_0, offset, row_major, block);
    return self;
}

struct u_atlas u_atlas_new_fill(oobj obj, vec2 size, int cols, int rows, enum u_atlas_start start, bool row_major)
{
    assert(cols>=1 && rows>=1);
    vec2 tex_size = {{size.x*(float)cols, size.y*(float)rows}};
    vec2 pos0, offset;
    switch (start) {
        default:
        case u_atlas_START_LB:
            pos0 = vec2_div(vec2_(-tex_size.x+size.x, -tex_size.y+size.y), 2.0f);
            offset = vec2_(size.x, size.y);
            break;
        case u_atlas_START_RB:
            pos0 = vec2_div(vec2_(+tex_size.x-size.x, -tex_size.y+size.y), 2.0f);
            offset = vec2_(-size.x, size.y);
            break;
        case u_atlas_START_LT:
            pos0 = vec2_div(vec2_(-tex_size.x+size.x, +tex_size.y-size.y), 2.0f);
            offset = vec2_(size.x, -size.y);
            break;
        case u_atlas_START_RT:
            pos0 = vec2_div(vec2_(+tex_size.x-size.x, +tex_size.y-size.y), 2.0f);
            offset = vec2_(-size.x, -size.y);
            break;
    }
    int block = row_major? cols : rows;
    return u_atlas_new_grid(obj, size, cols * rows, pos0, offset, row_major, block);
}

struct u_atlas u_atlas_new_fill_tex(oobj obj, oobj tex, int cols, int rows, enum u_atlas_start start, bool row_major)
{
    vec2 tex_size = RTex_size(tex);
    vec2 size = vec2_div_v(tex_size, vec2_(cols, rows));
    return u_atlas_new_fill(obj, size, cols, rows, start, row_major);
}

void u_atlas_move(struct u_atlas *self, oobj into)
{
    o_mem_move(self->parent, into, self->rects);
    self->parent = into;
}

void u_atlas_free(struct u_atlas *self)
{
    o_free(self->parent, self->rects);
    *self = (struct u_atlas) {0};
}

void u_atlas_set_size(struct u_atlas self, vec2 size)
{
    for(int i=0; i<self.num; i++) {
        self.rects[i].zw = size;
    }
}

void u_atlas_set_grid(struct u_atlas self, vec2 pos_0, vec2 offset, bool row_major, int block)
{
    for(int i=0; i<self.num; i++) {
        int c, r;
        if(row_major) {
            c = i % block;
            r = i / block;
        } else {
            c = i / block;
            r = i % block;
        }
        self.rects[i].xy = vec2_add_scaled_v(pos_0, offset, vec2_(c, r));
    }
}
