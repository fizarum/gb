#include "scene.h"

#include <stack.h>
#include <stdio.h>
#include <stdlib.h>

#include "../physics/collision_checker.h"
#include "../sprite/sprite.h"
#include "../types/region.h"
#include "../utils/screen_utils.h"
#include "../utils/utils.h"
#include "game_object.h"
#include "objects_holder.h"
#include "sprites_holder.h"

typedef struct Scene_t {
  SpritesHolder_t* spritesHolder;
  ObjectsHolder_t* objectsHolder;
  TileMap_t* tilemap;

  // Indicator of changed layer. It helps to understand which layers
  // should be processed during pixel's color calculation
  LayerType_t layerChanged;
} Scene_t;

Scene_t* _scene = NULL;

// TODO: temp workaround to replace complex logic of Tracker
#define regionsCount 20
static Region_t* durtyRegions[regionsCount];
static bool entireScreenisDurty = false;

void UpdataAnimatedSprites(Scene_t* scene);

static inline Region_t* _findNextEmptyRegion() {
  for (_u8 index = 0; index < regionsCount; index++) {
    Region_t* reg = durtyRegions[index];
    if (Region_IsEmpty(reg)) {
      return reg;
    }
  }
  return NULL;
}

static inline Region_t* _findSameRegion(_u16 l, _u16 t, _u16 r, _u16 b) {
  for (_u8 index = 0; index < regionsCount; index++) {
    Region_t* reg = durtyRegions[index];
    if (Region_isSame(reg, l, t, r, b)) {
      return reg;
    }
  }
  return NULL;
}

static inline void _resetAllRegions() {
  for (_u8 index = 0; index < regionsCount; index++) {
    Region_Reset(durtyRegions[index]);
  }
  entireScreenisDurty = false;
}

void SceneSetDurtyRegion(Scene_t* scene, const Rectangle_t* region,
                         const LayerType_t layer);
static inline void SceneSetDurtySprite(Scene_t* scene, const Sprite_t* sprite);

Scene_t* Scene_Create(SpritesHolder_t* spritesHolder,
                      ObjectsHolder_t* objectsHolder,
                      LayerType_t* layerChanged) {
  Scene_t* scene = (Scene_t*)malloc(sizeof(Scene_t));

  if (scene == NULL) return NULL;

  entireScreenisDurty = true;

  scene->spritesHolder = spritesHolder;
  scene->objectsHolder = objectsHolder;
  scene->layerChanged = *layerChanged;

  // TODO: remove this!
  _scene = scene;

  for (_u16 index = 0; index < regionsCount; index++) {
    Region_t* reg = (Region_t*)malloc(sizeof(Region_t));
    reg->l = 0;
    reg->t = 0;
    reg->r = 0;
    reg->b = 0;
    durtyRegions[index] = reg;
  }

  return scene;
}

void Scene_Destroy(Scene_t* scene) { free(scene); }

void Scene_Update(Scene_t* scene) { UpdataAnimatedSprites(scene); }

void Scene_Pause(Scene_t* scene) {
  _resetAllRegions();
  entireScreenisDurty = true;
  scene->layerChanged = LAYER_UI;
}

void Scene_Resume(Scene_t* scene) {
  _resetAllRegions();
  entireScreenisDurty = true;
  scene->layerChanged = LAYER_UI;
}

void Scene_CleanupRegions(Scene_t* scene, OnRegionRedrawRequested callback) {
  if (scene->layerChanged == LAYER_NONE) {
    return;
  }

  // redraw entire screen
  if (entireScreenisDurty) {
    callback(0, 0, ScreenConfig_GetRealWidth() - 1,
             ScreenConfig_GetRealHeight() - 1);
    entireScreenisDurty = false;
    scene->layerChanged = LAYER_NONE;
    return;
  }

  printf("requested cleanup regions\n");

  for (_u8 index = 0; index < regionsCount; index++) {
    Region_t* reg = durtyRegions[index];
    if (Region_IsEmpty(reg) == false) {
      callback(reg->l, reg->t, reg->r, reg->b);
      reg->l = 0;
      reg->t = 0;
      reg->r = 0;
      reg->b = 0;
    }
  }
  entireScreenisDurty = false;
  scene->layerChanged = LAYER_NONE;
}

TileMap_t* Scene_SetupTileMap(Scene_t* scene, SpriteId* tiles, const _u16 count,
                              const _u8 width) {
  if (tiles == NULL || count == 0 || width == 0) return NULL;

  // 1. obtain tile size by taking it value from first sprite
  Sprite_t* sprite = (Sprite_t*)tiles[0];
  LayerType_t layer = Sprite_GetLayer(sprite);

  _u8 tileSize = Sprite_GetWidth(sprite);

  TileMap_t* tilemap = TileMapCreate(tileSize);

  // 2. fill data into tilemap
  TileMapSet(tilemap, tiles, count, width);

  // 3. and request to refresh
  Rectangle_t* mapBounds = (Rectangle_t*)TileMapGetBounds(tilemap);
  SceneSetDurtyRegion(scene, mapBounds, layer);

  scene->tilemap = tilemap;

  return tilemap;
}

SpriteId Scene_AddSprite(Scene_t* scene, const SpriteData_t* data,
                         const LayerType_t layer) {
  SpriteId sid = SpritesHolder_AddSprite(scene->spritesHolder, data, layer);
  if (sid != OBJECT_ID_NA) {
    Sprite_t* sprite = (Sprite_t*)sid;

    SceneSetDurtySprite(scene, sprite);

    if (layer > scene->layerChanged) {
      scene->layerChanged = layer;
    }
  }

  return sid;
}

