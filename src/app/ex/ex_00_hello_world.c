/**
 * Mia's hello world.
 * Simply shows a rotating mia icon and logs "hello world" once at startup
 */


/**
 * "mia.h" will include (most) of the Mia engine.
 * Which will result in the modules:
 *
 * - "o" Object oriented standard library
 * - "m" Math and linear algebra
 * - "s" Sound stuff
 * - "r" Render stuff working on OpenGL(ES|WEB)
 * - "a" App Scenes, user input, logins
 * - "u" Utilities
 * - "w" Widgets: GUI windows, buttons, input fields
 * - "x" Extended stuff like a virtual keyboard or color picker widget
 *
 * Each module has a dependency to the upper one.
 */
#include "mia.h"



/**
 * Naming Conventions:
 * 
 * - **Types and Objects:** 
 *   - oobj: simple typedef to void* (primitive types in o/common.h).
 *   - OObj, OArray, RTex, AView: PascalCase for object names.
 *   
 * - **Functions and Variables:**
 *   - o_clear, o_str_equals: snake_case for functions and variables.
 *   
 * - **Object-Specific Functions:**
 *   - OObj_name, RTex_blit: ObjectName_snake_case.
 *   
 * - **Prefixes:**
 *   - Functions and objects have a module prefix: "o_"log, "O"Obj, "R"Tex, "A"View.
 *   
 * - **Header Files:**
 *   - Named by their object or specific group of functions.
 *   - Functions in these headers are usually prefixed: o/str.h -> o_str_clone, o_str_find.
 *   
 */


/**
 * All stuff used in this example put in a static struct.
 */
static struct {

    /**
     * oobj is a simple typedef to void* ( o/common.h ).
     * Mia uses this to pass OObj's and it's subclasses (mia object; o/OObj.h )
     * In this case, sprite will be of type RTex* ( r/RTex.h )
     *      which is an object to handle GPU textures of the "r"ender module
     */
    oobj sprite;

    /**
     * angle position in [0:1]
     */
    float sprite_t;

} ex_00_L;
/**
 * The "ex"amples module may be configured as unity built (default, speeds up compile+link time >a lot<),
 *   so needs different names for the local data in each example script
 */


/**
 * This function is called once, when this example is started
 * @param view AView object of this example scene ( a/AView.h )
 *             More infos about AView is at the end of this example
 * @note O_STATIC -> static for function definitions ( o/common.h )
 */
O_STATIC
void ex_00_setup(oobj view) {
    /**
     * logs an info string ( o/log.h )
     * also accepts formatted strings like: o_log("value[%i]=%f", i, value[i]);
     */
    o_log("hello world");


    /**
     * memset to 0 ( o/common.h )
     * the common header functions and stuff only use "o_" as prefix, not "o_common_clear" as others would do.
     * Because the classic memset is in the group of function that expects a byte size,
     *      Mia wraps it into (element_size, count) to avoid (common) bugs
     * Others are o_memcpy and o_memmove which also have an element_size as parameter
     */
    o_clear(&ex_00_L, sizeof ex_00_L, 1);



    /**
     * Mia's object management system automatically handles the deletion of resources
     * and objects through a hierarchical parent-child relationship.
     * When an object (e.g. view) is deleted, all associated resources (textures, sounds, etc.)
     * and child objects are recursively deleted. This significantly reduces the need for manual
     * memory management (e.g. free calls), making resource management more intuitive and less error-prone.
     */

    /**
     * An RTex is an object which holds and works with texture data on the GPU ( r/RTex.h )
     * "*_new" are constructors,
     * "*_new_file" is a specialized constructor, in this case it loads the contents of the given image file.
     *
     * view (AView object of this setup function) will get the parent of the RTex
     *
     * All files in the directory "res"ources packed into the app
     */
    ex_00_L.sprite = RTex_new_file(view, "res/ex/icon16.png");
}

/**
 * This function is called every frame
 * @param view AView object of this example scene
 * @param tex RTex object to render to (should be done in the render function)
 * @param dt delta time; time between this and the last frame
 */
O_STATIC
void ex_00_update(oobj view, oobj tex, float dt) {
    /**
     * Place your simulation loop here.
     * Also (pointer) events should be handled here and not in the render function
     */
}

/**
 * This function is called every frame to actually render stuff
 * @param view AView object of this example scene
 * @param tex RTex object to render to
 * @param dt delta time; time between this and the last frame (in seconds)
 */
