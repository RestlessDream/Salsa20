#include "unity.h"
#include "../src/salsa20.h"
#include "../src/util.h"
#include "stdio.h"

void setUp(void) {
}

void tearDown(void) {
}

void test_encrypt() {
    byte to_encrypt[64] = { 0, 1, 2, 3 };
    byte expected[64] = { 0, 1, 2, 3 };
    byte key[32] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    byte nonce[8] = { 101, 102, 103, 104, 105, 106, 107, 108 };

    encrypt(key, KEYLEN_128, nonce, to_encrypt, 64, to_encrypt);
    encrypt(key, KEYLEN_128, nonce, to_encrypt, 64, to_encrypt);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, to_encrypt, 64);
}

void test_encrypt2() {
    byte to_encrypt[64] = {
        39, 173, 46, 248, 30, 200, 82, 17, 48, 67, 254, 239, 37, 18, 13, 247, 
        241, 200, 61, 144, 10, 55, 50, 185, 6, 47, 246, 253, 143, 86, 187, 225, 
        134, 85, 110, 246, 161, 163, 43, 235, 231, 94, 171, 51, 145, 214, 112, 29, 
        14, 232, 5, 16, 151, 140, 183, 141, 171, 9, 122, 181, 104, 182, 177, 193 };

    byte expected[64] = {
        39, 173, 46, 248, 30, 200, 82, 17, 48, 67, 254, 239, 37, 18, 13, 247, 
        241, 200, 61, 144, 10, 55, 50, 185, 6, 47, 246, 253, 143, 86, 187, 225, 
        134, 85, 110, 246, 161, 163, 43, 235, 231, 94, 171, 51, 145, 214, 112, 29, 
        14, 232, 5, 16, 151, 140, 183, 141, 171, 9, 122, 181, 104, 182, 177, 193 };

    byte key[32] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    byte nonce[8] = { 101, 102, 103, 104, 105, 106, 107, 108 };

    encrypt(key, KEYLEN_256, nonce, to_encrypt, 64, to_encrypt);
    encrypt(key, KEYLEN_256, nonce, to_encrypt, 64, to_encrypt);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, to_encrypt, 64);
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_encrypt);
    RUN_TEST(test_encrypt2);

    return UNITY_END();
}
