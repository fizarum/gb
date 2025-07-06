#ifndef GAME_SDK_SCENE_H
#define GAME_SDK_SCENE_H

#include <stdint.h>

#include "../screen/palette.h"
#include "../screen/screen_config.h"
#include "../sprite/sprite_data.h"
#include "../types/types.h"
#include "objects_holder.h"
#include "sprites_holder.h"
#include "tilemap.h"

typedef void (*OnRegionRedrawRequested)(_u16 l, _u16 t, _u16 r, _u16 b);

typedef struct Scene_t Scene_t;

Scene_t* SceneCreate(SpritesHolder_t* spritesHolder,
                     ObjectsHolder_t* objectsHolder, LayerType_t* layerChanged);
void SceneDestroy(Scene_t* scene);

void SceneUpdate(Scene_t* scene);
void Scene_Pause(Scene_t* scene);
void Scene_Resume(Scene_t* scene);
void Scene_CleanupRegions(Scene_t* scene, OnRegionRedrawRequested callback);

TileMap_t* SceneSetupTileMap(Scene_t* scene, SpriteId* tiles, const _u16 count,
                             const _u8 width);

SpriteId SceneAddSprite2(Scene_t* scene, const SpriteData_t* data,
                         const LayerType_t spriteType);

ObjectId SceneAddGameObject(Scene_t* scene, const SpriteData_t* data,
                            const LayerType_t spriteType, const bool collidable,
                            const bool obstacle, const bool gravitable);

void SceneMoveSprite2By(Scene_t* scene, SpriteId id, _i8 x, _i8 y);
void SceneMoveSprite2To(Scene_t* scene, SpriteId id, _u16 x, _u16 y);

void SceneMoveGameObjectBy(Scene_t* scene, ObjectId id, _i8 x, _i8 y);
void SceneMoveGameObjectTo(Scene_t* scene, ObjectId id, _u16 x, _u16 y);

void SceneChangeSpriteAnimationSpeed(SpriteId id, const AnimationSpeed_t speed);
#endif  // GAME_SDK_SCENE_H