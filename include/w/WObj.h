#ifndef W_WOBJ_H
#define W_WOBJ_H

/**
 * @file WObj.h
 *
 * Object
 *
 * Widget base class.
 * Widgets use the OObj resource tree as a widget tree.
 * So each widget can have multiple children, which will be rendered on top of them (by default, see WBox).
 * The default widget's make use of a WTheme instance (text, buttons, etc.).
 * These default widgets are all rendered into a single batched WTheme draw call.
 * For custom widgets, like displaying an image (RTex), pay attention to the render order.
 * To render all default widgets in a WTheme, call WTheme_render(_ex)
 *
 * Normally, every sub object has the super name as prefix, here WObj,
 *   but this module is a special case and all the sub objects (WText, WButton, ...) all derive from WObj
 *   (all but WObj, WStyle, WTheme of course...)
 *
 * To break the render hierarchy e. g. you want to allocate an WObj (or sub class),
 *      pass another WObj as parent, but DON'T want to auto render / update it, use an OObj in between:
 *      oobj container = OObj_new(another_wobj);
 *      oobj new_wobj = WObj_new(container);        // breaks the recursion update call
 */


#include "o/OObj.h"
#include "r/common.h"
#include "r/rect.h"
#include "a/pointer.h"

/** object id */
#define WObj_ID OObj_ID "WObj"

/**
 * Virtual update function.
 * Should call update the children.
 * @param obj WObj object
 * @param lt left top
 * @param min_size minimal size that may be used for this widget
 * @param theme WTheme object (already overridden with self->theme)
 * @param pointer_fn pointer function to use (already overridden with self->pointer_fn)
 * @return used size
 */
typedef vec2 (*WObj__update_fn)(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn);


/**
 * Virtual list function.
 * @param obj WObj object
 * @param opt_out_num if not NULL, returns the list size/num
 * @return all children of that obj (default is WObj_list_direct)
 */
typedef oobj *(*WObj__list_fn)(oobj obj, osize *opt_out_num);


enum WObj_focus_navigation {
    WObj_focus_CLEAR,
    WObj_focus_NEXT,
    WObj_focus_PREV,
    WObj_focus_NUM_NAVIGATION_MODES
};


typedef struct {
    OObj super;

    // minimal used size
    vec2 min_size;
    
    // overwrites size if >=0 (default -1)
    vec2 fixed_size;

    // ltrb
    vec4 padding;

    bool hide;

    // render in focus mode (to make sense for the user, only a single widget should have focus)
    bool focus;

    // update overrides, if not NULL
    oobj theme;
    
    // either inherit from parent, or self created, or set (not NULL)
    oobj style;
    
    a_pointer__fn pointer_fn;

    // OMap with string keys and string values
    oobj option_map;

    // if true, WObj__alloc_rects will get a noop
    bool ignore_rects_alloc;

    // generated data from update
    struct {
        // used theme
        oobj theme;

        int rects_theme_back_idx;
        int rects_num;

        // actual internal left top and size:
        vec2 lt;
        vec2 used_size;

        // either used_size, or fixed
        vec2 size;

        // with padding border (so padding_size >= size ...):
        vec2 padding_lt;
        vec2 padding_size;
    } gen;

    //
    // vfuncs
    //
    WObj__update_fn v_update;
    WObj__list_fn v_list;
    OObj__event_fn v_style_apply;
    OObj__event_fn v_opt_update_event;
    OObj__event_fn v_opt_focus_active_event;
    OObj__event_fn v_opt_focus_trigger_event;
} WObj;

/**
 * Initializes the object, used by sub classes, so no new...
 * @param obj WObj object
 * @param parent to inherit from
 * @return obj casted as WObj
 */
O_EXTERN
WObj *WObj_init(oobj obj, oobj parent);

/**
 * Creates a new the WObj object
 * @param parent to inherit from
 * @return The new object
 */
O_INLINE
WObj *WObj_new(oobj parent)
{
    OObj_DECL_IMPL_NEW(WObj, parent);
}

//
// virtual implementations
//

/**
 * Default implementation of WObj update.
 * Just calls WObj__update_children_stacked, which updates all children on top of each other on its size.
 *
 * Virtual update function.
 * Should call update on the children.
 * @param obj WObj object
 * @param lt left top
 * @param min_size minimal size that may be used for this widget
 * @param theme WTheme object (already overridden with self->theme)
 * @param pointer_fn pointer function to use (already overridden with self->pointer_fn)
 * @return used size
 */
