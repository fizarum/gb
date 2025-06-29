#include "sprites_holder.h"

#include <stdlib.h>

#include "../collections/array.h"

// max sprites count is 100
#define SPRITES_TILEMAP_MAX_COUNT 10
#define SPRITES_FAR_MAX_COUNT 25
#define SPRITES_MID_MAX_COUNT 30
#define SPRITES_NEAR_MAX_COUNT 20
#define SPRITES_UI_MAX_COUNT 15

typedef struct SpritesHolder_t {
  Array_t* tilemapSprites;
  Array_t* farSprites;
  Array_t* midSprites;
  Array_t* nearSprites;
  Array_t* uiSprites;
} SpritesHolder_t;

inline void DestroySprite(void* sprite);

Array_t* SelectContainer(const SpritesHolder_t* holder,
                         const LayerType_t layer);

SpritesHolder_t* SpritesHolderCreate() {
  SpritesHolder_t* holder = (SpritesHolder_t*)malloc(sizeof(SpritesHolder_t));

  if (holder == NULL) return NULL;

  holder->tilemapSprites = ArrayCreate(SPRITES_TILEMAP_MAX_COUNT);
  holder->farSprites = ArrayCreate(SPRITES_FAR_MAX_COUNT);
  holder->midSprites = ArrayCreate(SPRITES_MID_MAX_COUNT);
  holder->nearSprites = ArrayCreate(SPRITES_NEAR_MAX_COUNT);
  holder->uiSprites = ArrayCreate(SPRITES_UI_MAX_COUNT);
  return holder;
}

void SpritesHolderDestroy(SpritesHolder_t* holder) {
  if (holder == NULL) return;

  ArrayForeach(holder->tilemapSprites, DestroySprite);
  ArrayForeach(holder->farSprites, DestroySprite);
  ArrayForeach(holder->midSprites, DestroySprite);
  ArrayForeach(holder->nearSprites, DestroySprite);
  ArrayForeach(holder->uiSprites, DestroySprite);

  ArrayDestroy(holder->tilemapSprites);
  ArrayDestroy(holder->farSprites);
  ArrayDestroy(holder->midSprites);
  ArrayDestroy(holder->nearSprites);
  ArrayDestroy(holder->uiSprites);

  free(holder);
}

SpriteId SpritesHolderAddSprite(SpritesHolder_t* holder,
                                const SpriteData_t* data,
                                const LayerType_t layer) {
  Sprite_t* sprite = Sprite_Create(data, layer);

  Array_t* sprites = SelectContainer(holder, layer);
  if (sprites == NULL) return OBJECT_ID_NA;

  bool isSpriteAdded = ArrayAdd(sprites, sprite);

  if (isSpriteAdded == true) {
    return (SpriteId)sprite;
  }

  return OBJECT_ID_NA;
}

_ci SpritesHolderGetColorIndex2(const SpritesHolder_t* holder,
                                const LayerType_t layer, const _u8 x,
                                const _u8 y, _ci fallback) {
  Array_t* sprites = SelectContainer(holder, layer);
  if (sprites == NULL || ArrayIsEmpty(sprites) == true) return fallback;

  Sprite_t* sprite = NULL;
  for (_u16 index = 0; index < ArraySize(sprites); index++) {
    sprite = (Sprite_t*)ArrayValueAt(sprites, index);
    if (SpriteContainsPoint(sprite, x, y) == true) {
      return SpriteGetColorIndex(sprite, x, y, fallback);
    }
  }
  return fallback;
}

void SpritesHolderForeachSprite(const SpritesHolder_t* holder,
                                const LayerType_t type,
                                void (*foreach)(SpriteId spriteId)) {
  Array_t* sprites = SelectContainer(holder, type);
  if (sprites == NULL) return;

  _u16 size = ArraySize(sprites);

  for (_u16 index = 0; index < size; index++) {
    SpriteId sid = (SpriteId)ArrayValueAt(sprites, index);
    foreach (sid);
  }
}

Array_t* SelectContainer(const SpritesHolder_t* holder,
                         const LayerType_t layer) {
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

void DestroySprite(void* sprite) { Sprite_Destroy((Sprite_t*)sprite); }
