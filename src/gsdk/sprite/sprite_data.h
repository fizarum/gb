#ifndef GAME_SDK_SPRITE_DATA_H
#define GAME_SDK_SPRITE_DATA_H

#include "../types/color.h"
#include "../types/types.h"

typedef struct SpriteData {
  _u8 width;
  _u8 height;
  _u16 indicesCount;
  _u8 frames;
  _u16 indicesPerFrame;
  _ci *indices;

} SpriteData;

#endif  // GAME_SDK_SPRITE_DATA_H