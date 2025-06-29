#include "collision_calculator.h"

#include "../utils/screen_utils.h"

/**
 * @brief Direction can have such values:
 *        1 2 3
 *        4 0 5
 *        6 7 8
 * where 0: no direction, 1: left-top, 8: right-bottom, etc.
 */
typedef uint8_t Direction;
#define DIRECTION_LT 1
#define DIRECTION_T 2
#define DIRECTION_RT 3
#define DIRECTION_L 4
#define DIRECTION_NONE 0
#define DIRECTION_R 5
#define DIRECTION_LB 6
#define DIRECTION_B 7
#define DIRECTION_RB 8

/**
 * @brief Get the Direction by x, y coords. All calculations are made
 * for starting point at (0,0). Check Direction definitions for details
 * @param x - offset value by x axis
 * @param y - offset value by y axis
 * @return direction value
 */
const static inline Direction getDirection(_i8 x, _i8 y) {
  if (x < 0 && y < 0) return DIRECTION_LT;
  if (x == 0 && y < 0) return DIRECTION_T;
  if (x > 0 && y < 0) return DIRECTION_RT;
  if (x < 0 && y == 0) return DIRECTION_L;
  if (x > 0 && y == 0) return DIRECTION_R;
  if (x < 0 && y > 0) return DIRECTION_LB;
  if (x == 0 && y > 0) return DIRECTION_B;
  if (x > 0 && y > 0) return DIRECTION_RB;
  return DIRECTION_NONE;
}

void CalculateNextPosition2(const Point_t* position, const _u8 width,
                            const _u8 height, const _i8 moveByX,
                            const _i8 moveByY, Point_t* corner1Pos,
                            Point_t* corner2Pos) {
  Direction direction = getDirection(moveByX, moveByY);

  switch (direction) {
    case DIRECTION_LT:
      corner1Pos->x = position->x + moveByX;
      corner1Pos->y = position->y + moveByY;

      corner2Pos->x = 0;
      corner2Pos->y = 0;
      break;

    case DIRECTION_T:
      corner1Pos->x = position->x;
      corner1Pos->y = position->y + moveByY;

      corner2Pos->x = GetRight(position, width);
      corner2Pos->y = corner1Pos->y;
      break;

    case DIRECTION_RT:
      corner1Pos->x = GetRight(position, width) + moveByX;
      corner1Pos->y = position->y + moveByY;

      corner2Pos->x = 0;
      corner2Pos->y = 0;
      break;

    case DIRECTION_L:
      corner1Pos->x = position->x + moveByX;
      corner1Pos->y = position->y;

      corner2Pos->x = corner1Pos->x;
      corner2Pos->y = GetBottom(position, height);
      break;

    case DIRECTION_R:
      corner1Pos->x = GetRight(position, width) + moveByX;
      corner1Pos->y = position->y;

      corner2Pos->x = corner1Pos->x;
      corner2Pos->y = GetBottom(position, height);
      break;

    case DIRECTION_LB:
      corner1Pos->x = position->x + moveByX;
      corner1Pos->y = GetBottom(position, height) + moveByY;

      corner2Pos->x = 0;
      corner2Pos->y = 0;
      break;

    case DIRECTION_B:
      corner1Pos->x = position->x;
      corner1Pos->y = GetBottom(position, height) + moveByY;

      corner2Pos->x = GetRight(position, width);
      corner2Pos->y = corner1Pos->y;
      break;

    case DIRECTION_RB:
      corner1Pos->x = GetRight(position, width) + moveByX;
      corner1Pos->y = GetBottom(position, height) + moveByY;

      corner2Pos->x = 0;
      corner2Pos->y = 0;
      break;

    default:
      corner1Pos->x = 0;
      corner1Pos->y = 0;

      corner2Pos->x = 0;
      corner2Pos->y = 0;
      break;
  }
}