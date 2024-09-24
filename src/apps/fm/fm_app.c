#include "fm_app.h"

#include <device_manager.h>
#include <dirent.h>
#include <gfx/gfx.h>
#include <palette.h>
#include <specifications/storage_data.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "../../devices/joystick/joystick.h"
#include "../../devices/storage/storage_utils.h"
#include "esp_log.h"

static StorageDeviceData_t* storageData;

static AppSpecification_t specs = {
    .name = "File Manager",
};

const _u8 startVPadding = 40;
const _u8 vSpacing = 18;
const _u8 startHPadding = 20;
const _u8 startHPaddingForFocusIndicator = 3;

const _u8 maxFilesOnScreen = 10;
// todo: maybe fix would be needed
const _u16 bottomScreenLimit = startVPadding + maxFilesOnScreen * vSpacing;
_u16 totalFilesInDir = 0;

static _u16 vPosOfText = startVPadding;
static _u8 focusedFileIndex = 0;

char buff1[30];
char buff2[36];

static void OnDirectoryItemGet(DirectoryItem* item);
static void DrawScreen();

static void handleKey(const void* keyData) {
  InputDeviceData_t* data = (InputDeviceData_t*)keyData;
  ESP_LOGI(specs.name, "provided keydata: %u", data->keymap);
  if (IsButtonUpReleased(data) == true) {
    focusedFileIndex--;
    if (focusedFileIndex > totalFilesInDir) {
      focusedFileIndex = totalFilesInDir - 1;
    }
    specs.redrawNeeded = RedrawPartial;
  } else if (IsButtonDownReleased(data) == true) {
    focusedFileIndex++;
    if (focusedFileIndex > totalFilesInDir - 1) {
      focusedFileIndex = 0;
    }
    specs.redrawNeeded = RedrawPartial;
  }
}

static void onAppStart(void) {
  ESP_LOGI(specs.name, "on app start...");
  DeviceManager_t* deviceManger = DeviceManagerGetInstance();
  Device_t* storageDevice = DeviceManagerGetByType(deviceManger, TypeStorage);
  storageData = (StorageDeviceData_t*)DeviceGetData(storageDevice);

  totalFilesInDir = 0;
  focusedFileIndex = 0;
}

static void onAppUpdate(void) {}

static void onAppRedraw(RedrawType_t redrawType) {
  if (redrawType == RedrawFull) {
    DrawScreen();
  } else {
    // Redraw only indicator of focused file. Later we need to redraw a list of
    // files
    GFXDrawFilledRect(startHPaddingForFocusIndicator, 20, startVPadding,
                      vSpacing * maxFilesOnScreen, COLOR_DARK_CHARCOAL);
    GFXDrawChar('>', startHPaddingForFocusIndicator,
                startVPadding + (vSpacing * focusedFileIndex));
  }
}

static void onAppPause(void) { ESP_LOGI(specs.name, "on app pause..."); }
static void onAppResume(void) { ESP_LOGI(specs.name, "on app resume..."); }
static void onAppStop(void) { ESP_LOGI(specs.name, "on app stop..."); }

AppSpecification_t* FileMangerAppSpecification(const _u16 appId) {
  specs.id = appId;
  specs.handleInput = &handleKey;
  specs.onStart = &onAppStart;
  specs.onPause = &onAppPause;
  specs.onResume = &onAppResume;
  specs.onUpdate = &onAppUpdate;
  specs.onRedraw = &onAppRedraw;
  specs.onStop = &onAppStop;

  return &specs;
}

static void OnDirectoryItemGet(DirectoryItem* item) {
  strncpy(buff1, item->d_name, 30);

  if (item->d_type == DT_DIR) {
    sprintf(buff2, "%s/", buff1);
  } else {
    sprintf(buff2, "%s", buff1);
  }

  GFXDrawString(buff2, startHPadding, vPosOfText);
  // todo: add screen bounds checking
  vPosOfText += vSpacing;
  if (vPosOfText >= bottomScreenLimit) {
    vPosOfText = startVPadding;
  }
}

static void DrawScreen() {
  GFXFillScreen(COLOR_DARK_CHARCOAL);
  GFXDrawString(specs.name, 30, 7);
  vPosOfText = startVPadding;
  SDShowDirectory(storageData->mountPoint, &totalFilesInDir,
                  &OnDirectoryItemGet);
  GFXDrawChar('>', startHPaddingForFocusIndicator,
              startVPadding + (vSpacing * focusedFileIndex));
}