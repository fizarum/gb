#ifndef GAME_SDK_OBJECTS_HOLDER_H
#define GAME_SDK_OBJECTS_HOLDER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "../types/point.h"
#include "../types/types.h"
#include "layer_type.h"

typedef struct ObjectsHolder_t ObjectsHolder_t;

ObjectsHolder_t* ObjectsHolderCreate();
void ObjectsHolderDestroy(ObjectsHolder_t* holder);

ObjectId ObjectsHolderAdd(ObjectsHolder_t* holder, SpriteId spriteId,
                          LayerType_t layer, bool collidable, bool obstacle,
                          bool gravitable);

ObjectId ObjectsHolderGetObject(const ObjectsHolder_t* holder,
                                const LayerType_t layer, const Point_t* point,
                                const ObjectId excepted);
#ifdef __cplusplus
}
#endif
#endif  // GAME_SDK_OBJECTS_HOLDER_H