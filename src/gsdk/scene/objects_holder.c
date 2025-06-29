#include "objects_holder.h"

#include <stdlib.h>

#include "../collections/array.h"
#include "../scene/game_object.h"
#include "../sprite/sprite.h"
// #include "sprites_holder.h"

// max objects count is 32
#define FAR_OBJECTS_MAX_COUNT 12
#define MID_OBJECTS_MAX_COUNT 20

typedef struct ObjectsHolder_t {
  Array_t* farObjects;
  Array_t* midObjects;
} ObjectsHolder_t;

inline void DestroyGameObject(void* gameObject);

Array_t* SelectObjectsContainer(const ObjectsHolder_t* holder,
                                LayerType_t layer);

ObjectsHolder_t* ObjectsHolderCreate() {
  ObjectsHolder_t* holder = (ObjectsHolder_t*)malloc(sizeof(ObjectsHolder_t));

  if (holder == NULL) return NULL;

  holder->farObjects = ArrayCreate(FAR_OBJECTS_MAX_COUNT);
  holder->midObjects = ArrayCreate(MID_OBJECTS_MAX_COUNT);

  return holder;
}

void ObjectsHolderDestroy(ObjectsHolder_t* holder) {
  if (holder == NULL) return;

  ArrayForeach(holder->farObjects, DestroyGameObject);
  ArrayForeach(holder->midObjects, DestroyGameObject);

  ArrayDestroy(holder->farObjects);
  ArrayDestroy(holder->midObjects);

  free(holder);
}

ObjectId ObjectsHolderAdd(ObjectsHolder_t* holder, SpriteId spriteId,
                          LayerType_t layer, bool collidable, bool obstacle,
                          bool gravitable) {
  GameObject_t* object =
      GameObjectCreate(spriteId, collidable, obstacle, gravitable);
  Array_t* container = SelectObjectsContainer(holder, layer);

  if (ArrayAdd(container, object) == true) {
    return (ObjectId)object;
  }
  return OBJECT_ID_NA;
}

ObjectId ObjectsHolderGetObject(const ObjectsHolder_t* holder,
                                const LayerType_t layer, const Point_t* point,
                                const ObjectId excepted) {
  Array_t* container = SelectObjectsContainer(holder, layer);

  for (_u16 index = 0; index < ArraySize(container); index++) {
    GameObject_t* object = ArrayValueAt(container, index);
    if (object == (GameObject_t*)excepted) {
      continue;
    }
    Sprite_t* sprite = (Sprite_t*)GameObjectGetSpriteId(object);
    if (SpriteContainsPoint(sprite, point->x, point->y)) {
      return (ObjectId)object;
    }
  }
  return OBJECT_ID_NA;
}

Array_t* SelectObjectsContainer(const ObjectsHolder_t* holder,
                                LayerType_t layer) {
  if (layer == LAYER_MID) {
    return holder->midObjects;
  } else {
    return holder->farObjects;
  }
}

void DestroyGameObject(void* gameObject) {
  GameObjectDestroy((GameObject_t*)gameObject);
}