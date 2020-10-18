#include "unity.h"
#include "../src/util.h"

void setUp(void) {
}

void tearDown(void) {
}

void test_rotate_left() {
    TEST_ASSERT_EQUAL_UINT8(0x150f0fd8, rotate_left(0xc0a8787e, 5));
}

void test_little_endian() {
  byte arr[] = { 86, 75, 30, 9 };
  word result = little_endian(arr);

  TEST_ASSERT_EQUAL_UINT32(0x091e4b56, result);
}

void test_rev_little_endian() {
    byte arr[4];
    byte expected[] = { 86, 75, 30, 9 };
    rev_little_endian(0x091e4b56, arr);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, arr, 4);
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_rotate_left);
    RUN_TEST(test_little_endian);
    RUN_TEST(test_rev_little_endian);

    return UNITY_END();
}
