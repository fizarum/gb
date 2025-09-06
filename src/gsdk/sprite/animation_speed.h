#ifndef GAME_SDK_ANIMATION_SPEED_H
#define GAME_SDK_ANIMATION_SPEED_H

/**
 * @brief Animation speed calculated from formula 1000 millis / 20 updates.
 * So every iteration (50 millis) is a minimal fraction of animation time
 * Thus, for super fast speed its 5, means 5 * 50 millis = 250 millis for one
 * frame or 4 fps
 * speed normal then is: 20 * 50 = 1000 millis or 1 fps, and so on...
 * This is still valid for faster screen updates (for example 30, fraction: 33
 * millis), since its close to first statement
 */
typedef enum {
  ANIMATION_SPEED_SUPER_SLOW = 40,
  ANIMATION_SPEED_SLOW = 30,
  ANIMATION_SPEED_NORMAL = 20,
  ANIMATION_SPEED_FAST = 10,
  ANIMATION_SPEED_SUPER_FAST = 5,
  ANIMATION_SPEED_NONE = 0
} AnimationSpeed_t;

#endif  // GAME_SDK_ANIMATION_SPEED_H