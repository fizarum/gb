#ifndef GAME_SDK_OBJECTS_HOLDER_H
#define GAME_SDK_OBJECTS_HOLDER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "../types/point.h"
#include "../types/types.h"
#include "layer_type.h"

typedef struct ObjectsHolder ObjectsHolder;

ObjectsHolder* ObjectsHolder_Create();
void ObjectsHolder_Destroy(ObjectsHolder* holder);

ObjectId ObjectsHolder_Add(ObjectsHolder* holder, SpriteId spriteId,
                           LayerType layer, bool collidable, bool obstacle,
                           bool gravitable);

ObjectId ObjectsHolder_GetObject(const ObjectsHolder* holder,
                                 const LayerType layer, const Point* point,
                                 const ObjectId excepted);
#ifdef __cplusplus
}
#endif
#endif  // GAME_SDK_OBJECTS_HOLDER_H