#ifndef GAME_SDK_SPRITE_H
#define GAME_SDK_SPRITE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "../types/layer_type.h"
#include "../types/point.h"
#include "../types/rectangle.h"
#include "animation_speed.h"
#include "sprite_data.h"

typedef struct Sprite Sprite;
Sprite* Sprite_Create(const SpriteData* data, const LayerType layer);
void Sprite_Destroy(Sprite* sprite);

const Point* Sprite_GetPosition(const Sprite* sprite);
const _u8 Sprite_GetWidth(const Sprite* sprite);
const _u8 Sprite_GetHeight(const Sprite* sprite);
void Sprite_GetBounds(const Sprite* sprite, Rectangle* bounds);

/**
 * @brief Get color index in sprite
 *
 * @param screenX - x coord on screen
 * @param screenY - y coord on screen
 * @param fallback - function will return this value if color index can not be
 * otained
 * @return ColorIndex
 */
const _ci Sprite_GetColorIndex(const Sprite* sprite, const _u16 x, const _u16 y,
                               const _ci fallback);

const bool Sprite_ContainsPoint(const Sprite* sprite, const _u16 x,
                                const _u16 y);

const LayerType Sprite_GetLayer(const Sprite* sprite);

void Sprite_MoveTo(Sprite* sprite, const _u16 x, const _u16 y);
void Sprite_MoveBy(Sprite* sprite, const _i8 x, const _i8 y);

const bool Sprite_IsOnDisplay(const Sprite* sprite, _u16 displayWidth,
                              _u16 displayHeight);

void Sprite_UpdateState(Sprite* sprite);
const bool Sprite_IsFrameChanged(const Sprite* sprite);
void Sprite_SetAnimationSpeed(Sprite* sprite, const AnimationSpeed speed);

#ifdef __cplusplus
}
#endif

#endif  // GAME_SDK_SPRITE_H