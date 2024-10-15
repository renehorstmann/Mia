/**
 * A simple tea timer app.
 * Choose between three times, with configurable times.
 * App structure:
 *
 * - include and definitions like the shared context
 * - helper functions
 * - state machine switch functions
 * - start text edit to change a time
 * - main AView virtual functions: setup, update, render
 * - tea's main function
 */


#include "ex/tea.h"
#include "mia.h"


/**
 * frames per second and a timer for the sound to prevent app suspend
 */
#define TEA_FPS 3.0f
#define TEA_ALARM_TIME 3.0f


/**
 * States for the state machine.
 * START    -> shows time buttons to start the TIMER and an EDIT button
 * TIMER    -> countdown of the tea timer and an abort button to get back to START
 * FINISHED -> after countdown with a rebrew button to get back to START
 * EDIT     -> shows the time buttons, but will open a number edit scene on press.
 *             Done button to get back to START
 */
enum tea_state {
    TEA_START,
    TEA_TIMER,
    TEA_FINISHED,
    TEA_EDIT,
    TEA_NUM_STATES
};

/**
 * Context installed on the AView's o_user
 */
struct tea_context {

    /**
     * background color for RTex_clear
     */
    vec4 bg_color;

    /**
     * Current state machine state
     */
    enum tea_state state;


    /**
     * STrack of the loaded bell sound to be played on countdown
     */
    oobj alarm;
    
    /**
     * RObjRect of all animated cups.
     * In total 4 rects (3 above the time buttons, 1 big animated)
     * The texture atlas is used for animation. The texture image has 4x4 sprites
     * Current frame is floor of the integrating float
     */
    oobj cup;
    struct u_atlas cup_atlas;
    float cup_frame;

    /**
     * Current selected tea sort for the big cup
     */
    int tea_sort;

    /**
     * WTheme and WBox as main gui
     */
    oobj theme;
    oobj gui;
    
    /**
     * Simple WObj with a fixed size as area for the animated cups, which are rendered with the "cup" RObjRect.
     * The whole WObj gui is rendered with the WTheme in a single draw call.
     * It is possible to change and add sprites into to WTheme texture, see WImg.
     * But this way (using another draw call and only calculate the area) is much more flexible.
     */
    oobj main_cup_area;
    oobj small_cup_areas[3];

    /**
     * WText for the title or time, etc.
     */
    oobj text;

    /**
     * WGrid for the small cups and there buttons
     */
    oobj grid;

    /**
     * WBtn for the time buttons, including an internal WText
     */
    oobj time_btns[3];

    /**
     * WBtn below the time button
     */
    oobj action_btn;
    oobj action_btn_text;

    /**
     * Timer time and alarm time.
     * The alarm time starts if finished and prevents the app from suspending
     */
    float tea_time;
    float alarm_time;

    /**
     * o_timer time to measure
     */
    ou64 timer;
};

//
// helper functions
//

/**
 * Some stuff from the WObj widgets depend on the previous sizes and states.
 * For example the WGrid or WAlign use the prev size to determine the available room for its childen.
 * So we set WTheme_reupdate which will create a dual update on WTheme_update.
 * Else one could see a frame in which the text changed but the position is wrong.
 *     Would results in "flickering".
 */
O_STATIC
void tea_gui_update(oobj view)
{
    struct tea_context *C = o_user(view);
    WTheme_reupdate_set(C->theme, true);
}

/**
 * Loads all tea times from a saved json.
 * @param out_times3 float[3] [in seconds]
 */
