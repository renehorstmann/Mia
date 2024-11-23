#ifndef EX_TEA_H
#define EX_TEA_H

/**
 * @file tea.h
 *
 * A tea timer app, also acts as an example.
 * This is a standalone app from the other examples.
 * But because mia can open and close scenes where it wants, its also part of the examples list
 */

#include "o/common.h"

/**
 * Starts the tea timer app
 * @param root object to allocate on
 * @return AScene object of the tea app
 */
O_EXTERN
oobj ex_tea_main(oobj root);

/**
 * Starts the tea timer app with a mia splash screen
 * @param root object to allocate on
 * @return AScene object of the used u_splash, which creates the tea scene
 */
O_EXTERN
oobj ex_tea_main_splashed(oobj root);



#endif //EX_TEA_H
