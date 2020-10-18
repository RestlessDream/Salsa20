#include "salsa20.h"

#include <stdlib.h>
#include "stdio.h"
#include "util.h"

extern const size_t expend_16[4][4];
extern const size_t expend_32[4][4];

typedef void (*key_expansion)(byte *, byte *, byte *);

/**
 * Perform Encryption
 *
 * Performs key expansion and xors input sequence with expanded key
 *
 * @param key
 * @param key_len - length of key (128 or 256 bit)
 * @param nonce - 64-bit nonce
 * @param buf - input text buffer/ rusult buffer
 * @param buf_len - length of buffer
 */
void encrypt(byte * key, enum key_len_t key_len, byte * nonce,
                                    byte * buf, size_t buf_len, byte * output) {
    byte key_stream[64];
    byte n[16];

    key_expansion exp_f;

    if (key_len == KEYLEN_256) {
        exp_f = &key_expansion32;
    } else {
        exp_f = &key_expansion16;
    }

    for (int i = 0; i < 8; ++i) {
        n[i] = nonce[i];
    }
    for (int i = 8; i < 16; ++i) {
        n[i] = 0;
    }

    for (word i = 0; i < buf_len; i++) {
        if (i % 64 == 0) {
            rev_little_endian(i / 64, n + 8);

            exp_f(key, n, key_stream);
        }

        output[i] = buf[i] ^ key_stream[i % 64];
    }
}

/**
 * Perform Decryption
 *
 * @param key
 * @param key_len - length of key (128 or 256 bit)
 * @param nonce - 64-bit nonce
 * @param buf - input text buffer/ rusult buffer
 * @param buf_len - length of buffer
 */
void decrypt(byte * key, enum key_len_t key_len, byte * nonce,
                                    byte * buf, size_t buf_len, byte * output) {
    encrypt(key, key_len, nonce, buf, buf_len, output);
}

/**
 * The main build block - 4 words transformation
 *
 * @param y[0-3] - word on which transformation is performing
 */
void quarter_round(word * y0, word * y1, word * y2, word * y3) {
    *y1 = *y1 ^ rotate_left(*y0 + *y3, 7);
    *y2 = *y2 ^ rotate_left(*y1 + *y0, 9);
    *y3 = *y3 ^ rotate_left(*y2 + *y1, 13);
    *y0 = *y0 ^ rotate_left(*y3 + *y2, 18);
}

/**
 * All rows of state matrix is shifted on size of their 
 * row index (starts from 0), then transformed by quarter round function
 *
 * @param y - state 
 */
void row_round(word * y) {
    quarter_round(&y[0], &y[1], &y[2], &y[3]);
    quarter_round(&y[5], &y[6], &y[7], &y[4]);
    quarter_round(&y[10], &y[11], &y[8], &y[9]);
    quarter_round(&y[15], &y[12], &y[13], &y[14]);
}

/**
 * All columns of state matrix is shifted on size of their 
 * column index (starts from 0), then transformed by quarter round function
 *
 * @param y - state 
 */
void column_round(word * y) {
    quarter_round(&y[0], &y[4], &y[8], &y[12]);
    quarter_round(&y[5], &y[9], &y[13], &y[1]);
    quarter_round(&y[10], &y[14], &y[2], &y[6]);
    quarter_round(&y[15], &y[3], &y[7], &y[11]);
}

/**
 * perform column round and row round functions on the state
 *
 * @param y - 16 word size state 
 */
void double_round(word * y) {
    column_round(y);
    row_round(y);
}


/**
 * Hash Function takes 64 bytes as input and returns a 64-byte sequence.
 *
 * Create two copies of the state in little endian;
 * The second copy is transformed 10 times by double
 * round function and than added with first
 *
 * @param s - 64 byte size sequence 
 */
void hash(byte * s) {
    word arr1[16];
    word arr2[16];

    for (int i = 0; i < 16; i++) {
        arr1[i] = arr2[i] = little_endian(s + 4 * i);
    }

    for (int i = 0; i < 10; i++) {
        double_round(arr2);
    }

    for (int i = 0; i < 16; i++) {
        arr2[i] += arr1[i];
        rev_little_endian(arr2[i], s + 4 * i);
    }
}

/**
 * Key expansion function for 16 bit key. Result is key stream.
 * Key stream populates by pre defined values,
 * that stored in expend_16 table. Then key and nonce getting stored
 * in pre defined place in key stream.
 * - key stream = (expand16(0), key(0), expand16(1), nonce,
 *                                  expand16(2), key(0), expand16(3))
 *
 * @param k - 16-bytes key
 * @param nonce - 16-bytes nonce
 * @param key_stream - 64-bytes key_stream
 */
void key_expansion16(byte * k, byte * nonce, byte * key_stream) {
    for (int i = 0; i < 64; i += 20) {
        for (int j = 0; j < 4; j++) {
            key_stream[i + j] = expend_16[i / 20][j];
        }
    }

    for (int i = 0; i < 16; ++i) {
        key_stream[i + 4] = k[i];
        key_stream[i + 24] = nonce[i];
        key_stream[i + 44] = k[i];
    }

    hash(key_stream);
}

/**
 * Key expansion function for 32 bit key. Result is key stream.
 * Key stream populates by pre defined values,
 * that stored in expend_32 table. Then key and nonce getting stored
 * in pre defined place in key stream.
 * - key stream = (expand32(0), key(0), expand32(1), nonce,
 *                                  expand32(2), key(1), expand32(3))
 *
 * @param k - 32-bytes key
 * @param nonce - 16-bytes nonce
 * @param key_stream - 64-bytes key_stream
 */
void key_expansion32(byte * k, byte * nonce, byte * key_stream) {
    for (int i = 0; i < 64; i += 20) {
        for (int j = 0; j < 4; j++) {
            key_stream[i + j] = expend_32[i / 20][j];
        }
    }

    for (int i = 0; i < 16; ++i) {
        key_stream[i + 4] = k[i];
        key_stream[i + 24] = nonce[i];
        key_stream[i + 44] = k[i + 16];
    }

    hash(key_stream);
}
