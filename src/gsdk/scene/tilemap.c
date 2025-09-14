#include "tilemap.h"

#include <stdlib.h>

#include "../collections/array.h"
#include "../sprite/sprite.h"

/**
 * @brief Structure representing map of tiles. Each item of TileMap_t is a
 * reference to some Sprite_t
 * The real size of map can be calculated (width for example) as:
 * tileSize * width
 * Also, worth to mention that all tiles should have the same value: tileSize
 * @param width - number of tiles in width
 * @param height - number of tiles in height
 * @param tileSize - size of one tile in pixels
 */
typedef struct TileMap_t {
  _u8 width;
  _u8 height;

  _u8 tileSize;
  SpriteId* tiles;

  _u16 tilesCount;
  Rectangle_t bounds;
} TileMap_t;

TileMap_t* TileMapCreate(const _u8 tileSize) {
  TileMap_t* tileMap = (TileMap_t*)malloc(sizeof(TileMap_t));

  if (tileMap == NULL) return NULL;

  tileMap->width = 0;
  tileMap->height = 0;
  tileMap->tileSize = tileSize;
  tileMap->tiles = NULL;
  tileMap->tilesCount = 0;
  Rectangle_Reset(&(tileMap->bounds));

  return tileMap;
}

void TileMapDestroy(TileMap_t* tileMap) {
  if (tileMap == NULL) return;

  free(tileMap->tiles);

  free(tileMap);
}

void TileMapSet(TileMap_t* tileMap, SpriteId* tiles, const _u16 count,
                const _u8 width) {
  if (tileMap->tilesCount > 0 && tileMap->tiles != NULL) {
    free(tileMap->tiles);
  }

  tileMap->tiles = (SpriteId*)calloc(count, sizeof(SpriteId));

  if (tileMap->tiles == NULL) return;

  for (_u16 index = 0; index < count; index++) {
    tileMap->tiles[index] = tiles[index];
  }
  tileMap->tilesCount = count;

  tileMap->width = width;
  tileMap->height = count / tileMap->width;

  if (count % tileMap->width > 0) {
    tileMap->height++;
  }

  // TODO: add check if size is bigger than screen !
  _u8 widthInPixels = (tileMap->width) * (tileMap->tileSize);
  _u8 heightInPixels = (tileMap->height) * (tileMap->tileSize);

  Rectangle_Resize(&(tileMap->bounds), widthInPixels, heightInPixels);
}

SpriteId TileMapGetTile(const TileMap_t* tileMap, const _u16 x, const _u16 y) {
  _u8 tileSize = tileMap->tileSize;

  _u8 tileAddressByX = x / tileSize;
  _u8 tileAddressByY = y / tileSize;

  if (tileAddressByX >= tileMap->width) return OBJECT_ID_NA;
  if (tileAddressByY >= tileMap->height) return OBJECT_ID_NA;

  _u16 index = tileAddressByY * (tileMap->width) + tileAddressByX;
  if (index >= tileMap->tilesCount) return OBJECT_ID_NA;

  return tileMap->tiles[index];
}

_ci TileMapGetPixel(const TileMap_t* tileMap, const _u16 x, const _u16 y,
                    const _ci fallback) {
  if (tileMap == NULL) return fallback;

  SpriteId id = TileMapGetTile(tileMap, x, y);
  if (id == OBJECT_ID_NA) return fallback;

  Sprite_t* sprite = (Sprite_t*)id;
  _u8 xOnSprite = x % Sprite_GetWidth(sprite);
  _u8 yOnSprite = y % Sprite_GetHeight(sprite);

  return Sprite_GetColorIndex(sprite, xOnSprite, yOnSprite, fallback);
}

const Rectangle_t* TileMapGetBounds(const TileMap_t* tileMap) {
  return &(tileMap->bounds);
}