O_STATIC
void tea_load_times(oobj view, float *out_times3)
{
    /**
     * Stuff allocated here is alloacated on this container. So we only need to o_del that.
     */
    oobj container = OObj_new(view);

    /**
     * Default times, if the loading fails
     */
    vecn_clone(out_times3, (float[]) {2*60.0f, 3*60.0f, 5*60.0f}, 3);

    /**
     * file records are for saving stuff to disk.
     * Emscripten (web app) creates an internal cache (db) and synchronizes that.
     * The returned name is either:
     * Emscripten: "record/ex_tea_time.json" (record is the synchronized dir)
     * Others:     "record.ex_tea_time.json"
     */
    char *record_file_path = o_file_record_name("ex_tea_time.json");

    /**
     * Read the file as json. In our case its just an array of the three times (hopefully).
     * when a function returns a struct oobj_opt instead of an oobj, it may be NULL internally.
     * Its a way to inform the user: "hey that may fail to NULL!".
     * The oobj is under ".o"
     */
    struct oobj_opt json = OJson_new_read_file(container, NULL, record_file_path);
    if(!json.o) {
        o_log("failed to load tea_time.json");
        goto CLEAN_UP;
    }

    /**
     * We expect 3 numbers for the tea times
     */
    for(int i=0; i<3; i++) {
        /**
         * OJson uses operator overloading, will call OJson_at (also o_num works)
         * Another way would be to call OJson_list to get all children in a list
         * the returned void * is either an OJson* or NULL
         */
        oobj jtime = o_at(json.o, i);
        if(!jtime) {
            o_log_warn("failed to get json tea index [%i]", i);
            goto CLEAN_UP;
        }

        /**
         * If this OJson is not a number, NULL is returned
         */
        double *time = OJson_number(jtime);
        if(!time) {
            o_log_warn("failed to convert json time [%i]", i);
            goto CLEAN_UP;
        }

        out_times3[i] = (float) *time;
    }

    CLEAN_UP:
    /**
     * Clean up all allocated stuff in this function.
     * o_del(json.o) would also work when not using the container...
     */
    o_del(container);
}

/**
 * Save all tea times into a json file
 * @param times3 float[3] [in seconds]
 */
O_STATIC
void tea_save_times(oobj view, const float *times3)
{
    oobj container = OObj_new(view);

    /**
     * OJson uses the OObj tree hierarchy for the represenstation
     * NULL is an optional name, if the parent is of type OJson_TYPE_OBJECT
     */
    oobj root = OJson_new_array(container, NULL);
    for(int i=0; i<3; i++) {
        OJson_new_number(root, NULL, times3[i]);
    }

    /**
     * As with the loading, we create the record file path and write to it.
     * After writing we call o_file_record_sync()
     *      which is needed by Emscripten to save to the cache (db)
     */
    char *record_file_path = o_file_record_name("ex_tea_time.json");
    OJson_write_file(root, record_file_path);
    o_file_record_sync();

    o_del(container);
}

/**
 * @return loaded tea time of a specific sort, simple helper
 */
O_STATIC
float tea_get_tea_time(oobj view, int sort)
{
    assert(sort>=0 && sort<3);
    float times[3];
    tea_load_times(view, times);
    return times[sort];
}

/**
 * Writes into the out_buf32 string the formated time in MM:SS
 * @param out_buf32 char[32]
 * @param time [in seconds]
 */
O_STATIC
void tea_time_to_str(char *out_buf32, float time)
{
    int secs = (int) m_ceil(time);
    int mins = secs / 60;
    secs %= 60;
    snprintf(out_buf32, 32,"%02i:%02i", mins, secs);
}

/**
 * Reads a string and returns the time [in seconds]
 * Either MM:SS or only MM
 * @return time [in seconds]; -1 on failure
 */
O_STATIC
float tea_str_to_time(const char *time_string)
{
    int mins = 0;
    int secs = 0;
    int result = sscanf(time_string, "%02i:%02i", &mins, &secs);
    if (result == 2 && mins >= 0 && secs >= 0 && secs < 60) {
        // got "SS"
        return (float) (mins * 60 + secs);
    }
    else if (result == 1 && mins >= 0) {
        // got "MM"
        return (float) (mins * 60);
    }
    // failed
    return -1;
}

/**
 * Helper to update the time buttons time text
 * @param btn or sort [0:2]
 */
O_STATIC
void tea_set_btn_time(oobj view, int btn)
{
    struct tea_context *C = o_user(view);
    float tea_time = tea_get_tea_time(view, btn);
    char buf[32];
    tea_time_to_str(buf, tea_time);

    /**
     * We search for the WText child in the direct children list of the button (0)
     * OObj_find can also look in recursion with the provided max depth (last parameter) (>0)
     */
    struct oobj_opt text = OObj_find(C->time_btns[btn], WText, NULL, 0);
    assert(text.o);
    WText_text_set(text.o, buf);
}

