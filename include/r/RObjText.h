#ifndef R_ROBJTEXT_H
#define R_ROBJTEXT_H

/**
 * @file RObjText.h
 *
 * Object
 *
 * Renders a text with an RObjQuad (quad for each character)
 *
 * update is a noop!
 * Call update on RObjText_quads if you change these!
 */
 
#include "RObj.h"

/** object id */
#define RObjText_ID RObj_ID "Text"

/**
 * @param obj RObjText object
 * @param c character
 * @return uv center position of c
 */
typedef vec2 (*RObjText__char_uv_fn)(oobj obj, char c);




/**
 * An RObjText can be setup to always render upper case characters
 */
enum RObjText_text_mode {
    RObjText_MODE_DEFAULT,
    RObjText_MODE_LOWER,
    RObjText_MODE_UPPER,
    RObjText_NUM_MODES
};

typedef struct {
    RObj super;
    
    // top left, using u_pose, size must be doubled
    mat4 pose;
    
    vec2 size;
    vec2 offset;

    enum RObjText_text_mode text_mode;
    
    // size of the current set text
    vec2 text_size;

    // RObjQuad
    oobj quads;
    
    // vfuncs
    RObjText__char_uv_fn v_char_uv;
    
} RObjText;


/**
 * Initializes the object
 * @param obj RObjText object
 * @param parent to inherit from
 * @param num of quads
 * @param tex RTex object, NULL safe
 * @param move_tex if true, tex is o_move'd into this object
 * @param size of a font char
 * @param offset to next char (y for next line)
 * @param char_uv virtual function to get uv center pos for a character in tex
 * @return obj casted as RObjText
 */
O_EXTERN
RObjText *RObjText_init(oobj obj, oobj parent, int num, oobj tex, bool move_tex, 
        vec2 size, vec2 offset, RObjText__char_uv_fn char_uv);


/**
 * Creates a new RObjText object
 * @param parent to inherit from
 * @param num of quads
 * @param tex RTex object, NULL safe;
 * @param move_tex if true, tex is o_move'd into this object
 * @param size of a font char
 * @param offset to next char (y for next line)
 * @param char_uv virtual function to get uv center pos for a character in tex
 * @return The new object
 */
O_INLINE
RObjText *RObjText_new(oobj parent, int num, oobj tex, bool move_tex, vec2 size, vec2 offset, RObjText__char_uv_fn char_uv)
{
    OObj_DECL_IMPL_NEW(RObjText, parent, num, tex, move_tex,
        size, offset, char_uv);
}


/**
 * Creates a new RObjText object.
 * Loads font file "res/r/font35.png" with 3x5 size
 * @param parent to inherit from
 * @param num of quads -> num = o_max(num, o_strlen(opt_text)); -> so num is safe to be 0
 * @param opt_text if not NULL -> RObjText_text_set
 * @return The new object
 * @note inits text_mode as UPPER, even if lower case is available
 */
O_EXTERN
RObjText *RObjText_new_font35(oobj parent, int num, const char *opt_text);

/**
 * Creates a new RObjText object.
 * Loads font file "res/r/font35_shadow.png" with 3x5 size
 * The default shadow is bvec4_(0, 0, 0, 128) and is equal to a contouring shader
 * with 2x2 kernel aligned bottom right
 * @param parent to inherit from
 * @param num of quads -> num = o_max(num, o_strlen(opt_text)); -> so num is safe to be 0
 * @param opt_text if not NULL -> RObjText_text_set
 * @param opt_shadow_color if NULL, bvec4_(0, 0, 0, 128) is used from the font sheet,
 *                         else replaced by the given color
 * @return The new object
 * @note inits text_mode as UPPER, even if lower case is available
 */
O_EXTERN
RObjText *RObjText_new_font35_shadow(oobj parent, int num, const char *opt_text, bvec4 *opt_shadow_color);


//
// virtual implementations:
//

O_EXTERN
void RObjText__v_update(oobj obj);

O_EXTERN
void RObjText__v_render(oobj obj, oobj tex, const struct r_proj *proj);

O_EXTERN
vec2 RObjText__v_font35_char_uv(oobj obj, char c);

//
// object functions:
//

/**
 * @param obj RObjText object
 * @return The used text pose (top left corner)
 * @note combined with used camera and passed to the quads as camera.
         Position will be set on an real pixel before combining.
         Using u_pose, size must be doubled (so 2x2 as default)
 */
OObj_DECL_GETSETREF(RObjText, mat4, pose)

/**
 * @param obj RObjText object
 * @return The used character offset
 */
OObj_DECL_GETSET(RObjText, vec2, offset)

/**
 * @param obj RObjText object
 * @return The used text mode like RObjText_MODE_UPPER
 */
OObj_DECL_GETSET(RObjText, enum RObjText_text_mode, text_mode)

/**
 * @param obj RObjText object
 * @return RObjQuad object used to render the characters
 * @note call RObj_update on the quads, if changed (color, etc.)
 */
OObj_DECL_GET(RObjText, oobj, quads)

/**
 * @param obj RObjText object
 * @param size for each character (quad uv + quad pose)0
 * @note useable to create shader effects on the text fonts, like shadow or outlining.
         The font files have typically transparent borders around each font for that.
         If you want to have a bigger printed size, see RObjText_pose
 */
O_EXTERN
void RObjText_uv_size_set(oobj obj, vec2 size);


/**
 * @param obj RObjText object
 * @return size if the full set text block, that was last set with RObjText_text_set
 */
OObj_DECL_GET(RObjText, vec2, text_size)

/**
 * Resets the text to render.
 * A newline character results in a new line...
 * Text index matches the RObjText_quads index
 * rendered right down from the pose. So pose is top left of the text.
 * @param obj RObjText object
 * @param text to set
 * @return size if the full set text block
 * @note sets RObjQuad_num_rendered_set to the o_strlen(text)
 */
O_EXTERN
vec2 RObjText_text_set(oobj obj, const char *text);

/**
 * Sets a (single) color for all used quads
 * @param obj RObjText object
 * @param color to set
 */
O_EXTERN
void RObjText_color_set(oobj obj, vec4 color);




#endif //R_ROBJTEXT_H
