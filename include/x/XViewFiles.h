#ifndef X_XVIEWFILES_H
#define X_XVIEWFILES_H

/**
 * @file XViewFiles.h
 *
 * object.
 *
 * AView which shows a file picker
 *
 * Subclass of the AView object.
 */

#include "a/AView.h"
#include "u/scroll.h"


/** object id */
#define XViewFiles_ID AView_ID "XViewFiles"


/**
 * Optional prompt functions on ok or cancel press.
 * @param obj XViewFiles object
 * @param ok if true, is in state OK, else CANCEL
 * @return NULL to ignore prompt, else a string/text allocated on obj which will be displayed
 */
typedef char *(*XViewFiles_prompt_fn)(oobj obj, bool ok);


enum XViewFiles_state {
    XViewFiles_RUNNING,
    XViewFiles_PROMPT_CANCEL,
    XViewFiles_PROMPT_OK,
    XViewFiles_CANCEL,
    XViewFiles_OK,
    XViewFiles_NUM_STATES
};

typedef struct {
    AView AView;

    enum XViewFiles_state state;

    // starting point "." for the dir, defaults to o_file_home(o_file_home_DEFAULT)
    char dir_home[512];

    // OArray of current dir stack (strings allocated on this)
    oobj dir_stack;
    // deep copy of dir stack, if working
    oobj dir_stack_prev;

    // fn's may be NULL
    OObj__event_fn event;
    XViewFiles_prompt_fn prompt;

    // widgets
    struct {
        oobj theme;

        oobj gui;
        oobj title;
        oobj entry_dir;
        oobj view;
        oobj entry_file;
        oobj btn_cancel, btn_ok;
        struct u_scroll scroll;

        oobj cached_dirs;
        oobj cached_files;


        struct {
            char *last_dir;
            int page;
            int last_page;
            oobj theme;
            oobj gui;
            oobj dialog;
            oobj prompt_dialog;
        } f;
    } w;
} XViewFiles;


/**
 * Initializes the object.
 * Creates an AView that renders a virtual keyboard.
 * @param obj XViewFiles object
 * @param parent to inherit from
 * @param title for a simple title text at top
 * @return obj casted as XViewFiles
 */
O_EXTERN
XViewFiles *XViewFiles_init(oobj obj, oobj parent, const char *title);

/**
 * Creates a new the OArray object
 * Creates an AView that renders a virtual keyboard.
 * @param parent to inherit from
 * @param title for a simple title text at top
 * @return The new object
 */
O_INLINE
XViewFiles *XViewFiles_new(oobj parent, const char *title)
{
    OObj_DECL_IMPL_NEW(XViewFiles, parent, title);
}


//
// virtual implementations
//

O_EXTERN
void XViewFiles__v_setup(oobj view);

O_EXTERN
void XViewFiles__v_update(oobj view, oobj tex, float dt);

O_EXTERN
void XViewFiles__v_render(oobj view, oobj tex, float dt);


//
// object functions:
//


/**
 * @param obj XViewFiles object
 * @return current state
 */
OObj_DECL_GET(XViewFiles, enum XViewFiles_state, state)

/**
 * @param obj XViewFiles object
 * @return OArray of current dir stack, strings are allocated on this object
 */
OObj_DECL_GETSET(XViewFiles, oobj, dir_stack)

/**
 * @param obj XViewFiles object
 * @return event that is triggered on cancel or ok, may be NULL
 */
OObj_DECL_SET(XViewFiles, OObj__event_fn, event)

/**
 * @param obj XViewFiles object
 * @return event that is triggered on cancel or ok clicked, may be NULL
 */
OObj_DECL_SET(XViewFiles, XViewFiles_prompt_fn , prompt)


/**
 * @param obj XViewFiles
 * @return home a path to be used as ".", defaults to o_file_home(o_file_home_DEFAULT)
 */
OObj_DECL_GET(XViewFiles, const char *, dir_home)

/**
 * Sets a new home directory
 * @param obj XViewFiles
 * @param home a path to be used as ".", defaults to o_file_home(o_file_home_DEFAULT)
 */
O_EXTERN
void XViewFiles_dir_home_set(oobj obj, const char *home);

/**
 * Pops the last dir element from the dir stack
 * @param obj XViewFiles
 */
O_EXTERN
void XViewFiles_dir_stack_pop(oobj obj);

/**
 * Pops / clears all items
 * @param obj XViewFiles
 */
O_EXTERN
void XViewFiles_dir_stack_clear(oobj obj);

/**
 * Add an element to the dir stack
 * @param obj XViewFiles
 * @param dir_file dir to append to the stack, if its ".." it may pop the last
 */
O_EXTERN
void XViewFiles_dir_stack_push(oobj obj, const char *dir_file);

/**
 * @param obj XViewFiles object
 * @return dir string joined from dir_stack, allocated on obj, without expanded home
 */
O_EXTERN
char *XViewFiles_dir_small(oobj obj);

/**
 * @param obj XViewFiles object
 * @return dir string joined from dir_stack, allocated on obj
 */
O_EXTERN
char *XViewFiles_dir(oobj obj);

/**
 * @param obj XViewFiles object
 * @return only the selected current file name, without the dir
 */
O_EXTERN
const char *XViewFiles_file_name(oobj obj);

/**
 * @param obj XViewFiles object
 * @return only the selected current file name, without the dir
 */
O_EXTERN
const char *XViewFiles_file_name_set(oobj obj, const char *name);

/**
 * @param obj XViewFiles object
 * @return title for a simple title text at top
 */
O_EXTERN
const char *XViewFiles_title(oobj obj);

/**
 * @param obj XViewFiles object
 * @return title for a simple title text at top
 */
O_EXTERN
const char *XViewFiles_title_set(oobj obj, const char *title);

/**
 * @param obj XViewFiles object
 * @return dir and file_name joined together for a full path, allocated on obj
 */
O_EXTERN
char *XViewFiles_file(oobj obj);


#endif //X_XVIEWFILES_H