O_EXTERN
vec2 WObj__v_update(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn);


/**
 * Default implementation of WObj list children.
 * Just calls WObj_list_direct
 *
 * Virtual list function.
 * @param obj WObj object
 * @param opt_out_num if not NULL, returns the list size/num
 * @return all children of that obj (default is WObj_list_direct)
 */
O_EXTERN
oobj *WObj__v_list(oobj obj, osize *opt_out_num);


/**
 * Default implementation (noop) of WObj apply style.
 * @param obj WObj object
 */
O_EXTERN
void WObj__v_style_apply(oobj obj);

//
// object functions
//

/**
 * @param obj WObj object
 * @return size minimum for this WObj, automatically compared with the update return
 * @note actual WObj_gen_size may be larger
 */
OObj_DECL_GETSETREF(WObj, vec2, min_size)

/**
 * @param obj WObj object
 * @return fixed size for this WObj (if >=0, defaults to -1)
 */
OObj_DECL_GETSETREF(WObj, vec2, fixed_size)



/**
 * @param obj WObj object
 * @return padding as vec4_(left, top, right, bottom)
 */
OObj_DECL_GETSETREF(WObj, vec4, padding)

/**
 * @param obj WObj object
 * @return true to create a noop of WObj_update
 */
OObj_DECL_GETSET(WObj, bool, hide)

/**
 * @param obj WObj object
 * @return true if the object is in focus mode
 */
OObj_DECL_GETSET(WObj, bool, focus)

/**
 * @param obj WObj object
 * @return WTheme, that overrides the passed theme, if not NULL
 */
OObj_DECL_GETSET(WObj, oobj, theme)

/**
 * @param obj WObj object
 * @return a_pointer function, that overrides the passed pointer_fn, if not NULL
 */
OObj_DECL_GETSET(WObj, a_pointer__fn , pointer_fn)

/**
 * @param obj WObj object
 * @return OMap options with string keys and string values
 * @note value strings are allocated with this map as parent
 * @sa WObj_option and WObj_option_set
 */
OObj_DECL_GET(WObj, oobj, option_map)

/**
 * @param obj WObj object
 * @return option string value for given key or NULL
 * @note return is a reference to the map data
 */
O_EXTERN
const char *WObj_option(oobj obj, const char *key);

/**
 * @param obj WObj object
 * @param key string
 * @param value string (cloned with WObj_option_map as parent)
 */
O_EXTERN
void WObj_option_set(oobj obj, const char *key, const char *value);


/**
 * Get the direct children of this WObj (allocated on this object)
 * @param obj WObj object
 * @param opt_out_num if not NULL, returns the list size/num
 * @return A null terminated list of the WObj children, call o_free(obj, list) to free it manually
 * @note the list is allocated on obj.
 *       Create an WObj **it=list to traverse through the list
 */
O_INLINE
WObj **WObj_list_direct(oobj obj, osize *opt_out_num)
{
    return (WObj **) OObj_list(obj, opt_out_num, WObj);
}


/**
 * Calls the virtual list function.
 * @param obj WObj object
 * @param opt_out_num if not NULL, returns the list size/num
 * @return A null terminated list of all WObj children, call o_free(obj, list) to free it manually7
 * @note Create an WObj **it=list to traverse through the list
 */
O_INLINE
WObj **WObj_list(oobj obj, osize *opt_out_num)
{
    OObj_assert(obj, WObj);
    WObj *self = obj;
    return (WObj **) self->v_list(self, opt_out_num);
}

/**
 * Calls the virtual style apply function.
 * @param obj WObj object
 */
O_INLINE
void WObj_style_apply(oobj obj)
{
    OObj_assert(obj, WObj);
    WObj *self = obj;
    self->v_style_apply(self);
}

/**
 * @param obj WObj object
 * @return WStyle object. Must be alive as long as this WObj is alive!
 */
OObj_DECL_GET(WObj, oobj, style)

/**
 * @param obj WObj object
 * @return WStyle object. Must be alive as long as this WObj is alive!
 */
O_INLINE
oobj WObj_style_set(oobj obj, oobj style, bool apply)
{
    OObj_assert(obj, WObj);
    WObj *self = obj;
    self->style = style;
    if(apply) {
        WObj_style_apply(self);
    }
    return style;
}

