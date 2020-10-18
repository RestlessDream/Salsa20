#include "util.h"

/**
 * Rotate word left (analog c++ <<< operation)
 *
 * @param value - rotated value
 * @param shift - the size by which the value is shifted
 * @return result - shifted value
 */
word rotate_left(word value, int shift) {
    return (value << shift) | (value >> (32 - shift));
}


/**
 * Returns word size result, that is in a little endian
 * representation of byte array
 *
 * @param value - 4-bytes array
 * @return result - representation of value in little endian
 */
word little_endian(byte * value) {
      return value[0] + (value[1] << 8) + (value[2] << 16) + (value[3] << 24);
}

/**
 * Reverse little endian function
 *
 * @param value - value in little endian
 * @param arr - 4-byte array
 */
void rev_little_endian(word value, byte * arr) {
    arr[0] = value;
    arr[1] = value >> 8;
    arr[2] = value >> 16;
    arr[3] = value >> 24;
}
