#include "info_app.h"

#include <device_manager.h>
#include <specifications/battery_data.h>
#include <specifications/storage_data.h>

#include "../../devices/battery/battery.h"
#include "../../devices/hal/chip_hal.h"
#include "../../devices/storage/storage.h"
#include "../../devices/storage/storage_utils.h"
#include "../apps_utils.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_log.h"
#include "esp_psram.h"
#include "soc/rtc.h"

#define BACKGROUND_COLOR COLOR_DARKGREEN
#define kbSize 1024
#define mbSize (1024 * 1024)

static char buff[20];

static AppSpecification_t specs = {
    .name = "Info",
    .onPause = &App_StubFunction,
    .onResume = &App_StubFunction,
    .onUpdate = &App_StubFunction,
    .onStop = &App_StubFunction,
};

static BatteryDeviceData_t* batteryData;
static StorageDeviceData_t* storageData;

static esp_chip_info_t chipInfo;
static rtc_cpu_freq_config_t conf;

static _u16 flashSizeInMb = 0;
static _u16 allRamInKb = 0;

static _u32 sdSizeInMB = 0;
static _u32 sdUsedSizeInMB = 0;

static void ShowInfo();

static void onAppStart(void) {
  ESP_LOGI(specs.name, "on app start...");
  GFX_SetBackground(BACKGROUND_COLOR);

  DeviceManager_t* deviceManger = DeviceManagerGetInstance();
  Device_t* powerDevice = DeviceManagerGetByType(deviceManger, TypePower);
  batteryData = (BatteryDeviceData_t*)DeviceGetData(powerDevice);

  Device_t* storageDevice = DeviceManagerGetByType(deviceManger, TypeStorage);
  storageData = (StorageDeviceData_t*)DeviceGetData(storageDevice);

  esp_chip_info(&chipInfo);
  rtc_clk_cpu_freq_get_config(&conf);

  uint32_t flashChipSize;
  esp_flash_get_size(NULL, &flashChipSize);
  flashSizeInMb = flashChipSize / mbSize;

  allRamInKb = esp_get_free_heap_size() / kbSize;

  sdSizeInMB = StorageGetTotalSizeInMBs(storageData->mountPoint);
  sdUsedSizeInMB = StorageGetUsedSizeInMBs(storageData->mountPoint);
}

static void handleKey(const void* keyData) {}

static void onAppRedraw(RedrawType_t redrawType) {
  if (redrawType == RedrawFull) {
    ShowInfo();
  }
}

AppSpecification_t* InfoAppSpecification(const _u16 appId) {
  specs.id = appId;
  specs.handleInput = &handleKey;
  specs.onStart = &onAppStart;
  specs.onRedraw = &onAppRedraw;

  return &specs;
};

static void ShowInfo() {
  uint8_t yPos = 40;
  uint8_t firstColumnXPos = 30;
  uint8_t secondColumnXPos = 160;
  uint8_t itemHeight = 18;

  App_DrawBackgroundAndTitle(specs.name, BACKGROUND_COLOR);

  // model
  GFXDrawString("model:", firstColumnXPos, yPos);
  sprintf(buff, "%s", Chip_GetModel());
  GFXDrawString(buff, secondColumnXPos, yPos);

  // rev
  yPos += itemHeight;
  GFXDrawString("revision:", firstColumnXPos, yPos);
  sprintf(buff, "%u", chipInfo.revision);
  GFXDrawString(buff, secondColumnXPos, yPos);

  // cores
  yPos += itemHeight;
  GFXDrawString("cores:", firstColumnXPos, yPos);
  sprintf(buff, "%d", chipInfo.cores);
  GFXDrawString(buff, secondColumnXPos, yPos);

  // cpu
  yPos += itemHeight;
  GFXDrawString("cpu speed:", firstColumnXPos, yPos);
  sprintf(buff, "%d Mhz", (_u16)conf.freq_mhz);
  GFXDrawString(buff, secondColumnXPos, yPos);

  // flash
  yPos += itemHeight;
  GFXDrawString("flash:", firstColumnXPos, yPos);
  sprintf(buff, "%u MB", flashSizeInMb);
  GFXDrawString(buff, secondColumnXPos, yPos);

  // ram
  yPos += itemHeight;
  GFXDrawString("free ram:", firstColumnXPos, yPos);
  sprintf(buff, "%d KB", allRamInKb);
  GFXDrawString(buff, secondColumnXPos, yPos);

  // sd
  yPos += itemHeight;
  GFXDrawString("sd size:", firstColumnXPos, yPos);
  sprintf(buff, "%lu MB", sdSizeInMB);
  GFXDrawString(buff, secondColumnXPos, yPos);

  yPos += itemHeight;
  GFXDrawString("sd used:", firstColumnXPos, yPos);
  sprintf(buff, "%lu MB", sdUsedSizeInMB);
  GFXDrawString(buff, secondColumnXPos, yPos);

  // battery
  yPos += itemHeight;
  GFXDrawString("battery:", firstColumnXPos, yPos);
  sprintf(buff, "%d %%", batteryData->chargeLevelPercents);
  GFXDrawString(buff, secondColumnXPos, yPos);
}