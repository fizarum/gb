#include "game_object.h"

#include <stdlib.h>

typedef struct GameObject_t {
  SpriteId spriteId;
  /**
   * @brief Indicates if collision detection should be triggered for this object
   */
  bool collidable;

  /**
   * @brief if set to true another object can't move through this one
   */
  bool obstacle;

  /**
   * @brief if set to true object will be added to gravity force calculations
   */
  bool gravitable;

  /**
   * @brief Precalculated position of object's corners during movement.
   * For movement by diagonal only one corner is updated, but when object moves
   * by horizontal/vertical axis 2 corners should be updated
   */
  Point_t nextPositionOfCorner1;
  Point_t nextPositionOfCorner2;

} GameObject_t;

GameObject_t* GameObjectCreate(SpriteId spriteId, const bool collidable,
                               const bool obstacle, const bool gravitable) {
  GameObject_t* object = (GameObject_t*)malloc(sizeof(GameObject_t));

  if (object == NULL) return NULL;

  object->spriteId = spriteId;
  object->collidable = collidable;
  object->obstacle = obstacle;
  object->gravitable = gravitable;
  return object;
}

void GameObjectDestroy(GameObject_t* object) { free(object); }

SpriteId GameObjectGetSpriteId(const GameObject_t* object) {
  return object->spriteId;
}

bool GameObjectIsCollidable(const GameObject_t* object) {
  return object->collidable;
}

bool GameObjectIsObstacle(const GameObject_t* object) {
  return object->obstacle;
}

const Point_t* GameObjectGetNextPositionOfCorner1(const GameObject_t* object) {
  return &(object->nextPositionOfCorner1);
}

const Point_t* GameObjectGetNextPositionOfCorner2(const GameObject_t* object) {
  return &(object->nextPositionOfCorner2);
}

void GameObjectSetNextPositionForCorner1(GameObject_t* object, const int16_t x,
                                         const int16_t y) {
  Point_t* point = &(object->nextPositionOfCorner1);
  point->x = x;
  point->y = y;
}

void GameObjectSetNextPositionForCorner2(GameObject_t* object, const int16_t x,
                                         const int16_t y) {
  Point_t* point = &(object->nextPositionOfCorner2);
  point->x = x;
  point->y = y;
}

void GameObjectResetNextPositionOfCorner1(GameObject_t* object) {
  Point_t* point = &(object->nextPositionOfCorner1);
  point->x = 0;
  point->y = 0;
}

void GameObjectResetNextPositionOfCorner2(GameObject_t* object) {
  Point_t* point = &(object->nextPositionOfCorner2);
  point->x = 0;
  point->y = 0;
}