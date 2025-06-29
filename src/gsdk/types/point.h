#ifndef GAME_SDK_POINT_H
#define GAME_SDK_POINT_H

#ifdef __cplusplus
extern "C" {
#endif
#include <types.h>

/**
 * @brief Structure of point in 2D space.
 *
 */
typedef struct Point_t {
  _i16 x;
  _i16 y;
} Point_t;

#ifdef __cplusplus
}
#endif

#endif  // GAME_SDK_POINT_H