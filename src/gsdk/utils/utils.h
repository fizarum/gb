#ifndef GAME_SDK_UTILS_H
#define GAME_SDK_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <ctype.h>
#include <stdbool.h>

#include "../types/types.h"

/**
 * @brief Set (to 1) the bit in byte, source value isn't mutated
 *
 * @param source source byte
 * @param position of bit to change, counts from 0
 * @return uint8_t
 */
inline _u8 setBit(_u8 source, _u8 position) {
  return source | (1UL << position);
}

/**
 * @brief Reset (to 0) the bit in byte, source value isn't mutated
 *
 * @param source source byte
 * @param position of bit to change, counts from 0
 * @return uint8_t
 */
inline _u8 resetBit(_u8 source, _u8 position) {
  // if position is 1 then it moves 0001 to 0010
  return source & (~(1UL << position));
}

/**
 * @brief Set or reset the bit in uint16_t, source value isn't mutated
 *
 * @param source source uint16_t
 * @param position of bit to change, counts from 0
 * @param isSet 0 means reset, > 0 - means set
 * @return updated copy of source value
 */
inline _u16 setBit16(_u16 source, _u8 position) {
  return source | (1UL << position);
}
inline uint32_t setBit32(uint32_t source, uint8_t position) {
  return source | (1UL << position);
}

inline bool isBitSet(uint16_t source, uint8_t position) {
  _u16 mask = (1UL << position);
  return (source & mask) == mask;
}

inline bool isBitSet32(uint32_t source, uint8_t position) {
  _u32 mask = (1UL << position);
  return (source & mask) == mask;
}

/**
 * @brief modify current string by changing all
 * letters to lower case
 *
 * @param str string to transform
 */
inline void strToLowerCase(char* str) {
  char* temp = str;
  while (*temp) {
    *temp = tolower(*temp);
    temp++;
  }
}

/**
 * align value to some bigger one, by blockSize
 * for example: alignTo(2737, 10) will return 2740
 */
inline _u32 alignTo(uint32_t value, uint16_t blockSize) {
  _u32 blockCounts = value / blockSize;
  _u32 rounded = blockCounts * blockSize;
  if (value == rounded) {
    return rounded;
  }
  rounded += blockSize;
  return rounded;
}

/**
 * @brief align value to limit. If value exceeds limit, limit-1 is
 * returned
 */
inline const static _i32 normalize(const _i32 value, const _i32 limit) {
  if (value < limit) {
    return value;
  }

  return limit - 1;
}

#ifdef __cplusplus
}
#endif

#endif  //  GAME_SDK_UTILS_H
