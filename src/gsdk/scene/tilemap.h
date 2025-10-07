#ifndef GAME_SDK_TILEMAP_H
#define GAME_SDK_TILEMAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "../screen/palette.h"
#include "../types/rectangle.h"
#include "../types/types.h"

typedef struct TileMap TileMap;

TileMap* TileMap_Create(const _u8 tileSize);
void TileMap_Destroy(TileMap* tileMap);

void TileMap_Set(TileMap* tileMap, SpriteId* tiles, const _u16 count,
                 const _u8 widthInTiles);
SpriteId TileMap_GetTile(const TileMap* tileMap, const _u16 x, const _u16 y);
_ci TileMap_GetPixel(const TileMap* tileMap, const _u16 x, const _u16 y,
                     const _ci fallback);

const Rectangle* TileMap_GetBounds(const TileMap* tileMap);

#ifdef __cplusplus
}
#endif

#endif  // GAME_SDK_TILEMAP_H