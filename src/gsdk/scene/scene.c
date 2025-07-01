#include "scene.h"

#include <stdio.h>
#include <stdlib.h>

#include "../physics/collision_checker.h"
#include "../sprite/sprite.h"
#include "../utils/screen_utils.h"
#include "../utils/utils.h"
#include "game_object.h"
#include "objects_holder.h"
#include "sprites_holder.h"

typedef struct Scene_t {
  Tracker_t* tracker;

  SpritesHolder_t* spritesHolder;
  ObjectsHolder_t* objectsHolder;
  TileMap_t* tilemap;

  // Indicator of changed layer. It helps to understand which layers
  // should be processed during pixel's color calculation
  LayerType_t layerChanged;
  ScreenConfig* config;
} Scene_t;

Scene_t* _scene = NULL;

void SceneSetDurtyRegion(Scene_t* scene, const Rectangle_t* region,
                         const LayerType_t layer);
static inline void SceneSetDurtySprite(Scene_t* scene, const Sprite_t* sprite);

void UpdataAnimatedSprites();

Scene_t* SceneCreate(SpritesHolder_t* spritesHolder,
                     ObjectsHolder_t* objectsHolder, Tracker_t* tracker,
                     LayerType_t* layerChanged, ScreenConfig* config) {
  Scene_t* scene = (Scene_t*)malloc(sizeof(Scene_t));

  if (scene == NULL) return NULL;

  scene->tracker = tracker;
  scene->spritesHolder = spritesHolder;
  scene->objectsHolder = objectsHolder;
  scene->layerChanged = *layerChanged;

  scene->config = config;

  // TODO: remove this!
  _scene = scene;

  return scene;
}

void SceneDestroy(Scene_t* scene) { free(scene); }

void SceneUpdate(Scene_t* scene) { UpdataAnimatedSprites(); }

TileMap_t* SceneSetupTileMap(Scene_t* scene, SpriteId* tiles, const _u16 count,
                             const _u8 width) {
  if (tiles == NULL || count == 0 || width == 0) return NULL;

  // 1. obtain tile size by taking it value from first sprite
  Sprite_t* sprite = (Sprite_t*)tiles[0];
  LayerType_t layer = SpriteGetLayer(sprite);

  _u8 tileSize = SpriteGetWidth(sprite);

  TileMap_t* tilemap = TileMapCreate(tileSize);

  // 2. fill data into tilemap
  TileMapSet(tilemap, tiles, count, width);

  // 3. and request to refresh
  Rectangle_t* mapBounds = TileMapGetBounds(tilemap);
  SceneSetDurtyRegion(scene, mapBounds, layer);

  scene->tilemap = tilemap;

  return tilemap;
}

SpriteId SceneAddSprite2(Scene_t* scene, const SpriteData_t* data,
                         const LayerType_t layer) {
  SpriteId sid = SpritesHolderAddSprite(scene->spritesHolder, data, layer);
  if (sid != OBJECT_ID_NA) {
    Sprite_t* sprite = (Sprite_t*)sid;

    SceneSetDurtySprite(scene, sprite);

    if (layer > scene->layerChanged) {
      scene->layerChanged = layer;
    }
  }

  return sid;
}

ObjectId SceneAddGameObject(Scene_t* scene, const SpriteData_t* data,
                            const LayerType_t layer, const bool collidable,
                            const bool obstacle, const bool gravitable) {
  SpriteId sid = SceneAddSprite2(scene, data, layer);

  return ObjectsHolderAdd(scene->objectsHolder, sid, layer, collidable,
                          obstacle, gravitable);
}

void SceneMoveSprite2By(Scene_t* scene, SpriteId id, _i8 x, _i8 y) {
  Sprite_t* sprite = (Sprite_t*)id;

  // set old region as durty
  SceneSetDurtySprite(scene, sprite);
  SpriteMoveBy(sprite, x, y);
  // as well as new
  SceneSetDurtySprite(scene, sprite);
}

