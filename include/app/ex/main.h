#ifndef EX_MAIN_H
#define EX_MAIN_H

/**
 * @file main.h
 *
 * Examples main module to start the examples app
 */

#include "o/common.h"

/**
 * Starts the Examples app
 * @param root object to allocate on
 * @return AScene object of the used u_splash, which creates the main scene
 */
O_EXTERN
oobj ex_main(oobj root);


/**
 * @return Root OObj to allocate on for ex module
 */
O_EXTERN
oobj ex_root(void);

#endif //EX_MAIN_H
