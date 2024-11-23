#ifndef MIA_WTHEME_H
#define MIA_WTHEME_H


/**
 * @file WTheme.h
 *
 * Object
 *
 * A theme bundles a render object for rects, its texture and an atlas for the sprites.
 * Used to render the default widgets.
 *
 * Apart from other objects in the 'w' module, WTheme is >NOT< a subobject of WObj.
 *
 * Operators:
 * o_num -> RObjRect_num
 * o_at -> RObjRect_at
 */


#include "o/OObj.h"
#include "r/rect.h"
#include "r/proj.h"
#include "a/pointer.h"
#include "u/atlas.h"

/** object id */
#define WTheme_ID OObj_ID "WTheme"


#define WTheme_FONT_NUM 96

// 3x3 rows, row major from top left (lt, ct, rt; lc, cc, rc; lb, cb, rb)
#define WTheme_BTN__SPRITES 9
// 3x3 rows, row major from top left (lt, ct, rt; lc, cc, rc; lb, cb, rb)
#define WTheme_PANE__SPRITES 9
// 3 sprites, start, center, end
#define WTheme_SLIDER_BAR__SPRITES 3

#define WTheme_CUSTOM_8_NUM 8
#define WTheme_CUSTOM_16_NUM 6
#define WTheme_CUSTOM_32_NUM 6
#define WTheme_CUSTOM_64_NUM 2

enum WTheme_indices {
    WTheme_FONT,
    WTheme_FONT_SHADOW = WTheme_FONT + WTheme_FONT_NUM,
    WTheme_WHITE = WTheme_FONT_SHADOW + WTheme_FONT_NUM,
    WTheme_ICON_CROSS,
    WTheme_ICON_CHECK,
    WTheme_ICON_WND_MIN,
    WTheme_ICON_WND_WND,
    WTheme_ICON_WND_MAX,
    WTheme_ICON_WND_CROSS,
    WTheme_ICON_LEFT,
    WTheme_ICON_RIGHT,
    WTheme_ICON_UP,
    WTheme_ICON_DOWN,
    WTheme_ICON_EXCLAMATION,
    WTheme_ICON_QUESTION,
    WTheme_ICON_GEAR,
    WTheme_ICON_SAVE,
    WTheme_ICON_LOAD,
    WTheme_ICON_SEARCH,
    WTheme_ICON_RECORD,
    WTheme_ICON_PLAY,
    WTheme_ICON_PAUSE,
    WTheme_ICON_STOP,
    WTheme_ICON_DIR_LEFT,
    WTheme_ICON_DIR_RIGHT,
    WTheme_ICON_DIR_UP,
    WTheme_ICON_DIR_DOWN,
    WTheme_ICON_DOTS,
    WTheme_ICON_PICKER_X,
    WTheme_ICON_PICKER_Y,
    WTheme_ICON_PICKER_XY,
    WTheme_ICON_SHIFT,
    WTheme_ICON_CAPS,
    WTheme_ICON_SHIFT_INV,
    WTheme_ICON_ENTER,
    WTheme_ICON_TAB,
    WTheme_ICON_ROMAN_1,
    WTheme_ICON_ROMAN_2,
    WTheme_ICON_FILE,
    WTheme_ICON_FILE_TXT,
    WTheme_ICON_FILE_MUS,
    WTheme_ICON_FILE_IMG,
    WTheme_BTN_FLAT,
    WTheme_BTN_FLAT__PRESSED = WTheme_BTN_FLAT + WTheme_BTN__SPRITES,
    WTheme_BTN_FLAT_ROUND = WTheme_BTN_FLAT + WTheme_BTN__SPRITES*2,
    WTheme_BTN_FLAT_ROUND__PRESSED = WTheme_BTN_FLAT_ROUND + WTheme_BTN__SPRITES,
    WTheme_BTN_DEF = WTheme_BTN_FLAT_ROUND + WTheme_BTN__SPRITES*2,
    WTheme_BTN_DEF__PRESSED = WTheme_BTN_DEF + WTheme_BTN__SPRITES,
    WTheme_BTN_DEF_ROUND = WTheme_BTN_DEF + WTheme_BTN__SPRITES*2,
    WTheme_BTN_DEF_ROUND__PRESSED = WTheme_BTN_DEF_ROUND + WTheme_BTN__SPRITES,
    WTheme_BTN_BIG = WTheme_BTN_DEF_ROUND + WTheme_BTN__SPRITES*2,
    WTheme_BTN_BIG__PRESSED = WTheme_BTN_BIG + WTheme_BTN__SPRITES,
    WTheme_BTN_BIG_ROUND = WTheme_BTN_BIG + WTheme_BTN__SPRITES*2,
    WTheme_BTN_BIG_ROUND__PRESSED = WTheme_BTN_BIG_ROUND + WTheme_BTN__SPRITES,
    WTheme_BTN_RADIO = WTheme_BTN_BIG_ROUND + WTheme_BTN__SPRITES*2,
    WTheme_BTN_RADIO__PRESSED = WTheme_BTN_RADIO + WTheme_BTN__SPRITES,
    WTheme_BTN_CHECK = WTheme_BTN_RADIO + WTheme_BTN__SPRITES*2,
    WTheme_BTN_CHECK__PRESSED = WTheme_BTN_CHECK + WTheme_BTN__SPRITES,
    WTheme_BTN_SWITCH = WTheme_BTN_CHECK + WTheme_BTN__SPRITES*2,
    WTheme_BTN_SWITCH__PRESSED = WTheme_BTN_SWITCH + WTheme_BTN__SPRITES,
    WTheme_PANE = WTheme_BTN_SWITCH + WTheme_BTN__SPRITES*2,
    WTheme_PANE_ROUND = WTheme_PANE + WTheme_PANE__SPRITES,
    WTheme_PANE_ROUND_TOP = WTheme_PANE_ROUND + WTheme_PANE__SPRITES,
    WTheme_PANE_ROUND_BOTTOM = WTheme_PANE_ROUND_TOP + WTheme_PANE__SPRITES,
    WTheme_SLIDER_H_BAR = WTheme_PANE_ROUND_BOTTOM + WTheme_PANE__SPRITES,
    WTheme_SLIDER_H_BADGE = WTheme_SLIDER_H_BAR + WTheme_SLIDER_BAR__SPRITES,
    WTheme_SLIDER_V_BAR,
    WTheme_SLIDER_V_BADGE = WTheme_SLIDER_V_BAR + WTheme_SLIDER_BAR__SPRITES,
    WTheme_CUSTOM_8,
    WTheme_CUSTOM_16 = WTheme_CUSTOM_8 + WTheme_CUSTOM_8_NUM,
    WTheme_CUSTOM_32 = WTheme_CUSTOM_16 + WTheme_CUSTOM_16_NUM,
    WTheme_CUSTOM_64 = WTheme_CUSTOM_32 + WTheme_CUSTOM_32_NUM,
    WTheme_MAX_IDX = WTheme_CUSTOM_64 + WTheme_CUSTOM_64_NUM
};

