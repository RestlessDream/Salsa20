#ifndef _UTIL_H_
#define _UTIL_H_

#include "types.h"

#include "stdlib.h"

word rotate_left(word value, int shift);

word little_endian(byte * value);
void rev_little_endian(word value, byte * arr);

#endif // UTIL_H_
