#include "info_app.h"

#include <device_manager.h>
#include <specifications/battery_data.h>
#include <specifications/storage_data.h>

#include "../../devices/battery/battery.h"
#include "../../devices/hal/chip_hal.h"
#include "../../devices/storage/storage.h"
#include "../../devices/storage/storage_utils.h"
#include "../../ui/composer.h"
#include "../../ui/views/toolbar.h"
#include "../apps_utils.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_log.h"
#include "esp_psram.h"
#include "soc/rtc.h"

#define kbSize 1024
#define mbSize (1024 * 1024)
#define PADDING 20

static char modelStr[16];
static char revisionStr[16];
static char coresStr[16];
static char cpuStr[20];
static char flashStr[20];
static char freeRAMStr[20];
static char sdSizeStr[20];
static char sdUsedStr[20];
static char batteryStr[24];
static char chargingStr[24];
static char osStr[24];

static AppSpecification_t specs = {
    .name = "Info",
    .onPause = &App_StubFunction,
    .onResume = &App_StubFunction,
    .onUpdate = &App_StubFunction,
    .onStop = &App_StubFunction,
};

static Theme theme = {
    .primaryColor = COLOR_ORANGE,
    .backgroundColor = COLOR_DARKGREEN,
};

static BatteryDeviceData_t* batteryData;
static StorageDeviceData_t* storageData;

static esp_chip_info_t chipInfo;
static rtc_cpu_freq_config_t conf;

static _u16 flashSizeInMb = 0;
static _u16 freeRamInKb = 0;

static _u32 sdSizeInMB = 0;
static _u32 sdUsedSizeInMB = 0;

static Composer_t* composer;

static void AddLabel(const _u16 rootBoxId, const char* text);

static void onAppStart(void) {
  composer = Composer_Create(GFX_GetCanwasWidth(), GFX_GetCanvasHeight());
  GFX_SetTheme(&theme);

  _u16 rootId = Composer_GetRootId(composer);
  if (rootId == TREE_INDEX_NONE) {
    return;
  }

  DeviceManager_t* deviceManger = DeviceManagerGetInstance();
  Device_t* powerDevice = DeviceManagerGetByType(deviceManger, TypePower);
  batteryData = (BatteryDeviceData_t*)DeviceGetData(powerDevice);

  Device_t* storageDevice = DeviceManagerGetByType(deviceManger, TypeStorage);
  storageData = (StorageDeviceData_t*)DeviceGetData(storageDevice);
  sdSizeInMB = StorageGetTotalSizeInMBs(storageData->mountPoint);
  sdUsedSizeInMB = StorageGetUsedSizeInMBs(storageData->mountPoint);

  esp_chip_info(&chipInfo);
  rtc_clk_cpu_freq_get_config(&conf);

  // toolbar
  View_t* toolbar = Toolbar_Create(specs.name, GFX_GetFont());
  Composer_AddView(composer, rootId, toolbar);

  // main box
  _u16 mainBoxId = Composer_AddVBox(composer, rootId);
  View_t* boxView = Composer_FindView(composer, mainBoxId);
  if (boxView != NULL) {
    View_SetHPadding(boxView, PADDING);
  }

  // model part
  sprintf(modelStr, "model: %s", Chip_GetModel());
  AddLabel(mainBoxId, modelStr);

  // revision
  sprintf(revisionStr, "rev: %u", chipInfo.revision);
  AddLabel(mainBoxId, revisionStr);

  // cores
  sprintf(coresStr, "cores: %d", chipInfo.cores);
  AddLabel(mainBoxId, coresStr);

  // cpu
  sprintf(cpuStr, "speed: %u Mhz", (_u16)conf.freq_mhz);
  AddLabel(mainBoxId, cpuStr);

  uint32_t flashChipSize;
  esp_flash_get_size(NULL, &flashChipSize);
  flashSizeInMb = flashChipSize / mbSize;

  // flash
  sprintf(flashStr, "flash: %u MB", flashSizeInMb);
  AddLabel(mainBoxId, flashStr);

  freeRamInKb = esp_get_free_heap_size() / kbSize;
  // ram
  sprintf(freeRAMStr, "RAM: %u KB", freeRamInKb);
  AddLabel(mainBoxId, freeRAMStr);

  // sd
  sprintf(sdSizeStr, "sd size: %lu MB", sdSizeInMB);
  AddLabel(mainBoxId, sdSizeStr);

  sprintf(sdUsedStr, "sd used: %lu MB", sdUsedSizeInMB);
  AddLabel(mainBoxId, sdUsedStr);

  // battery
  sprintf(batteryStr, "battery: %d %%", batteryData->chargeLevelPercents);
  AddLabel(mainBoxId, batteryStr);

  sprintf(chargingStr, "charging: %s", batteryData->charging ? "yes" : "no");
  AddLabel(mainBoxId, chargingStr);

  sprintf(osStr, "OS ver: %s", osVersion);
  AddLabel(mainBoxId, osStr);

  Composer_Recompose(composer);
}

static void handleKey(const void* keyData) {}

static void onAppRedraw(RedrawType_t redrawType) {
  if (redrawType == RedrawFull) {
    GFX_FillScreen(GFX_GetTheme()->backgroundColor);
  }
  Composer_Draw(composer);
  GFX_Redraw();
}

static void onAppStop() { Composer_Clear(composer); }

AppSpecification_t* InfoAppSpecification() {
  specs.handleInput = &handleKey;
  specs.onStart = &onAppStart;
  specs.onRedraw = &onAppRedraw;
  specs.onStop = &onAppStop;

  return &specs;
};

static void AddLabel(const _u16 rootBoxId, const char* text) {
  View_t* label = Label_Create(text, GFX_GetFont());
  Composer_AddView(composer, rootBoxId, label);
}