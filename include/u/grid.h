#ifndef MP_GRID_H
#define MP_GRID_H

/**
 * @file grid.h
 *
 * grid positions for equally sized items
 */

#include "m/types/int.h"
#include "m/vec/vec2.h"

struct u_grid {
    // position of item 0
    vec2 pos;
    // may be negative, stride to next item
    vec2 stride;
    // maximal items in the grid
    int cols, rows;
    // order of items
    bool row_major;
};

/**
 * @param pos_lb outer grid left bottom position (NOT center position of first item)
 * @param size outer size
 * @param item_size size of a single item
 * @param spacing spacing between items
 * @param row_major order
 * @return cached grid data
 */
O_INLINE
struct u_grid u_grid_new_lb(vec2 pos_lb, vec2 size, vec2 item_size, vec2 spacing, bool row_major)
{
    struct u_grid self = {0};
    self.stride = vec2_add_v(item_size, spacing);
    self.cols = (int) m_floor(size.x / self.stride.x);
    self.rows = (int) m_floor(size.y / self.stride.y);
    self.row_major = row_major;
    self.pos = vec2_(pos_lb.x+item_size.x/2, pos_lb.y+item_size.y/2);
    return self;
}


/**
 * @param pos_lb outer grid left top position (NOT center position of first item)
 * @param size outer size
 * @param item_size size of a single item
 * @param spacing spacing between items
 * @param row_major order
 * @return cached grid data
 */
O_INLINE
struct u_grid u_grid_new_lt(vec2 pos_lt, vec2 size, vec2 item_size, vec2 spacing, bool row_major)
{
    struct u_grid self = {0};
    self.stride = vec2_add_v(item_size, spacing);
    self.cols = (int) m_floor(size.x / self.stride.x);
    self.rows = (int) m_floor(size.y / self.stride.y);
    self.row_major = row_major;
    self.pos = vec2_(pos_lt.x+item_size.x/2, pos_lt.y-item_size.y/2);
    self.stride.y *= -1;
    return self;
}


/**
 * @param pos_lb outer grid right bottom position (NOT center position of first item)
 * @param size outer size
 * @param item_size size of a single item
 * @param spacing spacing between items
 * @param row_major order
 * @return cached grid data
 */
O_INLINE
struct u_grid u_grid_new_rb(vec2 pos_rb, vec2 size, vec2 item_size, vec2 spacing, bool row_major)
{
    struct u_grid self = {0};
    self.stride = vec2_add_v(item_size, spacing);
    self.cols = (int) m_floor(size.x / self.stride.x);
    self.rows = (int) m_floor(size.y / self.stride.y);
    self.row_major = row_major;
    self.pos = vec2_(pos_rb.x-item_size.x/2, pos_rb.y+item_size.y/2);
    self.stride.x *= -1;
    return self;
}

/**
 * @param pos_lb outer grid right top position (NOT center position of first item)
 * @param size outer size
 * @param item_size size of a single item
 * @param spacing spacing between items
 * @param row_major order
 * @return cached grid data
 */
O_INLINE
struct u_grid u_grid_new_rt(vec2 pos_rt, vec2 size, vec2 item_size, vec2 spacing, bool row_major)
{
    struct u_grid self = {0};
    self.stride = vec2_add_v(item_size, spacing);
    self.cols = (int) m_floor(size.x / self.stride.x);
    self.rows = (int) m_floor(size.y / self.stride.y);
    self.row_major = row_major;
    self.pos = vec2_(pos_rt.x-item_size.x/2, pos_rt.y-item_size.y/2);
    self.stride.x *= -1;
    self.stride.y *= -1;
    return self;
}

/**
 * @return the maximal number of items in the grid
 */
O_INLINE
int u_grid_num(struct u_grid self) {
    return self.cols * self.rows;
}

/**
 * @return the col and row of an item in the grid
 * @note ignores out of bounds
 */
O_INLINE
ivec2 u_grid_position(struct u_grid self, int idx)
{
    ivec2 position;
    if(self.row_major) {
        position.x = idx%self.cols;
        position.y = idx/self.cols;
    } else {
        position.y = idx%self.rows;
        position.x = idx/self.rows;
    }
    return position;
}


/**
 * @return the center position of an item at a specific col and row
 * @note ignores out of bounds
 */
O_INLINE
vec2 u_grid_at_position(struct u_grid self, int col, int row)
{
    vec2 pos = vec2_scale_v(self.stride, vec2_(col, row));
    pos = vec2_add_v(self.pos, pos);
    return pos;
}

/**
 * @return the center position of an item at a specific item index
 * @note ignores out of bounds
 */
O_INLINE
vec2 u_grid_at(struct u_grid self, int idx)
{
    ivec2 position = u_grid_position(self, idx);
    return u_grid_at_position(self, m_2(position));
}

/**
 * @return the center position of an item at a specific item index
 * @note if out of grid bounds, "out_of_bounds" is returned instead (could be set to vec2_(m_MAX) or smth)
 */
O_INLINE
vec2 u_grid_bounds(struct u_grid self, int idx, vec2 out_of_bounds)
{
    if(idx >= u_grid_num(self)) {
        return out_of_bounds;
    }
    return u_grid_at(self, idx);
}

#endif //MP_GRID_H
