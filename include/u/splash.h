#ifndef U_SPLASH_H
#define U_SPLASH_H

/**
 * @file splash.h
 *
 * Creates an AScene as splashscreen.
 * After a few frames, the actual scene is created and their scene stack positions are switched.
 * So if the actual scene blocks during loadup, the (static) splash screen keeps showing
 */

#include "o/common.h"
#include "m/types/flt.h"

/**
 * @param splash_view AScene object created with u_splash_new*
 * @return the AScene object to be created "actual", or NULL if not created yet
 */
O_EXTERN
struct oobj_opt u_splash_actual_scene(oobj splash_view);

/**
 * Creates a splash scene, which involves the creation of the actual scene internally
 * @param parent to allocate on
 * @param splash_view AView for the splashscreen
 * @param move_splash_view if true, moved into the created AScene
 * @param actual_view AView to create an AScene from
 * @param move_actual_view_in_its_scene if true, actual_view is moved into the created actual scene
 * @param time_until_actual_scene_creation if this time has ended, the actual scene is created (0 would be next frame)
 * @return AScene of the splash screen
 */
O_EXTERN
oobj u_splash_new(oobj parent,
                  oobj splash_view, bool move_splash_view,
                  oobj actual_view, bool move_actual_view_in_its_scene,
                  float time_until_actual_scene_creation);


/**
 * Creates a splash scene, which involves the creation of the actual scene internally.
 * Mia version which just displays a text for the author, like "HORSIMANN".
 * May be changed in the future...
 * @param parent to allocate on
 * @param actual_view AView to create an AScene from
 * @param move_actual_view_in_its_scene if true, actual_view is moved into the created actual scene
 * @param bg_color for the full background (RTex_clear_full)
 * @param text_color for the author text
 * @paragraph author text to be displayed
 * @param min_time minimal time this splash is running (more if actual scene blocks while loading)
 * @param fade_time last part of the splash is linear fading away in that time span
 * @return AScene of the splash screen
 */
O_EXTERN
oobj u_splash_new_mia(oobj parent,
                      oobj actual_view, bool move_actual_view_in_its_scene,
                      vec4 bg_color, vec4 text_color, const char *author, float min_time, float fade_time);


#endif //U_SPLASH_H
