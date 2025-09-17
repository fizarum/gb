#include "scene.h"

#include <stack.h>
#include <stdio.h>
#include <stdlib.h>

#include "../game_object/game_object.h"
#include "../physics/collision_checker.h"
#include "../sprite/sprite.h"
#include "../utils/screen_utils.h"
#include "../utils/utils.h"
#include "objects_holder.h"
#include "sprites_holder.h"

typedef struct Scene_t {
  SpritesHolder_t* spritesHolder;
  ObjectsHolder_t* objectsHolder;
  TileMap* tilemap;

  // Indicator of changed layer. It helps to understand which layers
  // should be processed during pixel's color calculation
  LayerType_t layerChanged;
} Scene_t;

Scene_t* _scene = NULL;

// TODO: temp workaround to replace complex logic of Tracker
#define regionsCount 10
static Rectangle_t* durtyRegions[regionsCount];
static bool entireScreenisDurty = false;

void updataAnimatedSprites(Scene_t* scene);
void setDurtyRegion(Scene_t* scene, const Rectangle_t* region,
                    const LayerType_t layer);
static inline void setDurtySprite(Scene_t* scene, const Sprite_t* sprite);
static inline Rectangle_t* findNextEmptyRegion();
static inline Rectangle_t* findSameRegion(_u16 l, _u16 t, _u16 r, _u16 b);
static inline void resetAllRegions();
static void updateAnimationStateCallback(SpriteId sid);

Scene_t* Scene_Create(SpritesHolder_t* spritesHolder,
                      ObjectsHolder_t* objectsHolder,
                      LayerType_t* layerChanged) {
  Scene_t* scene = (Scene_t*)malloc(sizeof(Scene_t));

  if (scene == NULL) return NULL;

  entireScreenisDurty = true;

  scene->spritesHolder = spritesHolder;
  scene->objectsHolder = objectsHolder;
  scene->layerChanged = *layerChanged;

  // TODO: rework this! we need to eliminate _scene reference
  _scene = scene;

  for (_u16 index = 0; index < regionsCount; index++) {
    Rectangle_t* reg = (Rectangle_t*)malloc(sizeof(Rectangle_t));
    Rectangle_Reset(reg);
    durtyRegions[index] = reg;
  }

  return scene;
}

void Scene_Destroy(Scene_t* scene) { free(scene); }

void Scene_Update(Scene_t* scene) { updataAnimatedSprites(scene); }

void Scene_Pause(Scene_t* scene) {
  resetAllRegions();
  entireScreenisDurty = true;
  scene->layerChanged = LAYER_UI;
}

void Scene_Resume(Scene_t* scene) {
  resetAllRegions();
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
    Rectangle_t* reg = durtyRegions[index];
    if (Rectangle_IsEmpty(reg) == false) {
      callback(reg->left, reg->top, reg->right, reg->bottom);
      Rectangle_Reset(reg);
    }
  }
  entireScreenisDurty = false;
  scene->layerChanged = LAYER_NONE;
}

TileMap* Scene_SetupTileMap(Scene_t* scene, SpriteId* tiles, const _u16 count,
                            const _u8 width) {
  if (tiles == NULL || count == 0 || width == 0) return NULL;

  // 1. obtain tile size by taking it value from first sprite
  Sprite_t* sprite = (Sprite_t*)tiles[0];
  LayerType_t layer = Sprite_GetLayer(sprite);

  _u8 tileSize = Sprite_GetWidth(sprite);

  TileMap* tilemap = TileMap_Create(tileSize);

  // 2. fill data into tilemap
  TileMap_Set(tilemap, tiles, count, width);

  // 3. and request to refresh
  Rectangle_t* mapBounds = (Rectangle_t*)TileMap_GetBounds(tilemap);
  setDurtyRegion(scene, mapBounds, layer);

  scene->tilemap = tilemap;

  return tilemap;
}

