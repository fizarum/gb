#ifndef GPIO_HAL_H
#define GPIO_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <driver/gpio.h>
#include <hal/gpio_types.h>
#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Configure pin as input or output
 *
 * @param pin
 * @param state
 * @return true
 * @return false
 */
static bool GPIO_Config(const int8_t pin, const gpio_mode_t mode) {
  if (pin < 0) {
    return false;
  }

  if (gpio_reset_pin(pin) != ESP_OK) {
    return false;
  }

  return gpio_set_direction(pin, mode) == ESP_OK;
}

static bool GPIO_Set(const int8_t pin, const uint8_t value) {
  return gpio_set_level(pin, value) == ESP_OK;
}

#ifdef __cplusplus
}
#endif

#endif  // GPIO_HAL_H