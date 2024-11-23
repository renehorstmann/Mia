/**
 * Fetching a http request and extracting its json data
 */


#include "mia.h"

struct ex_08_context {
    /**
     * RObjText
     */
    oobj text;

    /**
     * OFetch
     */
    oobj fetch;
};


O_STATIC
void ex_08_check_fetch(oobj view)
{
    struct ex_08_context *C = o_user(view);

    /**
     * Check for the async fetch state.
     * struct oobj_opt is a returned oobj that may be NULL.
     * In this case NULL if not finished yet.
     */
    bool error;
    struct oobj_opt response = OFetch_response(C->fetch, &error);

    if(!error && !response.o) {
        // loading...
        return;
    }

    /**
     * Container object to allocate during this frame,
     * this and all allocated will be deleted at the end of the function.
     * See below.
     */
    oobj container = OObj_new(view);


    char *text;
    bool failed = false;

    if (error) {
        text = "ERROR!";
    } else {
        if (!response.o) {
            text = "LOADING...";
        } else {

            /**
             * response.o is now an OArray of the received data.
             * o_at(..., 0) is the begin address of the data / received string.
             */

            const char *received = o_at(response.o, 0);

            struct oobj_opt json = OJson_new_read_string(container, "root", received);
            failed = true;
            if (json.o) {
                struct oobj_opt date_json = OJson_get(json.o, "date");
                struct oobj_opt time_json = OJson_get(json.o, "time");
                struct oobj_opt timezone_json = OJson_get(json.o, "timeZone");

                const char *date = date_json.o ? OJson_string(date_json.o) : NULL;
                const char *time = time_json.o ? OJson_string(time_json.o) : NULL;
                const char *timezone = timezone_json.o ? OJson_string(timezone_json.o) : NULL;

                if (date && time && timezone) {
                    text = o_strf(container, "GOT INTERNET!\n\ndate: %s\ntime: %s\nzone: %s", date, time, timezone);
                    failed = false;
                }
            }
        }
    }

    if (failed) {
        text = "FAILED!";
    }

    /**
     * Update the text
     */
    RObjText_text_set(C->text, text);

    /**
     * Deleting the oobj container and so the allocated text if succeeded.
     */
    o_del(container);

    /**
     * Deleting the handled fetch, will set it to NULL so that this function won't be executed again
     */
    o_del(C->fetch);
}


O_STATIC
void ex_08_setup(oobj view)
{
    struct ex_08_context *C = o_user_set(view, o_new0(view, *C, 1));

    C->text = RObjText_new_font35_shadow(view, 128, "LOADING", NULL);

    /**
     * Directly starts to fetch in async as GET request
     */
    C->fetch = OFetch_new(view, "https://timeapi.io/api/Time/current/zone?timeZone=UTC", NULL, 0);
}

O_STATIC
void ex_08_update(oobj view, oobj tex, float dt)
{
    /**
     * Nothing to do here
     */
}

O_STATIC
void ex_08_render(oobj view, oobj tex, float dt)
{
    struct ex_08_context *C = o_user(view);

    // if NULL do nothing with it...
    if (C->fetch) {
        ex_08_check_fetch(view);
    }

    /**
     * Sets text pose to be 2x scaled in the center
     */
    vec2 size = RObjText_text_size(C->text);
    RObjText_pose_set(C->text, u_pose_new(-size.x, 0, 4, 4));

    RTex_ro(tex, C->text);
}


O_EXTERN
oobj ex_08_main(oobj root)
{
    oobj view = AView_new(root, ex_08_setup, ex_08_update, ex_08_render);
    oobj scene = AScene_new(root, view, true);
    return scene;
}
