#ifndef GAME_SDK_GAME_OBJECT_H
#define GAME_SDK_GAME_OBJECT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "../types/point.h"
#include "../types/types.h"

typedef struct GameObject GameObject;

GameObject *GameObjectCreate(SpriteId spriteId, const bool collidable,
                             const bool obstacle, const bool gravitable);
void GameObjectDestroy(GameObject *object);

SpriteId GameObjectGetSpriteId(const GameObject *object);

bool GameObjectIsCollidable(const GameObject *object);
bool GameObjectIsObstacle(const GameObject *object);

const Point *GameObjectGetNextPositionOfCorner1(const GameObject *object);
const Point *GameObjectGetNextPositionOfCorner2(const GameObject *object);

void GameObjectSetNextPositionForCorner1(GameObject *object, const int16_t x,
                                         const int16_t y);
void GameObjectSetNextPositionForCorner2(GameObject *object, const int16_t x,
                                         const int16_t y);

void GameObjectResetNextPositionOfCorner1(GameObject *object);
void GameObjectResetNextPositionOfCorner2(GameObject *object);

#ifdef __cplusplus
}
#endif

#endif  // GAME_SDK_GAME_OBJECT_H