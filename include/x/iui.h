#ifndef X_IUI_H
#define X_IUI_H

/**
 * @file iui.h
 *
 * Immediate user interface.
 * Creates, uses and may destroy w widgets.
 * Useful for testing and setting values at runtime.
 * Makes use of the current scenes loop list.
 */

#include "o/common.h"
#include "m/types/flt.h"

/**
 * Function interface to create a custom iui widget.
 * @param wobj_parent is the for this custom iui widget created parent (WObj or WWindow)
 * @param user_data passed from the functions
 */
typedef void (*x_iui_custom__fn)(oobj wobj_parent, void *user_data);


/**
 * Creates a WObj iui item.
 * @param title used as identifier in an internal OMap
 * @param setup_fn called once
 * @param update_fn called each time this function is called
 * @param user_data passed to the custom functions
 * @sa x_iui_custom_window to create a WWindow as base widget
 */
O_EXTERN
void x_iui_custom(const char *title, x_iui_custom__fn setup_fn, x_iui_custom__fn update_fn, void *user_data);

/**
 * Creates a WWindow iui item.
 * @param title used as title and identifier in an internal OMap
 * @param setup_fn called once
 * @param update_fn called each time this function is called
 * @param user_data passed to the custom functions
 */
O_EXTERN
void x_iui_custom_window(const char *title, x_iui_custom__fn setup_fn, x_iui_custom__fn update_fn, void *user_data);

/**
 * @param title identifier of an internal OMap
 * @return the base WWObj or WWindow, if available.
 */
O_EXTERN
struct oobj_opt x_iui_wobj(const char *title);


/**
 * @return default window left top padding (defaults to 0, 0)
 * @note different for each AView
 */
O_EXTERN
vec2 x_iui_init_lt(void);

/**
 * @return default window left top padding (defaults to 0, 0)
 * @note different for each AView
 */
O_EXTERN
vec2 x_iui_init_lt_set(vec2 set);


/**
 * @param title window title and identifier of an internal OMap
 * @param val copied on setup, set on each call
 * @param min, max range for the WNum
 * @note want to get the internal WNum? use OObj_find(x_iui_wobj(...), WNum, NULL, oi32_MAX);
 */
O_EXTERN
void x_iui_float(const char *title, float *val, float min, float max);

/**
 * @param title window title and identifier of an internal OMap
 * @param val copied on setup, set on each call
 * @param min, max range for the WNum
 * @note want to get the internal WNum? use OObj_find(x_iui_wobj(...), WNum, "X", oi32_MAX);   (or "Y" ...)
 */
O_EXTERN
void x_iui_vec2(const char *title, vec2 *val, vec2 min, vec2 max);

/**
 * @param title window title and identifier of an internal OMap
 * @param val copied on setup, set on each call
 * @param min, max range for the WNum
 * @note want to get the internal WNum? use OObj_find(x_iui_wobj(...), WNum, "X", oi32_MAX);   (or "Y" ...)
 */
O_EXTERN
void x_iui_vec3(const char *title, vec3 *val, vec3 min, vec3 max);

/**
 * @param title window title and identifier of an internal OMap
 * @param val copied on setup, set on each call
 * @param min, max range for the WNum
 * @note want to get the internal WNum? use OObj_find(x_iui_wobj(...), WNum, "X", oi32_MAX);   (or "Y" ...)
 */
O_EXTERN
void x_iui_vec4(const char *title, vec4 *val, vec4 min, vec4 max);

/**
 * @param title window title and identifier of an internal OMap
 * @param val copied on setup, set on each call
 * @note want to get the internal XWObjColor? use OObj_find(x_iui_wobj(...), XWObjColor, NULL, oi32_MAX);
 */
O_EXTERN
void x_iui_rgb(const char *title, vec3 *val);

/**
 * @param title window title and identifier of an internal OMap
 * @param val copied on setup, set on each call
 * @note want to get the internal XWObjColor? use OObj_find(x_iui_wobj(...), XWObjColor, NULL, oi32_MAX);
 */
O_EXTERN
void x_iui_rgba(const char *title, vec4 *val);





#endif //X_IUI_H
