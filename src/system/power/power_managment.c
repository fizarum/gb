
#include "power_managment.h"

#include <device_manager.h>
#include <esp_check.h>

#include "driver/rtc_io.h"
#include "esp_log.h"
#include "esp_private/esp_clk.h"
#include "esp_sleep.h"
#include "esp_timer.h"
#include "power_managment_utils.h"
#include "soc/rtc.h"

static DeviceManager* _deviceManager = NULL;
static Device* _displayDevice = NULL;
static Device* _keyboardDevice = NULL;
static gpio_num_t _wakeupPin;
static volatile bool powerLockAcquired = false;

static void (*_callback)(PowerMode, ModeChangedBy) = NULL;

static int64_t lastUpdated = 0;
static _u32 sleepInMicros = 0;
static bool waitingForSleep = false;
static PowerMode powerMode;

void PowerManager_Init(gpio_num_t wakeupPin, const _u32 sleepInMillis,
                       onPowerChangeCallback callback) {
  _callback = callback;
  _deviceManager = DeviceManagerGetInstance();
  _displayDevice = DeviceManager_GetByType(TypeDisplay);
  _keyboardDevice = DeviceManager_GetByType(TypeInput);
  _wakeupPin = wakeupPin;

  ESP_LOGI(TAG, "scheduled sleep in %d millis", sleepInMillis);
  sleepInMicros = sleepInMillis * 1000;
  PowerManager_ResetSleepTimer();
}

void PowerManager_Update() {
  if (powerLockAcquired) {
    return;
  }

  if (waitingForSleep && lastUpdated + sleepInMicros <= esp_timer_get_time()) {
    ESP_LOGI(TAG, "its time to sleep");
    waitingForSleep = false;
    PowerManager_SetPowerMode(Nap, ByTimer);
  }
}

void PowerManager_SetSleepTimer(const _u32 sleepInMillis) {
  PowerManager_ResetSleepTimer();
  sleepInMicros = sleepInMillis * 1000;
  ESP_LOGI(TAG, "scheduled sleep in %d millis", sleepInMillis);
}

void PowerManager_ResetSleepTimer() {
  lastUpdated = esp_timer_get_time();
  waitingForSleep = true;
}

void enableExt1WakeupSource(gpio_num_t pin) {
  uint64_t mask = BIT64(pin);
  esp_err_t result =
      esp_sleep_enable_ext1_wakeup_io(mask, ESP_EXT1_WAKEUP_ANY_LOW);

  if (result != ESP_OK) {
    ESP_LOGI(TAG, "configure gpio as wakeup source failed");
  }

  // pin is tie to VCC in order to wake up in LOW
  rtc_gpio_pulldown_dis(pin);
  // Disable PULL_DOWN in order to allow it to wakeup on LOW
  rtc_gpio_pullup_en(pin);
}

void PowerManager_SetPowerMode(const PowerMode mode,
                               const ModeChangedBy changedBy) {
  powerMode = mode;
  enableExt1WakeupSource(_wakeupPin);
  switch (mode) {
    case Normal:
      ESP_LOGI(TAG, "set normal mode");
      if (_callback != NULL) {
        _callback(mode, changedBy);
      }
      setCPUFrequencyMhz(240);
      break;

    case Balanced:
      ESP_LOGI(TAG, "set balanced mode");
      if (_callback != NULL) {
        _callback(mode, changedBy);
      }
      setCPUFrequencyMhz(160);
      break;

    case SavePower:
      ESP_LOGI(TAG, "set save power mode");
      if (_callback != NULL) {
        _callback(mode, changedBy);
      }
      setCPUFrequencyMhz(80);
      break;

    case Nap:
      if (powerLockAcquired) {
        ESP_LOGW(TAG, "can't set Nap power mode, release power lock first!");
        return;
      }

      ESP_LOGI(TAG, "set nap power mode");
      if (_callback != NULL) {
        _callback(mode, changedBy);
      }
      waitingForSleep = false;
      setCPUFrequencyMhz(10);
      DeviceEnable(_displayDevice, false);
      DeviceEnable(_keyboardDevice, false);
      break;

    case LightSleep:
      if (powerLockAcquired) {
        ESP_LOGW(TAG,
                 "can't set LightSleep power mode, release power lock first!");
        return;
      }

      ESP_LOGI(TAG, "set light sleep mode");
      if (_callback != NULL) {
        _callback(mode, changedBy);
      }
      waitingForSleep = false;
      esp_light_sleep_start();
      break;

    case DeepSleep:
      if (powerLockAcquired) {
        ESP_LOGW(TAG,
                 "can't set DeepSleep power mode, release power lock first!");
        return;
      }

      ESP_LOGI(TAG, "set deep sleep mode");
      if (_callback != NULL) {
        _callback(mode, changedBy);
      }
      waitingForSleep = false;
      esp_deep_sleep_start();
      break;

    default:
      ESP_LOGE(TAG, "unknown power mode:%d, ignore command");
      break;
  }
}

PowerMode PowerManager_GetPowerMode() { return powerMode; }

void PowerManager_ResetPowerMode(const ModeChangedBy changedBy) {
  ESP_LOGI(TAG, "exit power save mode!");
  PowerManager_SetPowerMode(Normal, changedBy);
  DeviceEnable(_displayDevice, true);
  DeviceEnable(_keyboardDevice, true);
  PowerManager_ResetSleepTimer();
}

void PowerManager_AcquirePowerLock() {
  powerLockAcquired = true;
  ESP_LOGI(TAG, "power lock acquired");
}

void PowerManager_ReleasePowerLock() {
  powerLockAcquired = false;
  ESP_LOGI(TAG, "power lock released");
}