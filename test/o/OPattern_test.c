#include "o/OPattern.h"

#define test(expr) o_assume(expr, "test failed")

O_STATIC
void test_match(oobj pattern, const char *str, struct o_str_range res,
                const struct o_str_range *captures, osize captures_size)
{
    test(captures_size == OPattern_captures(pattern));
    struct o_str_range *caps = o_new(pattern, *caps, captures_size);
    struct o_str_range range = OPattern_find(pattern, str, caps);
    test(range.start == res.start && range.len == res.len);
    for (osize i = 0; i < captures_size; i++) {
        test(caps[i].start == captures[i].start && caps[i].len == captures[i].len);
    }
    // disable not used warning...
    range.start++;
}


int OPattern__test(oobj obj)
{
    OPattern *pattern = OPattern_new(obj, ".*");
    test(pattern && pattern->groups_size == 1);
    test(pattern->groups[0].mode == OPattern_group_MODE_MUL);
    for (int i = 0; i < 256; i++) {
        test(OPattern_group_check(&pattern->groups[0], i));
    }

    test_match(OPattern_new(obj, "he(llo)"), "hello",
               (struct o_str_range) {0, 5},
               (struct o_str_range[1]) {{2, 3}}, 1);

    test_match(OPattern_new(obj, "(%l*)%s(%u*)"), "BBBBhello WORLDaaaa",
               (struct o_str_range) {4, 11},
               (struct o_str_range[2]) {{4,  5},
                                        {10, 5}}, 2);


    // Test Case: Pattern with Optional Character
    pattern = OPattern_new(obj, "colou?r");
    test_match(pattern, "colour",
               (struct o_str_range) {0, 6},
               NULL, 0);
    test_match(pattern, "color",
               (struct o_str_range) {0, 5},
               NULL, 0);

    // Test Case: Pattern with Optional Character
    pattern = OPattern_new(obj, "colou?r");
    test_match(pattern, "colour",
               (struct o_str_range) {0, 6},
               NULL, 0);
    test_match(pattern, "color",
               (struct o_str_range) {0, 5},
               NULL, 0);

    // Test Case: Pattern with Multiple Optional Characters
    pattern = OPattern_new(obj, "app(l?e?)(s?)");
    test_match(pattern, "apple",
               (struct o_str_range) {0, 5},
               (struct o_str_range[2]) {{3, 2}, {0, 0}}, 2);
    test_match(pattern, "apples",
               (struct o_str_range) {0, 6},
               (struct o_str_range[2]) {{3, 2}, {5, 1}}, 2);
    test_match(pattern, "app",
               (struct o_str_range) {0, 3},
               (struct o_str_range[2]) {{0, 0}, {0, 0}}, 2);

    // Test Case: Pattern with Wildcard and Escape Character
    pattern = OPattern_new(obj, "a%%(%d+)");
    test_match(pattern, "gga%12345ff",
               (struct o_str_range) {2, 7},
               (struct o_str_range[1]) {{4, 5}}, 1);
    test_match(pattern, "a%987",
               (struct o_str_range) {0, 5},
               (struct o_str_range[1]) {{2, 3}}, 1);
    test_match(pattern, "a%bc",
               (struct o_str_range) {0, 0},
               (struct o_str_range[1]) {{0, 0}}, 1);

    // Test Case: Pattern with multiple captures
    pattern = OPattern_new(obj, "a(%d+),(%d+),(%d+)");
    test_match(pattern, "a123,456,789",
               (struct o_str_range) {0, 12},
               (struct o_str_range[3]) {{1, 3}, {5, 3}, {9, 3}}, 3);

    // Test Case: Pattern with Nested Groups
    pattern = OPattern_new(obj, "a(b(cd))e");
    test_match(pattern, "abcde",
               (struct o_str_range) {0, 5},
               (struct o_str_range[2]) {{1, 3}, {2, 2}}, 2);

    // Test Case: email
    pattern = OPattern_new(obj, "^[%w.]+@%w+%.%w+$");
    test_match(pattern, "user.company@domain.ns",
               (struct o_str_range) {0, 22},
               NULL, 0);


    // Test Case: Optional captures
    pattern = OPattern_new(obj, "(a?)(b?)(c?)");
    test_match(pattern, "abc",
               (struct o_str_range) {0, 3},
               (struct o_str_range[3]) {{0, 1}, {1, 1}, {2, 1}}, 3);
    test_match(pattern, "bc",
               (struct o_str_range) {0, 2},
               (struct o_str_range[3]) {{0, 0}, {0, 1}, {1, 1}}, 3);
    test_match(pattern, "c",
               (struct o_str_range) {0, 1},
               (struct o_str_range[3]) {{0, 0}, {0, 0}, {0, 1}}, 3);
    test_match(pattern, "ac",
               (struct o_str_range) {0, 2},
               (struct o_str_range[3]) {{0, 1}, {0, 0}, {1, 1}}, 3);
    test_match(pattern, "b",
               (struct o_str_range) {0, 1},
               (struct o_str_range[3]) {{0, 0}, {0, 1}, {0, 0}}, 3);
    test_match(pattern, "a",
               (struct o_str_range) {0, 1},
               (struct o_str_range[3]) {{0, 1}, {0, 0}, {0, 0}}, 3);


    return 0;
}
