#include "fm_app.h"

#include <array.h>
#include <device_manager.h>
#include <dirent.h>
#include <specifications/storage_data.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "../../devices/joystick/joystick.h"
#include "../../devices/storage/storage_utils.h"
#include "../apps_utils.h"
#include "esp_log.h"
#include "scroll_bar.h"

#define BACKGROUND_COLOR COLOR_DARK_CHARCOAL

// todo: move to outer layer
#define FS_DISPLAY_WIDTH 320
#define FS_DISPLAY_HEIGHT 240

static StorageDeviceData_t* storageData;

static AppSpecification_t specs = {
    .name = "File Manager",
};

const _u8 startVPadding = 40;
const _u8 vSpacing = 18;
const _u8 startHPadding = 20;
const _u8 startHPaddingForFocusIndicator = 3;
static _u16 vPosOfText;

Array_t* filesOnPage;

// pagination
const _u8 filesPerPage = 10;
_u16 totalFiles = 0;
_u8 filesOnCurrentPage = 0;
long indexOfLastGotFile;
static _u8 focusedFileIndex = 0;

_u8 pages = 0;
_u8 currentPage = 0;

char buff[64];

static void OnDirectoryItemGet(_u16 index, DirectoryItem* item);
static void ShowDirectoryContent(const char* path, _u8 page,
                                 DirectoryItemIterator iterator);

static void PrepareCache();
static void CleanupCache();

static void SwitchPreviousPage();
static void SwitchNextPage();

static void OpenFile();

static void handleKey(const void* keyData) {
  InputDeviceData_t* data = (InputDeviceData_t*)keyData;

  // ESP_LOGI(specs.name, "provided keydata: %u", data->keymap);
  if (IsButtonUpReleased(data) == true) {
    focusedFileIndex--;

    if (focusedFileIndex > filesPerPage - 1) {
      SwitchPreviousPage();
    }

    specs.redrawNeeded = RedrawPartial;
  } else if (IsButtonDownReleased(data) == true) {
    focusedFileIndex++;

    if (focusedFileIndex > filesPerPage - 1) {
      SwitchNextPage();
    } else if (focusedFileIndex >= filesOnCurrentPage) {
      focusedFileIndex = filesOnCurrentPage - 1;
    }

    specs.redrawNeeded = RedrawPartial;
  } else if (IsButtonYReleased(data)) {
    FileItem_t* item = ArrayValueAt(filesOnPage, focusedFileIndex);
    if (item != NULL) {
      sprintf(buff, "%s//%s", storageData->mountPoint, item->name);
      ESP_LOGI(specs.name, "trying to open file: %s", buff);
      OpenFile(buff);
    }
  }
}

static void onAppStart(void) {
  ESP_LOGI(specs.name, "on app start...");
  DeviceManager_t* deviceManger = DeviceManagerGetInstance();
  Device_t* storageDevice = DeviceManagerGetByType(deviceManger, TypeStorage);
  storageData = (StorageDeviceData_t*)DeviceGetData(storageDevice);

  totalFiles = 0;
  focusedFileIndex = 0;
  const char* mnt = storageData->mountPoint;
  vPosOfText = startVPadding;

  PrepareCache();

  // first scan files count in current directory
  esp_err_t result = SDCalculateFilesInDirectory(mnt, &totalFiles);

  if (result == ESP_OK) {
    ESP_LOGI(specs.name, "found %d files in directory: %s", totalFiles, mnt);

    // calculate pages count
    if (totalFiles > 0) {
      pages = totalFiles / filesPerPage;
      // add + 1 page if we have some leftovers
      pages = totalFiles % filesPerPage > 0 ? pages + 1 : pages;
    }

    currentPage = 0;
    ShowDirectoryContent(mnt, currentPage, &OnDirectoryItemGet);
  }
}

static void onAppUpdate(void) {}

