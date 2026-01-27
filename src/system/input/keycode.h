#ifndef GAME_SDK_KEYCODE_H
#define GAME_SDK_KEYCODE_H

#ifdef __cplusplus
extern "C" {
#endif
// keycode - used in system as genral key data
typedef enum {
  KEY_NONE = 0,
  KEY_LEFT = 1,
  KEY_UP = 2,
  KEY_RIGHT = 4,
  KEY_DOWN = 8,
  KEY_X = 16,
  KEY_Y = 32,
  KEY_A = 64,
  KEY_B = 128,
  KEY_SELECT = 256,
  KEY_MENU = 512,
  // reserved
  //  KEY_LEFT_TRIGGER = 1024,
  //  KEY_RIGHT_TRIGGER = 2048,
} KeyCode;

#ifdef __cplusplus
}
#endif

#endif  // GAME_SDK_KEYCODE_H