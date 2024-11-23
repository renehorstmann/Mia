#ifndef EX_THUNDER_H
#define EX_THUNDER_H

/**
 * @file thunder.h
 *
 * A thunder button which plays a random thunder sound.
 */

#include "o/common.h"

/**
 * Starts the thunder timer app
 * @param root object to allocate on
 * @return AScene object of the thunder app
 */
O_EXTERN
oobj ex_thunder_main(oobj root);

/**
 * Starts the thunder timer app with a mia splash screen
 * @param root object to allocate on
 * @return AScene object of the used u_splash, which creates the thunder scene
 */
O_EXTERN
oobj ex_thunder_main_splashed(oobj root);



#endif //EX_THUNDER_H
