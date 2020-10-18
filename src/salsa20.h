/*
 * Salsa20 Algorithm realization
 *
 * Made by Igor Klapatnjuk
 *
 * MIT License
 *
 * Copyright (c) 2020 RestlessDream
 */

#ifndef RC4_
#define RC4_

#include "types.h"

void encrypt(byte * key, enum key_len_t key_len, byte * nonce,
                                byte * buf, size_t buf_len, byte * output);
void decrypt(byte * key, enum key_len_t key_len, byte * nonce,
                                byte * buf, size_t buf_len, byte * output);
void quarter_round(word * y0, word * y1, word * y2, word * y3);
void row_round(word * y);
void column_round(word * y);
void double_round(word * y);
void hash(byte * s);
void key_expansion16(byte * k, byte * nonce, byte * key_stream);
void key_expansion32(byte * k, byte * nonce, byte * key_stream);

#endif
