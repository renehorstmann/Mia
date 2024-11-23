#ifndef X_XVIEWKEYS_H
#define X_XVIEWKEYS_H

/**
 * @file XViewKeys.h
 *
 * object.
 *
 * AView which shows a virtual keyboard.
 *
 * Subclass of the AView object.
 */

#include "m/vec/vec2.h"
#include "a/AView.h"


/** object id */
#define XViewKeys_ID AView_ID "XViewKeys"


enum XViewKeys_mode {
    XViewKeys_ASCII,
    XViewKeys_NUM,
    XViewKeys_NUM_MODES
};


typedef struct {
    AView AView;

    enum XViewKeys_mode mode;
    enum XViewKeys_mode prev_mode;

    bool ascii_txt;
    bool ascii_upper;
    bool ascii_single_shift;
    bool ascii_alt_first_page;

    // WTheme and WGrid for gui
    oobj theme;
    oobj gui;

    // OStreamArray from a_input_key_stream
    oobj real_key_stream;
    
    // resulting FIFO OStreamArray to read on (writing pushs back new keys...)
    oobj stream;
} XViewKeys;


/**
 * Initializes the object.
 * Creates an AView that renders a virtual keyboard.
 * @param obj XViewKeys object
 * @param parent to inherit from
 * @return obj casted as XViewKeys
 */
O_EXTERN
XViewKeys *XViewKeys_init(oobj obj, oobj parent);

/**
 * Creates a new the OArray object
 * Creates an AView that renders a virtual keyboard.
 * @param parent to inherit from
 * @return The new object
 */
O_INLINE
XViewKeys *XViewKeys_new(oobj parent)
{
    OObj_DECL_IMPL_NEW(XViewKeys, parent);
}


//
// virtual implementations
//

O_EXTERN
void XViewKeys__v_setup(oobj view);

O_EXTERN
void XViewKeys__v_update(oobj view, oobj tex, float dt);

O_EXTERN
void XViewKeys__v_render(oobj view, oobj tex, float dt);


//
// object functions:
//


/**
 * @param obj XViewKeys object
 * @return keyboard mode
 */
OObj_DECL_GETSET(XViewKeys, enum XViewKeys_mode, mode)


/**
 * @param obj XViewKeys object
 * @return resulting FIFO OStreamArray to read on (writing pushs back new keys...)
 * @note stream may contain "\b" for backspace or escape codes like "\x1B[A" for cursor Up
 *       escaped codes may be of:
 *       "\x1B[A" Cursor Up
 *       "\x1B[B" Cursor Down
 *       "\x1B[C" Cursor Right
 *       "\x1B[D" Cursor Left
 */
OObj_DECL_GET(XViewKeys, oobj, stream)


#endif //X_XVIEWKEYS_H
