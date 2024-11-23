#include "o/str.h"

#define test(expr) o_assume(expr, "test failed")

O_STATIC
void test_base64(oobj obj) {

    // Test null
    test(o_str_base64_encode(obj, NULL, 0) == NULL);

    // Test super simple input
    char *encoded = o_str_base64_encode(obj, "A", 1);
    test(o_str_equals(encoded, "QQ=="));

    // Test simple input
    const char *input = "Hello, World!";
    char *encoded_str = o_str_base64_encode(obj, input, o_strlen(input));
    test(o_str_equals(encoded_str, "SGVsbG8sIFdvcmxkIQ=="));

    // Test binary input
    const unsigned char binary_data[] = {0xDE, 0xAD, 0xBE, 0xEF};
    char *encoded_binary = o_str_base64_encode(obj, binary_data, sizeof(binary_data));
    test(o_str_equals(encoded_binary, "3q2+7w=="));

    // Test empty input
    osize out_size;
    test(o_str_base64_decode(obj, &out_size, NULL) == NULL);

    // Test simple input
    encoded_str = "SGVsbG8sIFdvcmxkIQ==";
    void *decoded_str = o_str_base64_decode(obj, &out_size, encoded_str);
    test(out_size == strlen("Hello, World!"));
    test(memcmp(decoded_str, "Hello, World!", strlen("Hello, World!")) == 0);

    // Test binary input
    encoded_binary = "3q2+7w==";
    void *decoded_binary = o_str_base64_decode(obj, &out_size, encoded_binary);
    const unsigned char expected_binary[] = {0xDE, 0xAD, 0xBE, 0xEF};
    test(out_size == sizeof(expected_binary));
    test(memcmp(decoded_binary, expected_binary, sizeof(expected_binary)) == 0);
}


int o_str__test(oobj obj)
{
    test_base64(obj);

    return 0;
}

