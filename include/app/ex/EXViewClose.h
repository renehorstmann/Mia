#ifndef EX_EXVIEWCLOSE_H
#define EX_EXVIEWCLOSE_H

/**
 * @file EXViewClose.h
 *
 * object.
 *
 * AView which shows a button, which will o_del a given object
 */

#include "a/AView.h"


/** object id */
#define EXViewClose_ID AView_ID "EXViewClose"

typedef struct {
    AView AView;

    OObj__event_fn event;

    oobj theme;
    oobj button;

} EXViewClose;


/**
 * Initializes the object.
 * Creates an AView that renders a button. If its pressed the del_target get's o_del'ed
 * @param obj EXViewClose object
 * @param parent to inherit from
 * @param event to be called on button clicked (may be NULL)
 * @return obj casted as EXViewClose
 */
O_EXTERN
EXViewClose *EXViewClose_init(oobj obj, oobj parent, OObj__event_fn event);

/**
 * Creates a new the OArray object
 * Creates an AView that renders a button. If its pressed the del_target get's o_del'ed
 * @param parent to inherit from
 * @param event to be called on button clicked (may be NULL)
 * @return The new object
 */
O_INLINE
EXViewClose *EXViewClose_new(oobj parent, OObj__event_fn event)
{
    OObj_DECL_IMPL_NEW(EXViewClose, parent, event);
}


//
// virtual implementations
//

O_EXTERN
void EXViewClose__v_setup(oobj view);

O_EXTERN
void EXViewClose__v_update(oobj view, oobj tex, float dt);

O_EXTERN
void EXViewClose__v_render(oobj view, oobj tex, float dt);


//
// object functions:
//

/**
 * @param obj EXViewClose object
 * @return event to be called on button clicked (may be NULL)
 */
OObj_DECL_GETSET(EXViewClose, OObj__event_fn , event)


#endif //EX_EXVIEWCLOSE_H
