#include "a/pointer.h"
#include "r/common.h"
#include "a/app.h"
#include "a/AView.h"
#include <SDL2/SDL_events.h>

#define O_LOG_LIB "a"

#include "o/log.h"


struct touch {
    int pointer_id;
    SDL_FingerID finger_id;
    vec4 pos_gl;
};


static struct {
    // last can't be set to handled
    struct a_pointer pointer_last[a_pointer_MAX];
    // current is a fresh copy of last on each frame (after events are handled)
    // can be set to handled for this frame
    struct a_pointer pointer_current[a_pointer_MAX];
    // history, if handled, remains handled
    struct a_pointer pointer_history[a_pointer_MAX][a_pointer_MAX_BACKTRACE];
    int pointer_history_current[a_pointer_MAX];
    int pointer_history_num[a_pointer_MAX];

    obyte mouse_pressed[a_pointer_MAX_BUTTON_IDS];

    struct touch touchs[a_pointer_MAX];
    int touchs_size;

    float last_touched_time;
} pointer_L;


O_STATIC
void set_pointer(int idx, bool active, vec4 pos, float pressure)
{
    assert(0 <= idx && idx < a_pointer_MAX);

    pointer_L.pointer_history_num[idx] = o_min(pointer_L.pointer_history_num[idx] + 1, a_pointer_MAX_BACKTRACE);
    pointer_L.pointer_history_current[idx]++;
    pointer_L.pointer_history_current[idx] %= a_pointer_MAX_BACKTRACE;
    int bt = pointer_L.pointer_history_current[idx];

    pointer_L.pointer_history[idx][bt].active = active;
    pointer_L.pointer_history[idx][bt].pos = pos;
    pointer_L.pointer_history[idx][bt].pressure = active ? pressure : 0.0f;

    pointer_L.pointer_last[idx] = pointer_L.pointer_history[idx][bt];
}

O_STATIC
void reset_touch_instantly(void)
{
    if (pointer_L.touchs_size > 0) {
        o_log_trace_s("a_app_reset_touch", "resetting touch pointers");
    }
    for (int i = 0; i < pointer_L.touchs_size; i++) {
        set_pointer(i, false, pointer_L.touchs[i].pos_gl, 0.0f);
        pointer_L.pointer_history_num[i] = 0;
    }
    pointer_L.touchs_size = 0;
}

O_STATIC
bool touch_pointer_id_already_exists(int pointer_id)
{
    for (int i = 0; i < pointer_L.touchs_size; i++) {
        if (pointer_L.touchs[i].pointer_id == pointer_id)
            return true;
    }
    return false;
}

O_STATIC
void touch_id_remove(int id)
{
    if (id < 0 || id >= pointer_L.touchs_size) {
        o_log_error_s("handle_event_touch", "invalid id: %i/%i", id, pointer_L.touchs_size);
        return;
    }
    for (int i = id; i < pointer_L.touchs_size - 1; i++) {
        pointer_L.touchs[i] = pointer_L.touchs[i + 1];
    }
    pointer_L.touchs_size--;
}

O_STATIC
void handle_events_pointer_touch(SDL_Event *event)
{
    float x = event->tfinger.x;
    float y = event->tfinger.y;
    SDL_FingerID finger_id = event->tfinger.fingerId;
    bool active = event->type == SDL_FINGERDOWN || event->type == SDL_FINGERMOTION;
    if (!active && event->type != SDL_FINGERUP) {
        return;
    }


    pointer_L.last_touched_time = 0;

    vec4 pos_gl;
    pos_gl.x = 2.0f * x - 1.0f;
    pos_gl.y = 1.0f - 2.0f * y;
    pos_gl.z = 0;
    pos_gl.w = 1;

    // check finger ids to cast to touch id
    int id = -1;
    for (int i = 0; i < pointer_L.touchs_size; i++) {
        if (pointer_L.touchs[i].finger_id == finger_id) {
            id = i;
            break;
        }
    }

    if (id >= 0 && event->type == SDL_FINGERDOWN) {
        o_log_trace_s("handle_event_touch", "got down, but also an id");
        touch_id_remove(id);
        id = -1;
    }

    if (id < 0) {
        if (event->type == SDL_FINGERUP) {
            o_log_trace_s("handle_event_touch",
                          "got an up without an active touch id, ignoring...");
            return;
        }
        if (pointer_L.touchs_size >= a_pointer_MAX) {
            reset_touch_instantly();
        }

        // create new id
        int pointer_id = 0;
        while (touch_pointer_id_already_exists(pointer_id)) {
            pointer_id++;
        }

        id = pointer_L.touchs_size++;
        pointer_L.touchs[id].pointer_id = pointer_id;
        pointer_L.touchs[id].finger_id = finger_id;

        if (event->type == SDL_FINGERMOTION) {
            o_log_trace_s("handle_event_touch",
                          "e_input_update: got move, but not an id");
        }
    }

    // update pos
    pointer_L.touchs[id].pos_gl = pos_gl;

    // stylus if available
    float pressure = event->tfinger.pressure;
    if (pressure < 0 || pressure > 1.0) {
        // if garbage:
        pressure = 1.0f;
    }

    int pointer_id = pointer_L.touchs[id].pointer_id;

    // remove ups
    if (!active) {
        touch_id_remove(id);
    }

    set_pointer(pointer_id, active, pos_gl, pressure);
}

