#ifndef U_USPLIT_H
#define U_USPLIT_H

/**
 * @file USplit.h
 *
 * Object
 */


#include "a/AView.h"

/** object id */
#define USplit_ID OObj_ID "USplit"

enum USplit_mode {
    USplit_MODE_H_LEFT,
    USplit_MODE_H_RIGHT,
    USplit_MODE_V_BOTTOM,
    USplit_MODE_V_TOP,
    USplit_NUM_MODES
};

typedef struct {
    OObj super;

    enum USplit_mode mode;
    
    // in tex units, according to mode
    float seperator;
    float spacing;
    bool even_sizes;

    // children are either USplit's or AView's
    oobj child_a;
    oobj child_b;

} USplit;


/**
 * Initializes the object.
 * @param obj USplit object
 * @param parent to inherit from
 * @param child_a either an USplit or AView (left or top)
 * @param child_b either an USplit or AView (right or bottom)
 * @param mode either horizontally or vertically
 * @param seperator relative position between a and b [0:1]
 * @param spacing empty distance between the children (in units)
 * @return obj casted as USplit
 */
O_EXTERN
USplit *USplit_init(oobj obj, oobj parent, oobj child_a, oobj child_b, enum USplit_mode mode, float seperator, float spacing);

/**
 * Creates a new USplit object
 * @param parent to inherit from
 * @param child_a either an USplit or AView (left or top)
 * @param child_b either an USplit or AView (right or bottom)
 * @param mode either horizontally or vertically
 * @param seperator relative position between a and b [0:1]
 * @param spacing empty distance between the children
 * @return The new object
 */
O_INLINE
USplit *USplit_new(oobj parent, oobj child_a, oobj child_b, enum USplit_mode mode, float seperator, float spacing)
{
    OObj_DECL_IMPL_NEW(USplit, parent, child_a, child_b, mode, seperator, spacing);
}


//
// object functions:
//

/**
 * @param obj USplit object
 * @return the used split mode
 */
OObj_DECL_GETSET(USplit, enum USplit_mode, mode)

/**
 * @param obj USplit object
 * @return in units, according to modr
 */
OObj_DECL_GETSET(USplit, float, seperator)

/**
 * @param obj USplit object
 * @return the used spacing distance (in units)
 */
OObj_DECL_GETSET(USplit, float, spacing)

/**
 * @param obj USplit object
 * @return if true, child sizes may be reduced to be even (in units)
 * @note if true (default), spacing may grow to generate the even sizes
 */
OObj_DECL_GETSET(USplit, bool, even_sizes)


/**
 * @param obj USplit object
 * @return child_a, either an USplit or an AView
 */
OObj_DECL_GETSET(USplit, oobj, child_a)

/**
 * @param obj USplit object
 * @return child_b, either an USplit or an AView
 */
OObj_DECL_GETSET(USplit, oobj, child_b)

/**
 * Updates and renders internal USplit's and AView's.
 * @param obj USplit object
 * @param tex RTex object to render on
 * @param viewport for this view in tex viewport coordinates
 */
O_EXTERN
void USplit_update_ex(oobj obj, oobj tex, ivec4 viewport);


/**
 * Updates internal USplit's and AView's.
 * Calls USplit_update_ex with the current set viewport on tex
 * @param obj USplit object
 * @param tex RTex object to render on
 */
O_EXTERN
void USplit_update(oobj obj, oobj tex);

/**
 * Renders internal USplit's and AView's.
 * @param obj USplit object
 * @param tex RTex object to render on
 */
O_EXTERN
void USplit_render(oobj obj, oobj tex);


/**
 * @param obj USplit object
 * @return Null terminated list of all internal AView objects
 */
O_EXTERN
AView **USplit_views(oobj obj);


#endif //U_USPLIT_H
