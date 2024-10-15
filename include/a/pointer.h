#ifndef A_POINTER_H
#define A_POINTER_H

/**
 * @file pointer.h
 *
 * struct for managing pointer input (mouse or multi q)
 */

#include "o/common.h"
#include "m/types/flt.h"


// maximal idx pointers
#define a_pointer_MAX 16

// maximal buttons (left, middle, right -> 0, 1, 2)
#define a_pointer_MAX_BUTTON_IDS 3

// maximal in between frames pointer
#define a_pointer_MAX_BACKTRACE 8

/**
 * pointer for mouse and touch info with multi touch support.
 * If hovering is not available, pos will be set to NAN in xy.
 * .idx       Mouse:    0 -> left button
 *                      1 -> middle button
 *                      2 -> right button
 *            Touch:    0 -> single touch / first finger
 *                      1+ -> multitouch
 *                      [0 : A_POINTER_MAX)
 * .pos in back buffer viewport [-1:1] (x, y, 0, 1)
 * .pressure [0:1] if available (else 1.0) (stylus)
 * .active if the pointer is currently "down" on the screen
 */
struct a_pointer {
    int idx;
    vec4 pos;
    float pressure;
    bool active;
};


/**
 * Default pointer function definition.
 * If hovering is not available, pos will be set to NAN in xy.
 * @param idx Mouse:    0 -> left button
 *                      1 -> middle button
 *                      2 -> right button
 *            Touch:    0 -> single touch / first finger
 *                      1+ -> multitouch
 *                      [0 : A_POINTER_MAX)
 * @param history 0 for newest,
 *                  [1 : a_pointer_history_num] for "old" pointer between this and the last frame (bigger:=older)
 * @return the pointer for the given index (0=default)
 */
typedef struct a_pointer (*a_pointer__fn)(int idx, int history);


/**
 * Returns a pointer in back buffer gl coordinations [-1.0 : +1.0] (x, y, 0, 1).
 * If hovering is not available, pos will be set to NAN in xy.
 * @param idx Mouse:    0 -> left button
 *                      1 -> middle button
 *                      2 -> right button
 *            Touch:    0 -> single touch / first finger
 *                      1+ -> multitouch
 *                      [0 : A_POINTER_MAX)
 * @param history 0 for newest,
 *                  [1 : a_pointer_history_num] for "old" pointer between this and the last frame (bigger:=older)
 * @return the pointer for the given index (0=default)
 * @note asserts bounds
 * @example Using history, a typical for loop would be as the following:
 * ```c
 * for(int bt=a_pointer_history_num(0); bt>0; bt--) {
 *     struct a_pointer p = a_pointer_back(0, bt);
 *     // ...
 * }
 * ```
 */
O_EXTERN
struct a_pointer a_pointer_back(int idx, int history);


/**
 * Returns a pointer in back buffer viewport coordinations [0: size) (x, y, 0, 1).
 * If hovering is not available, pos will be set to NAN in xy.
 * @param idx Mouse:    0 -> left button
 *                      1 -> middle button
 *                      2 -> right button
 *            Touch:    0 -> single touch / first finger
 *                      1+ -> multitouch
 *                      [0 : A_POINTER_MAX)
 * @param history 0 for newest,
 *                  [1 : a_pointer_history_num] for "old" pointer between this and the last frame (bigger:=older)
 * @return the pointer for the given index (0=default)
 * @note asserts bounds
 * @example Using history, a typical for loop would be as the following:
 * ```c
 * for(int bt=a_pointer_history_num(0); bt>0; bt--) {
 *     struct a_pointer p = a_pointer_vp(0, bt);
 *     // ...
 * }
 * ```
 */
O_EXTERN
struct a_pointer a_pointer_back_viewport(int idx, int history);



//
// AView's delegations
//


