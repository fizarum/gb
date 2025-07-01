#ifndef GAME_SDK_SCENE_H
#define GAME_SDK_SCENE_H

#include <stdint.h>

#include "../screen/palette.h"
#include "../screen/screen_config.h"
#include "../screen/tracker.h"
#include "../sprite/sprite_data.h"
#include "../types/types.h"
#include "objects_holder.h"
#include "sprites_holder.h"
#include "tilemap.h"

typedef void (*OnPixelUpdateCallback)(_u16 x, _u16 y, Color color);
typedef void (*OnPixelsUpdateCallback)(_u16 x, _u16 y, Color* colors,
                                       _u8 colorsCount);
typedef void (*OnPixelsBufferUpdateCallback)(_u32 start, Color* colors,
                                             _u8 colorsCount);

typedef struct Scene_t Scene_t;

Scene_t* SceneCreate(SpritesHolder_t* spritesHolder,
                     ObjectsHolder_t* objectsHolder, Tracker_t* tracker,
                     LayerType_t* layerChanged, ScreenConfig* config);
void SceneDestroy(Scene_t* scene);

void SceneUpdate(Scene_t* scene);

TileMap_t* SceneSetupTileMap(Scene_t* scene, SpriteId* tiles, const _u16 count,
                             const _u8 width);

SpriteId SceneAddSprite2(Scene_t* scene, const SpriteData_t* data,
                         const LayerType_t spriteType);

ObjectId SceneAddGameObject(Scene_t* scene, const SpriteData_t* data,
                            const LayerType_t spriteType, const bool collidable,
                            const bool obstacle, const bool gravitable);

void SceneMoveSprite2By(Scene_t* scene, SpriteId id, _i8 x, _i8 y);
void SceneMoveSprite2To(Scene_t* scene, SpriteId id, _u8 x, _u8 y);

void SceneMoveGameObjectBy(Scene_t* scene, ObjectId id, _i8 x, _i8 y);
void SceneMoveGameObjectTo(Scene_t* scene, ObjectId id, _u8 x, _u8 y);

void SceneChangeSpriteAnimationSpeed(SpriteId id, const AnimationSpeed_t speed);
#endif  // GAME_SDK_SCENE_H