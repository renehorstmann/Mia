#include "o/timer.h"
#include <SDL2/SDL_timer.h>


ou64 o_timer(void)
{
    return SDL_GetPerformanceCounter();
}

ou64 o_timer_freq(void)
{
    return SDL_GetPerformanceFrequency();
}