static void onAppRedraw(RedrawType_t redrawType) {
  if (redrawType == RedrawFull) {
    AppDrawBackgroundAndTitle(specs.name, BACKGROUND_COLOR);
  }
  vPosOfText = startVPadding;
  _u16 vPosOfTextBox = vPosOfText + vSpacing;

  for (_u8 index = 0; index < filesPerPage; index++) {
    FileItem_t* fileItem = ArrayValueAt(filesOnPage, index);
    if (fileItem == NULL) {
      ESP_LOGW(specs.name, "file item is null on index: %d", index);
      continue;
    }

    // redraw file item
    GFXDrawFilledRect(startHPadding, FS_DISPLAY_WIDTH, vPosOfText,
                      vPosOfTextBox, BACKGROUND_COLOR);
    if (fileItem->initialized == true) {
      GFXDrawString(fileItem->name, startHPadding, vPosOfText);
    }

    vPosOfText += vSpacing;
    vPosOfTextBox = vPosOfText + vSpacing;
    if (vPosOfTextBox >= FS_DISPLAY_HEIGHT) {
      vPosOfTextBox = FS_DISPLAY_HEIGHT - 1;
    }
  }

  // draw focus indicator
  GFXDrawFilledRect(startHPaddingForFocusIndicator, 20, startVPadding,
                    FS_DISPLAY_HEIGHT, BACKGROUND_COLOR);
  GFXDrawChar('>', startHPaddingForFocusIndicator,
              startVPadding + (vSpacing * focusedFileIndex));

  // draw scroll bar
  DrawScrollBar(currentPage, pages, FS_DISPLAY_WIDTH, FS_DISPLAY_HEIGHT,
                GFXGetFontColor(), BACKGROUND_COLOR);
}

static void onAppPause(void) { ESP_LOGI(specs.name, "on app pause..."); }
static void onAppResume(void) { ESP_LOGI(specs.name, "on app resume..."); }
static void onAppStop(void) {
  ESP_LOGI(specs.name, "on app stop...");
  CleanupCache();
}

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

/**
 * @brief Prepare cache for shown file items
 */
static void PrepareCache() {
  filesOnPage = ArrayCreate(filesPerPage);
  for (_u8 index = 0; index < filesPerPage; index++) {
    FileItem_t* item = (FileItem_t*)malloc(sizeof(FileItem_t));
    item->initialized = false;

    ArrayAdd(filesOnPage, item);
  }
}

/**
 * @brief Removed cache for shown file items
 */
static void CleanupCache() {
  for (_u8 index = 0; index < ArraySize(filesOnPage); index++) {
    FileItem_t* item = (FileItem_t*)ArrayValueAt(filesOnPage, index);
    free(item);
  }
  ArrayDestroy(filesOnPage);
}

static void SwitchPreviousPage() {
  currentPage--;
  if (currentPage >= pages) {
    currentPage = pages - 1;
  }
  ShowDirectoryContent(storageData->mountPoint, currentPage,
                       &OnDirectoryItemGet);
  focusedFileIndex = filesOnCurrentPage - 1;
}

static void SwitchNextPage() {
  focusedFileIndex = 0;

  currentPage++;
  if (currentPage >= pages) {
    currentPage = 0;
  }
  ShowDirectoryContent(storageData->mountPoint, currentPage,
                       &OnDirectoryItemGet);
}

static void OnDirectoryItemGet(_u16 index, DirectoryItem* item) {
  _u8 indexInPage = index % filesPerPage;

  FileItem_t* fileItem = ArrayValueAt(filesOnPage, indexInPage);
  FileItemFromDirent(fileItem, item);
  fileItem->initialized = true;
}

static void DisableAllFileItemsIterator(void* item) {
  ((FileItem_t*)item)->initialized = false;
}

static void ShowDirectoryContent(const char* path, _u8 page,
                                 DirectoryItemIterator iterator) {
  _u16 start = page * filesPerPage;
  _u16 end = start + filesPerPage - 1;

  ArrayForeach(filesOnPage, &DisableAllFileItemsIterator);
  SDShowDirectory(path, start, end, &indexOfLastGotFile, &OnDirectoryItemGet);

  filesOnCurrentPage = indexOfLastGotFile > end
                           ? filesPerPage
                           : indexOfLastGotFile % filesPerPage;

  ESP_LOGI(specs.name, "page[%d] finished at: %ld current page has %d files",
           page, indexOfLastGotFile, filesOnCurrentPage);
}

// open file logic

static bool OnDataCallback(const char* data, _u8 length) {
  ESP_LOGI(specs.name, "--> read data: %s", data);
  return true;
}

static void OpenFile() {
  FileItem_t* item = ArrayValueAt(filesOnPage, focusedFileIndex);
  if (item != NULL) {
    sprintf(buff, "%s//%s", storageData->mountPoint, item->name);
    ESP_LOGI(specs.name, "opening file: %s", buff);
    SDReadFile(buff, &OnDataCallback);
  }
}