/**
 * Returns a pointer in current AView's tex gl coordinations [-1.0 : +1.0] (x, y, 0, 1).
 * If hovering is not available, pos will be set to NAN in xy.
 * @param idx Mouse:    0 -> left button
 *                      1 -> middle button
 *                      2 -> right button
 *            Touch:    0 -> single touch / first finger
 *                      1+ -> multitouch
 *                      [0 : A_POINTER_MAX)
 * @param history 0 for newest,
 *                  [1 : a_pointer_history_num] for "old" pointer between this and the last frame (bigger:=older)
 * @return the pointer for the given index (0=default)
 * @note asserts bounds
 *       Calls AView_pointer with a_app_view
 * @example Using history, a typical for loop would be as the following:
 * ```c
 * for(int bt=a_pointer_history_num(0); bt>0; bt--) {
 *     struct a_pointer p = a_pointer_back(0, bt);
 *     // ...
 * }
 * ```
 */
O_EXTERN
struct a_pointer a_pointer_raw(int idx, int history);


/**
 * Returns a pointer in current AView's cam viewport coordinations
 * with only the perspective cam (p_only) (x, y, 0, 1).
 * If hovering is not available, pos will be set to NAN in xy.
 * @param obj AView object
 * @param idx Mouse:    0 -> left button
 *                      1 -> middle button
 *                      2 -> right button
 *            Touch:    0 -> single touch / first finger
 *                      1+ -> multitouch
 *                      [0 : A_POINTER_MAX)
 * @return pointer pos (x, y, 0, 1) in views cam, active=false if not within scenes quad
 * @param history 0 for newest,
 *                  [1 : a_pointer_history_num] for "old" pointer between this and the last frame (bigger:=older)
 * @note asserts bounds
 *       Calls AView_pointer with a_app_view
 */
O_EXTERN
struct a_pointer a_pointer_p(int idx, int history);


/**
 * Returns a pointer in current AView's cam viewport coordinations (x, y, 0, 1).
 * If hovering is not available, pos will be set to NAN in xy.
 * @param obj AView object
 * @param idx Mouse:    0 -> left button
 *                      1 -> middle button
 *                      2 -> right button
 *            Touch:    0 -> single touch / first finger
 *                      1+ -> multitouch
 *                      [0 : A_POINTER_MAX)
 * @return pointer pos (x, y, 0, 1) in views cam, active=false if not within scenes quad
 * @param history 0 for newest,
 *                  [1 : a_pointer_history_num] for "old" pointer between this and the last frame (bigger:=older)
 * @note asserts bounds
 *       Calls AView_pointer with a_app_view
 */
O_EXTERN
struct a_pointer a_pointer(int idx, int history);


//
// utility functions
//

/**
 * Returns the number of valid history pointers.
 * @param idx see a_pointer
 * @return number of new history pointers, may be 0 if nothing changed
 */
O_EXTERN
int a_pointer_history_num(int idx);

/**
 * Sets a pointer as handled, so scenes down the stack will get an unactive pointer#
 * @param idx see a_pointer, -1 for all
 * @param history see a_pointer, -1 for all
 * @note asserts bounds
 */
O_EXTERN
void a_pointer_handled(int idx, int history);


/**
 * @return true for touch screens
 * @note may be changed during runtime to true, after first usage of the touch screen
 *       just calls and returns a_app_is_touch
 */
O_EXTERN
bool a_pointer_is_touch(void);


/**
 * @param self current frames pointer
 * @param prev pointer of the previous frame
 * @return true if the pointer changed to active -> DOWN
 */
O_INLINE
bool a_pointer_down(struct a_pointer self, struct a_pointer prev)
{
    return self.active && prev.pressure<=0;
}

/**
 * @param self current frames pointer
 * @param prev pointer of the previous frame
 * @return true if the pointer changed to NOT active -> UP
 */
O_INLINE
bool a_pointer_up(struct a_pointer self, struct a_pointer prev)
{
    return self.pressure<=0 && prev.active;
}


#endif //A_POINTER_H
