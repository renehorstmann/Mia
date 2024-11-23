#include "o/clipboard.h"
#include "o/str.h"

#include <SDL2/SDL.h>


bool o_clipboard_system_available(void)
{
#ifdef MIA_PLATFORM_EMSCRIPTEN
    return false;
#endif
    return true;
}

bool o_clipboard_available(void)
{
    return SDL_HasClipboardText() == SDL_TRUE;
}

char *o_clipboard_get(oobj parent)
{
    if(!o_clipboard_available()) {
        return NULL;
    }
    char *sdl_clipboard = SDL_GetClipboardText();
    char *paste = o_str_clone(parent, sdl_clipboard);
    SDL_free(sdl_clipboard);
    return paste;
}

void o_clipboard_set(const char *set)
{
    // better not setting NULL
    set = o_or(set, "");
    SDL_SetClipboardText(set);
}
