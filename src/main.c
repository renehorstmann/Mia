/**
 * Welcome to Mia :)
 * This will be the starting point of the app
 *      (at the end of this file is the main function)
 * Feel free to tinker around.
 * Or start the Examples App; Tea App or Mia Paint.
 *
 * Also have a look at the examples which act as a tutorial for coding in Mia ;)
 */

/** Includes most of Mia */
#include "mia.h"


/** Includes the Examples App and Tea App (which is part of the examples) */
#include "ex/ex.h"

/** Includes Mia Paint */
#include "mp/mp.h"


/**
 * Context Data for the hello world AView
 */
struct context {

    /**
     * RObjText to render "HELLO WORLD" (Render Object)
     */
    oobj hello;
};

/**
 * Called once at start of this AView
 * @param view AView object
 */
O_STATIC
void setup(oobj view)
{
    /** Get user context */
    struct context *C = o_user_set(view, o_new0(view, *C, 1));

    /** Logs an info string, works with formatting like printf */
    o_log("hello world");

    /**
     * Creates the render object with:
     * view as parent (manages memory, etc.)
     * 32 maximal characters to render
     * "HELLO WORLD" as init text
     */
    C->hello = RObjText_new_font35(view, 32, "HELLO WORLD");
}

/**
 * Called every frame before rendering to handle simulation, events, etc.
 * @param view AView object
 * @param tex RTex to render to (or NULL for the backbuffer...)
 *            Even if available, please render in the render function
 * @param dt delta time in seconds (time between this and the last frame)
 */
O_STATIC
void update(oobj view, oobj tex, float dt)
{
    struct context *C = o_user(view);

    /** nothing to do here... */
}

/**
 * Called every frame to render
 */
O_STATIC
void render(oobj view, oobj tex, float dt)
{
    struct context *C = o_user(view);

    /** Clears RObj tex to a gray color of vec4_(0.33, 0.33, 0.33, 1.0) */
    RTex_clear(tex, R_GRAY_X(0.33));

    /** Renders the render object hello onto tex */
    RTex_ro(tex, C->hello);
}



/**
 * Starting point of the mia app with all systems initialized
 * @param root OObj to allocate on, acts as root parent for all objects
 */
O_STATIC
void app_main(oobj root)
{

    /**
     * Apps
     * Remove line comments to start
     */

    /** Examples */
    //ex_main(root);
    //return;

    /** Tea Timer */
    //ex_tea_main_splashed(root);
    //return;

    /** Mia Paint */
    //mp_main(root);
    //return;



    /**
     * AScene with the AView of this main.c hello world file
     */
    oobj view = AView_new(root, setup, update, render);
    oobj scene = AScene_new(root, view, true);
}


/**
 * Actual c main entry point.
 */
int main(int argc, char **argv)
{
    /**
     * NULL could be replaced with some app options like title or window size, etc.
     */
    a_app_run(app_main, NULL);
    return 0;
}
