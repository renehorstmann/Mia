#include "w/WStack.h"
#include "o/OObj_builder.h"
#include "o/OArray.h"
#include "o/str.h"
#include "m/vec/vec2.h"
#include "w/WWindow.h"
#include "r/RTex.h"

#define O_LOG_LIB "w"
#include "o/log.h"

//
// public
//

int WStack_child_order(oobj obj_child)
{
    const char *string = WObj_option(obj_child, WStack_order_KEY);
    if(!string) {
        return 0;
    }
    int order;
    int parsed = sscanf(string, "%i", &order);
    if(parsed != 1) {
        return 0;
    }
    return order;
}

int WStack_child_order_set(oobj obj_child, int order)
{
    // faster than creating and dumping a json...
    char *string = o_strf(obj_child, "%i", order);
    WObj_option_set(obj_child, WStack_order_KEY, string);
    o_free(obj_child, string);
    return order;
}


WStack *WStack_init(oobj obj, oobj parent)
{
    WObj *super = obj;
    WStack *self = obj;
    o_clear(self, sizeof *self, 1);

    WObj_init(obj, parent);
    OObj_id_set(self, WStack_ID);

    self->window_auto_mode = false;

    // vfuncs
    super->v_update = WStack__v_update;
    super->v_list = WStack__v_list;

    return self;
}

//
// vfuncs
//

vec2 WStack__v_update(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn)
{
    osize list_num;
    WObj **list = WObj_list(obj, &list_num);
    
    vec2 child_size = WObj__update_list_stacked(list, list_num, lt, min_size, theme, pointer_fn);

    if(WStack_window_auto_mode(obj)) {
        osize dragged_idx = -1;
        for(osize i=0; i<list_num; i++) {
            if(OObj_check(list[i], WWindow) && WWindow_dragging(list[i])) {
                dragged_idx = i;
                break;
            }
        }
        if(dragged_idx>=0) {
            for(osize i=0; i<list_num; i++) {
                if(OObj_check(list[i], WWindow)) {
                    WStack_child_order_set(list[i], i==dragged_idx? 1 : 0);
                }
            }
        }
    }

    o_free(obj, list);

    return child_size;
}

oobj *WStack__v_list(oobj obj, osize *opt_out_num)
{
    osize list_num;
    WObj **list = WObj_list_direct(obj, &list_num);

    // ivec2_(order, idx)
    oobj data = OArray_new_dyn(obj, NULL, sizeof(ivec2), 0, list_num);

    for(int idx=0; idx<list_num; idx++) {
        int order = WStack_child_order(list[idx]);

        // if order is bigger than all, it should be set to the last position
        int pos = (int) o_num(data);
        for(int sort_i=0; sort_i<o_num(data); sort_i++) {
            const ivec2 *item = o_at(data, sort_i);
            if(item->v0 > order) {
                pos = sort_i;
                break;
            }
        }

        ivec2 item_set = {{order, idx}};
        OArray_push_at(data, pos, &item_set);
    }

    assert(o_num(data) == list_num);

    // create the resulting sorted list
    WObj **sorted = o_new(obj, WObj *, list_num+1);
    for(int i=0; i<list_num; i++) {
        const ivec2 *item = o_at(data, i);
        sorted[i] = list[item->v1];
    }
    // NULL terminator...
    sorted[list_num] = NULL;

    o_free(obj, list);
    o_del(data);

    o_opt_set(opt_out_num, list_num);
    return (oobj *) sorted;
}

vec2 WStack_update(oobj obj, oobj tex, oobj theme, vec2 *opt_out_lt, a_pointer__fn pointer_fn)
{
    struct r_proj *proj = RTex_proj(tex);
    
     // to get projection size and lt for the window field
    vec2 full_size = vec2_cast_int(r_proj_size_int(proj).v);
    
    vec2 lt = vec2_(r_proj_left_int(proj), r_proj_top_int(proj));
    
    o_opt_set(opt_out_lt, lt);
    
    return WObj_update(obj, lt, full_size, theme, pointer_fn);
}