/**
 * Helper which updates the app title.
 * This is also called each frame in update.
 */
O_STATIC
void tea_set_app_title(const char *title)
{
    /**
     * Logging is disabled (with an exception of ERROR and above) in this block
     * (Uses o_log_quiet_set)
     */
    o_log_quiet_block {
        // would create an info log each frame otherwise
        a_app_title_set(title);
    }
}



//
// state machine
//


/**
 * Show start state.
 * TEA TIMER text
 * 3 cups of animated tea
 * 3 time buttons
 * EDIT button
 */
O_STATIC
void tea_state_start(oobj view)
{
    struct tea_context *C = o_user(view);
    o_log("state -> start");
    C->state = TEA_START;

    tea_set_app_title("Tea");
    WText_text_set(C->text, "TEA TIMER");
    WText_text_set(C->action_btn_text, "EDIT");
    WObj_hide_set(C->main_cup_area, true);
    WObj_hide_set(C->grid, false);
    for(int i=0; i<3; i++) {
        WObj_hide_set(C->small_cup_areas[i], false);
        WObj_style_apply(C->time_btns[i]);
        tea_set_btn_time(view, i);
    }
    tea_gui_update(view);
}

/**
 * Starts the timer state.
 * Big animated cup of the selected tea.
 * Time running down
 * ABORT button
 */
O_STATIC
void tea_state_timer(oobj view, int sort, float time)
{
    struct tea_context *C = o_user(view);
    o_log("state -> timer # sort|time: %i|%f", sort, time);
    C->state = TEA_TIMER;

    // C->text and app title will be updated on each frame to the current timer
    WText_text_set(C->action_btn_text, "ABORT");
    WObj_hide_set(C->main_cup_area, false);
    WObj_hide_set(C->grid, true);
    for(int i=0; i<3; i++) {
        // also hide these, so that there rect will have a size of 0
        WObj_hide_set(C->small_cup_areas[i], true);
    }

    C->tea_sort = sort;
    C->tea_time = time;
    C->timer = o_timer();
    tea_gui_update(view);
}

/**
 * Shows tea is ready finish state
 * Big animated cup of the selected tea.
 * TEA IS READY text
 * REBREW button
 */
O_STATIC
void tea_state_finished(oobj view)
{
    struct tea_context *C = o_user(view);
    o_log("state -> finished");
    C->state = TEA_FINISHED;

    tea_set_app_title("TEA READY");
    WText_text_set(C->text, "TEA IS READY!");
    WText_text_set(C->action_btn_text, "REBREW?");
    WObj_hide_set(C->main_cup_area, false);
    WObj_hide_set(C->grid, true);
    for(int i=0; i<3; i++) {
        // also hide these, so that there rect will have a size of 0
        WObj_hide_set(C->small_cup_areas[i], true);
    }

    C->alarm_time=TEA_ALARM_TIME;
    s_play(C->alarm, 0, 1);
    tea_gui_update(view);
}

/**
 * Show edit state.
 * EDIT TIMES text
 * 3 cups of animated tea
 * 3 time buttons for editing
 * DONE button
 */
O_STATIC
void tea_state_edit(oobj view)
{
    struct tea_context *C = o_user(view);
    o_log("state -> edit");
    C->state = TEA_EDIT;

    tea_set_app_title("Tea");
    WText_text_set(C->text, "EDIT TIMES");
    WText_text_set(C->action_btn_text, "DONE");
    WObj_hide_set(C->main_cup_area, true);
    WObj_hide_set(C->grid, false);
    for(int i=0; i<3; i++) {
        WObj_hide_set(C->small_cup_areas[i], false);
        WBtn_color_set(C->time_btns[i], vec4_(0.6, 0.3, 0.0, 1.0));
        tea_set_btn_time(view, i);
    }
    tea_gui_update(view);
}


//
// tea edit scene
//

/**
 * Context for the edit scene
 */
struct tea_edit_context {
    // this is "our" tea AView, as with the other functions
    oobj view;
    // clicked btn [0:2]
    int btn_edit;
};

/**
 * This event is called when the scene finished, either cancel or ok
 */
