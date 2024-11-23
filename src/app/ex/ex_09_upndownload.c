/**
 * File uploading and downloading.
 * Opens the system file dialog.
 * For this example, an image can be uploaded from which a funny colored collage is created.
 * That collage can be downloaded
 */


#include "mia.h"


/**
 * Size of the collage image and the render size
 */
static const vec2 EX_09_IMG_SIZE = {{1024, 1024}};
static const vec2 EX_09_IMG_AREA = {{128, 128}};

/**
 * Num of collage items
 */
#define EX_09_MANIPULATE_RUNS 64

/**
 * file up and download need a saved file
 */
#define EX_09_FILE "tmp.img.png"

struct ex_09_context {
    oobj view;

    /**
     * Widget stuff
     */
    oobj theme;
    oobj gui;
    oobj img_area;
    oobj upload_btn;
    oobj download_btn;

    /**
     * RTex of the collage
     */
    oobj img;
};


/**
 * Creates the collage image into the context RTex C->img
 * @param img is the source item to render for the collage (RTex)
 */
O_STATIC
void ex_09_create_collage(oobj view, oobj img)
{
    struct ex_09_context *C = o_user(view);

    /**
     * RTex_clear could also be used, but clear_full ignores the viewport and is faster
     */
    RTex_clear_full(C->img, R_BLACK);

    /**
     * Constant sizes
     */
    vec2 img_size = RTex_size(img);
    float img_min_size = m_min(img_size.x, img_size.y);
    float res_min_size = m_min(EX_09_IMG_SIZE.x, EX_09_IMG_SIZE.y);

    /**
     * We create a render object that is able to draw the full collage,
     *      batched in a single draw call.
     * It has EX_09_MANIPULATE_RUNS quads and the given img as RTex to render
     */
    oobj ro = RObjQuad_new_color(view, EX_09_MANIPULATE_RUNS, img, false);

    /**
     * Setup each quad.
     * They are rendered on top of each other, so first is rendered first
     */
    for(int i=0; i<EX_09_MANIPULATE_RUNS; i++) {

        /**
         * scaling from big (2.0f) to small (0.2f) according to the minimal result size
         */
        float t = (float) i / (float) (EX_09_MANIPULATE_RUNS-1);
        float scale = m_mix(2.0f, 0.2f, t) * res_min_size / img_min_size;

        /**
         * Calc quad stuff like render size, pos and angle to create the pose
         */
        vec2 render_size = vec2_scale(img_size, scale);
        vec2 pos = vec2_random_noise_v(vec2_(0), vec2_scale(EX_09_IMG_SIZE, 0.66));
        float angle = m_random() * 2 * m_PI;

        mat4 pose = u_pose_new_angle(m_2(pos), m_2(render_size), angle);

        /**
         * the color shader uses the additional 's','t','u','v'
         */
        vec4 hsva_shift = vec4_(m_random(), 0, 0, 0);

        /**
         * Set quad stuff
         */
        struct r_quad *q = o_at(ro, i);
        q->pose = pose;
        q->t = hsva_shift;
    }

    /**
     * Batched draw call
     */
    RTex_ro(C->img, ro);

    o_del(ro);
}


O_STATIC
void ex_09_on_file_uploaded(void *user_data, const char *file, const char *opt_user_file, bool success)
{
    struct ex_09_context *C = user_data;

    if(!success) {
        o_log("uploading failed");
        return;
    }

    /**
     * Load uploaded file as RTex
     */
    oobj img = RTex_new_file(C->view, EX_09_FILE);
    if(!RTex_valid(img)) {
        o_log("upload failed, invalid image");
        o_del(img);
        return;
    }

    /**
     * Create collage
     */
    ex_09_create_collage(C->view, img);
    o_del(img);
}

/**
 * This event is called if the upload button is clicked
 */
O_STATIC
void ex_09_upload(oobj btn)
{
    struct ex_09_context *C = o_user(btn);

    o_log("upload");

    /**
     * This call >blocks< until the system dialog finishes.
     * The selected file will be saved/copied under the given file path.
     * Have a look at x_file_upload (and x_file_download) for a fallback version in which XViewFiles is created
     * @note noop on MIA_PLATFORM_CXXDROID
     */
    o_file_upload(EX_09_FILE, ex_09_on_file_uploaded, C);

}


