#include "info_app.h"

#include <device_manager.h>
#include <specifications/battery_data.h>
#include <specifications/storage_data.h>

#include "../../devices/battery/battery.h"
#include "../../devices/hal/chip_hal.h"
#include "../../devices/storage/storage.h"
#include "../../devices/storage/storage_utils.h"
#include "../../ui/composer.h"
#include "../../ui/views/label.h"
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

static esp_chip_info_t chipInfo;
static rtc_cpu_freq_config_t conf;

static _u16 flashSizeInMb = 0;
static _u16 freeRamInKb = 0;

static _u32 sdSizeInMB = 0;
static _u32 sdUsedSizeInMB = 0;

static void AddLabel(const _u16 rootBoxId, const char* text, Font_t* font);

static void onAppStart(void) {
  Composer_Init(GFX_GetCanvasWidth(), GFX_GetCanvasHeight());

  Font_t* font = GFX_GetTheme()->fontNormal;

  _u16 rootId = Composer_GetRootId();
  if (rootId == TREE_INDEX_NONE) {
    return;
  }

  BatteryDeviceExtension* battery =
      (BatteryDeviceExtension*)DeviceManager_GetExtension(TypePower);
  StorageDeviceExtension* storage =
      (StorageDeviceExtension*)DeviceManager_GetExtension(TypeStorage);
  sdSizeInMB = Storage_GetTotalSizeInMBs(storage->mountPoint);
  sdUsedSizeInMB = Storage_GetUsedSizeInMBs(storage->mountPoint);

  esp_chip_info(&chipInfo);
  rtc_clk_cpu_freq_get_config(&conf);

  // toolbar
  View_t* toolbar = Toolbar_Create(specs.name, font);
  Composer_AddView(rootId, toolbar);

  // main box
  _u16 mainBoxId = Composer_AddVBox(rootId);
  View_t* boxView = Composer_FindView(mainBoxId);
  if (boxView != NULL) {
    View_SetHPadding(boxView, PADDING);
  }

  // model part
  sprintf(modelStr, "model: %s", Chip_GetModel());
  AddLabel(mainBoxId, modelStr, font);

  // revision
  sprintf(revisionStr, "rev: %u", chipInfo.revision);
  AddLabel(mainBoxId, revisionStr, font);

  // cores
  sprintf(coresStr, "cores: %d", chipInfo.cores);
  AddLabel(mainBoxId, coresStr, font);

  // cpu
  sprintf(cpuStr, "speed: %u Mhz", (_u16)conf.freq_mhz);
  AddLabel(mainBoxId, cpuStr, font);

  uint32_t flashChipSize;
  esp_flash_get_size(NULL, &flashChipSize);
  flashSizeInMb = flashChipSize / mbSize;

  // flash
  sprintf(flashStr, "flash: %u MB", flashSizeInMb);
  AddLabel(mainBoxId, flashStr, font);

  freeRamInKb = esp_get_free_heap_size() / kbSize;
  // ram
  sprintf(freeRAMStr, "RAM: %u KB", freeRamInKb);
  AddLabel(mainBoxId, freeRAMStr, font);

  // sd
  sprintf(sdSizeStr, "sd size: %lu MB", sdSizeInMB);
  AddLabel(mainBoxId, sdSizeStr, font);

  sprintf(sdUsedStr, "sd used: %lu MB", sdUsedSizeInMB);
  AddLabel(mainBoxId, sdUsedStr, font);

  // battery
  sprintf(batteryStr, "battery: %d %%", battery->chargeLevelPercents);
  AddLabel(mainBoxId, batteryStr, font);

  sprintf(chargingStr, "charging: %s", battery->charging ? "yes" : "no");
  AddLabel(mainBoxId, chargingStr, font);

  sprintf(osStr, "OS ver: %s", osVersion);
  AddLabel(mainBoxId, osStr, font);

  Composer_Recompose();
}

static void handleKey(const void* keyData) {}

static void onAppRedraw(RedrawType_t redrawType) {
  if (redrawType == RedrawFull) {
    GFX_FillScreen(GFX_GetTheme()->backgroundColor);
  }
  Composer_Draw();
  GFX_Redraw();
}

static void onAppStop() { Composer_Clear(); }

AppSpecification_t* InfoAppSpecification() {
  specs.handleInput = &handleKey;
  specs.onStart = &onAppStart;
  specs.onRedraw = &onAppRedraw;
  specs.onStop = &onAppStop;

  return &specs;
}

static void AddLabel(const _u16 rootBoxId, const char* text, Font_t* font) {
  View_t* label = Label_Create(text, font);
  Composer_AddView(rootBoxId, label);
}