O_STATIC
void tea_time_edit_done(oobj scene)
{
    struct tea_edit_context *eC = o_user(scene);

    /**
     * XViewText is the AView of the edit scene, which shows the text edit
     */
    oobj viewtext = AScene_view(scene);
    if(XViewText_state(viewtext) != XViewText_OK) {
        /**
         * simple noop if canceled
         */
        return;
    }

    /**
     * The input field uses an OArray of char
     * text_string will be the C like start of the string (OArray is always element 0 terminated)
     */
    oobj text_array = XViewText_text_array(viewtext);
    char *text_string = o_at(text_array, 0);

    /**
     * Replace '.' with ':'
     * The NUM mode of XViewText does have a '.' button, so we change it while editing
     */
    o_str_replace_char_this(text_string, '.', ':');
    float time = tea_str_to_time(text_string);
    if(time>0 && time<=99*60+59) {
        /**
         * Reading the user time succeeded.
         * So we load all 3 times, change the edited one and save back
         */
        float times[3];
        tea_load_times(eC->view, times);
        times[eC->btn_edit] = time;
        tea_save_times(eC->view, times);

        /**
         * On success we switch back to the start state.
         * Else the AScene just finishes and the edit state keeps running.
         */
        tea_state_start(eC->view);
    } else {
        o_log("failed to parse time from user string: <%s>", text_string);
    }
}

/**
 * Starts an AScene with an XViewText (as AView)
 */
O_STATIC
void tea_start_time_edit(oobj view, int btn)
{
    /**
     * Starts the text input scene.
     * 5: maximal characters (MM:SS)
     */
    oobj scene = x_viewtext_scene(view, "TEA TIME IN:   \'MM\'  OR  \'MM.SS\'", 5, tea_time_edit_done, tea_time_edit_done);

    /**
     * Installing the edit context for the callback event
     */
    struct tea_edit_context *eC = o_user_set(scene, o_new0(scene, *eC, 1));
    eC->view = view;
    eC->btn_edit = btn;

    /**
     * XViewText (AView) of the scene
     */
    oobj viewtext = AScene_view(scene);

    /**
     * XViewKeys (also an AView) is the used virtual keyboard.
     * We switch the mode from ASCII to NUM for easier user input
     */
    oobj keys = XViewText_viewkeys(viewtext);
    XViewKeys_mode_set(keys, XViewKeys_NUM);

    /**
     * The XViewText let's us define a couple of custom replace buttons.
     * If one of these is pressed, its text is cloned into the input field.
     * So we create simple minute buttons from 1-8 mins as "MM".
     */
    for(int i=1; i<=8; i++) {
        char repl[16];
        o_strf_buf(repl, "%02i", i);
        XViewText_custom_replace(viewtext, repl, &vec4_(0.6, 0.3, 0.0, 1.0));
    }

    /**
     * Current buttons tea time as MM:SS string.
     * But we replace the ':' with '.', because the num keyboard has a button for that
     */
    char buf[32];
    tea_time_to_str(buf, tea_get_tea_time(view, btn));
    o_str_replace_char_this(buf, ':', '.');

    /**
     * XViewText uses an OArray of char for the input line.
     * We clear it first and append the string of the current time (MM.SS)
     */
    oobj text_array = XViewText_text_array(viewtext);
    OArray_clear(text_array);
    OArray_append_string(text_array, buf);
}

//
// view functions
//

/**
 * Called once at the start of the app
 * @param view AView object to render to
 */
