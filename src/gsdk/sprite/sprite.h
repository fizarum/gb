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

const Point_t* Sprite_GetPosition(const Sprite_t* sprite);
const _u8 Sprite_GetWidth(const Sprite_t* sprite);
const _u8 Sprite_GetHeight(const Sprite_t* sprite);

/**
 * @brief Get color index in sprite
 *
 * @param screenX - x coord on screen
 * @param screenY - y coord on screen
 * @param fallback - function will return this value if color index can not be
 * otained
 * @return ColorIndex
 */
const _ci Sprite_GetColorIndex(const Sprite_t* sprite, const _u16 x,
                               const _u16 y, const _ci fallback);

const bool Sprite_ContainsPoint(const Sprite_t* sprite, const _u16 x,
                                const _u16 y);

const LayerType_t Sprite_GetLayer(const Sprite_t* sprite);

void Sprite_MoveTo(Sprite_t* sprite, const _u16 x, const _u16 y);
void Sprite_MoveBy(Sprite_t* sprite, const _i8 x, const _i8 y);

const bool Sprite_IsOnDisplay(const Sprite_t* sprite, _u16 displayWidth,
                              _u16 displayHeight);

void Sprite_UpdateState(Sprite_t* sprite);
const bool Sprite_IsFrameChanged(const Sprite_t* sprite);
void Sprite_SetAnimationSpeed(Sprite_t* sprite, const AnimationSpeed speed);

#ifdef __cplusplus
}
#endif

#endif  // GAME_SDK_SPRITE_H