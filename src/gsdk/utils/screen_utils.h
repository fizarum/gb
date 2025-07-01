#ifndef GAME_SDK_SCREEN_UTILS_H
#define GAME_SDK_SCREEN_UTILS_H

#include <stdbool.h>

#include "../types/point.h"
#include "../types/types.h"

inline const static bool IsPointInside(const _i16 x, const _i16 y,
                                       const _i16 left, const _i16 top,
                                       const _i16 right, const _i16 bottom) {
  return x >= left && x <= right && y >= top && y <= bottom;
}

/**
 * @brief Finds 1D index in 2D array with given width
 */
inline const static _u32 GetIndexIn2DSpace(const _u16 x, const _u16 y,
                                           const _u16 width) {
  return y * width + x;
}

/**
 * @brief Converts index to x,y coords in 2D array with given width
 */
inline static void GetXYFromIndex(const _u32 index, const _u16 width, _u16* x,
                                  _u16* y) {
  *y = index / width;
  *x = index % width;
}

inline const static _u16 GetVisibleRight(const Point_t* position,
                                         const _u16 width, const _u16 limit) {
  int result = position->x + width - 1;
  result = result >= limit ? limit - 1 : result;
  result = result < 0 ? 0 : result;

  return (_u16)result;
}

inline const static _u16 GetVisibleBottom(const Point_t* position,
                                          const _u16 height, const _u16 limit) {
  int result = position->y + height - 1;
  result = result >= limit ? limit - 1 : result;
  result = result < 0 ? 0 : result;

  return (_u16)result;
}

inline const static _i16 GetRight(const Point_t* position, const _u16 width) {
  int right = position->x + width - 1;
  return (_i16)right;
}

inline const static _i16 GetBottom(const Point_t* position, const _u16 height) {
  int bottom = position->y + height - 1;
  return (_i16)bottom;
}

#endif  // GAME_SDK_SCREEN_UTILS_H