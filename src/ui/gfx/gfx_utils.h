#ifndef GAME_SDK_GFX_UTILS_H
#define GAME_SDK_GFX_UTILS_H

#include <types.h>

inline const static bool GFX_IsPointInside(const _i16 x, const _i16 y,
                                           const _i16 left, const _i16 top,
                                           const _i16 right,
                                           const _i16 bottom) {
  return x >= left && x <= right && y >= top && y <= bottom;
}

/**
 * @brief Finds 1D index in 2D array with given width
 */
inline const static _u32 GFX_GetIndexFromXY(const _u16 x, const _u16 y,
                                               const _u16 width) {
  return y * width + x;
}

/**
 * @brief Converts index to x,y coords in 2D array with given width
 */
inline static void GFX_GetXYFromIndex(const _u32 index, const _u16 width,
                                      _u16* x, _u16* y) {
  *y = index / width;
  *x = index % width;
}

#endif  // GAME_SDK_SCREEN_UTILS_H