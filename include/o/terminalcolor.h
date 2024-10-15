#ifndef O_TERMINALCOLOR_H
#define O_TERMINALCOLOR_H

/**
 * @file terminalcolor.h
 *
 * Colors for a terminal
 */

//
// colors for a terminal
//

#include "common.h"

//
// options:
//

/** define to not use terminal colors */
// MIA_TERMINALCOLOR_OFF


/**
 * Setups the terminal to handle color codes
 * @note Just needed for Windows...
 */
O_EXTERN
void o_terminalcolor_start(void);

/**
 * Stops the terminal from handling color codes
 * @note Just needed for Windows...
 */
O_EXTERN
void o_terminalcolor_stop(void);

//
// colors:
//

#ifndef MIA_TERMINALCOLOR_OFF

#define O_TERMINALCOLOR_RESET   "\x1B[0m"
#define O_TERMINALCOLOR_BLACK   "\x1B[0;30m"
#define O_TERMINALCOLOR_RED     "\x1B[0;31m"
#define O_TERMINALCOLOR_GREEN   "\x1B[0;32m"
#define O_TERMINALCOLOR_YELLOW  "\x1B[0;33m"
#define O_TERMINALCOLOR_BLUE    "\x1B[0;34m"
#define O_TERMINALCOLOR_MAGENTA "\x1B[0;35m"
#define O_TERMINALCOLOR_CYAN    "\x1B[0;36m"
#define O_TERMINALCOLOR_WHITE   "\x1B[0;37m"
#define O_TERMINALCOLOR_BOLD_BLACK   "\x1B[1;30m"
#define O_TERMINALCOLOR_BOLD_RED     "\x1B[1;31m"
#define O_TERMINALCOLOR_BOLD_GREEN   "\x1B[1;32m"
#define O_TERMINALCOLOR_BOLD_YELLOW  "\x1B[1;33m"
#define O_TERMINALCOLOR_BOLD_BLUE    "\x1B[1;34m"
#define O_TERMINALCOLOR_BOLD_MAGENTA "\x1B[1;35m"
#define O_TERMINALCOLOR_BOLD_CYAN    "\x1B[1;36m"
#define O_TERMINALCOLOR_BOLD_WHITE   "\x1B[1;37m"
#define O_TERMINALCOLOR_UNDERLINE_BLACK   "\x1B[4;30m"
#define O_TERMINALCOLOR_UNDERLINE_RED     "\x1B[4;31m"
#define O_TERMINALCOLOR_UNDERLINE_GREEN   "\x1B[4;32m"
#define O_TERMINALCOLOR_UNDERLINE_YELLOW  "\x1B[4;33m"
#define O_TERMINALCOLOR_UNDERLINE_BLUE    "\x1B[4;34m"
#define O_TERMINALCOLOR_UNDERLINE_MAGENTA "\x1B[4;35m"
#define O_TERMINALCOLOR_UNDERLINE_CYAN    "\x1B[4;36m"
#define O_TERMINALCOLOR_UNDERLINE_WHITE   "\x1B[4;37m"
#define O_TERMINALCOLOR_HIGHINTENSITY_BLACK   "\x1B[0;90m"
#define O_TERMINALCOLOR_HIGHINTENSITY_RED     "\x1B[0;91m"
#define O_TERMINALCOLOR_HIGHINTENSITY_GREEN   "\x1B[0;92m"
#define O_TERMINALCOLOR_HIGHINTENSITY_YELLOW  "\x1B[0;93m"
#define O_TERMINALCOLOR_HIGHINTENSITY_BLUE    "\x1B[0;94m"
#define O_TERMINALCOLOR_HIGHINTENSITY_MAGENTA "\x1B[0;95m"
#define O_TERMINALCOLOR_HIGHINTENSITY_CYAN    "\x1B[0;96m"
#define O_TERMINALCOLOR_HIGHINTENSITY_WHITE   "\x1B[0;97m"
#define O_TERMINALCOLOR_HIGHINTENSITY_BOLD_BLACK   "\x1B[1;90m"
#define O_TERMINALCOLOR_HIGHINTENSITY_BOLD_RED     "\x1B[1;91m"
#define O_TERMINALCOLOR_HIGHINTENSITY_BOLD_GREEN   "\x1B[1;92m"
#define O_TERMINALCOLOR_HIGHINTENSITY_BOLD_YELLOW  "\x1B[1;93m"
#define O_TERMINALCOLOR_HIGHINTENSITY_BOLD_BLUE    "\x1B[1;94m"
#define O_TERMINALCOLOR_HIGHINTENSITY_BOLD_MAGENTA "\x1B[1;95m"
#define O_TERMINALCOLOR_HIGHINTENSITY_BOLD_CYAN    "\x1B[1;96m"
#define O_TERMINALCOLOR_HIGHINTENSITY_BOLD_WHITE   "\x1B[1;97m"
#define O_TERMINALCOLOR_BACKGROUND_BLACK   "\x1B[40m"
#define O_TERMINALCOLOR_BACKGROUND_RED     "\x1B[41m"
#define O_TERMINALCOLOR_BACKGROUND_GREEN   "\x1B[42m"
#define O_TERMINALCOLOR_BACKGROUND_YELLOW  "\x1B[43m"
#define O_TERMINALCOLOR_BACKGROUND_BLUE    "\x1B[44m"
#define O_TERMINALCOLOR_BACKGROUND_MAGENTA "\x1B[45m"
#define O_TERMINALCOLOR_BACKGROUND_CYAN    "\x1B[46m"
#define O_TERMINALCOLOR_BACKGROUND_WHITE   "\x1B[47m"
#define O_TERMINALCOLOR_BACKGROUND_HIGHINTENSITY_BLACK   "\x1B[0;100m"
#define O_TERMINALCOLOR_BACKGROUND_HIGHINTENSITY_RED     "\x1B[0;101m"
#define O_TERMINALCOLOR_BACKGROUND_HIGHINTENSITY_GREEN   "\x1B[0;102m"
#define O_TERMINALCOLOR_BACKGROUND_HIGHINTENSITY_YELLOW  "\x1B[0;103m"
#define O_TERMINALCOLOR_BACKGROUND_HIGHINTENSITY_BLUE    "\x1B[0;104m"
#define O_TERMINALCOLOR_BACKGROUND_HIGHINTENSITY_MAGENTA "\x1B[0;105m"
#define O_TERMINALCOLOR_BACKGROUND_HIGHINTENSITY_CYAN    "\x1B[0;106m"
#define O_TERMINALCOLOR_BACKGROUND_HIGHINTENSITY_WHITE   "\x1B[0;107m"