O_STATIC
void tea_setup(oobj view)
{
    /**
     * We create our context installed on the AView
     */
    struct tea_context *C = o_user_set(view, o_new0(view, *C, 1));

    /**
     * The default camera has a minimal unit size of 180x180.
     * We use a smaller one. 96 would exaclty fit our 3x32 animated cups.
     * So we add a small border around that.
     * Normally, an AView automatically sets the minimal unit size to the parents camera scale.
     * So that it fits the whole app. We turn off that setting to apply our custom unit size.
     */
    oobj cam = AView_cam(view);
    RCam_min_units_size_set(cam, vec2_(96+8));
    RCam_update(cam);
    AView_cam_units_auto_set(view, false);

    /**
     * Background color with random hue angle. m_random() is already in [0:1]
     */
    vec4 hsva = {{m_random(), 0.66, 0.33, 1.0}};
    C->bg_color = vec4_hsv2rgb(hsva);


    /**
     * To enable sound, we first have to open the audio device once (multiple calls are ignored)
     * Will open the audio on first touch click, so that it works well with emscripten browsers
     */
    a_app_audio_device_open();

    /**
     * Alarm sound (ring bell)
     * Returns a struc oobj_opt, but we assume it to succeed.
     * o_assume is like an assert, but for runtime checks
     */
    C->alarm = s_ogg_load_track(view, "res/ex/tea_alarm.ogg", NULL).o;
    o_assume(C->alarm, "failed to load alarm file");

    /**
     * Load and setup the cup render object and its atlas
     */
    oobj cup_tex = RTex_new_file(view, "res/ex/tea_cup.png");
    C->cup = RObjRect_new(view, 4, cup_tex, true);
    C->cup_atlas = u_atlas_new_fill_tex(view, cup_tex, 4, 4, u_atlas_START_LT, true);

    /**
     * WTheme on which the gui is rendered
     */
    C->theme = WTheme_new_tiny(view);

    /**
     * Root WObj as vertical WBox
     */
    C->gui = WBox_new(view, WBox_LAYOUT_V);

    /**
     * main_cup is the big (double sized) animated cup for the timer and finish state.
     * We make use of WAlign to center it in the row.
     */
    oobj main_cup_align = WAlign_new_center_h(C->gui);
    C->main_cup_area = WObj_new(main_cup_align);
    WObj_fixed_size_set(C->main_cup_area, vec2_(64, 64));
    WObj_hide_set(C->main_cup_area, true);

    /**
     * Main text which will also show the count down.
     * Also centered with a WAlign.
     * Char size is doubled and the text is padded on top and bottom with 4 units/pixels
     */
    oobj text_align = WAlign_new_center_h(C->gui);
    C->text = WTextShadow_new(text_align, "");
    WText_char_scale_set(C->text, vec2_(2));
    WObj_padding_set(C->text, vec4_(0, 4, 0, 4));

    /**
     * aligned cup and time button grid
     */
    oobj grid_align = WAlign_new_center_h(C->gui);
    C->grid = WGrid_new(grid_align, 3, 2, vec2_(-1, -1));
    WGrid_align_set(C->grid, WGrid_align_CENTER, WGrid_align_CENTER);

    // row 0: optional small cups
    // row 1: time buttons

    /**
     * Create the areas for the small cups and the time buttons
     */
    for(int i=0; i<3; i++) {
        C->small_cup_areas[i] = WObj_new(C->grid);
        WGrid_child_cell_set(C->small_cup_areas[i], ivec4_(i, 0, 1, 1));
        WObj_fixed_size_set(C->small_cup_areas[i], vec2_(32, 32));
        C->time_btns[i] = WBtn_new(C->grid);
        WGrid_child_cell_set(C->time_btns[i], ivec4_(i, 1, 1, 1));
        WObj_padding_set(C->time_btns[i], vec4_(0, 2, 0, 2));
        oobj text = WTextShadow_new(C->time_btns[i], "");
//        WText_color_set(text, R_GRAY_X(0.2));
        WObj_padding_set(text, vec4_(1));
    }

    /**
     * Action button (edit; abort; rebrew; done)
     */
    oobj action_align = WAlign_new_center_h(C->gui);
    C->action_btn = WBtn_new(action_align);
    WBtn_color_set(C->action_btn, vec4_(0.66));
    C->action_btn_text = WTextShadow_new(C->action_btn, "");
    WText_color_set(C->action_btn_text, vec4_(1, 1, 1, 0.66));
    WObj_padding_set(C->action_btn_text, vec4_(1));


    /**
     * Updates the app icon
     */
    a_app_icon_set_file("res/ex/tea_icon.png");

    /**
     * Start of the state machine
     */
    tea_state_start(view);
}

/**
 * Called each frame to update
 */
