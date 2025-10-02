#ifndef GAME_SDK_GAME_OBJECT_H
#define GAME_SDK_GAME_OBJECT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "../types/point.h"
#include "../types/types.h"

typedef struct GameObject GameObject;

GameObject* GameObject_Create(SpriteId spriteId, const bool collidable,
                              const bool obstacle, const bool gravitable);
void GameObject_Destroy(GameObject* object);

SpriteId GameObject_GetSpriteId(const GameObject* object);

bool GameObject_IsCollidable(const GameObject* object);
bool GameObject_IsObstacle(const GameObject* object);

const Point* GameObject_GetNextPositionOfCorner1(const GameObject* object);
const Point* GameObject_GetNextPositionOfCorner2(const GameObject* object);

void GameObject_SetNextPositionForCorner1(GameObject* object, const _i16 x,
                                          const _i16 y);
void GameObject_SetNextPositionForCorner2(GameObject* object, const _i16 x,
                                          const _i16 y);

void GameObject_ResetNextPositionOfCorner1(GameObject* object);
void GameObject_ResetNextPositionOfCorner2(GameObject* object);

#ifdef __cplusplus
}
#endif

#endif  // GAME_SDK_GAME_OBJECT_H