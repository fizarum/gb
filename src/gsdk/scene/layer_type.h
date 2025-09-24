#ifndef GAME_SDK_LAYER_TYPE_H
#define GAME_SDK_LAYER_TYPE_H

#ifdef __cplusplus
extern "C" {
#endif
typedef enum {
  LAYER_NONE = -1,
  LAYER_TILEMAP = 0,
  LAYER_FAR = 1,
  LAYER_MID = 2,
  LAYER_NEAR = 3,
  LAYER_UI = 4,
  LAYER_MENU_FAR = 5,
  LAYER_MENU_NEAR = 6,
} LayerType;

#ifdef __cplusplus
}
#endif

#endif  // GAME_SDK_LAYER_TYPE_H