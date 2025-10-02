#include "objects_holder.h"

#include <stdlib.h>

#include "../collections/array.h"
#include "../game_object/game_object.h"
#include "../sprite/sprite.h"

// max objects count is 32
#define FAR_OBJECTS_MAX_COUNT 12
#define MID_OBJECTS_MAX_COUNT 20

typedef struct ObjectsHolder {
  Array_t* farObjects;
  Array_t* midObjects;
} ObjectsHolder;

static inline void DestroyGameObject(void* gameObject);

static inline Array_t* SelectObjectsContainer(const ObjectsHolder* holder,
                                              LayerType layer) {
  if (layer == LAYER_MID) {
    return holder->midObjects;
  } else {
    return holder->farObjects;
  }
}

ObjectsHolder* ObjectsHolder_Create() {
  ObjectsHolder* holder = (ObjectsHolder*)malloc(sizeof(ObjectsHolder));

  if (holder == NULL) return NULL;

  holder->farObjects = ArrayCreate(FAR_OBJECTS_MAX_COUNT);
  holder->midObjects = ArrayCreate(MID_OBJECTS_MAX_COUNT);

  return holder;
}

void ObjectsHolder_Destroy(ObjectsHolder* holder) {
  if (holder == NULL) return;

  ArrayForeach(holder->farObjects, DestroyGameObject);
  ArrayForeach(holder->midObjects, DestroyGameObject);

  ArrayDestroy(holder->farObjects);
  ArrayDestroy(holder->midObjects);

  free(holder);
}

ObjectId ObjectsHolder_Add(ObjectsHolder* holder, SpriteId spriteId,
                           LayerType layer, bool collidable, bool obstacle,
                           bool gravitable) {
  GameObject* object =
      GameObject_Create(spriteId, collidable, obstacle, gravitable);
  Array_t* container = SelectObjectsContainer(holder, layer);

  if (ArrayAdd(container, object) == true) {
    return (ObjectId)object;
  }
  return OBJECT_ID_NA;
}

ObjectId ObjectsHolder_GetObject(const ObjectsHolder* holder,
                                 const LayerType layer, const Point* point,
                                 const ObjectId excepted) {
  Array_t* container = SelectObjectsContainer(holder, layer);

  for (_u16 index = 0; index < ArraySize(container); index++) {
    GameObject* object = ArrayValueAt(container, index);
    if (object == (GameObject*)excepted) {
      continue;
    }
    Sprite* sprite = (Sprite*)GameObject_GetSpriteId(object);
    if (Sprite_ContainsPoint(sprite, point->x, point->y)) {
      return (ObjectId)object;
    }
  }
  return OBJECT_ID_NA;
}

static inline void DestroyGameObject(void* gameObject) {
  GameObject_Destroy((GameObject*)gameObject);
}