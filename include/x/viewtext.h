#ifndef X_VIEWTEXT_H
#define X_VIEWTEXT_H

/**
 * @file viewtext.h
 *
 * Create a scene with a XViewText for user text input
 */

#include "o/OObj.h"



/**
 * Creates a full opaque scene, which shows XViewText
 * @param parent to inherit from
 * @param title above the text edit line
 * @param max_chars maximal characters for the text line
 * @param on_ok_event called when ok is clicked
 * @param opt_on_cancel_event if not NULL, shows the cancel btn and called when clicked
 * @return AScene object
 * @note the passed object to the event functions is the created and returned AScene
 */
O_EXTERN
oobj x_viewtext_scene(oobj parent, const char *title, int max_chars, OObj__event_fn on_ok_event, OObj__event_fn opt_on_cancel_event);




#endif //X_VIEWTEXT_H
