#ifndef W_WTEXT_H
#define W_WTEXT_H

/**
 * @file WText.h
 *
 * Object (derives WObj)
 *
 * Widget that renders text with WTheme
 */

#include "WObj.h"

/** object id */
#define WText_ID WObj_ID "Text"


/**
 * An WText can be setup to always render upper case characters
 */
enum WText_text_mode {
    WText_MODE_DEFAULT,
    WText_MODE_LOWER,
    WText_MODE_UPPER,
    WText_NUM_MODES
};

typedef struct {
    WObj super;

    enum WText_text_mode text_mode;

    vec2 char_size;
    vec2 char_offset;
    vec2 char_scale;

    vec4 color;
    char *text;
    int num;

    vec2 text_size;
} WText;


/**
 * Initializes the object
 * @param obj WText object
 * @param parent to inherit from
 * @param text to render
 * @return obj casted as WText
 */
O_EXTERN
WText *WText_init(oobj obj, oobj parent, const char *text);


/**
 * Creates a new WText object
 * @param parent to inherit from
 * @param text to render
 * @return The new object
 */
O_INLINE
WText *WText_new(oobj parent, const char *text)
{
    OObj_DECL_IMPL_NEW(WText, parent, text);
}

//
// virtual implementations:
//


/**
 * Virtual implementation that renders the text into the given WTheme object.
 * Calls WObj__v_update afterwards.
 */
O_EXTERN
vec2 WText__v_update(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn);

/**
 * Virtual implementation that sets the text color
 */
O_EXTERN
void WText__v_style_apply(oobj obj);
  

//
// object functions:
//


/**
 * @param obj WText object
 * @return text mode used
 */
OObj_DECL_GETSET(WText, enum WText_text_mode, text_mode)

/**
 * Will update WText_char_size and WText_char_offset (if char_size.x<0)
 * @param obj WText object
 * @param theme WTheme object
 */
O_EXTERN
void WText_char_size_update(oobj obj, oobj theme);


/**
 * @param obj WText object
 * @return character size
 * @note inits with first update call using WTheme (char '0') (update if char_size.x<0)
 *       or call WText_char_size_update first
 */
OObj_DECL_GETSET(WText, vec2, char_size)

/**
 * @param obj WText object
 * @return character offset
 * @note inits with first update call using WTheme (char '0') (char_size + 1) (update if char_size.x<0)
 *       or call WText_char_size_update first
 */
OObj_DECL_GETSET(WText, vec2, char_offset)

/**
 * @param obj WText object
 * @return character scaling, defaults to vec2_(1)
 * @note if using pixel art, use integers to be pixel perfect
 */
OObj_DECL_GETSET(WText, vec2, char_scale)

/**
 * @param obj WText object
 * @return color for each char on an update call
 */
OObj_DECL_GETSET(WText, vec4, color)

/**
 * @param obj WText object
 * @return used text
 */
OObj_DECL_GET(WText, const char*, text)

/**
 * @param obj WText object
 * @param text to set (will be cloned)
 * @return new allocated cloned text
 */
O_EXTERN
const char *WText_text_set(oobj obj, const char *text);

/**
 * @param obj WText object
 * @return used text length == o_strlen(text)
 */
OObj_DECL_GET(WText, int, num)


#endif //W_WTEXT_H
