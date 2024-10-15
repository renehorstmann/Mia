#ifndef W_WPROGRESS_H
#define W_WPROGRESS_H

/**
 * @file WProgress.h
 *
 * Object (derives WObj)
 *
 * Widget that shows a progress bar in its min_size.
 *
 * In horizontal mode (default): left is 0.0, right is 1.0.
 * In vertical mode:             bottom is 0.0, top is 1.0.
 */

#include "WObj.h"

/** object id */
#define WProgress_ID WObj_ID "Progress"


typedef struct {
    WObj super;

    vec4 color;
    
    float progress;

    bool vertical;
    bool on_unit;
    
} WProgress;


/**
 * Initializes the object
 * @param obj WProgress object
 * @param parent to inherit from
 * @return obj casted as WProgress
 */
O_EXTERN
WProgress *WProgress_init(oobj obj, oobj parent);


/**
 * Creates a new WProgress object
 * @param parent to inherit from
 * @return The new object
 */
O_INLINE
WProgress *WProgress_new(oobj parent)
{
    OObj_DECL_IMPL_NEW(WProgress, parent);
}

//
// virtual implementations:
//


/**
 * Virtual implementation that renders the background color.
 * Calls WObj__v_update afterwards.
 */
O_EXTERN
vec2 WProgress__v_update(oobj obj, vec2 lt, vec2 min_size, oobj theme, a_pointer__fn pointer_fn);

/**
 * Virtual implementation that sets the color 
 */
O_EXTERN
void WProgress__v_style_apply(oobj obj);

//
// object functions:
//

/**
 * @param obj WProgress object
 * @return color for the progress bar
 */
OObj_DECL_GETSET(WProgress, vec4, color)

/**
 * @param obj WProgress object
 * @return progress [0:1]
 */
OObj_DECL_GETSET(WProgress, float, progress)

/**
 * @param obj WProgress object
 * @return if true, the slider is rendered vertically, else horizontally (default is false)
 */
OObj_DECL_GETSET(WProgress, bool, vertical)

/**
 * @param obj WProgress object
 * @return if true, the progress bar is ceiled to a unit (default is true)
 */
OObj_DECL_GETSET(WProgress, bool, on_unit)




#endif //W_WPROGRESS_H