O_STATIC
void handle_events_pointer_mouse(SDL_Event *event)
{
    int btn_sdl = event->button.button;
    int id = btn_sdl - 1;

    vec4 pos_gl;
    {
        ivec2 pos;
        SDL_GetMouseState(&pos.x, &pos.y);
        vec2 wnd_size = r_back_size();
        pos_gl.x = (2.0f * (float) pos.x) / wnd_size.x - 1.0f;
        pos_gl.y = 1.0f - (2.0f * (float) pos.y) / wnd_size.y;
        pos_gl.z = 0.0f;
        pos_gl.w = 1.0f;
    }

    switch (event->type) {
        case SDL_MOUSEBUTTONDOWN:
            if (id < 0 || id >= a_pointer_MAX_BUTTON_IDS)
                break;
            set_pointer(id, true, pos_gl, 1.0f);
            pointer_L.mouse_pressed[id] = true;
            break;
        case SDL_MOUSEMOTION:
            for (int i = 0; i < a_pointer_MAX_BUTTON_IDS; i++) {
                set_pointer(i, pointer_L.mouse_pressed[i], pos_gl, 1.0f);
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (id < 0 || id >= a_pointer_MAX_BUTTON_IDS)
                break;
            set_pointer(id, false, pos_gl, 0.0f);
            pointer_L.mouse_pressed[id] = false;
            break;
    }
}


//
// protected
//


O_EXTERN
void a_pointer__init(void)
{
    for (int i = 0; i < a_pointer_MAX; i++) {
        for (int bt = 0; bt < a_pointer_MAX_BACKTRACE; bt++) {
            pointer_L.pointer_history[i][bt] = (struct a_pointer) {.idx = i};
        }
    }
}


O_EXTERN
void a_pointer__update(void)
{
    for (int i = 0; i < a_pointer_MAX; i++) {
        // clear history
        pointer_L.pointer_history_num[i] = 0;
    }
}


O_EXTERN
void a_pointer__update_events_handled(void)
{
    for (int i = 0; i < a_pointer_MAX; i++) {
        pointer_L.pointer_current[i] = pointer_L.pointer_last[i];
    }
}

O_EXTERN
void a_pointer__handle_event(SDL_Event *event)
{
    if (a_app_is_touch()) {
        handle_events_pointer_touch(event);
    } else {
        handle_events_pointer_mouse(event);
    }
}

O_EXTERN
void a_pointer__reset_touch_instantly(void)
{
    reset_touch_instantly();
}


//
// public
//

struct a_pointer a_pointer_back(int idx, int history)
{
    assert(idx >= 0 && idx < a_pointer_MAX);
    assert(history >= 0 && history <= a_pointer_MAX_BACKTRACE);
    if(history<=0) {
        return pointer_L.pointer_current[idx];
    }
    if (history > pointer_L.pointer_history_num[idx]) {
        o_log_warn_s(__func__, "history invalid, using last");
        history = pointer_L.pointer_history_num[idx];
    }
    history--;
    int bt = pointer_L.pointer_history_current[idx] - history;
    bt = o_mod(bt, a_pointer_MAX_BACKTRACE);
    return pointer_L.pointer_history[idx][bt];
}

struct a_pointer a_pointer_back_viewport(int idx, int history)
{
    struct a_pointer res = a_pointer_back(idx, history);
    vec2 wnd_size = r_back_size();
    res.pos.x = (res.pos.x + 1.0f) * wnd_size.x * 0.5f;
    res.pos.y = (res.pos.y + 1.0f) * wnd_size.y * 0.5f;
    return res;
}

//
// AView's delegations
//

struct a_pointer a_pointer_raw(int idx, int history)
{
    return AView_pointer_raw(a_app_view(), idx, history);
}


struct a_pointer a_pointer_p(int idx, int history)
{
    return AView_pointer_p(a_app_view(), idx, history);
}

struct a_pointer a_pointer(int idx, int history)
{
    return AView_pointer(a_app_view(), idx, history);
}


//
// utility functions
//

int a_pointer_history_num(int idx)
{
    assert(idx >= 0 && idx < a_pointer_MAX);
    return pointer_L.pointer_history_num[idx];
}

void a_pointer_handled(int idx, int history)
{
    if (idx < 0) {
        for (int i = 0; i < a_pointer_MAX; i++) {
            a_pointer_handled(i, history);
        }
        return;
    }
    assert(idx >= 0 && idx < a_pointer_MAX);
    if (history < 0) {
        for (int bt = 0; bt <= a_pointer_history_num(idx); bt++) {
            a_pointer_handled(idx, bt);
        }
        return;
    }
    if(history == 0) {
        pointer_L.pointer_current[idx].active = false;
    } else {
        history--;
        assert(history >= 0 && history < a_pointer_MAX_BACKTRACE);

        int bt = pointer_L.pointer_history_current[idx] - history;
        bt = o_mod(bt, a_pointer_MAX_BACKTRACE);
        pointer_L.pointer_history[idx][bt].active = false;
    }
}

bool a_pointer_is_touch(void)
{
    return a_app_is_touch();
}
