#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <cmocka.h>

#include "../src/address.h"

static void test_internal_key_to_seed(void **state) {
    // given
    (void)state;

    // default speculous seed
    const uint8_t internal_private_key[32] = {
        0x4F, 0xD5, 0x54, 0x70, 0x26, 0xD2, 0x4A, 0x12,
        0x0A, 0x26, 0x8D, 0x24, 0x6E, 0x90, 0xDD, 0x23,
        0x56, 0x84, 0x59, 0xA1, 0x6B, 0xCF, 0xCB, 0x54,
        0xFC, 0xEE, 0x86, 0xEB, 0xCD, 0x18, 0x34, 0xFA
    };
    char seed[55] = {0};

    // when
    bool result = internal_key_to_seed(internal_private_key, sizeof(internal_private_key), seed);

    // then
    assert_true(result);
    assert_string_equal(seed, "bkqvfpdxraasqnmqfqjpesghfszenwzgifkvzalsdxqpfebpewiqxtq");
}

int main() {
    const struct CMUnitTest tests[] = {cmocka_unit_test(test_internal_key_to_seed)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}