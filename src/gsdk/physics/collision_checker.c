#include "collision_checker.h"

#include "../scene/game_object.h"
#include "../sprite/sprite.h"
#include "collision_calculator.h"

static Point_t __nextPoint1ForCollision, __nextPoint2ForCollision;

ObjectId CollisionCheckerGetObstacleId(const ObjectsHolder_t* holder,
                                       const ObjectId id, const _i8 x,
                                       const _i8 y) {
  GameObject_t* object = (GameObject_t*)id;

  // if object to move isn't collidable, allow movement
  if (GameObjectIsCollidable(object) == false) {
    return OBJECT_ID_NA;
  }

  Sprite_t* sprite = (Sprite_t*)GameObjectGetSpriteId(object);

  const LayerType_t layer = Sprite_GetLayer(sprite);
  // no layer type defined for sprite (probably invisible) - allow movement
  if (layer == LAYER_NONE) return OBJECT_ID_NA;

  const Point_t* position = Sprite_GetPosition(sprite);
  const _u8 width = Sprite_GetWidth(sprite);
  const _u8 height = Sprite_GetHeight(sprite);

  CalculateNextPosition2(position, width, height, x, y,
                         &__nextPoint1ForCollision, &__nextPoint2ForCollision);

  ObjectId anotherObjectId =
      ObjectsHolder_GetObject(holder, layer, &__nextPoint1ForCollision, id);
  if (anotherObjectId == OBJECT_ID_NA) {
    anotherObjectId =
        ObjectsHolder_GetObject(holder, layer, &__nextPoint2ForCollision, id);
  }

  // if no obstacles detected - allow movement
  if (anotherObjectId == OBJECT_ID_NA || anotherObjectId == id) {
    return OBJECT_ID_NA;
  }

  GameObject_t* anotherObject = (GameObject_t*)anotherObjectId;
  if (GameObjectIsCollidable(anotherObject) == true) {
    // if anotherObject is obstacle - stop movement
    if (GameObjectIsObstacle(anotherObject) == true) {
      return (ObjectId)anotherObject;
    }
  }
  return OBJECT_ID_NA;
}