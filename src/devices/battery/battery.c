#include "battery.h"

#include <specifications/battery_data.h>
#include <stddef.h>

#include "battery_adc_utils.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"

#define CHG_MONITOR_PIN 4
/**
 * This part is commented because of some hardware changes.
 * There is a bug current pcb version (0.3) which will be fixed in next
 * iteration. In version 0.3 battery monitor isn't supported
 */
// #define BATTERY_PIN 34
// GPIO34 is attached to ADC1_CH6
// adc_unit_t unit = ADC_UNIT_1;
// adc_channel_t channel = ADC_CHANNEL_6;

// by voltage divider 2.2 mOhm & 4.7 mOhm
// and max battery voltage of 4.2V we can get 2.861V
// #define BATTERY_LEVEL_MAX 2861

// by voltage divider 2.2 mOhm & 4.7 mOhm
// and min battery voltage of 2.8V we can get 1.907V
// #define BATTERY_LEVEL_MIN 1907

const uint32_t delayBetweenUpdatesInMicroSeconds = 5000000;
static int lastUpdatedTimeAt, now = 0;
int voltage = 0;

static DeviceSpecification_t specs = {
    .name = "Battery",
    .type = TypePower,
};

static BatteryDeviceData_t deviceData;

bool isCalibrated = false;

static bool onInit(void) {
  // isCalibrated = BatteryADCInit(unit, channel);
  gpio_config_t ioConf = {
      .intr_type = GPIO_INTR_DISABLE,
      .mode = GPIO_MODE_INPUT,
      .pull_up_en = GPIO_PULLUP_ENABLE,
      .pin_bit_mask = CHG_MONITOR_PIN,
  };
  esp_err_t result = gpio_config(&ioConf);
  ESP_LOGI(specs.name, "on init: %d", result == ESP_OK);
  return result == ESP_OK;
}

static void onEnable(bool enable) {
  ESP_LOGI(specs.name, "enable: %d", enable);
}

static void onUpdate(void) {
  now = esp_timer_get_time();

  if (lastUpdatedTimeAt + delayBetweenUpdatesInMicroSeconds < now) {
    lastUpdatedTimeAt = now;
    deviceData.charging = gpio_get_level(CHG_MONITOR_PIN);
    // ESP_LOGI(specs.name, "charging: %d", deviceData.charging);

    // voltage = BatteryADCRead(unit, channel, isCalibrated);
    // if (voltage >= BATTERY_LEVEL_MAX) {
    //   batteryDeviceData.chargeLevelPercents = 100;
    // } else {
    //   batteryDeviceData.chargeLevelPercents =
    //       (_u8)((float)voltage / (float)BATTERY_LEVEL_MAX * 100);
    // }

    // ESP_LOGI(specs.name, "voltage %%: %d",
    //          batteryDeviceData.chargeLevelPercents);
  }
}

DeviceSpecification_t* BatterySpecification() {
  specs.data = &deviceData;

  specs.onInit = &onInit;
  specs.onEnable = &onEnable;
  specs.onUpdate = &onUpdate;
  return &specs;
}