ObjectId Scene_AddGameObject(Scene_t* scene, const SpriteData_t* data,
                             const LayerType_t layer, const bool collidable,
                             const bool obstacle, const bool gravitable) {
  SpriteId sid = Scene_AddSprite(scene, data, layer);

  return ObjectsHolder_Add(scene->objectsHolder, sid, layer, collidable,
                           obstacle, gravitable);
}

void Scene_MoveSpriteBy(Scene_t* scene, SpriteId id, _i8 x, _i8 y) {
  Sprite_t* sprite = (Sprite_t*)id;

  // set old region as durty
  SceneSetDurtySprite(scene, sprite);
  Sprite_MoveBy(sprite, x, y);
  // as well as new
  SceneSetDurtySprite(scene, sprite);
}

void Scene_MoveSpriteTo(Scene_t* scene, SpriteId id, _u16 x, _u16 y) {
  Sprite_t* sprite = (Sprite_t*)id;

  // set old region as durty
  SceneSetDurtySprite(scene, sprite);
  Sprite_MoveTo(sprite, x, y);
  // as well as new
  SceneSetDurtySprite(scene, sprite);
}

Point_t __nextPoint1ForCollision, __nextPoint2ForCollision;

void Scene_MoveGameObjectBy(Scene_t* scene, ObjectId id, _i8 x, _i8 y) {
  GameObject_t* object = (GameObject_t*)id;
  SpriteId sid = GameObjectGetSpriteId(object);

  ObjectId obstacleId =
      CollisionCheckerGetObstacleId(scene->objectsHolder, id, x, y);
  if (obstacleId == OBJECT_ID_NA) {
    Scene_MoveSpriteBy(scene, sid, x, y);
  } else {
    printf("hit registered with object: %ju\n", obstacleId);
  }
}

void Scene_MoveGameObjectTo(Scene_t* scene, ObjectId id, _u16 x, _u16 y) {
  SpriteId sid = GameObjectGetSpriteId((GameObject_t*)id);
  Scene_MoveSpriteTo(scene, sid, x, y);
}

void Scene_ChangeSpriteAnimationSpeed(SpriteId sid,
                                      const AnimationSpeed_t speed) {
  Sprite_t* sprite = (Sprite_t*)sid;
  Sprite_SetAnimationSpeed(sprite, speed);
}

//------------------------------------------------------- private functions
void SceneSetDurtyRegion(Scene_t* scene, const Rectangle_t* region,
                         const LayerType_t layer) {
  if (entireScreenisDurty) {
    return;
  }
  _u16 left = Rectangle_GetVisibleLeft(region);
  _u16 top = Rectangle_GetVisibleTop(region);
  _u16 right = Rectangle_GetVisibleRight(region, ScreenConfig_GetRealWidth());
  _u16 bottom =
      Rectangle_GetVisibleBottom(region, ScreenConfig_GetRealHeight());

  Region_t* reg = _findNextEmptyRegion();
  if (reg != NULL) {
    reg->l = left;
    reg->t = top;
    reg->r = right;
    reg->b = bottom;
  }

  if (layer > scene->layerChanged) {
    scene->layerChanged = layer;
  }
}

static inline void SceneSetDurtySprite(Scene_t* scene, const Sprite_t* sprite) {
  if (entireScreenisDurty) {
    return;
  }

  if (Sprite_IsOnDisplay(sprite, ScreenConfig_GetRealWidth(),
                         ScreenConfig_GetRealHeight()) == false) {
    return;
  }

  const Point_t* position = Sprite_GetPosition(sprite);
  const LayerType_t layer = Sprite_GetLayer(sprite);

  const _u8 width = Sprite_GetWidth(sprite);
  const _u8 height = Sprite_GetHeight(sprite);

  _u16 l = normalize(position->x, ScreenConfig_GetRealWidth());
  _u16 t = normalize(position->y, ScreenConfig_GetRealHeight());
  _u16 r = normalize(GetRight(position, width), ScreenConfig_GetRealWidth());
  _u16 b = normalize(GetBottom(position, height), ScreenConfig_GetRealHeight());

  // TODO: investigate why we still have duplicates here
  //  preventing adding duplicated
  Region_t* reg = _findSameRegion(l, t, r, b);
  if (reg != NULL) {
    return;
  }

  reg = _findNextEmptyRegion();
  if (reg != NULL) {
    reg->l = l;
    reg->t = t;
    reg->r = r;
    reg->b = b;
  }
  if (layer > scene->layerChanged) {
    scene->layerChanged = layer;
  }
}

static void UpdateAnimationStateCallback(SpriteId sid) {
  Sprite_t* sprite = (Sprite_t*)sid;

  Sprite_UpdateState(sprite);
  bool isNewFrame = Sprite_IsFrameChanged(sprite);
  if (isNewFrame == true) {
    SceneSetDurtySprite(_scene, sprite);
  }
}

void UpdataAnimatedSprites(Scene_t* scene) {
  SpritesHolder_ForeachSprite(scene->spritesHolder, LAYER_FAR,
                              UpdateAnimationStateCallback);
  SpritesHolder_ForeachSprite(scene->spritesHolder, LAYER_MID,
                              UpdateAnimationStateCallback);
  SpritesHolder_ForeachSprite(scene->spritesHolder, LAYER_NEAR,
                              UpdateAnimationStateCallback);
}