void SceneMoveSprite2To(Scene_t* scene, SpriteId id, _u8 x, _u8 y) {
  Sprite_t* sprite = (Sprite_t*)id;

  // set old region as durty
  SceneSetDurtySprite(scene, sprite);
  SpriteMoveTo(sprite, x, y);
  // as well as new
  SceneSetDurtySprite(scene, sprite);
}

Point_t __nextPoint1ForCollision, __nextPoint2ForCollision;

void SceneMoveGameObjectBy(Scene_t* scene, ObjectId id, _i8 x, _i8 y) {
  GameObject_t* object = (GameObject_t*)id;
  SpriteId sid = GameObjectGetSpriteId(object);

  ObjectId obstacleId =
      CollisionCheckerGetObstacleId(scene->objectsHolder, id, x, y);
  if (obstacleId == OBJECT_ID_NA) {
    SceneMoveSprite2By(scene, sid, x, y);
  } else {
    printf("hit registered with object: %ju\n", obstacleId);
  }
}

void SceneMoveGameObjectTo(Scene_t* scene, ObjectId id, _u8 x, _u8 y) {
  SpriteId sid = GameObjectGetSpriteId((GameObject_t*)id);
  SceneMoveSprite2To(scene, sid, x, y);
}

void SceneChangeSpriteAnimationSpeed(SpriteId sid,
                                     const AnimationSpeed_t speed) {
  Sprite_t* sprite = (Sprite_t*)sid;
  SpriteSetAnimationSpeed(sprite, speed);
}

//------------------------------------------------------- private functions
void SceneSetDurtyRegion(Scene_t* scene, const Rectangle_t* region,
                         const LayerType_t layer) {
  _u8 left = Rectangle_GetVisibleLeft(region);
  _u8 top = Rectangle_GetVisibleTop(region);
  _u8 right = Rectangle_GetVisibleRight(region, scene->config->width);
  _u8 bottom = Rectangle_GetVisibleBottom(region, scene->config->height);

  TrackerSetRegion(scene->tracker, left, top, right, bottom);

  if (layer > scene->layerChanged) {
    scene->layerChanged = layer;
  }
}

static inline void SceneSetDurtySprite(Scene_t* scene, const Sprite_t* sprite) {
  if (SpriteIsOnDisplay(sprite, scene->config->width, scene->config->height) ==
      false) {
    return;
  }

  const Point_t* position = SpriteGetPosition(sprite);
  const LayerType_t layer = SpriteGetLayer(sprite);

  const _u8 width = SpriteGetWidth(sprite);
  const _u8 height = SpriteGetHeight(sprite);

  _u16 l = normalize(position->x, scene->config->width);
  _u16 t = normalize(position->y, scene->config->height);
  _u16 r = normalize(GetRight(position, width), scene->config->width);
  _u16 b = normalize(GetBottom(position, height), scene->config->height);

  TrackerSetRegion(scene->tracker, l, t, r, b);

  if (layer > scene->layerChanged) {
    scene->layerChanged = layer;
  }
}

static void UpdateAnimationStateCallback(SpriteId sid) {
  Sprite_t* sprite = (Sprite_t*)sid;

  SpriteUpdateState(sprite);
  bool isNewFrame = SpriteIsFrameChanged(sprite);
  if (isNewFrame == true) {
    SceneSetDurtySprite(_scene, sprite);
  }
}

void UpdataAnimatedSprites() {
  SpritesHolderForeachSprite(_scene->spritesHolder, LAYER_FAR,
                             UpdateAnimationStateCallback);
  SpritesHolderForeachSprite(_scene->spritesHolder, LAYER_MID,
                             UpdateAnimationStateCallback);
  SpritesHolderForeachSprite(_scene->spritesHolder, LAYER_NEAR,
                             UpdateAnimationStateCallback);
}