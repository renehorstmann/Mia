#include "o/OArray.h"

#define test(expr) o_assume(expr, "test failed")

O_STATIC
void test_default(oobj obj)
{
    oobj a = OArray_new(obj, NULL, sizeof(char), 0);
    for(int i=0; i<16; i++) {
        char val = 'A'+i;
        OArray_push(a, &val);
    }
    test(o_num(a) == 16);
    test(OArray_capacity(a) == 16);
    test(OArray_reserve_front(a) == 0);
    for(int i=0; i<16; i++) {
        char val = 'A'+i;
        OArray_push_front(a, &val);
    }
    test(o_num(a) == 32);
    test(OArray_capacity(a) == 32);
    test(OArray_reserve_front(a) == 0);

    OArray_resize(a, 0);
    test(o_num(a) == 0);
    test(OArray_capacity(a) == 32);
    test(OArray_reserve_front(a) == 0);

    OArray_capacity_fit(a);
    test(o_num(a) == 0);
    test(OArray_capacity(a) == 0);
    test(OArray_reserve_front(a) == 0);

}

O_STATIC
void test_dyn(oobj obj)
{
    oobj a = OArray_new_dyn(obj, NULL, sizeof(char), 0, 0);
    for(int i=0; i<16; i++) {
        char val = 'A'+i;
        OArray_push(a, &val);
    }
    test(o_num(a) == 16);
    for(int i=0; i<16; i++) {
        char val = 'A'+i;
        char *test = o_at(a, i);
        test(val == *test);
    }
    OArray_dyn_realloc_fn_set(a, OArray__v_dyn_realloc_doubled_front);
    OArray_resize(a, 0);
    for(int i=0; i<16; i++) {
        char val = 'A'+i;
        OArray_push_front(a, &val);
    }
    test(o_num(a) == 16);
    for(int i=0; i<16; i++) {
        char val = 'A'+i;
        char *test = o_at(a, o_num(a)-i-1);
        test(val == *test);
    }
}

O_STATIC
void test_copy(oobj obj)
{
    const char *data = "Hello World";
    oobj a = OArray_new(obj, data, sizeof(char), strlen(data));
    test(o_num(a) == (osize) strlen(data));
    test(memcmp(data, o_at(a, 0), strlen(data)) == 0);
}

O_STATIC
void test_struct(oobj obj)
{
    struct test {
        char c;
        int i;
    };
    struct test *t;

    oobj a = OArray_new(obj, NULL, sizeof (struct test), 2);
    t = o_at(a, 0);
    t->c = 'a';
    t->i = 10;
    t = o_at(a, 1);
    t->c = 'a'+ 7;
    t->i = 10+7;

    for(int i=1; i<=6; i++) {
        struct test val;
        val.c = 'a'+ i;
        val.i = 10 + i;
        OArray_push_at(a, i, &val);
    }

    test(o_num(a) == 8);
    for(int i=0; i<8; i++) {
        struct test val;
        val.c = 'a'+ i;
        val.i = 10 + i;
        t = o_at(a, i);
        test(val.c == t->c && val.i == t->i);
    }
}

O_STATIC
void test_take(oobj obj)
{
    oobj a = OArray_new(obj, NULL, sizeof (int), 0);
    for(int i=0; i<5; i++) {
        OArray_push(a, &i);
    }
    int taken;
    OArray_take_at(a, 2, NULL, 1);
    OArray_pop(a, &taken);
    test(taken == 4);
    OArray_pop_front(a, &taken);
    test(taken == 0);

    test(o_num(a) == 2);
    int val;
    val = *OArray_at(a, 0, int);
    test(val == 1);

    val = *OArray_at(a, 1, int);
    test(val == 3);
}

void test_append(oobj obj)
{
    oobj a = OArray_new_dyn_mode(obj, NULL, sizeof (int), 0, 0, OArray_REALLOC_DOUBLED_FRONT);
    int idx = 0;
    int sum = 0;
    for(int i=0; i<32; i++) {
        int buff[8];
        int buff_cnt = o_rand()%8;
        for(int j=0; j<buff_cnt; j++) {
            buff[j] = idx++;
            sum += buff[j];
        }
        OArray_append_front(a, buff, buff_cnt);
    }
    test(o_num(a) == idx);

    int check_sum = 0;
    for(osize i=0; i<o_num(a); i++) {
        int *val = o_at(a, i);
        check_sum += *val;
    }
    test(check_sum == sum);

}

int OArray__test(oobj obj)
{
    test_default(obj);
    test_dyn(obj);
    test_copy(obj);
    test_struct(obj);
    test_take(obj);
    test_append(obj);
    return 0;
}

