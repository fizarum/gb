#include "battery.h"

#include <specifications/battery_data.h>
#include <stddef.h>

#include "battery_adc_utils.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"

#define CHG_MONITOR_PIN 21

#define BATTERY_PIN 4
// GPIO4 is controlled by ADC1_CH3, according to datasheet (page 50, table 3-3.
// Peripheral pin configurations)
adc_unit_t unit = ADC_UNIT_1;
adc_channel_t channel = ADC_CHANNEL_3;

/**
 * @brief by voltage divider 2.2 mOhm & 4.7 mOhm and max battery voltage of 4.2V
 * we can get 2.861V
 */
#define BATTERY_LEVEL_MAX 2861

/**
 * @brief by voltage divider 2.2 mOhm & 4.7 mOhm and min battery voltage of 2.8V
 * we can get 1.907V
 */
#define BATTERY_LEVEL_MIN 1907
// 10 seconds
const uint32_t delayBetweenUpdatesInMicroSeconds = 10 * 1000000;
static int lastUpdatedTimeAt, now = 0;
int voltage = 0;

static DeviceSpecification_t specs = {
    .name = "Battery",
    .type = TypePower,
};

static BatteryDeviceData_t deviceData;

bool isCalibrated = false;

static bool onInit(void) {
  isCalibrated = BatteryADCInit(unit, channel);

  gpio_config_t batteryLevelPinConf = {
      .intr_type = GPIO_INTR_DISABLE,
      .mode = GPIO_MODE_INPUT,
      .pull_up_en = GPIO_PULLUP_ENABLE,
      .pin_bit_mask = BATTERY_PIN,
  };

  bool chargingMonitorPinConfigured =
      gpio_set_direction(CHG_MONITOR_PIN, GPIO_MODE_INPUT) == ESP_OK;

  bool batteryPinConfigured = gpio_config(&batteryLevelPinConf) == ESP_OK;

  return batteryPinConfigured && chargingMonitorPinConfigured;
}

static bool onEnable(bool enable) {
  ESP_LOGI(specs.name, "enable: %d", enable);
  // actually battery cant be disabled, so always true
  return true;
}

static void onUpdate(void) {
  now = esp_timer_get_time();

  if (lastUpdatedTimeAt + delayBetweenUpdatesInMicroSeconds < now) {
    lastUpdatedTimeAt = now;
    deviceData.charging = gpio_get_level(CHG_MONITOR_PIN);
    ESP_LOGI(specs.name, "charging: %d", deviceData.charging);

    voltage = BatteryADCRead(unit, channel, isCalibrated);
    if (voltage >= BATTERY_LEVEL_MAX) {
      deviceData.chargeLevelPercents = 100;
    } else {
      deviceData.chargeLevelPercents =
          (_u8)((float)voltage / (float)BATTERY_LEVEL_MAX * 100);
    }

    ESP_LOGI(specs.name, "voltage %%: %d", deviceData.chargeLevelPercents);
  }
}

DeviceSpecification_t* BatterySpecification() {
  specs.data = &deviceData;

  specs.onInit = &onInit;
  specs.onEnable = &onEnable;
  specs.onUpdate = &onUpdate;
  return &specs;
}
