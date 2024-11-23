/**
 * Mia's Widget System.
 * This is a small example on how to setup and use widgets.
 * Will show a button with an icon and a text, that changes its style if clicked.
 */


#include "mia.h"

struct ex_04_context {

    /**
     * WTheme object ( w/WTheme.h ).
     * The widget system is designed to be rendered in a single batched draw call.
     * While updating, all WObj widgets will add there render rects into the themes rect list.
     * The theme also holds the texture and an atlas for the WObj's to use.
     * After updating all the WObj system, call WTheme_render to start the draw call.
     */
    oobj theme;

    /**
     * WObj object ( w/WObj.h ).
     * Like the root of the widget tree.
     * Mia widgets make use of the OObj resource tree hierarchy.
     * The default virtual update function of an WObj will
     * render all its WObj children on top of each other.
     */
    oobj gui;

    /**
     * WBtn object, derives from WObj as most objects from the w module do ( w/WBtn.h ).
     * This is a button that can be setup to be clicked, or toggled, or pressed to keep its state.
     * Can also be used to be setup as radio button or switch or checkbox.
     */
    oobj btn;


    /**
     * WText(Shadow) object, also derives from WObj to display a text ( w/WText(Shadow).h ).
     */
    oobj info;

};


O_STATIC
void ex_04_setup(oobj view)
{
    struct ex_04_context *C = o_user_set(view, o_new0(view, *C, 1));


    /**
     * Creates the WTheme with the "tiny" texture set.
     */
    C->theme = WTheme_new_tiny(view);

    /**
     * Root WObj, only this needs to be updated later.
     */
    C->gui = WObj_new(view);

    /**
     * The WBtn will be a child of the gui, so if gui is updated, the btn will also get updated
     */
    C->btn = WBtn_new(C->gui);

    /**
     * If you use the WBtn as parent, its children will get placed inside the button frame.
     * In this case we create a horizontal box, so that their children will be placed next to each other
     */
    oobj box = WBox_new(C->btn, WBox_LAYOUT_H);

    /**
     * Each WObj and its subclasses may change the padding (default is of cource 0).
     * Padding is a vec4 as left, top, right, bottom.
     * If you use the "vec4_" constructor, you >don't< need to pass every member.
     * In contrast to C's initializer list "{...}" this will >NOT< set them to 0.
     * The last given member is used to for setting the remaining ones.
     * So this is equal to vec4_(2,2,2,2) -> padding 2 to all borders
     */
    WObj_padding_set(box, vec4_(2));

    /**
     * First widget in the box, so appears left.
     * Will show an icon from the (tiny) texture, in this case a play icon (triangle). ( w/WIcon.h )
     * The tiny texture pack has icon's in the size of 7x7.
     */
    oobj icon = WIcon_new(box, WTheme_ICON_PLAY);
    
    /**
     * Sets the play icons color green
     */
    WIcon_color_set(icon, vec4_(0.1, 0.7, 0.2, 1.0));

    /**
     * Second widget in the box, so right next to the play icon.
     * Will display a text.
     * In this case for the tiny texture pack, its a 3x5 font.
     * Padding is set to 1 for all borders, so that it aligns centered to the 7x7 icon.
     * The mia widget system is also able to auto align, that'll be part in the next example.
     */
    oobj text = WText_new(box, "CLICK ME!");
    WObj_padding_set(text, vec4_(1));


    /**
     * Info text which style for the button is used.
     * The gui is a simple WObj object that renders its children on top of each other.
     * We can use padding to set a fixed position for the text "left 8; top 32 (so down...)"
     */
    C->info = WTextShadow_new(C->gui, NULL);
    WObj_padding_set(C->info, vec4_(8, 32, 0, 0));
}

O_STATIC
void ex_04_update(oobj view, oobj tex, float dt)
{
    struct ex_04_context *C = o_user(view);

    /**
     * Updates the whole gui widget tree hierarchy.
     * This helper function first calls WTheme_clear to clear the internal render list.
     * And afterwards calls WObj_update(...) on the gui widget to update the hierarchy into the theme.
     * As with WObj_update, this function needs:
     * @param lt left top start for the widgets. The widget system pops up to right bottom, as most do.
     * @param min_size minimal size for the gui to use. In this case 0 so it appears as small as possible.
     *                 Some widgets will stretch up to the available minimal size.
     * @param pointer_fn is the function which will be called to retrieve pointer events for the objects.
     *                   Its mostly the default a_pointer function.
     */
    vec2 lt = vec2_(-32, 32);
    WTheme_update(C->theme, C->gui, lt, vec2_(0), a_pointer);


    /**
     * Reads the current button style
     */
    enum WBtn_style style = WBtn_style(C->btn);

    /**
     * Returns true once if the button was clicked (pressed and released).
     * For a WBtn to work, either one of the following functions must be used:
     *      WBtn_clicked
     *      WBtn_pressed
     *      WBtn_toggled
     * Or the WBtn_auto_mode(_set) has to be set.
     * In the latter case, a callback may be registered for the event or the upper functions can be used nevertheless.
     */
    if (WBtn_clicked(C->btn)) {
        /**
         * update the style of the button.
         * Will traverse between:
         *
         *     WBtn_FLAT,
         *     WBtn_FLAT_ROUND,
         *     WBtn_DEF,
         *     WBtn_DEF_ROUND,
         *     WBtn_BIG,
         *     WBtn_BIG_ROUND,
         *
         * Also available are the following styles, but these work best with no internal children:
         *
         *     WBtn_RADIO,
         *     WBtn_CHECK,
         *     WBtn_SWITCH,
         */
        style++;
        style %= (WBtn_BIG_ROUND + 1);
        WBtn_style_set(C->btn, style);
    }


    /**
     * Update the text according to the current style
     */
    char *text = (char *[]) {
            "FLAT",
            "FLAT_ROUND",
            "DEF",
            "DEF_ROUND",
            "BIG",
            "BIG_ROUND"
    }[style];

    WText_text_set(C->info, text);
}

O_STATIC
void ex_04_render(oobj view, oobj tex, float dt)
{
    struct ex_04_context *C = o_user(view);

    /**
     * Clears the background with vec4_(0.6, 0.6, 0.6, 1) which is 60% gray
     */
    RTex_clear(tex, R_GRAY_X(0.6));

    /**
     * Renders all widgets in a single draw call
     */
    WTheme_render(C->theme, tex);
}


O_EXTERN
oobj ex_04_main(oobj root)
{
    oobj view = AView_new(root, ex_04_setup, ex_04_update, ex_04_render);
    oobj scene = AScene_new(root, view, true);
    return scene;
}

/**
 * Summary:
 *
 * This example shows a simple setup using Mia's widget system.
 * Its simple be design, the next example "windows" will show much more widget stuff.
 * The widgets make use of the OObj's tree hierarchy for the relative positions.
 */
