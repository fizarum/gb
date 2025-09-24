#include "collision_checker.h"

#include "../game_object/game_object.h"
#include "../sprite/sprite.h"
#include "collision_calculator.h"

static Point __nextPoint1ForCollision, __nextPoint2ForCollision;

ObjectId CollisionChecker_GetObstacle(const ObjectsHolder* holder,
                                      const ObjectId id, const _i8 x,
                                      const _i8 y) {
  GameObject* object = (GameObject*)id;

  // if object to move isn't collidable, allow movement
  if (GameObjectIsCollidable(object) == false) {
    return OBJECT_ID_NA;
  }

  Sprite* sprite = (Sprite*)GameObjectGetSpriteId(object);

  const LayerType layer = Sprite_GetLayer(sprite);
  // no layer type defined for sprite (probably invisible) - allow movement
  if (layer == LAYER_NONE) return OBJECT_ID_NA;

  const Point* position = Sprite_GetPosition(sprite);
  const _u8 width = Sprite_GetWidth(sprite);
  const _u8 height = Sprite_GetHeight(sprite);

  CalculateNextPosition(position, width, height, x, y,
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

  GameObject* anotherObject = (GameObject*)anotherObjectId;
  if (GameObjectIsCollidable(anotherObject) == true) {
    // if anotherObject is obstacle - stop movement
    if (GameObjectIsObstacle(anotherObject) == true) {
      return (ObjectId)anotherObject;
    }
  }
  return OBJECT_ID_NA;
}