O_STATIC
void ex_00_render(oobj view, oobj tex, float dt) {

    /**
     * A full rotation in 8 seconds
     */
    ex_00_L.sprite_t += dt / 8.0f;

    /**
     * float modulo from 0.0 -> 1.0, m_fract could have also been used ( m/sca/flt.h; sca:=scalar )
     */
    ex_00_L.sprite_t = m_mod(ex_00_L.sprite_t, 1.0f);

    /**
     * x and y for the center of the icon sprite.
     * The circle has a radius of 82 units.
     * Mia's default camera has a size of min. 180 units and is pixel perfect.
     * Its origin (xy=0) is at the center of the screen
     * The icon has a size of 16x16 == RTex_size(ex_00_L.sprite);
     */
    const float radius = (180.0f-16.0f)/2.0f;
    float rad = ex_00_L.sprite_t * 2 * m_PI;

    /**
     * m_cos, m_sin, m_mod, m_PI are part of the math module "m" and located in ( m/sca/flt.h )
     * The math module uses a slightly different naming approach.
     * It contains functions (and types) for dimensions [1:4] for the types:
     * float:   m_*,  vec2_*;  vec3_*;  vec4_*
     * double:  md_*, dvec2_*; dvec3_*; dvec4_*
     * int:     mi_*, ivec2_*; ivec3_*; ivec4_*
     * obyte:   mb_*, bvec2_*; bvec3_*; bvec4_*
     */
    float x = radius * m_cos(rad);
    float y = radius * m_sin(rad);


    /**
     * blits (not blends) the icon RTex (loaded in setup) onto the RTex "tex" from the AView.
     * Blitting replaces old image pixels with the new one, ignoring alpha blending
     * ( r/RTex.h )
     */
    RTex_blit(tex, ex_00_L.sprite, x, y);

    /**
     * Will show a rotating mia icon that may touch your screen borders.
     * The screen is cleared to black on each frame at the frame start.
     * If your screen size is a common one like 1080x1920, it will touch the border.
     * That's why the default camera min unit size is 180.
     *      Which is an anti prime and the smaller size of common screen sizes are a multiple of it.
     *      Like 1080=6*180 (6 pixel form a unit) or 1440=8*180; 720=4*180; ...
     */
}



/**
 * Starting point of this example,
 * called from src/ex/main.c which itself is called by src/main.c
 *
 * > If you want to create your own stuff, edit src/main.c ;) <
 *
 * @param root OObj to allocate on, if this gets deleted, the full example will get deleted (in recursion)
 *             Exactly this happens if you click on the "X" button on the upper left.
 *                  Which is a transparent scene rendered on top of the example scenes.
 * @note O_EXTERN functions will be global to the linker
 */
O_EXTERN
oobj ex_00_main(oobj root)
{
    /**
     * Creates an AView object with the functions above,
     * allocated on the given root object ( a/AView.h )
     */
    oobj view = AView_new(root, ex_00_setup, ex_00_update, ex_00_render);

    /**
     * The actual AScene which will display the AView on the screen ( a/AScene.h )
     * AScene's work together with the app's scenes stack to render the AView's.
     * An AView itself will not be rendered until its functions are executed elsewhere.
     * So each AScene needs an AView, but AView's could be used elsewhere (like in a USplit ( u/USplit.h ) )
     * the last parameter of the AScene_new constructor is a bool flag if the view should be moved into the AScene.
     * Because each OObj has a single parent, its sometimes necessary to move objects to change the parent.
     * if true, the AView "view" will be o_move'd into the new AScene like: o_move(view, scene).
     */
    oobj scene = AScene_new(root, view, true);
    return scene;
}


/**
 * Summary:
 *
 * In this example, we've explored the basics of creating a simple scene with Mia,
 * demonstrating object initialization, resource management, and rendering.
 * Understanding these core concepts is essential as you delve deeper into the engine.
 * I hope you got already familiar with the base concepts of Mia.
 * Which are:
 * - An object system which also manages resources in C.
 * - An easy to follow set of naming rules, which differ from other standards,
 *      but makes navigating and tracing objects more charming, also nice for auto completion.
 *      Feel free to surf through the header files of the different modules.
 *      Each header file acts as classic documentation.
 * - Platform independent app development with a fast compile/link time in mind.
 *      Just try it! Clone the repo, have a look at "doc/install.md" and compile the whole engine in a few seconds.
 *      Working with mia feels like working in a scripting language, a small change can be tested moments later!
 */
