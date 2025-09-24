#ifndef GAME_SDK_COLLISION_CALCULATOR_H
#define GAME_SDK_COLLISION_CALCULATOR_H

#include <types.h>

#include "../types/point.h"
#include "../types/rectangle.h"

/**
 * @brief calculates next position of point closest to any possible obstacle
 *
 * @param geometry - original object to move
 * @param moveByX - value on which original object moves by horizontal
 * @param moveByY - value on which original object moves by vertical
 * @param corner1Pos, corner2Pos - corners which are closest to obstacle. For
 *movement by diagonal only one corner will hit an obstacle, so corner2Pos is
 *reset then. for example, for right-top movement:
 *
 *             #############
 *             #  obstacle #
 *         >   #           #
 * ---------+  #############
 *  object  | ^
 *    to    |
 *   move   |
 * ---------+
 *
 * But, for movement by one side, we need to calculate 2 corners, which may hit
 *one or more obstacles. For example, right movement:
 *
 *             ###############
 *         >   #  obstacle 1 #
 * ---------+  ###############
 *  object  | ^
 *    to    |
 *   move   | ˇ
 * ---------+  ###############
 *			   #  obstacle 2 #
 *         >   ###############
 */
void CalculateNextPosition(const Point* position, const _u8 width,
                           const _u8 height, const _i8 moveByX,
                           const _i8 moveByY, Point* corner1Pos,
                           Point* corner2Pos);

#endif  // GAME_SDK_COLLISION_CALCULATOR_H