/**
 * @param obj WObj object
 * @return used WTheme object
 */
O_INLINE
oobj WObj_gen_theme(oobj obj)
{
    OObj_assert(obj, WObj);
    WObj *self = obj;
    return self->gen.theme;
}


/**
 * @param obj WObj object
 * @return index of the first rect in the theme array
 */
O_INLINE
int WObj_gen_rects_theme_back_idx(oobj obj)
{
    OObj_assert(obj, WObj);
    WObj *self = obj;
    return self->gen.rects_theme_back_idx;
}

/**
 * @param obj WObj object
 * @return reference to the generated rects, or NULL if not set
 * @note not to be confused with WObj_gen_rect (for u_rect)
 */
O_EXTERN
struct r_rect *WObj_gen_rects(oobj obj);


/**
 * @param obj WObj object
 * @return number of generated rects
 */
O_INLINE
int WObj_gen_rects_num(oobj obj)
{
    OObj_assert(obj, WObj);
    WObj *self = obj;
    return self->gen.rects_num;
}

/**
 * Sets the .a color of the generated rects (do this before rendering with WTheme_render(_ex)...
 * @param obj WObj object
 * @param color color to set
 */
O_EXTERN
void WObj_gen_rects_color_set(oobj obj, vec4 color);


/**
 * @param obj WObj object
 * @return generated left top position
 */
O_INLINE
vec2 WObj_gen_lt(oobj obj)
{
    OObj_assert(obj, WObj);
    WObj *self = obj;
    return self->gen.lt;
}

/**
 * @param obj WObj object
 * @return generated used size, vec2_(0) on hide
 */
O_INLINE
vec2 WObj_gen_used_size(oobj obj)
{
    OObj_assert(obj, WObj);
    WObj *self = obj;
    return self->hide? vec2_(0) : self->gen.used_size;
}

/**
 * @param obj WObj object
 * @return generated used size or fixed, vec2_(0) on hide
 */
O_INLINE
vec2 WObj_gen_size(oobj obj)
{
    OObj_assert(obj, WObj);
    WObj *self = obj;
    return self->hide? vec2_(0) : self->gen.size;
}

/**
 * @param obj WObj object
 * @return a rect like in u_rect, u_rect_new_hidden on hide
 * @note not to be confused with WObj_gen_rects (for r_rect)
 */
O_EXTERN
vec4 WObj_gen_rect(oobj obj);

/**
 * @param obj WObj object
 * @return a pose created with u_pose, according to WObj_gen_lt and WObj_gen_size
 *         u_pose_new_hidden on hide
 */
O_EXTERN
mat4 WObj_gen_pose(oobj obj);


/**
 * @param obj WObj object
 * @return generated left top position, with padding border
 */
O_INLINE
vec2 WObj_gen_padding_lt(oobj obj)
{
    OObj_assert(obj, WObj);
    WObj *self = obj;
    return self->gen.padding_lt;
}

/**
 * @param obj WObj object
 * @return generated used size, with padding border, vec2_(0) on hide
 */
O_INLINE
vec2 WObj_gen_padding_size(oobj obj)
{
    OObj_assert(obj, WObj);
    WObj *self = obj;
    return self->hide? vec2_(0) : self->gen.padding_size;
}



/**
 * @param obj WObj object
 * @param opt_fn focus event function if update is called with active focus,
 *               or NULL (default is NULL)
 */
O_INLINE
void WObj_focus_active_event_fn_set(oobj obj, OObj__event_fn opt_fn)
{
    OObj_assert(obj, WObj);
    WObj *self = obj;
    self->v_opt_focus_active_event = opt_fn;
}


/**
 * @param obj WObj object
 * @param opt_fn focus event function if focus is triggered on this object
 *               or NULL to disable being focusable (default is NULL)
 */
O_INLINE
void WObj_focus_trigger_event_fn_set(oobj obj, OObj__event_fn opt_fn)
{
    OObj_assert(obj, WObj);
    WObj *self = obj;
    self->v_opt_focus_trigger_event = opt_fn;
}

/**
 * @param obj WObj object
 * @return true if the object has a focus event function (default is false)
 */
O_INLINE
bool WObj_focusable(oobj obj)
{
    OObj_assert(obj, WObj);
    WObj *self = obj;
    return self->v_opt_focus_trigger_event != NULL;
}

