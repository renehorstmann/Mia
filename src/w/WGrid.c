#include "w/WGrid.h"
#include "o/OObj_builder.h"
#include "o/str.h"
#include "o/OJson.h"
#include "m/vec/vec2.h"
#include "m/vec/ivec2.h"
#include "m/io/ivec4.h"

#include "m/io/vecn.h"

#define O_LOG_LIB "w"
#include "o/log.h"

O_STATIC
vec4 update_child(WGrid *self, oobj child, vec2 cell_lt, vec2 cell_size, oobj theme, a_pointer__fn pointer_fn)
{
    const char *option_h = WObj_option(child, WGrid_align_KEY_H);
    const char *option_v = WObj_option(child, WGrid_align_KEY_V);

    enum WGrid_align_mode mode_h = self->align_h;
    enum WGrid_align_mode mode_v = self->align_v;

    if(option_h || option_v) {
        for (int i = 0; i < WGrid_align_NUM_MODES; i++) {
            if (o_str_equals(option_h, WGrid_align_VALUES[i])) {
                mode_h = i;
            }
            if (o_str_equals(option_v, WGrid_align_VALUES[i])) {
                mode_v = i;
            }
        }
    }

    vec2 prev_size = WObj_gen_size(child);

    vec2 room = vec2_sub_v(cell_size, prev_size);
    room = vec2_max(room, 0);
    vec2 min_size = vec2_(0);

    switch(mode_h) {
        default:
        case WGrid_align_START:
            // nothing to do
            break;
        case WGrid_align_CENTER:
            cell_lt.x += m_floor(room.x/2);
            break;
        case WGrid_align_END:
            cell_lt.x += room.x;
            break;
        case WGrid_align_FIT:
            min_size.x = cell_size.x;
            break;
    }

    switch(mode_v) {
        default:
        case WGrid_align_START:
            // nothing to do
            break;
        case WGrid_align_CENTER:
            cell_lt.y -= m_floor(room.y/2);
            break;
        case WGrid_align_END:
            cell_lt.y -= room.y;
            break;
        case WGrid_align_FIT:
            min_size.y = cell_size.y;
            break;
    }

    vec2 child_size = WObj_update(child, cell_lt, min_size, theme, pointer_fn);
    return vec4_(m_2(cell_lt), m_2(child_size));
}

O_STATIC
vec4 get_cell_lt_size(WGrid *self, vec2 lt, int col, int row, int w, int h,
                      const float *col_sizes, const float *row_sizes) {
    lt.x += vecn_sum(col_sizes, col);
    lt.y -= vecn_sum(row_sizes, row);

    vec2 size;
    size.x = vecn_sum(col_sizes+col, w);
    size.y = vecn_sum(row_sizes+row, h);
    return vec4_(m_2(lt), m_2(size));
}

O_STATIC
vec2 get_cell_child_size(WGrid *self, vec2 child_size, int col, int row, int w, int h,
                         const float *col_sizes, const float *row_sizes)
{
    child_size.x -= vecn_sum(col_sizes + col, w-1);
    child_size.y -= vecn_sum(row_sizes + row, h-1);
    return child_size;
}

//
// public
//

ivec4 WGrid_child_cell(oobj obj_child)
{
    const char *string = WObj_option(obj_child, WGrid_cell_KEY);
    if(!string) {
        return ivec4_(0, 0, 1, 1);
    }
    ivec4 position;
    int consumed;
    int parsed = sscanf(string, "[%i,%i,%i,%i]%n",
                        &position.x, &position.y, &position.v2, &position.v3,
                        &consumed);
    if(parsed != 4 || consumed==0
       || position.x<0 || position.y<0
       || position.v2<=0 || position.v3<=0) {
        return ivec4_(0, 0, 1, 1);
    }
    return position;
}

ivec4 WGrid_child_cell_set(oobj obj_child, ivec4 cell)
{
    // faster than creating and dumping a json...
    char *string = o_strf(obj_child, "[%i,%i,%i,%i]", m_4(cell));
    WObj_option_set(obj_child, WGrid_cell_KEY, string);
    o_free(obj_child, string);
    return cell;
}


WGrid *WGrid_init(oobj obj, oobj parent, int cols, int rows, vec2 cell_size)
{
    WObj *super = obj;
    WGrid *self = obj;
    o_clear(self, sizeof *self, 1);

    WObj_init(obj, parent);
    OObj_id_set(self, WGrid_ID);

    assert(cols>0 && rows>0);

    self->col_sizes = o_new0(self, float, (cols + rows)*3);
    self->col_used_sizes = self->col_sizes + cols;
    self->col_weights = self->col_used_sizes + cols;
    self->row_sizes = self->col_weights + cols;
    self->row_used_sizes = self->row_sizes + rows;
    self->row_weights = self->row_used_sizes + rows;
    self->cols = cols;
    self->rows = rows;

    vecn_set(self->col_sizes, cell_size.x, cols);
    vecn_set(self->row_sizes, cell_size.y, rows);

    self->align_h = self->align_v = WGrid_align_START;

    // vfuncs
    super->v_update = WGrid__v_update;

    return self;
}

//
// vfuncs
//


