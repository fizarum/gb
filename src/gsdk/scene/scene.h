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

Scene_t* Scene_Create(SpritesHolder_t* spritesHolder,
                      ObjectsHolder_t* objectsHolder,
                      LayerType_t* layerChanged);
void Scene_Destroy(Scene_t* scene);

void Scene_Update(Scene_t* scene);
void Scene_Pause(Scene_t* scene);
void Scene_Resume(Scene_t* scene);
void Scene_CleanupRegions(Scene_t* scene, OnRegionRedrawRequested callback);

TileMap* Scene_SetupTileMap(Scene_t* scene, SpriteId* tiles, const _u16 count,
                            const _u8 width);

SpriteId Scene_AddSprite(Scene_t* scene, const SpriteData_t* data,
                         const LayerType_t spriteType);

ObjectId Scene_AddGameObject(Scene_t* scene, const SpriteData_t* data,
                             const LayerType_t spriteType,
                             const bool collidable, const bool obstacle,
                             const bool gravitable);

void Scene_MoveSpriteBy(Scene_t* scene, SpriteId id, _i8 x, _i8 y);
void Scene_MoveSpriteTo(Scene_t* scene, SpriteId id, _u16 x, _u16 y);

void Scene_MoveGameObjectBy(Scene_t* scene, ObjectId id, _i8 x, _i8 y);
void Scene_MoveGameObjectTo(Scene_t* scene, ObjectId id, _u16 x, _u16 y);

void Scene_ChangeSpriteAnimationSpeed(SpriteId id, const AnimationSpeed speed);
#endif  // GAME_SDK_SCENE_H