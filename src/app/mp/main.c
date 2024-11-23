#include "app/mp/main.h"
#include "a/app.h"
#include "a/AScene.h"

#include "r/common.h"
#include "u/USplit.h"
#include "app/mp/views.h"
#include "r/RTex.h"
#include "o/timer.h"

static struct {
    oobj root;
    
    oobj canvas, palette, pattern;
    oobj split;
} main_L;


O_STATIC
void setup(oobj view) {
    // init systems
    oobj mp_brush__init(oobj parent);
    mp_brush__init(view);

    oobj mp_canvas__init(oobj parent);
    mp_canvas__init(view);


    main_L.canvas = mp_views_new_canvas(view);
    main_L.palette = mp_views_new_palette(view);
    main_L.pattern = mp_views_new_pattern(view);

    oobj sub_split = USplit_new(view, main_L.canvas, main_L.pattern, USplit_MODE_V_TOP, 16, 1);
    main_L.split = USplit_new(view, main_L.palette, sub_split, USplit_MODE_V_BOTTOM, 48, 1);
}

O_STATIC
void update(oobj view, oobj tex, float dt) {
    oobj cam = a_app_cam();

    USplit_update(main_L.split, tex);

}

O_STATIC
void render(oobj view, oobj tex, float dt) {
    oobj cam = a_app_cam();

    RTex_clear(tex, R_YELLOW);
    USplit_render(main_L.split, tex);
}




oobj mp_main(oobj root)
{
    o_clear(&main_L, sizeof main_L, 1);
    
    oobj view = AView_new(root, setup, update, render);
    AView_scale_auto_set(view, true);
    oobj scene = AScene_new(root, view, true);
    AScene_full_even_size_set(scene, true);

    main_L.root = OObj_new(scene);
    return scene;
}

oobj mp_root(void)
{
    return main_L.root;
}
