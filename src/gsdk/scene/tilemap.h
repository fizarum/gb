#ifndef GAME_SDK_TILEMAP_H
#define GAME_SDK_TILEMAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "../screen/palette.h"
#include "../types/rectangle.h"
#include "../types/types.h"

typedef struct TileMap_t TileMap_t;

TileMap_t* TileMapCreate(const _u8 tileSize);
void TileMapDestroy(TileMap_t* tileMap);

void TileMapSet(TileMap_t* tileMap, SpriteId* tiles, const _u16 count,
                const _u8 width);
SpriteId TileMapGetTile(const TileMap_t* tileMap, const _u16 x, const _u16 y);
_ci TileMapGetPixel(const TileMap_t* tileMap, const _u16 x, const _u16 y,
                    const _ci fallback);

const Rectangle_t* TileMapGetBounds(const TileMap_t* tileMap);

#ifdef __cplusplus
}
#endif

#endif  // GAME_SDK_TILEMAP_H