#else


#define O_TERMINALCOLOR_RESET   ""
#define O_TERMINALCOLOR_BLACK   ""
#define O_TERMINALCOLOR_RED     ""
#define O_TERMINALCOLOR_GREEN   ""
#define O_TERMINALCOLOR_YELLOW  ""
#define O_TERMINALCOLOR_BLUE    ""
#define O_TERMINALCOLOR_MAGENTA ""
#define O_TERMINALCOLOR_CYAN    ""
#define O_TERMINALCOLOR_WHITE   ""
#define O_TERMINALCOLOR_BOLD_BLACK   ""
#define O_TERMINALCOLOR_BOLD_RED     ""
#define O_TERMINALCOLOR_BOLD_GREEN   ""
#define O_TERMINALCOLOR_BOLD_YELLOW  ""
#define O_TERMINALCOLOR_BOLD_BLUE    ""
#define O_TERMINALCOLOR_BOLD_MAGENTA ""
#define O_TERMINALCOLOR_BOLD_CYAN    ""
#define O_TERMINALCOLOR_BOLD_WHITE   ""
#define O_TERMINALCOLOR_UNDERLINE_BLACK   ""
#define O_TERMINALCOLOR_UNDERLINE_RED     ""
#define O_TERMINALCOLOR_UNDERLINE_GREEN   ""
#define O_TERMINALCOLOR_UNDERLINE_YELLOW  ""
#define O_TERMINALCOLOR_UNDERLINE_BLUE    ""
#define O_TERMINALCOLOR_UNDERLINE_MAGENTA ""
#define O_TERMINALCOLOR_UNDERLINE_CYAN    ""
#define O_TERMINALCOLOR_UNDERLINE_WHITE   ""
#define O_TERMINALCOLOR_HIGHINTENSITY_BLACK   ""
#define O_TERMINALCOLOR_HIGHINTENSITY_RED     ""
#define O_TERMINALCOLOR_HIGHINTENSITY_GREEN   ""
#define O_TERMINALCOLOR_HIGHINTENSITY_YELLOW  ""
#define O_TERMINALCOLOR_HIGHINTENSITY_BLUE    ""
#define O_TERMINALCOLOR_HIGHINTENSITY_MAGENTA ""
#define O_TERMINALCOLOR_HIGHINTENSITY_CYAN    ""
#define O_TERMINALCOLOR_HIGHINTENSITY_WHITE   ""
#define O_TERMINALCOLOR_HIGHINTENSITY_BOLD_BLACK   ""
#define O_TERMINALCOLOR_HIGHINTENSITY_BOLD_RED     ""
#define O_TERMINALCOLOR_HIGHINTENSITY_BOLD_GREEN   ""
#define O_TERMINALCOLOR_HIGHINTENSITY_BOLD_YELLOW  ""
#define O_TERMINALCOLOR_HIGHINTENSITY_BOLD_BLUE    ""
#define O_TERMINALCOLOR_HIGHINTENSITY_BOLD_MAGENTA ""
#define O_TERMINALCOLOR_HIGHINTENSITY_BOLD_CYAN    ""
#define O_TERMINALCOLOR_HIGHINTENSITY_BOLD_WHITE   ""
#define O_TERMINALCOLOR_BACKGROUND_BLACK   ""
#define O_TERMINALCOLOR_BACKGROUND_RED     ""
#define O_TERMINALCOLOR_BACKGROUND_GREEN   ""
#define O_TERMINALCOLOR_BACKGROUND_YELLOW  ""
#define O_TERMINALCOLOR_BACKGROUND_BLUE    ""
#define O_TERMINALCOLOR_BACKGROUND_MAGENTA ""
#define O_TERMINALCOLOR_BACKGROUND_CYAN    ""
#define O_TERMINALCOLOR_BACKGROUND_WHITE   ""
#define O_TERMINALCOLOR_BACKGROUND_HIGHINTENSITY_BLACK   ""
#define O_TERMINALCOLOR_BACKGROUND_HIGHINTENSITY_RED     ""
#define O_TERMINALCOLOR_BACKGROUND_HIGHINTENSITY_GREEN   ""
#define O_TERMINALCOLOR_BACKGROUND_HIGHINTENSITY_YELLOW  ""
#define O_TERMINALCOLOR_BACKGROUND_HIGHINTENSITY_BLUE    ""
#define O_TERMINALCOLOR_BACKGROUND_HIGHINTENSITY_MAGENTA ""
#define O_TERMINALCOLOR_BACKGROUND_HIGHINTENSITY_CYAN    ""
#define O_TERMINALCOLOR_BACKGROUND_HIGHINTENSITY_WHITE   ""

#endif

#endif //O_TERMINALCOLOR_H