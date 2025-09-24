#include "sprites_holder.h"

#include <stdlib.h>

#include "../collections/array.h"

// max sprites count is 100
#define SPRITES_TILEMAP_MAX_COUNT 10
#define SPRITES_FAR_MAX_COUNT 25
#define SPRITES_MID_MAX_COUNT 30
#define SPRITES_NEAR_MAX_COUNT 20
#define SPRITES_UI_MAX_COUNT 15

typedef struct SpritesHolder {
  Array_t* tilemapSprites;
  Array_t* farSprites;
  Array_t* midSprites;
  Array_t* nearSprites;
  Array_t* uiSprites;
} SpritesHolder;

static inline void destroySprite(void* sprite) {
  Sprite_Destroy((Sprite*)sprite);
}

static inline Array_t* selectContainer(const SpritesHolder* holder,
                                       const LayerType layer);

SpritesHolder* SpritesHolder_Create() {
  SpritesHolder* holder = (SpritesHolder*)malloc(sizeof(SpritesHolder));

  if (holder == NULL) return NULL;

  holder->tilemapSprites = ArrayCreate(SPRITES_TILEMAP_MAX_COUNT);
  holder->farSprites = ArrayCreate(SPRITES_FAR_MAX_COUNT);
  holder->midSprites = ArrayCreate(SPRITES_MID_MAX_COUNT);
  holder->nearSprites = ArrayCreate(SPRITES_NEAR_MAX_COUNT);
  holder->uiSprites = ArrayCreate(SPRITES_UI_MAX_COUNT);
  return holder;
}

void SpritesHolder_Destroy(SpritesHolder* holder) {
  if (holder == NULL) return;

  ArrayForeach(holder->tilemapSprites, destroySprite);
  ArrayForeach(holder->farSprites, destroySprite);
  ArrayForeach(holder->midSprites, destroySprite);
  ArrayForeach(holder->nearSprites, destroySprite);
  ArrayForeach(holder->uiSprites, destroySprite);

  ArrayDestroy(holder->tilemapSprites);
  ArrayDestroy(holder->farSprites);
  ArrayDestroy(holder->midSprites);
  ArrayDestroy(holder->nearSprites);
  ArrayDestroy(holder->uiSprites);

  free(holder);
}

SpriteId SpritesHolder_AddSprite(SpritesHolder* holder, const SpriteData* data,
                                 const LayerType layer) {
  Sprite* sprite = Sprite_Create(data, layer);

  Array_t* sprites = selectContainer(holder, layer);
  if (sprites == NULL) return OBJECT_ID_NA;

  bool isSpriteAdded = ArrayAdd(sprites, sprite);

  if (isSpriteAdded == true) {
    return (SpriteId)sprite;
  }

  return OBJECT_ID_NA;
}

_ci SpritesHolder_GetColorIndex(const SpritesHolder* holder,
                                const LayerType layer, const _u16 x,
                                const _u16 y, _ci fallback) {
  Array_t* sprites = selectContainer(holder, layer);
  if (sprites == NULL || ArrayIsEmpty(sprites) == true) return fallback;

  Sprite* sprite = NULL;
  for (_u16 index = 0; index < ArraySize(sprites); index++) {
    sprite = (Sprite*)ArrayValueAt(sprites, index);
    if (Sprite_ContainsPoint(sprite, x, y) == true) {
      return Sprite_GetColorIndex(sprite, x, y, fallback);
    }
  }
  return fallback;
}

void SpritesHolder_ForeachSprite(const SpritesHolder* holder,
                                 const LayerType type,
                                 void (*foreach)(SpriteId spriteId)) {
  Array_t* sprites = selectContainer(holder, type);
  if (sprites == NULL) return;

  _u16 size = ArraySize(sprites);

  for (_u16 index = 0; index < size; index++) {
    SpriteId sid = (SpriteId)ArrayValueAt(sprites, index);
    foreach (sid);
  }
}

Array_t* selectContainer(const SpritesHolder* holder, const LayerType layer) {
  switch (layer) {
    case LAYER_TILEMAP:
      return holder->tilemapSprites;
    case LAYER_FAR:
      return holder->farSprites;
    case LAYER_MID:
      return holder->midSprites;
    case LAYER_NEAR:
      return holder->nearSprites;
    case LAYER_UI:
      return holder->uiSprites;
    default:
      return NULL;
  }
}
