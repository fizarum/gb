#ifndef PWM_HAL_H
#define PWM_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "driver/ledc.h"

#define MAX_DUTY (8192)
#define DUTY_PER_PERCENT MAX_DUTY / 100

#define LEDC_MODE LEDC_LOW_SPEED_MODE
#define LEDC_CHANNEL LEDC_CHANNEL_0

// Frequency in Hertz. Set frequency at 4 kHz
#define LEDC_FREQUENCY (4000)

// Set duty resolution to 13 bits
#define LEDC_DUTY_RES LEDC_TIMER_13_BIT
#define LEDC_TIMER LEDC_TIMER_0

static ledc_timer_config_t ledcTimer = {
    .speed_mode = LEDC_MODE,
    .duty_resolution = LEDC_DUTY_RES,
    .timer_num = LEDC_TIMER,
    .freq_hz = LEDC_FREQUENCY,
    .clk_cfg = LEDC_AUTO_CLK,
};

static ledc_channel_config_t ledcChannel = {
    .speed_mode = LEDC_MODE,
    .channel = LEDC_CHANNEL,
    .timer_sel = LEDC_TIMER,
    .intr_type = LEDC_INTR_DISABLE,
    .gpio_num = -1,
    .duty = 0,
    .hpoint = 0,
};

/**
 * @brief Set pwm value in percents
 *
 * @param gpioPin gpio pin used for pwm
 * @param level [0..100] when set to 0 backlight if off
 */
bool PWM_SetValueInPercents(const int8_t gpioPin, uint8_t percents) {
  if (gpioPin < 0) {
    return false;
  }

  ledcChannel.gpio_num = gpioPin;

  if (percents > 100) {
    percents = 100;
  }

  uint16_t duty = MAX_DUTY;
  if (percents > 0) {
    duty -= (DUTY_PER_PERCENT * percents);
  }

  if (ledc_timer_config(&ledcTimer) != ESP_OK) {
    return false;
  }
  if (ledc_channel_config(&ledcChannel) != ESP_OK) {
    return false;
  }

  if (ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, duty) != ESP_OK) {
    return false;
  }

  if (ledc_update_duty(LEDC_MODE, LEDC_CHANNEL) != ESP_OK) {
    return false;
  }
  return true;
}

#ifdef __cplusplus
}
#endif

#endif  // PWM_HAL_H