O_STATIC
void ex_09_on_file_downloaded(void *user_data, const char *file, const char *opt_user_file, bool success)
{
    o_log("file downloaded as: %s", o_or(opt_user_file, "?"));
}
/**
 * This event is called when the download button is clicked
 */
O_STATIC
void ex_09_download(oobj btn)
{
    struct ex_09_context *C = o_user(btn);

    o_log("download");

    RTex_write_file(C->img, EX_09_FILE);

    /**
     * Like o_file_upload, this function >blocks< and opens the system file chooser.
     * @note noop on MIA_PLATFORM_CXXDROID
     */
    o_file_download(EX_09_FILE, ex_09_on_file_downloaded, C);
}

//
// view functions
//

O_STATIC
void ex_09_setup(oobj view)
{
    struct ex_09_context *C = o_user_set(view, o_new0(view, *C, 1));
    C->view = view;


    C->theme = WTheme_new_tiny(view);
    C->gui = WBox_new(view, WBox_LAYOUT_V);
    WBox_spacing_set(C->gui, vec2_(2));

    oobj title_align = WAlign_new_center_h(C->gui);
    oobj title = WTextShadow_new(title_align, "UP 'N DOWNLOAD\nCOLLAGE CREATOR");
    WText_char_scale_set(title, vec2_(2));

    oobj img_frame = WFrame_new(C->gui);
    C->img_area = WObj_new(img_frame);
    WObj_fixed_size_set(C->img_area, EX_09_IMG_AREA);


    oobj btn_box_align = WAlign_new_center_h(C->gui);
    oobj btn_box = WBox_new(btn_box_align, WBox_LAYOUT_H);
    WBox_spacing_set(btn_box, vec2_(8));

    C->upload_btn = WBtn_new(btn_box);
    oobj upload_text = WText_new(C->upload_btn, "UPLOAD");
    WObj_padding_set(upload_text, vec4_(4));
    WBtn_auto_mode_set(C->upload_btn, WBtn_AUTO_CLICKED);
    WBtn_auto_event_set(C->upload_btn, ex_09_upload);
    o_user_set(C->upload_btn, C);

    C->download_btn = WBtn_new(btn_box);
    oobj download_text = WText_new(C->download_btn, "DOWNLOAD");
    WObj_padding_set(download_text, vec4_(4));
    WBtn_auto_mode_set(C->download_btn, WBtn_AUTO_CLICKED);
    WBtn_auto_event_set(C->download_btn, ex_09_download);
    o_user_set(C->download_btn, C);


    C->img = RTex_new(view, NULL, m_2(EX_09_IMG_SIZE));

    oobj img = RTex_new_file(view, "res/ex/icon16.png");
    ex_09_create_collage(view,img);
    o_del(img);
}

O_STATIC
void ex_09_update(oobj view, oobj tex, float dt)
{
    struct ex_09_context *C = o_user(view);

    WTheme_update(C->theme, C->gui, vec2_(-64,88), vec2_(128,0), a_pointer);
}

O_STATIC
void ex_09_render(oobj view, oobj tex, float dt)
{
    struct ex_09_context *C = o_user(view);

    RTex_clear(tex, vec4_(0.4, 0.5, 0.4, 1.0));
    WTheme_render(C->theme, tex);

    /**
     * Renders the full image above the created area pose
     */
    mat4 pose = WObj_gen_pose(C->img_area);
    mat4 uv = RTex_pose(C->img, 0, 0);
    RTex_blit_ex(tex, C->img, pose, uv);
}


O_EXTERN
oobj ex_09_main(oobj root)
{
    oobj view = AView_new(root, ex_09_setup, ex_09_update, ex_09_render);
    oobj scene = AScene_new(root, view, true);
    return scene;
}

/**
 * Summary:
 *
 * This example shows how to up 'n download files from the user.
 * The uploaded image will be used to create a funny collage in a batched draw call.
 */
