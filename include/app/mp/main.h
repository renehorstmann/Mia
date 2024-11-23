#ifndef MP_MAIN_H
#define MP_MAIN_H

/**
 * @file main.h
 *
 * Mia Paint main module to start the app
 */

#include "o/common.h"

/**
 * Starts the Mia Paint app
 * @param root object to allocate on
 * @return AScene object on which all of mia paint is built upon
 */
O_EXTERN
oobj mp_main(oobj root);


/**
 * @return Root OObj to allocate on for mp module
 */
O_EXTERN
oobj mp_root(void);

#endif //MP_MAIN_H
