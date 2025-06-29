#ifndef GAME_SDK_SPRITE_H
#define GAME_SDK_SPRITE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "../types/layer_type.h"
#include "../types/point.h"
#include "animation_speed.h"
#include "sprite_data.h"

typedef struct Sprite_t Sprite_t;
Sprite_t* Sprite_Create(const SpriteData_t* data, const LayerType_t layer);
void Sprite_Destroy(Sprite_t* sprite);

const Point_t* SpriteGetPosition(const Sprite_t* sprite);
const _u8 SpriteGetWidth(const Sprite_t* sprite);
const _u8 SpriteGetHeight(const Sprite_t* sprite);

/**
 * @brief Get color index in sprite
 *
 * @param screenX - x coord on screen
 * @param screenY - y coord on screen
 * @param fallback - function will return this value if color index can not be
 * otained
 * @return ColorIndex
 */
const _ci SpriteGetColorIndex(const Sprite_t* sprite, const _u8 x, const _u8 y,
                              const _ci fallback);

const bool SpriteContainsPoint(const Sprite_t* sprite, const _i16 x,
                               const _i16 y);

const LayerType_t SpriteGetLayer(const Sprite_t* sprite);

void SpriteMoveTo(Sprite_t* sprite, const _u8 x, const _u8 y);
void SpriteMoveBy(Sprite_t* sprite, const _i8 x, const _i8 y);

const bool SpriteIsOnDisplay(const Sprite_t* sprite, _u16 displayWidth,
                             _u16 displayHeight);

void SpriteUpdateState(Sprite_t* sprite);
const bool SpriteIsFrameChanged(const Sprite_t* sprite);
void SpriteSetAnimationSpeed(Sprite_t* sprite, const AnimationSpeed_t speed);

#ifdef __cplusplus
}
#endif

#endif  // GAME_SDK_SPRITE_H