/**
 * Traverses through the WObj (resource) hierarchy recursivly.
 * Searches the first focusable WObj
 * e. g. focus'ed button clicked with keyboard | gamepad.
 * @param obj WObj object
 * @return the first focusable WObj, or NULL if not found
 */
O_EXTERN
WObj *WObj_focusable_find(oobj obj);

/**
 * Traverses through the WObj (resource) hierarchy recursivly.
 * Searches the first focusable WObj
 * e. g. focus'ed button clicked with keyboard | gamepad.
 * @param obj WObj object
 * @return the first focusable WObj, or NULL if not found
 */
O_EXTERN
WObj *WObj_focusable_find_last(oobj obj);

/**
 * Traverses through the WObj (resource) hierarchy recursivly.
 * Searches the first focusable WObj with focus=true
 * e. g. focus'ed button clicked with keyboard | gamepad.
 * @param obj WObj object
 * @return the first focusable WObj with focus=true, or NULL if not found
 */
O_EXTERN
WObj *WObj_focus_find(oobj obj);

/**
 * Navigate the focus WObj in the hierarchy
 * @param obj WObj object
 * @param navigate mode to navigate
 */
O_EXTERN
void WObj_focus_navigate(oobj obj, enum WObj_focus_navigation navigate);

/**
 * Calls the first focusable virtual optional focus event function, where focus=true
 * @param obj WObj object
 * @return true if an event was found to emit
 * @note uses WObj_focus_find to search
 */
O_EXTERN
bool WObj_focus_trigger_event(oobj obj);


/**
 * Allocates the gen rects from the theme
 * @param obj WObj object
 * @param theme WTheme to allocate on
 * @param num number to allocate
 * @return reference to the generated rects, or NULL if not set
 * @note bool self->ignore_rects_alloc creates a noop and returns the last gen idx
 */
O_EXTERN
struct r_rect *WObj__alloc_rects(oobj obj, oobj theme, int num);

/**
 * Sets the flag ignore_rects_alloc in the full WObj hierarchy
 * @param obj WObj object
 * @param set true to ignore the allocation of WObj__alloc_rects for obj and its children
 */
O_EXTERN
void WObj__ignore_alloc_rects_set(oobj obj, bool set);


/**
 * @param obj WObj object
 * @param opt_fn update event function, which is called after the virtual update function is called.
 *               Or NULL (default)
 */
O_INLINE
void WObj_update_event_fn_set(oobj obj, OObj__event_fn opt_fn)
{
    OObj_assert(obj, WObj);
    WObj *self = obj;
    self->v_opt_update_event = opt_fn;
}

/**
 * Updates the given null terminated list stacked on top of each other on its size.
 * Used for the default implementation of the virtual update function: WObj__v_update.
 * This function should only be called within a replacement for the virtual update function (__)
 * @param list of WObj objects
 * @param list_num list size
 * @param lt left top
 * @param min_size minimal size that may be used for this widget
 * @param theme WTheme object (already overridden with self->theme)
 * @param pointer_fn pointer function to use (already overridden with self->pointer_fn)
 * @return used size of the children
 */
O_EXTERN
vec2 WObj__update_list_stacked(WObj **list, osize list_num, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn);

/**
 * Updates all children on top of each other on its size.
 * Used for the default implementation of the virtual update function: WObj__v_update.
 * This function should only be called within a replacement for the virtual update function (__)
 * @param obj WObj object
 * @param lt left top
 * @param min_size minimal size that may be used for this widget
 * @param theme WTheme object (already overridden with self->theme)
 * @param pointer_fn pointer function to use (already overridden with self->pointer_fn)
 * @return used size of the children
 */
O_EXTERN
vec2 WObj__update_children_stacked(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn);

/**
 * Calls the virtual update function.
 * Updates the widget tree to generate the sizes and respond to events, etc.
 * @param obj RObj object
 * @param lt left top
 * @param min_size minimal size that may be used for this widget
 * @param theme WTheme object to use, may get overridden with self->theme
 * @param pointer_fn pointer function to use, may get overridden with self->pointer_fn
 *                   in most cases "a_pointer" (or "a_pointer_p" if rendered onto the p matrix of the cam)
 *                   asserts not NULL
 * @return used size, compared with min_size and fixed_size
 */
O_EXTERN
vec2 WObj_update(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn);


#endif //W_WOBJ_H
