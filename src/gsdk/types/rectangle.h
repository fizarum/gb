#ifndef GAME_SDK_RECTANGLE_H
#define GAME_SDK_RECTANGLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <types.h>

typedef struct Rectangle {
  _i16 left;
  _i16 top;
  _i16 right;
  _i16 bottom;
} Rectangle;

static inline bool Rectangle_IsEmpty(Rectangle* rectangle) {
  return rectangle->left == 0 && rectangle->top == 0 && rectangle->right == 0 &&
         rectangle->bottom == 0;
}

static inline void Rectangle_Reset(Rectangle* rectangle) {
  rectangle->left = 0;
  rectangle->top = 0;
  rectangle->right = 0;
  rectangle->bottom = 0;
}

static inline void Rectangle_Set(Rectangle* rectangle, const _u16 left,
                                 const _u16 top, const _u16 right,
                                 const _u16 bottom) {
  rectangle->left = left;
  rectangle->top = top;
  rectangle->right = right;
  rectangle->bottom = bottom;
}

static inline void Rectangle_Resize(Rectangle* rectangle, const _u16 width,
                                    const _u16 height) {
  rectangle->right = rectangle->left + width - 1;
  rectangle->bottom = rectangle->top + height - 1;
}

static inline _u16 Rectangle_GetVisibleLeft(const Rectangle* rectangle) {
  return rectangle->left < 0 ? 0 : rectangle->left;
}

static inline _u16 Rectangle_GetVisibleTop(const Rectangle* rectangle) {
  return rectangle->top < 0 ? 0 : rectangle->top;
}

static inline _u16 Rectangle_GetVisibleRight(const Rectangle* rectagle,
                                             const _u16 limit) {
  // if object is out of screen
  if (rectagle->right < 0) return 0;
  return rectagle->right >= limit ? limit - 1 : rectagle->right;
}

static inline _u16 Rectangle_GetVisibleBottom(const Rectangle* rectangle,
                                              const _u16 limit) {
  // if object is out of screen
  if (rectangle->bottom < 0) return 0;
  return rectangle->bottom >= limit ? limit - 1 : rectangle->bottom;
}

static inline bool Rectangle_isSame(const Rectangle* rectangle, const _u16 l,
                                    const _u16 t, const _u16 r, const _u16 b) {
  return rectangle->left == l && rectangle->top == t && rectangle->right == r &&
         rectangle->bottom == b;
}

#ifdef __cplusplus
}
#endif

#endif  // GAME_SDK_RECTANGLE_H