#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "driver/gpio.h"

typedef enum PowerMode {
  Normal,
  Balanced,
  SavePower,
  Nap,
  LightSleep,
  DeepSleep,
} PowerMode;

void PowerManager_Init(gpio_num_t wakeupPin);
void PowerManager_SetPowerMode(PowerMode mode);
void PowerManager_ResetPowerMode();

#ifdef __cplusplus
}
#endif