#ifndef GAME_SDK_PIXEL_CALCULATOR_H
#define GAME_SDK_PIXEL_CALCULATOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../menu/menu.h"
#include "../scene/sprites_holder.h"
#include "../scene/tilemap.h"
#include "../types/types.h"
#include "palette.h"

static _ci index;
SpriteId id = OBJECT_ID_NA;

static inline Color CalculatePixel(Palette_t* palette, _u8 x, _u8 y,
                                   SpritesHolder_t* sprites, TileMap_t* tilemap,
                                   LayerType_t layerChanged, _ci fallbackCI) {
  index = fallbackCI;
  id = OBJECT_ID_NA;

  if (layerChanged == LAYER_UI) {
    index = SpritesHolderGetColorIndex2(sprites, LAYER_UI, x, y, fallbackCI);
    if (index != fallbackCI) {
      return palette->colors[index];
    }
  }

  if (layerChanged >= LAYER_NEAR) {
    index = SpritesHolderGetColorIndex2(sprites, LAYER_NEAR, x, y, fallbackCI);
    if (index != fallbackCI) {
      return palette->colors[index];
    }
  }

  if (layerChanged >= LAYER_MID) {
    index = SpritesHolderGetColorIndex2(sprites, LAYER_MID, x, y, fallbackCI);
    if (index != fallbackCI) {
      return palette->colors[index];
    }
  }

  if (layerChanged >= LAYER_FAR) {
    index = SpritesHolderGetColorIndex2(sprites, LAYER_FAR, x, y, fallbackCI);
    if (index != fallbackCI) {
      return palette->colors[index];
    }

    index = TileMapGetPixel(tilemap, x, y, fallbackCI);
    if (index != fallbackCI) {
      return palette->colors[index];
    }
  }

  return palette->backgoundColor;
}

static inline Color CalculatePixelForMenu(Palette_t* palette, _u8 x, _u8 y,
                                          Menu_t* menu, _ci fallbackCI) {
  _ci index = MenuGetColorIndex(menu, x, y, fallbackCI);
  return palette->colors[index];
}
#ifdef __cplusplus
}
#endif

#endif  // GAME_SDK_PIXEL_CALCULATOR_H