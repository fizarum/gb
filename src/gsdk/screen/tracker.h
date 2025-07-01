#ifndef GAME_SDK_TRACKER2_H
#define GAME_SDK_TRACKER2_H

#ifdef __cplusplus
extern "C" {
#endif

#include <types.h>

#include "../screen/screen_config.h"

#define CELL_SIZE 8
#define CELL_MAX_ADDR 7

typedef void (*TrackerCallback)(uint16_t cellIndex, uint8_t cellData);

typedef struct Tracker_t Tracker_t;

Tracker_t* TrackerCreate(ScreenConfig* config);
void TrackerDestroy(Tracker_t* tracker);

/**
 * @brief Clear durty regions of tracker. This resets tracker to initial state
 *
 * @param tracker to reset
 */
void TrackerClear(Tracker_t* tracker);

/**
 * @brief Sets region as durty - which will be redrawn on next draw session
 */
void TrackerSetRegion(Tracker_t* tracker, const uint16_t l, const uint16_t t,
                      const uint16_t r, const uint16_t b);

/**
 * @brief Obtain all durty regions line by line. For each durty line callback is
 * triggered
 *
 * @param tracker
 * @param callback - Triggered callback with durty line and its index
 */
void TrackerGetRegion(const Tracker_t* tracker, TrackerCallback callback);

/**
 * @brief Sets entire screen as durty - which will be redrawn on next draw
 * session
 */
void TrackerSetAll(Tracker_t* tracker);

#ifdef __cplusplus
}
#endif

#endif  // GAME_SDK_TRACKER2_H