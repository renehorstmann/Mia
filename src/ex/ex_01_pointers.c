/**
 * Renders a batch of colored quads, which can be positioned by a pointer.
 * A pointer is either a mouse cursor or touch input
 */


/**
 * Instead of including all of mia (with mia.h) we can include the needed modules.
 *
 * "o" is the base library, containing the object oriented base class, logging, arrays, ...
 * "r" is for rendering
 * "a" is for app stuff like creating the AScene
 * "u" is for utilities like u_pose stuff
 */
#include "o/o.h"
#include "r/r.h"
#include "a/a.h"
#include "u/u.h"


/**
 * Number of colored quads to render
 */
#define NUM_QUADS 14


/**
 * As with example 00, all stuff used here.
 * But this time not as a static struct, but as object context struct.
 * This has the advantage that the context is >NOT< shared by multiple instances of the same view.
 * Each view gets its own context in contrast to a static struct, which is shared by all.
 */
struct ex_01_context {
    /**
     * RObjQuad to render a batch of colored quads in a single draw call
     */
     oobj batch;

     /**
      * stores the previous (last frame) pointer, see update function
      */
     struct a_pointer prev;

     /**
      * stores which quad is currently dragged/picked
      * osize is one of the number primitives, in this case int64_t or o64.
      * Mia uses signed sizes so it can detect overflows and can pass -1 for failures, etc.
      */
     osize dragged_idx;
};


O_STATIC
void ex_01_setup(oobj view) {

    /**
     * Each OObj may have user data.
     * We create the cleared context struct with o_new0 with view as parent.
     * So its automatically free'd when view is deleted.
     * o_user can save any void*, feel free to use it.
     */
    struct ex_01_context *C = o_user_set(view, o_new0(view, *C, 1));

    /**
     * returns a shared RTex with a single white pixel, should not be deleted (asserts it)
     */
    oobj white = r_tex_white();

    /**
     * An RObjQuad can render multiple (NUM_QUADS) quads batched in a single draw call
     * "*_new" are constructors,
     * "*_new_color" is a specialized constructor, in this case the shader makes use of passed colors.
     *
     * We pass false for the "move_tex" parameter, because this object should >not< delete the shared RTex
     */
    C->batch = RObjQuad_new_color(view, NUM_QUADS, white, false);

    /**
     * o_num is one of mia's object operators, its overloaded in RObjQuad to return the number of quads
     */
    for(osize i=0; i<o_num(C->batch); i++) {
        /**
         * o_at is another mia object operator to access an array item.
         * The RObjQuad has an internal array of r_quad's
         */
        struct r_quad *q = o_at(C->batch, i);

        /**
         * We set the pose for each r_quad.
         * The batch's array quad order is kept while rendering, so first will be drawn first
         * (x, y, w, h)
         * mia uses 3d poses as 4x4 matrices (mat4), more to this topic, see ( "u/pose.h" )
         */
        q->pose = u_pose_new(0, -80 + i*10, 20, 20);

        /**
         * converting a hsv(a) color to rgb(a)
         * vec4 -> 4x float
         * vec4_(...) creates an vec4 inplace
         *
         * rgba -> red, green, blue, alpha (transparency) [0:1]
         * hsva -> hue (color angle), saturation, value, alpha [0:1]
         */
        float hue = (float) i / (float) o_num(C->batch);
        vec4 hsva = vec4_(hue, 1, 1, 1);
        vec4 rgba = vec4_hsv2rgb(hsva);

        /**
         * An r_quad has besides pose and uv additional fields for shaders, called 's' 't' 'u' 'v'
         * In this case the color shader uses vec4 's' for the rgba color
         */
        q->s = rgba;
    }

    C->dragged_idx = -1;
}

