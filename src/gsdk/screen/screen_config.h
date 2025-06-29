#ifndef GAME_SDK_SCREEN_CONFIG_H
#define GAME_SDK_SCREEN_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <types.h>

typedef struct ScreenConfig {
  _u16 width;
  _u16 height;

  /*
   * Virtual pixel size. Bigger pixel size - better performance
   * but smaller resolution
   */
  // #define PIXEL_SIZE 2  // 160x120
  //  #define PIXEL_SIZE 3  // 106x80
  //  #define PIXEL_SIZE 4  // 80x60
  //  #define PIXEL_SIZE 5  // 64x48
  //  the best performance/pixel size value
  //  #define PIXEL_SIZE 8  // 40x30
  //  #define PIXEL_SIZE 10  // 32x24
  //  #define PIXEL_SIZE 20  // 16x12
  //  #define PIXEL_SIZE 40  // 8x6
  _u8 pixelSize;
} ScreenConfig;

#ifdef __cplusplus
}
#endif

#endif  // GAME_SDK_SCREEN_CONFIG_H