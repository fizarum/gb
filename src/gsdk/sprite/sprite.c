#include "sprite.h"

#include <stdbool.h>
#include <stdlib.h>

#include "../screen/palette.h"
#include "../types/point.h"
#include "../utils/screen_utils.h"

typedef struct Sprite_t {
  SpriteData_t* data;
  Point_t position;
  LayerType_t layer;

  // animation part
  AnimationSpeed_t animationSpeed;
  _u8 frameIndex;
  _i8 remainTicksToUpdateFrame;
  bool frameChanged;
} Sprite_t;

Sprite_t* Sprite_Create(const SpriteData_t* data, const LayerType_t layer) {
  Sprite_t* sprite = (Sprite_t*)malloc(sizeof(Sprite_t));

  if (sprite == NULL) return NULL;

  sprite->data = (SpriteData_t*)data;
  sprite->position.x = 0;
  sprite->position.y = 0;
  sprite->layer = layer;

  sprite->frameIndex = 0;
  sprite->animationSpeed =
      sprite->data->frames > 1 ? ANIMATION_SPEED_NORMAL : ANIMATION_SPEED_NONE;

  // animation speed already contains count of updates required before switching
  // on next frame
  sprite->remainTicksToUpdateFrame = sprite->animationSpeed;

  // first draw = false, redraw will be called later by DRTacker
  sprite->frameChanged = false;

  return sprite;
}

void Sprite_Destroy(Sprite_t* sprite) { free(sprite); }

const _u8 SpriteGetWidth(const Sprite_t* sprite) { return sprite->data->width; }

const _u8 SpriteGetHeight(const Sprite_t* sprite) {
  return sprite->data->height;
}

const Point_t* SpriteGetPosition(const Sprite_t* sprite) {
  return &(sprite->position);
}

const _ci SpriteGetColorIndex(const Sprite_t* sprite, const _u8 x, const _u8 y,
                              const _ci fallback) {
  bool inside = IsPointInside(x, y, sprite->position.x, sprite->position.y,
                              sprite->position.x + sprite->data->width - 1,
                              sprite->position.y + sprite->data->height - 1);

  if (inside == false) {
    return fallback;
  }

  _i16 localX = x - sprite->position.x;
  _i16 localY = y - sprite->position.y;

  // we have 2 real pixels per item in "pixels" array we have to divide the
  // result of RectangleIndexOf() by 2
  _u16 index = GetIndexIn2DSpace(localX, localY, sprite->data->width) / 2;

  if (sprite->animationSpeed != ANIMATION_SPEED_NONE) {
    _u16 offset = (sprite->frameIndex * (sprite->data->indicesPerFrame));
    index += offset;
  }

  ColorIndexes indexes = sprite->data->indices[index];

  bool isOdd = (localX & 1) == 1;
  if (isOdd) {
    return getSecondIndex(indexes);
  }
  return getFirstIndex(indexes);
}

const bool SpriteContainsPoint(const Sprite_t* sprite, const _i16 x,
                               const _i16 y) {
  return IsPointInside(x, y, sprite->position.x, sprite->position.y,
                       sprite->position.x + sprite->data->width - 1,
                       sprite->position.y + sprite->data->height - 1);
}

const LayerType_t SpriteGetLayer(const Sprite_t* sprite) {
  return sprite->layer;
}

void SpriteMoveTo(Sprite_t* sprite, const _u8 x, const _u8 y) {
  sprite->position.x = x;
  sprite->position.y = y;
}

void SpriteMoveBy(Sprite_t* sprite, const _i8 x, const _i8 y) {
  sprite->position.x += x;
  sprite->position.y += y;
}

const bool SpriteIsOnDisplay(const Sprite_t* sprite, _u16 displayWidth,
                             _u16 displayHeight) {
  _i16 l = sprite->position.x;
  _i16 t = sprite->position.y;
  _i16 r = GetRight(&(sprite->position), sprite->data->width);
  _i16 b = GetBottom(&(sprite->position), sprite->data->height);

  // left && right cases
  if (r < 0 || l >= displayWidth) {
    return false;
  }
  // top & bottom cases
  if (b < 0 || t >= displayHeight) {
    return false;
  }

  return true;
}

void SpriteUpdateState(Sprite_t* sprite) {
  if (sprite->animationSpeed == ANIMATION_SPEED_NONE) return;

  sprite->remainTicksToUpdateFrame--;
  sprite->frameChanged = false;

  if (sprite->remainTicksToUpdateFrame < 0) {
    sprite->remainTicksToUpdateFrame = sprite->animationSpeed;
    sprite->frameChanged = true;
    sprite->frameIndex++;
    if (sprite->frameIndex >= sprite->data->frames) {
      sprite->frameIndex = 0;
    }
  }
}

const bool SpriteIsFrameChanged(const Sprite_t* sprite) {
  return sprite->frameChanged;
}

void SpriteSetAnimationSpeed(Sprite_t* sprite, const AnimationSpeed_t speed) {
  sprite->animationSpeed = speed;
}