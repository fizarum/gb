#ifndef GAME_SDK_SPRITE_HOLDER_H
#define GAME_SDK_SPRITE_HOLDER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../sprite/sprite.h"
#include "../types/types.h"
#include "layer_type.h"

typedef struct SpritesHolder SpritesHolder;

SpritesHolder* SpritesHolder_Create();
void SpritesHolder_Destroy(SpritesHolder* holder);

/**
 * @brief Creates Sprite instance and add to internal container
 *
 * @param holder
 * @param data
 * @param type
 * @param x
 * @param y
 * @return SpriteId unique id of sprite if it is added or OBJECT_ID_NA otherwise
 */
SpriteId SpritesHolder_AddSprite(SpritesHolder* holder, const SpriteData* data,
                                 const LayerType layer);

/**
 * @brief Get Color Index by [x,y] coordinates
 *
 *
 * @param holder
 * @param type
 * @param x
 * @param y
 * @param fallback color index if no one can be found
 * @return Color Index (_ci) from sprite by [x,y] coords or fallback if sprite
 * cant be found
 */
_ci SpritesHolder_GetColorIndex(const SpritesHolder* holder,
                                const LayerType layer, const _u16 x,
                                const _u16 y, _ci fallback);

void SpritesHolder_ForeachSprite(const SpritesHolder* holder,
                                 const LayerType type,
                                 void (*foreach)(SpriteId spriteId));

#ifdef __cplusplus
}
#endif

#endif  // GAME_SDK_SPRITE_HOLDER_H