SpriteId Scene_AddSprite(Scene_t* scene, const SpriteData_t* data,
                         const LayerType_t layer) {
  SpriteId sid = SpritesHolder_AddSprite(scene->spritesHolder, data, layer);
  if (sid != OBJECT_ID_NA) {
    Sprite_t* sprite = (Sprite_t*)sid;

    setDurtySprite(scene, sprite);

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
  setDurtySprite(scene, sprite);
  Sprite_MoveBy(sprite, x, y);
  // as well as new
  setDurtySprite(scene, sprite);
}

void Scene_MoveSpriteTo(Scene_t* scene, SpriteId id, _u16 x, _u16 y) {
  Sprite_t* sprite = (Sprite_t*)id;

  // set old region as durty
  setDurtySprite(scene, sprite);
  Sprite_MoveTo(sprite, x, y);
  // as well as new
  setDurtySprite(scene, sprite);
}

void Scene_MoveGameObjectBy(Scene_t* scene, ObjectId id, _i8 x, _i8 y) {
  GameObject_t* object = (GameObject_t*)id;
  SpriteId sid = GameObjectGetSpriteId(object);

  ObjectId obstacleId =
      CollisionChecker_GetObstacle(scene->objectsHolder, id, x, y);
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
                                      const AnimationSpeed speed) {
  Sprite_t* sprite = (Sprite_t*)sid;
  Sprite_SetAnimationSpeed(sprite, speed);
}

//------------------------------------------------------- private functions
void setDurtyRegion(Scene_t* scene, const Rectangle_t* region,
                    const LayerType_t layer) {
  if (entireScreenisDurty) {
    return;
  }
  _u16 left = Rectangle_GetVisibleLeft(region);
  _u16 top = Rectangle_GetVisibleTop(region);
  _u16 right = Rectangle_GetVisibleRight(region, ScreenConfig_GetRealWidth());
  _u16 bottom =
      Rectangle_GetVisibleBottom(region, ScreenConfig_GetRealHeight());

  Rectangle_t* reg = findNextEmptyRegion();
  if (reg != NULL) {
    Rectangle_Set(reg, left, top, right, bottom);
  }

  if (layer > scene->layerChanged) {
    scene->layerChanged = layer;
  }
}

static inline void setDurtySprite(Scene_t* scene, const Sprite_t* sprite) {
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

  _u16 left = normalize(position->x, ScreenConfig_GetRealWidth());
  _u16 top = normalize(position->y, ScreenConfig_GetRealHeight());

  _u16 right = left + width - 1;
  right = normalize(right, ScreenConfig_GetRealHeight());

  _u16 bottom = top + height - 1;
  bottom = normalize(bottom, ScreenConfig_GetRealHeight());

  // TODO: investigate why we still have duplicates here
  //  preventing adding duplicated
  Rectangle_t* reg = findSameRegion(left, top, right, bottom);
  if (reg != NULL) {
    return;
  }

  reg = findNextEmptyRegion();
  if (reg != NULL) {
    Rectangle_Set(reg, left, top, right, bottom);
  }
  if (layer > scene->layerChanged) {
    scene->layerChanged = layer;
  }
}

static inline Rectangle_t* findNextEmptyRegion() {
  for (_u8 index = 0; index < regionsCount; index++) {
    Rectangle_t* reg = durtyRegions[index];
    if (Rectangle_IsEmpty(reg)) {
      return reg;
    }
  }
  return NULL;
}

static inline Rectangle_t* findSameRegion(_u16 l, _u16 t, _u16 r, _u16 b) {
  for (_u8 index = 0; index < regionsCount; index++) {
    Rectangle_t* reg = durtyRegions[index];
    if (Rectangle_isSame(reg, l, t, r, b)) {
      return reg;
    }
  }
  return NULL;
}

static inline void resetAllRegions() {
  for (_u8 index = 0; index < regionsCount; index++) {
    Rectangle_Reset(durtyRegions[index]);
  }
  entireScreenisDurty = false;
}

static void updateAnimationStateCallback(SpriteId sid) {
  Sprite_t* sprite = (Sprite_t*)sid;

  Sprite_UpdateState(sprite);
  if (Sprite_IsFrameChanged(sprite) == true) {
    setDurtySprite(_scene, sprite);
  }
}

void updataAnimatedSprites(Scene_t* scene) {
  SpritesHolder_ForeachSprite(scene->spritesHolder, LAYER_FAR,
                              updateAnimationStateCallback);
  SpritesHolder_ForeachSprite(scene->spritesHolder, LAYER_MID,
                              updateAnimationStateCallback);
  SpritesHolder_ForeachSprite(scene->spritesHolder, LAYER_NEAR,
                              updateAnimationStateCallback);
}