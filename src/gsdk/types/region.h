#ifndef GAME_SDK_REGION_H
#define GAME_SDK_REGION_H

#ifdef __cplusplus
extern "C" {
#endif

#include <types.h>

typedef struct Region_t {
  _u16 l;
  _u16 t;
  _u16 r;
  _u16 b;
} Region_t;

static inline bool Region_IsEmpty(Region_t *region) {
  return region->l == 0 && region->t == 0 && region->r == 0 && region->b == 0;
}

static inline void Region_Reset(Region_t *region) {
  region->l = 0;
  region->t = 0;
  region->r = 0;
  region->b = 0;
}

static inline bool Region_isSame(const Region_t *region, const _u16 l,
                                 const _u16 t, const _u16 r, const _u16 b) {
  return region->l == l && region->t == t && region->r == r && region->b == b;
}

#ifdef __cplusplus
}
#endif

#endif  // GAME_SDK_REGION_H