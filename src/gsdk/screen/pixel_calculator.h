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

static _ci _index;

static inline Color CalculatePixel(Palette* palette, _u16 x, _u16 y,
                                   SpritesHolder* sprites, TileMap* tilemap,
                                   _ci fallbackCI) {
  _index = SpritesHolder_GetColorIndex(sprites, LAYER_UI, x, y, fallbackCI);
  if (_index != fallbackCI) {
    return palette->colors[_index];
  }

  _index = SpritesHolder_GetColorIndex(sprites, LAYER_NEAR, x, y, fallbackCI);
  if (_index != fallbackCI) {
    return palette->colors[_index];
  }

  _index = SpritesHolder_GetColorIndex(sprites, LAYER_MID, x, y, fallbackCI);
  if (_index != fallbackCI) {
    return palette->colors[_index];
  }

  _index = SpritesHolder_GetColorIndex(sprites, LAYER_FAR, x, y, fallbackCI);
  if (_index != fallbackCI) {
    return palette->colors[_index];
  }

  _index = TileMap_GetPixel(tilemap, x, y, fallbackCI);
  if (_index != fallbackCI) {
    return palette->colors[_index];
  }

  return palette->colors[fallbackCI];
}

static inline Color CalculatePixelForMenu(Palette* palette, _u8 x, _u8 y,
                                          Menu* menu, _ci fallbackCI) {
  _ci _index = MenuGetColorIndex(menu, x, y, fallbackCI);
  return palette->colors[_index];
}
#ifdef __cplusplus
}
#endif

#endif  // GAME_SDK_PIXEL_CALCULATOR_H