vec2 WGrid__v_update(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn)
{
    OObj_assert(obj, WGrid);
    WGrid *self = obj;

    oobj container = OObj_new(self);

    WObj **list = WObj_list_direct(self, NULL);
    o_mem_move(self, container, list);


    float *col_sizes = o_new0(container, float, self->cols + self->rows);
    float *row_sizes = col_sizes + self->cols;


    // setup col and row sizes
    {
        float weight_sum = 0;
        float room = min_size.x;

        // default fixed stuff (got size or weight is 0)
        for(int i=0; i<self->cols; i++) {
            if(self->col_sizes[i] >= 0) {
                // fixed
                col_sizes[i] = self->col_sizes[i];
                room -= self->col_sizes[i];
            } else if (self->col_weights[i] <= 0) {
                // using last size
                col_sizes[i] = self->col_used_sizes[i];
                room -= self->col_used_sizes[i];
            } else {
                // weighted
                col_sizes[i] = 0;
                weight_sum += self->col_weights[i];
            }
        }
        if(room > 0 && weight_sum > 0) {
            for(int i=0; i<self->cols; i++) {
                if(self->col_sizes[i] < 0 && self->col_weights[i] > 0) {
                    col_sizes[i] = room * self->col_weights[i] / weight_sum;
                    col_sizes[i] = m_floor(col_sizes[i]);
                }
            }
        }
    }
    {
        float weight_sum = 0;
        float room = min_size.y;

        // default fixed stuff (got size or weight is 0)
        for(int i=0; i<self->rows; i++) {
            if(self->row_sizes[i] >= 0) {
                // fixed
                row_sizes[i] = self->row_sizes[i];
                room -= self->row_sizes[i];
            } else if (self->row_weights[i] <= 0) {
                // using last size
                row_sizes[i] = self->row_used_sizes[i];
                room -= self->row_used_sizes[i];
            } else {
                // weighted
                row_sizes[i] = 0;
                weight_sum += self->row_weights[i];
            }
        }
        if(room > 0) {
            for(int i=0; i<self->rows; i++) {
                if(self->row_sizes[i] < 0 && self->row_weights[i] > 0) {
                    row_sizes[i] = room * self->row_weights[i] / weight_sum;
                    row_sizes[i] = m_floor(row_sizes[i]);
                }
            }
        }
    }

    // reset used sizes
    vecn_set(self->col_used_sizes, 0, self->cols);
    vecn_set(self->row_used_sizes, 0, self->rows);

    vec2 max_rb = lt;

    WObj **it = list;
    while(*it) {
        oobj child = *it++;

        // get cell position
        ivec4 position = WGrid_child_cell(child);

        position.xy = ivec2_clamp_v(position.xy, ivec2_(0), ivec2_(self->cols-1, self->rows-1));
        ivec2 max_extend = ivec2_sub_v(ivec2_(self->cols, self->rows), position.xy);
        position.zw = ivec2_clamp_v(position.zw, ivec2_(1), max_extend);

        vec4 cell_lt_size = get_cell_lt_size(self, lt, m_4(position), col_sizes, row_sizes);

        vec4 child_lt_size = update_child(self, child, cell_lt_size.xy, cell_lt_size.zw, theme, pointer_fn);

        vec2 rb = vec2_(child_lt_size.x + child_lt_size.v2,
                        child_lt_size.y - child_lt_size.v3);

        max_rb.x = o_max(max_rb.x, rb.x);
        max_rb.y = o_min(max_rb.y, rb.y);

        vec2 cell_child_size = get_cell_child_size(self, child_lt_size.zw, m_4(position), col_sizes, row_sizes);
        self->col_used_sizes[position.x+position.v2-1] =
                o_max(self->col_used_sizes[position.x+position.v2-1], cell_child_size.x);
        self->row_used_sizes[position.y+position.v3-1] =
                o_max(self->row_used_sizes[position.y+position.v3-1], cell_child_size.y);
    }

    o_del(container);

    vec2 grid_used_child_size = vec2_(max_rb.x - lt.x, lt.y - max_rb.y);
    vec2 grid_cell_sum_size;
    grid_cell_sum_size.x = vecn_sum(self->col_sizes, self->cols);
    grid_cell_sum_size.y = vecn_sum(self->row_sizes, self->rows);

    vec2 full_size = vec2_max_v(grid_used_child_size, grid_cell_sum_size);

    return full_size;
}


void WGrid_resize(oobj obj, int cols, int rows)
{
    OObj_assert(obj, WGrid);
    WGrid *self = obj;
    if(cols == self->cols && rows == self->rows) {
        // noop
        return;
    }

    float *col_sizes = o_new(self, float, (cols + rows)*3);
    float *col_used_sizes = col_sizes + cols;
    float *col_weights = col_used_sizes + cols;
    float *row_sizes = col_used_sizes + cols;
    float *row_used_sizes = row_sizes + rows;
    float *row_weights = row_used_sizes + rows;

    vecn_clone(col_sizes, self->col_sizes, o_min(cols, self->cols));
    vecn_clone(col_used_sizes, self->col_used_sizes, o_min(cols, self->cols));
    vecn_clone(col_weights, self->col_weights, o_min(cols, self->cols));
    vecn_clone(row_sizes, self->row_sizes, o_min(rows, self->rows));
    vecn_clone(row_used_sizes, self->row_used_sizes, o_min(rows, self->rows));
    vecn_clone(row_weights, self->row_weights, o_min(rows, self->rows));

    o_free(self, self->col_sizes);

    self->col_sizes = col_sizes;
    self->row_sizes = row_sizes;
    self->col_used_sizes = col_used_sizes;
    self->row_used_sizes = row_used_sizes;
    self->col_weights = col_weights;
    self->row_weights = row_weights;
    self->cols = cols;
    self->rows = rows;
}
