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

typedef struct SpritesHolder_t SpritesHolder_t;

SpritesHolder_t* SpritesHolderCreate();
void SpritesHolderDestroy(SpritesHolder_t* holder);

/**
 * @brief Creates Sprite_t instance and add to internal container
 *
 * @param holder
 * @param data
 * @param type
 * @param x
 * @param y
 * @return SpriteId unique id of sprite if it is added or OBJECT_ID_NA otherwise
 */
SpriteId SpritesHolderAddSprite(SpritesHolder_t* holder,
                                const SpriteData_t* data,
                                const LayerType_t layer);

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
_ci SpritesHolderGetColorIndex2(const SpritesHolder_t* holder,
                                const LayerType_t layer, const _u16 x,
                                const _u16 y, _ci fallback);

void SpritesHolderForeachSprite(const SpritesHolder_t* holder,
                                const LayerType_t type,
                                void (*foreach)(SpriteId spriteId));

#ifdef __cplusplus
}
#endif

#endif  // GAME_SDK_SPRITE_HOLDER_H
