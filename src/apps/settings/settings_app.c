#include "settings_app.h"

#include <device_manager.h>
#include <gfx/gfx.h>
#include <palette.h>
#include <specifications/battery_data.h>
#include <specifications/storage_data.h>

#include "../../devices/battery/battery.h"
#include "../../devices/storage/storage.h"
#include "../../devices/storage/storage_utils.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_log.h"
#include "esp_psram.h"
#include "soc/rtc.h"

static AppSpecification_t specs = {
    .name = "Settings",
};

DeviceSpecification_t* batterySpecification;
BatteryDeviceData_t* batteryData;

DeviceSpecification_t* storageSpecification;
StorageDeviceData_t* storageData;

esp_chip_info_t chipInfo;
rtc_cpu_freq_config_t conf;

char _settingsBuff[20];

const _u16 kbSize = 1024;

void GetChipModel(const esp_chip_model_t model);

static void handleKey(const void* keyData) {}
static void onAppLoading(void) {
  ESP_LOGI(specs.name, "on app loading...");

  batterySpecification = BatterySpecification();
  batteryData = batterySpecification->getData();

  storageSpecification = StorageSpecification();
  storageData = storageSpecification->getData();
}

static void onAppUpdate(void) {}
static void onAppPause(void) { ESP_LOGI(specs.name, "on app pause..."); }

static void onAppResume(void) {
  ESP_LOGI(specs.name, "on app resume...");
  GFXFillScreen(COLOR_DARKGREEN);
  GFXDrawString(specs.name, 30, 7);

  esp_chip_info(&chipInfo);
  rtc_clk_cpu_freq_get_config(&conf);

  uint32_t flashChipSize;
  esp_flash_get_size(NULL, &flashChipSize);
  uint16_t flashSizeInMb = flashChipSize / (1024 * 1024);

  _u16 allRam = esp_get_free_heap_size() / kbSize;

  _u32 sdSizeInMB = StorageGetTotalSizeInMBs(storageData->mountPoint);
  _u32 sdUsedSizeInMB = StorageGetUsedSizeInMBs(storageData->mountPoint);

  uint8_t yPos = 40;
  uint8_t firstColumnXPos = 30;
  uint8_t secondColumnXPos = 160;
  uint8_t itemHeight = 18;

  // model
  GFXDrawString("model:", firstColumnXPos, yPos);
  GetChipModel(chipInfo.model);
  GFXDrawString(_settingsBuff, secondColumnXPos, yPos);

  // rev
  yPos += itemHeight;
  GFXDrawString("revision:", firstColumnXPos, yPos);
  sprintf(_settingsBuff, "%u", chipInfo.revision);
  GFXDrawString(_settingsBuff, secondColumnXPos, yPos);

  // cores
  yPos += itemHeight;
  GFXDrawString("cores:", firstColumnXPos, yPos);
  sprintf(_settingsBuff, "%d", chipInfo.cores);
  GFXDrawString(_settingsBuff, secondColumnXPos, yPos);

  // cpu
  yPos += itemHeight;
  GFXDrawString("cpu speed:", firstColumnXPos, yPos);
  sprintf(_settingsBuff, "%d Mhz", (_u16)conf.freq_mhz);
  GFXDrawString(_settingsBuff, secondColumnXPos, yPos);

  // flash
  yPos += itemHeight;
  GFXDrawString("flash:", firstColumnXPos, yPos);
  sprintf(_settingsBuff, "%u MB", flashSizeInMb);
  GFXDrawString(_settingsBuff, secondColumnXPos, yPos);

  // ram
  yPos += itemHeight;
  GFXDrawString("free ram:", firstColumnXPos, yPos);
  sprintf(_settingsBuff, "%d KB", allRam);
  GFXDrawString(_settingsBuff, secondColumnXPos, yPos);

  // sd
  yPos += itemHeight;
  GFXDrawString("sd size:", firstColumnXPos, yPos);
  sprintf(_settingsBuff, "%lu MB", sdSizeInMB);
  GFXDrawString(_settingsBuff, secondColumnXPos, yPos);

  yPos += itemHeight;
  GFXDrawString("sd used:", firstColumnXPos, yPos);
  sprintf(_settingsBuff, "%lu MB", sdUsedSizeInMB);
  GFXDrawString(_settingsBuff, secondColumnXPos, yPos);

  // battery
  yPos += itemHeight;
  GFXDrawString("battery:", firstColumnXPos, yPos);
  sprintf(_settingsBuff, "%d %%", batteryData->chargeLevelPercents);
  GFXDrawString(_settingsBuff, secondColumnXPos, yPos);
}

static void onAppStop(void) { ESP_LOGI(specs.name, "on app stop..."); }

AppSpecification_t* SettingsAppSpecification(const _u16 appId) {
  specs.id = appId;
  specs.handleInput = &handleKey;
  specs.onInit = &onAppLoading;
  specs.onStart = &onAppResume;
  specs.onPause = &onAppPause;
  specs.onResume = &onAppResume;
  specs.onUpdate = &onAppUpdate;
  specs.onStop = &onAppStop;

  return &specs;
};

void GetChipModel(const esp_chip_model_t model) {
  switch (model) {
    case CHIP_ESP32:
      sprintf(_settingsBuff, "%s", "ESP32");
      break;

    case CHIP_ESP32S2:
      sprintf(_settingsBuff, "%s", "ESP32 S2");
      break;

    case CHIP_ESP32S3:
      sprintf(_settingsBuff, "%s", "ESP32 S3");
      break;

    case CHIP_ESP32C3:
      sprintf(_settingsBuff, "%s", "ESP32 C3");
      break;

    case CHIP_ESP32C2:
      sprintf(_settingsBuff, "%s", "ESP32 C2");
      break;

    case CHIP_ESP32C6:
      sprintf(_settingsBuff, "%s", "ESP32 C6");
      break;

    case CHIP_ESP32H2:
      sprintf(_settingsBuff, "%s", "ESP32 H2");
      break;

    case CHIP_ESP32P4:
      sprintf(_settingsBuff, "%s", "ESP32 P4");
      break;

    default:
      sprintf(_settingsBuff, "%s", "N/A");
      break;
  }
}