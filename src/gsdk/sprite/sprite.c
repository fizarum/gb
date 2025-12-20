#include "sprite.h"

#include <stdbool.h>
#include <stdlib.h>

#include "../screen/palette.h"
#include "../types/point.h"
#include "../utils/screen_utils.h"
#include "esp_timer.h"

typedef struct Sprite {
  SpriteData* data;
  Point position;
  LayerType layer;

  // animation part
  AnimationSpeed animationSpeed;
  _u8 frameIndex;
  _i8 remainTicksToUpdateFrame;
  bool frameChanged;
} Sprite;

Sprite* Sprite_Create(const SpriteData* data, const LayerType layer) {
  Sprite* sprite = (Sprite*)malloc(sizeof(Sprite));

  if (sprite == NULL) return NULL;

  sprite->data = (SpriteData*)data;
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

void Sprite_Destroy(Sprite* sprite) { free(sprite); }

const _u8 Sprite_GetWidth(const Sprite* sprite) { return sprite->data->width; }

const _u8 Sprite_GetHeight(const Sprite* sprite) {
  return sprite->data->height;
}

void Sprite_GetBounds(const Sprite* sprite, Rectangle* bounds) {
  // TODO: recheck limits (Rectangle has _i16, Point - _i32)
  bounds->left = sprite->position.x;
  bounds->top = sprite->position.y;
  bounds->right = bounds->left + sprite->data->width;
  bounds->bottom = bounds->top + sprite->data->height;
}

const Point* Sprite_GetPosition(const Sprite* sprite) {
  return &(sprite->position);
}

const _ci Sprite_GetColorIndex(const Sprite* sprite, const _u16 x, const _u16 y,
                               const _ci fallback) {
  const Point* pos = &(sprite->position);

  bool inside =
      IsPointInside(x, y, pos->x, pos->y, pos->x + sprite->data->width - 1,
                    pos->y + sprite->data->height - 1);

  if (inside == false) {
    return fallback;
  }

  _i32 localX = x - pos->x;
  _i32 localY = y - pos->y;

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

const bool Sprite_ContainsPoint(const Sprite* sprite, const _u16 x,
                                const _u16 y) {
  return IsPointInside(x, y, sprite->position.x, sprite->position.y,
                       sprite->position.x + sprite->data->width - 1,
                       sprite->position.y + sprite->data->height - 1);
}

const LayerType Sprite_GetLayer(const Sprite* sprite) { return sprite->layer; }

void Sprite_MoveTo(Sprite* sprite, const _u16 x, const _u16 y) {
  sprite->position.x = x;
  sprite->position.y = y;
}

void Sprite_MoveBy(Sprite* sprite, const _i8 x, const _i8 y) {
  sprite->position.x += x;
  sprite->position.y += y;
}

bool Sprite_IsOnDisplay(const Sprite* sprite, _u16 displayWidth,
                        _u16 displayHeight) {
  _i32 l = sprite->position.x;
  _i32 t = sprite->position.y;
  _i32 r = l + sprite->data->width - 1;
  _i32 b = t + sprite->data->height - 1;

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
int64_t animationUpdatedAt = 0;
int64_t now = 0;

void Sprite_UpdateState(Sprite* sprite) {
  if (sprite->animationSpeed == ANIMATION_SPEED_NONE) return;

  now = esp_timer_get_time() / 1000;
  if (animationUpdatedAt + sprite->animationSpeed < now) {
    sprite->frameChanged = true;
    sprite->frameIndex++;
    if (sprite->frameIndex >= sprite->data->frames) {
      sprite->frameIndex = 0;
    }
    animationUpdatedAt = now;
  }
}

bool Sprite_IsFrameChanged(const Sprite* sprite) {
  return sprite->frameChanged;
}

void Sprite_SetFrameRedrawn(Sprite* sprite) { sprite->frameChanged = false; }

void Sprite_SetAnimationSpeed(Sprite* sprite, const AnimationSpeed speed) {
  sprite->animationSpeed = speed;
}