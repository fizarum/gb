#ifndef GAME_SDK_PHYSICS_WORLD_H
#define GAME_SDK_PHYSICS_WORLD_H

#include <stdbool.h>

#include "../scene/objects_holder.h"

ObjectId CollisionCheckerGetObstacleId(const ObjectsHolder_t *holder,
                                       const ObjectId id, const _i8 x,
                                       const _i8 y);

#endif  // GAME_SDK_PHYSICS_WORLD_H