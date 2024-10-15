#ifndef O_CLIPBOARD_H
#define O_CLIPBOARD_H

/**
 * @file clipboard.h
 *
 * Get and set clipboard strings (copy paste)
 */

#include "common.h"

/**
 * @return true on platforms, where this module may work
 * @note emscripten does not work, for example
 */
O_EXTERN
bool o_clipboard_system_available(void);

/**
 * @return true if a string is available on the clipboard
 * @sa o_clipboard_get
 */
O_EXTERN
bool o_clipboard_available(void);

/**
 * @param parent OObj to allocate on
 * @return clipboard string null terminated, or NULL if not available (paste)
 *         needs to be o_free'd
 */
O_EXTERN
char *o_clipboard_get(oobj parent);

/**
 * @param set new null terminated string to set into the clipboard (copy)
 */
O_EXTERN
void o_clipboard_set(const char *set);

#endif //O_CLIPBOARD_H
