#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <types.h>

#include "driver/gpio.h"

typedef enum PowerMode {
  DeepSleep,
  LightSleep,
  Nap,
  SavePower,
  Balanced,
  Normal,
} PowerMode;

typedef enum ModeChangedBy {
  ByButton,
  ByTimer,
  BySystem,
} ModeChangedBy;

typedef void(onPowerChangeCallback)(PowerMode, ModeChangedBy);

void PowerManager_Init(gpio_num_t wakeupPin, const _u32 sleepInMillis,
                       onPowerChangeCallback callback);
void PowerManager_Update();
void PowerManager_SetSleepTimer(const _u32 sleepInMillis);
void PowerManager_ResetSleepTimer();
void PowerManager_SetPowerMode(const PowerMode mode,
                               const ModeChangedBy changedBy);
PowerMode PowerManager_GetPowerMode();
bool PowerManager_IsSleeping();
bool PowerManager_IsAwake();
void PowerManager_ResetPowerMode(const ModeChangedBy changedBy);

/**
 * @brief Used to lock power mode in "Normal mode" and prevents device sleeping
 * Used some user apps where device can't sleep (games for example)
 */
void PowerManager_AcquirePowerLock();

/**
 * @brief Release previously acquired power lock
 */
void PowerManager_ReleasePowerLock();

#ifdef __cplusplus
}
#endif