O_STATIC
void ex_01_update(oobj view, oobj tex, float dt) {
    /**
     * Get user context from the view, created by setup
     */
    struct ex_01_context *C = o_user(view);

    /**
     * Get current pointer
     * @param idx=0: is the multitouch index, so just ignoring and using the first pointer
     * @param history=0: can be used to travel back in history.
     *                   touch events may have a higher frequency then the screen refresh (often 60 fps)
     *                   so we can access back in history to get the in between pointers.
     *                   a_pointer_history_num() to get the number of history pointer items
     *
     */
    struct a_pointer p = a_pointer(0, 0);

    /**
     * Compares the current pointer with the one from the previous frame.
     * Check if its down (first frame to touch or click)
     */
    if(a_pointer_down(p, C->prev)) {
        /**
         * Search which quad contains the pointer position
         */
        C->dragged_idx = -1;
        for(osize i=0; i<o_num(C->batch); i++) {
            struct r_quad *q = o_at(C->batch, i);

            /**
             * Returns true if the position is within the borders of the pose. (with respect to rotation)
             * q: current checked quad from the batch
             * p: current pointer
             * p.pos: vec4 of the pointer position (x, y, 0,  1)
             */
            if(u_pose_contains(q->pose, p.pos)) {
                C->dragged_idx = i;
            }
        }
        if(C->dragged_idx) {
            o_log("picked: %i", C->dragged_idx);
        }
    }

    /**
     * As with down, when the mouse unclicks or the touch ends
     */
    if(a_pointer_up(p, C->prev)) {
        o_log("lost: %i", C->dragged_idx);
        C->dragged_idx = -1;
    }

    /**
     * While dragging, update the quads position
     */
    if(C->dragged_idx>=0) {
        struct r_quad *q = o_at(C->batch, C->dragged_idx);

        /**
         * A short excursion on the subject of the math types from the m module ( "m/m_types/flt.h" )
         * The vector and matrix types are not just simple structs ala "struct {float x, y, z, w;};"
         * But make use of the union type.
         * A union >shares< memory for different internal names / types
         * Like:
         *
         *     union example {
         *         obyte bytes[4];
         *         int value;
         *     };
         *
         *     union example a;
         *     a.value = 1234;
         *
         * You can now access the bytes of the value with the field a.bytes and vice versa.
         * The vec2 type is formed as:
         *
         *     typedef union {
         *        float v[2];
         *        struct {
         *            float v0, v1;
         *        };
         *        struct {
         *            float r, g;
         *        };
         *        struct {
         *            float x, y;
         *        };
         *    } m_vec2;
         *
         * > typedef'fed to vec2 in ( "m/types/flt.h" )
         * So you can access the fields of the vec2 with the classic
         *      x and y
         *      OR r(ed) and (g)reen
         *      OR v0 and v1
         *      OR v[0] and v[1] (or pass the array pointer with .v)
         *
         * The vec3 gets a little bit more complicated:
         *
         *     typedef union {
         *         float v[3];
         *         struct {
         *             float v0, v1, v2;
         *         };
         *         struct {
         *             float r, g, b;
         *         };
         *         m_vec2 xy;
         *         struct {
         *             float x;
         *             union {
         *                 struct {
         *                     float y, z;
         *                 };
         *                 m_vec2 yz;
         *             };
         *         };
         *     } m_vec3;
         *
         * Apart from accessing a single element like x, y, z,
         * its also possible to access internal sub vec2 parts with .xy and .yz
         * vec4 also make use of this an addition to internal sub vec3 parts ala .xyz
         *
         * Back to this example we copy the pointers position into the quads pose and access x and y of the union vec4
         */
        u_pose_set_xy(&q->pose, p.pos.x, p.pos.y);
    }

    /**
     * Save current pointer as previous for the next frame
     */
    C->prev = p;

    /**
     * Pointer stuff should be handled in the update function.
     * When multiple transparent scenes work the same time, pointers are "handled" in the right order.
     */
}

O_STATIC
void ex_01_render(oobj view, oobj tex, float dt) {
    /**
     * Get user context from the view, created by setup
     */
    struct ex_01_context *C = o_user(view);

    /**
     * Renders the batch onto the texture.
     * RTex_ro(tex, C->batch) could also be used.
     */
    RObj_render(C->batch, tex);
}


O_EXTERN
oobj ex_01_main(oobj root)
{
    oobj view = AView_new(root, ex_01_setup, ex_01_update, ex_01_render);
    oobj scene = AScene_new(root, view, true);
    return scene;
}


/**
 * Summary:
 *
 * In this example, we demonstrated interactive pointer handling and rendering with Mia:
 *
 * - **Pointer Handling:** The update function tracks touch or mouse input to determine which quad is being dragged.
 *   It manages pointer down events to pick a quad, updates its position while dragging, and handles pointer up events.
 *
 * - **Rendering:** The RObjQuad is used to batch and render multiple colored quads in a single draw call.
 *   The quads' positions and colors are updated based on user input, with rendering handled in the render function
 */
