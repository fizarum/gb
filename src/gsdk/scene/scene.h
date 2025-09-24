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

typedef struct Scene Scene;

Scene* Scene_Create(SpritesHolder* spritesHolder, ObjectsHolder* objectsHolder,
                    LayerType* layerChanged);
void Scene_Destroy(Scene* scene);

void Scene_Update(Scene* scene);
void Scene_Pause(Scene* scene);
void Scene_Resume(Scene* scene);
void Scene_CleanupRegions(Scene* scene, OnRegionRedrawRequested callback);

TileMap* Scene_SetupTileMap(Scene* scene, SpriteId* tiles, const _u16 count,
                            const _u8 width);

SpriteId Scene_AddSprite(Scene* scene, const SpriteData* data,
                         const LayerType spriteType);

ObjectId Scene_AddGameObject(Scene* scene, const SpriteData* data,
                             const LayerType spriteType, const bool collidable,
                             const bool obstacle, const bool gravitable);

void Scene_MoveSpriteBy(Scene* scene, SpriteId id, _i8 x, _i8 y);
void Scene_MoveSpriteTo(Scene* scene, SpriteId id, _u16 x, _u16 y);

void Scene_MoveGameObjectBy(Scene* scene, ObjectId id, _i8 x, _i8 y);
void Scene_MoveGameObjectTo(Scene* scene, ObjectId id, _u16 x, _u16 y);

void Scene_ChangeSpriteAnimationSpeed(SpriteId id, const AnimationSpeed speed);
#endif  // GAME_SDK_SCENE_H