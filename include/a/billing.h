#ifndef A_BILLING_H
#define A_BILLING_H

/**
 * @file billing.h
 *
 * api to buy items with real money
 * MIA_PLATFORM_ANDROID only in the moment, other return false and are noop's
 */

#include "o/common.h"

// return true to consume the purchase
typedef bool (*a_billing_purchased_fn)(oobj obj, int idx);


// makes an "expensive" jni call
bool a_billing_loaded(void);

// setup at app start or smth. so even on restart it may work
void a_billing_init(oobj callback_obj, a_billing_purchased_fn callback);

// asserts a_billing_init was called before to set the callback
void a_billing_buy(int idx);

#endif //A_BILLING_H
