#ifndef A_INPUT_H
#define A_INPUT_H

/**
 * @file input.h
 *
 * inputs like mouse wheel, keys, gamepad.
 *      Note: for pointer events, see pointer.h
 */

#include "o/common.h"
#include "m/types/int.h"


#define A_INPUT_ESCAPE_UP       "\x1B[A"
#define A_INPUT_ESCAPE_DOWN     "\x1B[B"
#define A_INPUT_ESCAPE_RIGHT    "\x1B[C"
#define A_INPUT_ESCAPE_LEFT     "\x1B[D"

#define a_input_key_stream_OOBJ_NAME "a_input_key_stream"

enum a_input_keys {
    A_INPUT_ENTER,
    A_INPUT_SPACE,
    A_INPUT_ESCAPE,
    A_INPUT_BACK,
    A_INPUT_UP,
    A_INPUT_LEFT,
    A_INPUT_DOWN,
    A_INPUT_RIGHT,
    A_INPUT_W,
    A_INPUT_A,
    A_INPUT_S,
    A_INPUT_D,
    A_INPUT_NUM_KEYS
};

/**
 * @param key one of the default keys
 * @return true if currently pressed down
 */
O_EXTERN
bool a_input_is_pressed(enum a_input_keys key);

/**
 * @param key one of the default keys
 * @return true if got pressed in this frame
 */
O_EXTERN
bool a_input_pressed(enum a_input_keys key);

/**
 * @param key one of the default keys
 * @return true if got released in this frame
 */
O_EXTERN
bool a_input_released(enum a_input_keys key);

/**
 * @return the mouse wheel movement or ivec2_(0) if not used
 */
O_EXTERN
ivec2 a_input_wheel(void);


/**
 * @param join_parent OJoin object to create an OWeakjoin on
 * @return resulting FIFO OStreamArray to read on (writing pushs back new keys...)
 * @note stream may contain "\b" for backspace or escape codes like "\x1B[A" for cursor Up
 *       escaped codes may be of:
 *       "\x1B[A" Cursor Up
 *       "\x1B[B" Cursor Down
 *       "\x1B[C" Cursor Right
 *       "\x1B[D" Cursor Left
 *       not all special characters work! only a handful and the alphanumerics
 */
O_EXTERN
oobj a_input_key_stream_join(oobj join_parent);

/**
 * @param parent OObj object to create an OWeakjoin on
 * @return resulting FIFO OStreamArray to read on (writing pushs back new keys...)
 * @note this is a wrapper to a_input_key_stream_join which creates an OJoin on the parent to bind to
 */
O_EXTERN
oobj a_input_key_stream(oobj parent, oobj *opt_out_join);



#endif //A_INPUT_H
