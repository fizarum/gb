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

#define kbSize 1024
#define mbSize (1024 * 1024)

static char buff[20];

static AppSpecification_t specs = {
    .name = "Info",
    .background = COLOR_DARKGREEN,
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
  GFX_SetBackground(specs.background);

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

  App_DrawBackgroundAndTitle(specs.name, specs.background);

  Font_t* font = GFX_GetFont();

  // model
  GFX_DrawString("model:", firstColumnXPos, yPos, font);
  sprintf(buff, "%s", Chip_GetModel());
  GFX_DrawString(buff, secondColumnXPos, yPos, font);

  // rev
  yPos += itemHeight;
  GFX_DrawString("revision:", firstColumnXPos, yPos, font);
  sprintf(buff, "%u", chipInfo.revision);
  GFX_DrawString(buff, secondColumnXPos, yPos, font);

  // cores
  yPos += itemHeight;
  GFX_DrawString("cores:", firstColumnXPos, yPos, font);
  sprintf(buff, "%d", chipInfo.cores);
  GFX_DrawString(buff, secondColumnXPos, yPos, font);

  // cpu
  yPos += itemHeight;
  GFX_DrawString("cpu speed:", firstColumnXPos, yPos, font);
  sprintf(buff, "%d Mhz", (_u16)conf.freq_mhz);
  GFX_DrawString(buff, secondColumnXPos, yPos, font);

  // flash
  yPos += itemHeight;
  GFX_DrawString("flash:", firstColumnXPos, yPos, font);
  sprintf(buff, "%u MB", flashSizeInMb);
  GFX_DrawString(buff, secondColumnXPos, yPos, font);

  // ram
  yPos += itemHeight;
  GFX_DrawString("free ram:", firstColumnXPos, yPos, font);
  sprintf(buff, "%d KB", allRamInKb);
  GFX_DrawString(buff, secondColumnXPos, yPos, font);

  // sd
  yPos += itemHeight;
  GFX_DrawString("sd size:", firstColumnXPos, yPos, font);
  sprintf(buff, "%lu MB", sdSizeInMB);
  GFX_DrawString(buff, secondColumnXPos, yPos, font);

  yPos += itemHeight;
  GFX_DrawString("sd used:", firstColumnXPos, yPos, font);
  sprintf(buff, "%lu MB", sdUsedSizeInMB);
  GFX_DrawString(buff, secondColumnXPos, yPos, font);

  // battery
  yPos += itemHeight;
  GFX_DrawString("battery:", firstColumnXPos, yPos, font);
  sprintf(buff, "%d %%", batteryData->chargeLevelPercents);
  GFX_DrawString(buff, secondColumnXPos, yPos, font);

  yPos += itemHeight;
  GFX_DrawString("charging:", firstColumnXPos, yPos, font);
  sprintf(buff, "%s", batteryData->charging ? "true" : "false");
  GFX_DrawString(buff, secondColumnXPos, yPos, font);
}