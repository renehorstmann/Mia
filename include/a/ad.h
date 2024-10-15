#ifndef A_AD_H
#define A_AD_H

/**
 * @file ad.h
 *
 * Reward ads and ump dialogs
 * MIA_PLATFORM_ANDROID only in the moment, other return false and are noop's
 */

#include "o/OObj.h"


// makes an "expensive" jni call
bool a_ad_ump_loaded(void);

// reshows the ump form, if available / loaded
void a_ad_ump_show(void);

// makes an "expensive" jni call
bool a_ad_loaded(void);

// the a_ad_show callback should always be the same
void a_ad_show(oobj callback_obj, OObj__event_fn callback);

#endif //A_AD_H
