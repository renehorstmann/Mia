#include "o/terminalcolor.h"


#ifdef MIA_PLATFORM_WINDOWS

#include <windows.h>
// Some old MinGW/CYGWIN distributions don't define this:
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING  0x0004
#endif
static HANDLE win_stdout_handle;
static HANDLE win_stderr_handle;
static DWORD win_out_mode_init;
static DWORD win_err_mode_init;

void o_terminalcolor_start(void)
{
    DWORD out_mode = 0;
    DWORD err_mode = 0;
    win_stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    win_stderr_handle = GetStdHandle(STD_ERROR_HANDLE);
    GetConsoleMode(win_stdout_handle, &out_mode);
    GetConsoleMode(win_stderr_handle, &err_mode);
    win_out_mode_init = out_mode;
    win_err_mode_init = err_mode;

    // Enable ANSI escape codes
    out_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    err_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(win_stdout_handle, out_mode);
    SetConsoleMode(win_stderr_handle, err_mode);
}

void o_terminalcolor_stop(void)
{
    // Reset console mode
    SetConsoleMode(win_stdout_handle, win_out_mode_init);
    SetConsoleMode(win_stderr_handle, win_err_mode_init);
}

#else // Windows

void o_terminalcolor_start(void)
{
    // noop
}

void o_terminalcolor_stop(void)
{
    // noop
}

#endif // Windows
