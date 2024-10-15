#ifndef X_XVIEWTEXT_H
#define X_XVIEWTEXT_H

/**
 * @file XViewText.h
 *
 * object.
 *
 * AView which shows a XViewKeys (virtual keyboard) at the bottom and an editable text line at top.
 * Only text without newlines are supported (single line)
 */

#include "m/vec/vec2.h"
#include "a/AView.h"


// for a camera of 180 width
#define XViewText_DEFAULT_MAX 40

/** object id */
#define XViewText_ID AView_ID "XViewText"

enum XViewText_state {
    XViewText_RUNNING,
    XViewText_CANCEL,
    XViewText_OK,
    XViewText_NUM_STATES
};

typedef struct {
    AView AView;

    enum XViewText_state state;

    // XViewKeys
    oobj viewkeys;

    // OArray of chars for the text
    oobj text_array;

    int cursor;
    float cursor_blink_time;

    char *ignore_filter;

    int max_chars;

    // background color
    vec4 bg;

    // may be NULL
    OObj__event_fn event;

    // widgets
    struct {
        oobj theme;

        oobj gui;
        oobj bg;
        oobj custom_box_container;
        oobj custom_box;
        oobj text;
        oobj cursor;
        oobj clear;
        oobj copy;
        oobj paste;
        oobj cancel;
        oobj ok;
    } w;

} XViewText;


/**
 * Initializes the object.
 * Creates an AView that renders a virtual keyboard.
 * @param obj XViewText object
 * @param parent to inherit from
 * @param title above the text edit line
 * @param show_cancel if true, enables a cancel button
 * @param max_chars maximal characters for the text line
 * @return obj casted as XViewText
 */
O_EXTERN
XViewText *XViewText_init(oobj obj, oobj parent, const char *title, bool show_cancel, int max_chars);

/**
 * Creates a new the OArray object
 * Creates an AView that renders a virtual keyboard.
 * @param parent to inherit from
 * @param title above the text edit line
 * @param show_cancel if true, enables a cancel button
 * @param max_chars maximal characters for the text line
 * @return The new object
 */
O_INLINE
XViewText *XViewText_new(oobj parent, const char *title, bool show_cancel, int max_chars)
{
    OObj_DECL_IMPL_NEW(XViewText, parent, title, show_cancel, max_chars);
}


//
// virtual implementations
//

O_EXTERN
void XViewText__v_setup(oobj view);

O_EXTERN
void XViewText__v_update(oobj view, oobj tex, float dt);

O_EXTERN
void XViewText__v_render(oobj view, oobj tex, float dt);


//
// object functions:
//


/**
 * @param obj XViewText object
 * @return current state
 */
OObj_DECL_GET(XViewText, enum XViewText_state, state)


/**
 * @param obj XViewText object
 * @return XViewKeys object
 */
OObj_DECL_GET(XViewText, oobj, viewkeys)


/**
 * @param obj XViewText object
 * @return OArray object of the current text line
 */
OObj_DECL_GET(XViewText, oobj, text_array)


/**
 * @param obj XViewText object
 * @return current cursor position
 */
OObj_DECL_GETSET(XViewText, int, cursor)


/**
 * @param obj XViewText object
 * @return maximal text size
 */
OObj_DECL_GETSET(XViewText, int, max_chars)


/**
 * @param obj XViewText object
 * @return null terminated string with characters to ignore from the stream
 */
OObj_DECL_GETSET(XViewText, char *, ignore_filter)


/**
 * @param obj XViewText object
 * @return background color
 */
OObj_DECL_GETSET(XViewText, vec4, bg)

/**
 * @param obj XViewText object
 * @return event that is triggered on cancel or ok clicked, may be NULL
 */
OObj_DECL_SET(XViewText, OObj__event_fn, event)


/**
 * Clears all custom buttons created by XViewText_custom_replace and XViewText_custom_append
 * @param obj XViewText object
 */
O_EXTERN
void XViewText_custom_clear(oobj obj);

/**
 * Add a custom button for a quick replacement
 * @param obj XViewText object
 * @param text will replace the text_array with
 * @param opt_color if not NULL, will be set as WBtn color
 */
O_EXTERN
void XViewText_custom_replace(oobj obj, const char *text, const vec4 *opt_color);

/**
 * Add a custom button for a quick append
 * @param obj XViewText object
 * @param text will append the text_array with
 * @param opt_color if not NULL, will be set as WBtn color
 */
O_EXTERN
void XViewText_custom_append(oobj obj, const char *text, const vec4 *opt_color);


#endif //X_XVIEWTEXT_H
