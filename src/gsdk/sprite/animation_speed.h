#ifndef GAME_SDK_ANIMATION_SPEED_H
#define GAME_SDK_ANIMATION_SPEED_H

/**
 * @brief Animation speed in milliseconds. Each value represents time in millis
 * per frame
 */
typedef enum {
  ANIMATION_SPEED_SUPER_SLOW = 2000,
  ANIMATION_SPEED_SLOW = 1500,
  ANIMATION_SPEED_NORMAL = 1000,
  ANIMATION_SPEED_FAST = 500,
  ANIMATION_SPEED_SUPER_FAST = 250,
  ANIMATION_SPEED_NONE = 0
} AnimationSpeed;

#endif  // GAME_SDK_ANIMATION_SPEED_H