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
static inline bool GPIO_Config(const int8_t pin, const gpio_mode_t mode) {
  if (pin < 0) {
    return false;
  }

  if (gpio_reset_pin(pin) != ESP_OK) {
    return false;
  }

  return gpio_set_direction(pin, mode) == ESP_OK;
}

static inline bool GPIO_Set(const int8_t pin, const uint8_t value) {
  return gpio_set_level(pin, value) == ESP_OK;
}

static inline uint8_t GPIO_Get(const int8_t pin) { return gpio_get_level(pin); }

static inline void GPIO_EnableInterrupt(const _i8 pin, gpio_isr_t isr_handler,
                                        void* args) {
  // Attach the ISR to the GPIO interrupt
  gpio_isr_handler_add(pin, isr_handler, NULL);
  // Enable the Interrupt
  gpio_intr_enable(pin);
}

static inline void GPIO_DisableInterrupt(const _i8 pin) {
  gpio_intr_disable(pin);
  gpio_isr_handler_remove(pin);
}

static inline void GPIO_SetInterrupt(const _i8 pin, gpio_int_type_t intr_type,
                                     gpio_isr_t isr_handler) {
  // Reset the pin
  gpio_reset_pin(pin);

  // Set the GPIOs to Output mode
  gpio_set_direction(pin, GPIO_MODE_INPUT);

  // Enable Pullup for Input Pin
  gpio_pullup_en(pin);

  // Disable pulldown for Input Pin
  gpio_pulldown_dis(pin);

  // Configure Raising Edge detection Interrupt for Input Pin
  gpio_set_intr_type(pin, intr_type);

  // install gpio isr service to default values
  gpio_install_isr_service(0);

  GPIO_EnableInterrupt(pin, isr_handler, NULL);
}

#ifdef __cplusplus
}
#endif

#endif  // GPIO_HAL_H