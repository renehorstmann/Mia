#include "o/OObjRoot.h"
#include "o/log.h"

// protected in o_common
void o__sanitizer_leak_check(const char *why, bool full);


O_STATIC
void test_module(const char *module, int (*fn)(oobj obj))
{
    o_log_base(O_LOG_DEBUG, "o", NULL, 0, module, "testing...");
    oobj root = OObjRoot_new_heap();
    int res = fn(root);
    if(res) {
        o_exit(module);
    }
    o_del(root);
//    o__sanitizer_leak_check(module, false);
}

#define TEST(module) do {                           \
int module ## __test(oobj root);                    \
test_module(#module "_test", module ## __test);     \
} while(0)



void o__test_main(void)
{
    TEST(OArray);
    TEST(o_str);
    TEST(OPattern);
    TEST(RTex);
}
