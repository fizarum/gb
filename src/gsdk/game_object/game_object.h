#ifndef GAME_SDK_GAME_OBJECT_H
#define GAME_SDK_GAME_OBJECT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "../types/point.h"
#include "../types/types.h"

typedef struct GameObject_t GameObject_t;

GameObject_t *GameObjectCreate(SpriteId spriteId, const bool collidable,
                               const bool obstacle, const bool gravitable);
void GameObjectDestroy(GameObject_t *object);

SpriteId GameObjectGetSpriteId(const GameObject_t *object);

bool GameObjectIsCollidable(const GameObject_t *object);
bool GameObjectIsObstacle(const GameObject_t *object);

const Point_t *GameObjectGetNextPositionOfCorner1(const GameObject_t *object);
const Point_t *GameObjectGetNextPositionOfCorner2(const GameObject_t *object);

void GameObjectSetNextPositionForCorner1(GameObject_t *object, const int16_t x,
                                         const int16_t y);
void GameObjectSetNextPositionForCorner2(GameObject_t *object, const int16_t x,
                                         const int16_t y);

void GameObjectResetNextPositionOfCorner1(GameObject_t *object);
void GameObjectResetNextPositionOfCorner2(GameObject_t *object);

#ifdef __cplusplus
}
#endif

#endif  // GAME_SDK_GAME_OBJECT_H