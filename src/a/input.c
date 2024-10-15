#include "a/input.h"
#include "o/OWeakjoin.h"
#include "o/OStreamArray.h"
#include "o/OArray.h"
#include "a/app.h"
#include <SDL2/SDL_events.h>

#define O_LOG_LIB "a"
#include "o/log.h"


static struct {
    bool keys_current[A_INPUT_NUM_KEYS];
    bool keys_prev[A_INPUT_NUM_KEYS];
    ivec2 wheel;

    // OArray of OWeakjoin of OStreamArray
    oobj stream_weak_array;
} input_L;


O_STATIC
const char *a_input__stream_text(SDL_Keysym keysym, bool shift);

//
// public
//

bool a_input_is_pressed(enum a_input_keys key)
{
    return input_L.keys_current[key];
}

bool a_input_pressed(enum a_input_keys key)
{
    return input_L.keys_current[key] && !input_L.keys_prev[key];
}

bool a_input_released(enum a_input_keys key)
{
    return !input_L.keys_current[key] && input_L.keys_prev[key];
}

ivec2 a_input_wheel(void)
{
    return input_L.wheel;
}

oobj a_input_key_stream_join(oobj join_parent)
{
    if(!input_L.stream_weak_array) {
        input_L.stream_weak_array = OArray_new_dyn(a_app_root(), NULL, sizeof (oobj), 0, 4);
    }
    oobj weak = OWeakjoin_new(input_L.stream_weak_array, join_parent);
    OArray_push(input_L.stream_weak_array, &weak);

    oobj stream = OStreamArray_new(join_parent, OArray_new_dyn(join_parent, NULL, sizeof(char), 0, 256),
                                   true, OStreamArray_FIFO);
    OObj_name_set(stream, a_input_key_stream_OOBJ_NAME);
    return stream;
}

oobj a_input_key_stream(oobj parent, oobj *opt_out_join)
{
    oobj join = OJoin_new_heap(parent);
    o_opt_set(opt_out_join, join);
    return a_input_key_stream_join(join);
}

//
// protected
//

O_EXTERN
void a_input__init(void)
{
    // noop
}

O_EXTERN
void a_input__update(void)
{
    input_L.wheel = ivec2_(0);
    o_memcpy(input_L.keys_prev, input_L.keys_current, sizeof input_L.keys_prev, 1);
}


O_EXTERN
void a_input__handle_key_event(SDL_Event *event)
{
    bool down = event->type == SDL_KEYDOWN;
    switch (event->key.keysym.sym) {
        case SDLK_RETURN:
            input_L.keys_current[A_INPUT_ENTER] = down;
            break;
        case SDLK_SPACE:
            input_L.keys_current[A_INPUT_SPACE] = down;
            break;
        case SDLK_ESCAPE:
            input_L.keys_current[A_INPUT_ESCAPE] = down;
            break;
        case SDLK_AC_BACK:
            input_L.keys_current[A_INPUT_BACK] = down;
            break;
        case SDLK_UP:
            input_L.keys_current[A_INPUT_UP] = down;
            break;
        case SDLK_LEFT:
            input_L.keys_current[A_INPUT_LEFT] = down;
            break;
        case SDLK_DOWN:
            input_L.keys_current[A_INPUT_DOWN] = down;
            break;
        case SDLK_RIGHT:
            input_L.keys_current[A_INPUT_RIGHT] = down;
            break;
        case SDLK_w:
            input_L.keys_current[A_INPUT_W] = down;
            break;
        case SDLK_a:
            input_L.keys_current[A_INPUT_A] = down;
            break;
        case SDLK_s:
            input_L.keys_current[A_INPUT_S] = down;
            break;
        case SDLK_d:
            input_L.keys_current[A_INPUT_D] = down;
            break;
    }

    if(down && input_L.stream_weak_array && o_num(input_L.stream_weak_array)>0) {
        SDL_Keysym keysym = event->key.keysym;
        bool shift = SDL_GetModState() & KMOD_SHIFT;
        const char *text = a_input__stream_text(keysym, shift);

        for(osize i=0; i<o_num(input_L.stream_weak_array); i++) {
            oobj *weak = o_at(input_L.stream_weak_array, i);
            struct oobj_opt join = OWeakjoin_acquire(*weak);
            bool delete = true;
            if(join.o) {
                struct oobj_opt stream = OObj_find(join.o, OStreamArray, a_input_key_stream_OOBJ_NAME, 0);
                if(stream.o) {
                    delete = false;
                    OStream_print(stream.o, text);
                }
            }

            if(delete) {
                o_log_debug_s("a_input_key_stream", "join got deleted, deleting weakjoin");
                OWeakjoin_release(*weak);
                o_del(*weak);
                OArray_pop_at(input_L.stream_weak_array, i, NULL);
                i--;
                continue;
            }
        }
    }
}

O_EXTERN
void a_input__handle_wheel_event(SDL_Event *event)
{
    input_L.wheel.x += event->wheel.x;
    input_L.wheel.y += event->wheel.y;
}


//
// private
//

// forwarded
const char *a_input__stream_text(SDL_Keysym keysym, bool shift)
{
    static char char_result[2] = {0};
    if (keysym.sym >= SDLK_a && keysym.sym <= SDLK_z) {
        if (shift) {
            char_result[0] = (char) toupper((char) keysym.sym);
            return char_result;
        } else {
            char_result[0] = (char) keysym.sym;
            return char_result;
        }
    } else if (keysym.sym >= SDLK_0 && keysym.sym <= SDLK_9) {
        char_result[0] = (char) keysym.sym;
        return char_result;
    } else {
        switch (keysym.sym) {
            case SDLK_SPACE: return " ";
            case SDLK_TAB: return "\t";
            case SDLK_BACKSPACE: return "\b";
            case SDLK_LEFT: return A_INPUT_ESCAPE_LEFT;
            case SDLK_RIGHT: return A_INPUT_ESCAPE_RIGHT;
            case SDLK_UP: return A_INPUT_ESCAPE_UP;
            case SDLK_DOWN: return A_INPUT_ESCAPE_DOWN;
            case SDLK_MINUS: return "-";
            case SDLK_EQUALS: return "=";
            case SDLK_LEFTBRACKET: return "[";
            case SDLK_RIGHTBRACKET: return "]";
            case SDLK_BACKSLASH: return "\\";
            case SDLK_SEMICOLON: return ";";
            case SDLK_QUOTE: return "'";
            case SDLK_COMMA: return ",";
            case SDLK_PERIOD: return ".";
            case SDLK_SLASH: return "/";
        }
    }
    return NULL; // Not a printable character or sequence
}

