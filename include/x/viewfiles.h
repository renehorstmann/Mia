#ifndef X_VIEWFILES_H
#define X_VIEWFILES_H

/**
 * @file viewfiles.h
 *
 * Create a scene with a XViewText for user text input
 */

#include "o/OObj.h"



/**
 * Creates a full opaque scene, which shows XViewFiles
 * @param parent to inherit from
 * @param title for a simple title text at top
 * @param on_ok_event called when ok is clicked
 * @param opt_on_cancel_event called when cancel is clicked, if not NULL
 * @return AScene object
 */
O_EXTERN
oobj x_viewfiles_scene(oobj parent, const char *title, OObj__event_fn on_ok_event, OObj__event_fn opt_on_cancel_event);




#endif //X_VIEWFILES_H