O_STATIC
void tea_update(oobj view, oobj tex, float dt)
{
    struct tea_context *C = o_user(view);

    /**
     * Update the gui
     */
    vec2 min_size = {{96, 96}};
    vec2 lt = {{-min_size.x/2, +min_size.y/2}};
    WTheme_update(C->theme, C->gui, lt, min_size, a_pointer);


    /**
     * Check for button presses.
     * WBtn_clicked must be called to let the button "work".
     * Else WBtn_auto_mode can be used
     */
    for(int i=0; i<3; i++) {
        if(WBtn_clicked(C->time_btns[i])) {
            if(C->state == TEA_START) {
                float tea_time = tea_get_tea_time(view, i);
                tea_state_timer(view, i, tea_time);
            }
            else if(C->state == TEA_EDIT) {
                tea_start_time_edit(view, i);
            }
        }
    }

    if(WBtn_clicked(C->action_btn)) {
        if(C->state == TEA_START){
            tea_state_edit(view);
        }
        else {
            tea_state_start(view);
        }
    }

    /**
     * Runs the timer and displays the text
     */
    if(C->state == TEA_TIMER) {
        float elapsed = (float) o_timer_elapsed_s(C->timer);
        float remaining = C->tea_time - elapsed;

        if(remaining>0) {
            char buf[32];
            tea_time_to_str(buf, remaining);
            WText_text_set(C->text, buf);
            tea_set_app_title(buf);

        } else {
            // this block is called once at the end of the countdown
            tea_state_finished(view);
        }

    }

    /**
     * cound down the alarm time. see below
     */
    if(C->state == TEA_FINISHED) {
        if(C->alarm_time>0) {
            C->alarm_time-=dt;
        }
    }


    /**
     * While timer state is running or the alarm is played, we want the app to ignore the app pause
     */
    bool ignore_pause = C->state==TEA_TIMER || (C->state==TEA_FINISHED && C->alarm_time>0);
    a_app_suspend_paused_set(!ignore_pause);

}

/**
 * Called each frame to render
 */
O_STATIC
void tea_render(oobj view, oobj tex, float dt)
{
    struct tea_context *C = o_user(view);

    /**
     * Render the backgrond color
     */
    vec4 bg = C->bg_color;
    if(C->state == TEA_EDIT) {
        // darken the rgb part of the color by half
        bg.xyz = vec3_scale(bg.xyz, 0.5);
    }
    RTex_clear_full(tex, bg);

    /**
     * Render all buttons and text, etc.
     */
    WTheme_render(C->theme, tex);

    /**
     * Animation time
     */
    C->cup_frame = m_mod(C->cup_frame + dt * TEA_FPS, 4);

    /**
     * Update the RObjRect rects.
     * 0 is the big main cup
     * 2-4 are the smaller cups above the time buttons
     */
    struct r_rect *cup_rect = o_at(C->cup, 0);
    cup_rect->rect = WObj_gen_rect(C->main_cup_area);
    cup_rect->uv_rect = u_atlas_rect(C->cup_atlas, (int) C->cup_frame + C->tea_sort*4);

    for(int i=0; i<3; i++) {
        cup_rect = o_at(C->cup, i+1);
        cup_rect->rect = WObj_gen_rect(C->small_cup_areas[i]);
        cup_rect->uv_rect = u_atlas_rect(C->cup_atlas, (int) C->cup_frame + i*4);
    }

    /**
     * Render the animated cups
     */
    RTex_ro(tex, C->cup);


    if(a_input_pressed(A_INPUT_BACK)) {
        o_log("exit due to back clicked (android back button for example)");
        a_app_scene_exit_current();
    }

}


/**
 * Starting point of the tea app
 * @param root parent to allocate on
 * @return AScene created of the tea app
 */
oobj ex_tea_main(oobj root)
{
    oobj view = AView_new(root, tea_setup, tea_update, tea_render);
    oobj scene = AScene_new(root, view, true);
    return scene;
}

/**
 * Another Starting point of the tea app. This time the scene is created during a splash screen
 * @param root parent to allocate on
 * @return AScene of the splash scene that creates the mia app
 */
oobj ex_tea_main_splashed(oobj root)
{
    oobj view = AView_new(root, tea_setup, tea_update, tea_render);
    oobj splash = u_splash_new_mia(root, view, true,
                                   R_BLACK, R_WHITE, "HORSIMANN", 1.0f, 0.2f);
    return splash;
}
