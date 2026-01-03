
#include "power_managment.h"

#include <device_manager.h>
#include <esp_check.h>
#include <types.h>

#include "driver/rtc_io.h"
#include "esp_log.h"
#include "esp_private/esp_clk.h"
#include "esp_sleep.h"
#include "power_managment_utils.h"
#include "soc/rtc.h"

static DeviceManager* _deviceManager = NULL;
static Device* _displayDevice = NULL;
static Device* _keyboardDevice = NULL;
static gpio_num_t _wakeupPin;

void PowerManager_Init(gpio_num_t wakeupPin) {
  _deviceManager = DeviceManagerGetInstance();
  _displayDevice = DeviceManager_GetByType(TypeDisplay);
  _keyboardDevice = DeviceManager_GetByType(TypeInput);
  _wakeupPin = wakeupPin;
}

void enableExt1WakeupSource(gpio_num_t pin) {
  uint64_t mask = BIT64(pin);
  esp_err_t result =
      esp_sleep_enable_ext1_wakeup_io(mask, ESP_EXT1_WAKEUP_ANY_LOW);

  if (result != ESP_OK) {
    printf("configure gpio as wakeup source failed\n");
  }

  // pin is tie to VCC in order to wake up in LOW
  rtc_gpio_pulldown_dis(pin);
  // Disable PULL_DOWN in order to allow it to wakeup on LOW
  rtc_gpio_pullup_en(pin);
}

void PowerManager_SetPowerMode(PowerMode mode) {
  enableExt1WakeupSource(_wakeupPin);
  switch (mode) {
    case Normal:
      ESP_LOGI(TAG, "set normal mode");
      setCPUFrequencyMhz(240);
      break;

    case Balanced:
      ESP_LOGI(TAG, "set balanced mode");
      setCPUFrequencyMhz(160);
      break;

    case SavePower:
      ESP_LOGI(TAG, "set save power mode");
      setCPUFrequencyMhz(80);
      break;

    case Nap:
      ESP_LOGI(TAG, "set nap power mode");
      setCPUFrequencyMhz(10);
      DeviceEnable(_displayDevice, false);
      DeviceEnable(_keyboardDevice, false);
      break;

    case LightSleep:
      ESP_LOGI(TAG, "set light sleep mode");
      esp_light_sleep_start();
      break;

    case DeepSleep:
      ESP_LOGI(TAG, "set deep sleep mode");
      esp_deep_sleep_start();
      break;

    default:
      ESP_LOGE(TAG, "unknown power mode:%d, ignore command");
      break;
  }
}

void PowerManager_ResetPowerMode() {
  ESP_LOGI(TAG, "<-- exit power save mode!");
  setCPUFrequencyMhz(240);
  DeviceEnable(_displayDevice, true);
  DeviceEnable(_keyboardDevice, true);
}