#ifndef W_WFRAME_H
#define W_WFRAME_H

/**
 * @file WFrame.h
 *
 * Object (derives WObj)
 *
 * Widget that renders a frame border around its children with an optional label.
 */

#include "WObj.h"

/** object id */
#define WFrame_ID WObj_ID "Frame"


typedef struct {
    WObj super;
    
    float border_size;
    vec4 border_color;

    // OObj to break the WObj update hierarchy (to allocate label)
    oobj container;

    // WObj to render stuff as label
    oobj label;
    
} WFrame;


/**
 * Initializes the object
 * @param obj WFrame object
 * @param parent to inherit from
 * @return obj casted as WFrame
 */
O_EXTERN
WFrame *WFrame_init(oobj obj, oobj parent);


/**
 * Creates a new WFrame object
 * @param parent to inherit from
 * @return The new object
 */
O_INLINE
WFrame *WFrame_new(oobj parent)

{
    OObj_DECL_IMPL_NEW(WFrame, parent);
}


/**
 * Creates a new WFrame object
 * @param parent to inherit from
 * @param title created as WText in WFrame_label
 * @param opt_out_title if not NULL set to the created title
 * @return The new object
 */
O_EXTERN
WFrame *WFrame_new_title(oobj parent, const char *title, oobj *opt_out_title);


/**
 * Creates a new WFrame object
 * @param parent to inherit from
 * @param title created as WTextShadow in WFrame_label
 * @param opt_out_title if not NULL set to the created title
 * @return The new object
 */
O_EXTERN
WFrame *WFrame_new_title_shadow(oobj parent, const char *title, oobj *opt_out_title);

//
// virtual implementations:
//


/**
 * Virtual implementation that sets the button rects and checks the pointer for clicked.
 * Calls WObj_update on all children.
 */
O_EXTERN
vec2 WFrame__v_update(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn);


/**
 * Virtual implementation that returns both the direct children and the label
 */
O_EXTERN
oobj *WFrame__v_list(oobj obj, osize *opt_out_num);

/**
 * Virtual implementation that sets the color and border size
 */
O_EXTERN
void WFrame__v_style_apply(oobj obj);

//
// object functions:
//

/**
 * @param obj WFrame object
 * @return stroke of the border rect
 */
OObj_DECL_GETSET(WFrame, float, border_size)



/**
 * @param obj WFrame object
 * @return color for the border
 */
OObj_DECL_GETSET(WFrame, vec4, border_color)


/**
 * @param obj WFrame object
 * @return WObj label
 * @note allocate WObj's on this label (WObj) object to render it at top left of the frame
 */
OObj_DECL_GET(WFrame, oobj, label)



#endif //W_WFRAME_H