/**
 * @param c character
 * @return WTheme atlas idx
 */
O_INLINE
int WTheme_char_idx(char c)
{
    if (c < ' ' || c > '~') {
        c = ' ';
    }
    return WTheme_FONT + c - ' ';
}

/**
 * @param c character
 * @return WTheme atlas idx
 */
O_INLINE
int WTheme_char_shadow_idx(char c)
{
    if (c < ' ' || c > '~') {
        c = ' ';
    }
    return WTheme_FONT_SHADOW + c - ' ';
}


typedef struct {
    OObj super;

    // RObjRect with _color shader
    oobj ro;
    struct u_atlas atlas;
    
    // may be set by WObj's in the update routine to acquire an aditional full cleared update
    bool reupdate;
} WTheme;


/**
 * Initializes the object
 * @param obj WTheme object
 * @param parent to inherit from
 * @param tex theme batch atlas texture
 * @param move_tex if true, tex is o_move'd into the internal ro
 * @param atlas atlas for the tex, according to the WTheme atlas indices
 * @param move_atlas if true, atlas is move'd into this object
 * @return obj casted as WTheme
 */
O_EXTERN
WTheme *WTheme_init(oobj obj, oobj parent, oobj tex, bool move_tex,
                    struct u_atlas atlas, bool move_atlas);

/**
 * Creates a new the WTheme object
 * @param parent to inherit from
 * @param tex theme batch atlas texture
 * @param move_tex if true, tex is o_move'd into the internal ro
 * @param atlas atlas for the tex, according to the WTheme atlas indices
 * @param move_atlas if true, atlas is move'd into this object
 * @return The new object
 */
O_INLINE
WTheme *WTheme_new(oobj parent, oobj tex, bool move_tex,
                   struct u_atlas atlas, bool move_atlas)
{
    OObj_DECL_IMPL_NEW(WTheme, parent, tex, move_tex, atlas, move_atlas);
}

/**
 * @return default tiny theme
 */
O_EXTERN
WTheme *WTheme_new_tiny(oobj parent);


//
// virtual implementations:
//

/**
 * virtual operator function
 * @param obj WTheme object
 * @return number of r_rect's
 */
O_EXTERN
osize WTheme__v_op_num(oobj obj);

/**
 * virtual operator function
 * @param obj WTheme object
 * @return r_rect at given idx
 */
O_EXTERN
void *WTheme__v_op_at(oobj obj, osize idx);


//
// object functions
//


/**
 * @param obj WTheme object
 * @return internal render object RObjRect
 */
OObj_DECL_GET(WTheme, oobj, ro)

/**
 * @param obj WTheme object
 * @return theme atlas
 */
OObj_DECL_GETSETREF(WTheme, struct u_atlas, atlas)

/**
 * Acquires an additional update during WTheme_update.
 * May be set by WObj's, if sizes changed
 * @param obj WTheme object
 * @note on init its true as default (first update)
 */
OObj_DECL_GETSET(WTheme, bool, reupdate)
/**
 * Clears / resets all internal rects
 * @param obj WTheme object
 */
O_EXTERN
void WTheme_clear(oobj obj);

/**
 * Allocates some new rects
 * @param obj WTheme object
 * @param num to allocate
 * @return back_idx for the first allocated
 */
O_EXTERN
int WTheme_alloc(oobj obj, int num);

/**
 * @param obj WTheme object
 * @return r_rect at back_idx
 */
O_EXTERN
struct r_rect *WTheme_at(oobj obj, int back_idx);

/**
 * @param obj WTheme object
 * @return number of r_rects allocated
 */
O_EXTERN
int WTheme_num(oobj obj);


/**
 * @param obj WTheme object
 * @return RTex object, just a wrapper to RObjRect_tex(WTheme_ro(obj))
 */
O_EXTERN
oobj WTheme_tex(oobj obj);

/**
 * @param obj WTheme object
 * @param btn_idx atlas index
 * @return the offset between pressed and unpressed for the inner field (the children)
 */
O_INLINE
vec2 WTheme_btn_press_offset(oobj obj, int btn_idx)
{
    struct u_atlas atlas = WTheme_atlas(obj);
    return vec2_sub_v(u_atlas_size(atlas, btn_idx+WTheme_BTN__SPRITES),
                      u_atlas_size(atlas, btn_idx));
}


/**
 * Calls WTheme_clear and then WObj_update
 * @param obj WTheme object
 * @param wobj WObj object to update (root widget)
 * @param lt left top
 * @param min_size minimal size that may be used for this widget
 * @param pointer_fn pointer function to use, may get overridden with self->pointer_fn
 *                   in most cases "a_pointer" (or "a_pointer_p" if rendered onto the p matrix of the cam)
 *                   asserts not NULL
 * @return used wobj size
 */
O_EXTERN
vec2 WTheme_update(oobj obj, oobj wobj, vec2 lt, vec2 min_size, a_pointer__fn pointer_fn);

/**
 * Calls WTheme_clear and then WStack_update
 * @param obj WTheme object
 * @param stack WStack object to update (root widget)
 * @param tex RTex object to gain r_proj from
 * @param opt_out_lt if not NULL, set to used lt
 * @param pointer_fn pointer function to use, may get overridden with self->pointer_fn
 *                   in most cases "a_pointer" (or "a_pointer_p" if rendered onto the p matrix of the cam)
 *                   asserts not NULL
 * @return used stack size
 */
O_EXTERN
vec2 WTheme_update_stack(oobj obj, oobj stack, oobj tex, vec2 *opt_out_lt, a_pointer__fn pointer_fn);


/**
 * Renders the whole WTheme.
 * Extended version.
 * @param obj WTheme object
 * @param tex RTex to render in, or NULL to use the back buffer
 * @param opt_proj the camera projection to use (vp), if NULL: RTex_proj(tex) is used instead
 * @param update if true: calls RObj_update first on the internal render object 
 */
O_EXTERN
void WTheme_render_ex(oobj obj, oobj tex, const struct r_proj *opt_proj, bool update);

/**
 * Renders the whole WTheme.
 * Simpler default version.
 * Renders the object to the bound frame buffer
 * @param obj RObj object
 * @param tex RTex to render in, or NULL to use the back buffer
 * @note just calls WTheme_render_ex(obj, tex, NULL, true, post_clear)
 */
O_INLINE
void WTheme_render(oobj obj, oobj tex)
{
    WTheme_render_ex(obj, tex, NULL, true);
}

#endif //MIA_WTHEME_H
