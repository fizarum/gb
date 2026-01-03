#include "settings_data.h"

#include <device_manager.h>
#include <devices/storage/storage_utils.h>
#include <specifications/storage_data.h>
#include <string.h>

typedef struct SettingsData {
  _u8 brightness;
  _u8 volume;
  _u8 powerSave;
  _u8 sleepIn;
} SettingsData;

static const char* const filename = "settings.ini";

static SettingsData data = {
    .brightness = 1, .volume = 0, .powerSave = 0, .sleepIn = 1};

static char optionsFilename[16];
static _u8 buff[64];

void SettingsData_Save() {
  StorageDeviceExtension* storageData =
      (StorageDeviceExtension*)DeviceManager_GetExtension(TypeStorage);
  sprintf(optionsFilename, "%s/%s", storageData->mountPoint, filename);

  sprintf(buff, "1:%d 2:%d 3:%d 4:%d", data.brightness, data.volume,
          data.powerSave, data.sleepIn);
  SD_WriteFile(optionsFilename, buff, strlen(buff));
}

static void ApplySetting(char* rawSetting) {
  if (rawSetting == NULL) {
    return;
  }

  char* strName = strtok(rawSetting, ":");
  char* strValue = strtok(NULL, ":");

  _u8 name = (_u8)atoi(strName);
  _u8 value = (_u8)atoi(strValue);

  switch (name) {
    case 1:
      SettingsData_SetBrightness(value);
      break;
    case 2:
      SettingsData_SetVolume(value);
      break;
    case 3:
      SettingsData_SetPowerSave(value == 1);
      break;
    case 4:
      SettingsData_SetSleepIn(value);
      break;

    default:
      break;
  }
}

static void ParseDataString() {
  static char _buff[16];
  _u16 settingIndex = 0;
  const char delimiter = ' ';
  for (_u16 index = 0; index < strlen(buff); ++index) {
    const char c = buff[index];
    if (c == delimiter || c == 0) {
      _buff[settingIndex] = 0;
      ApplySetting(_buff);
      settingIndex = 0;
    } else {
      _buff[settingIndex] = c;
      ++settingIndex;
    }
  }
  ApplySetting(_buff);
}

void SettingsData_Load() {
  StorageDeviceExtension* storage =
      (StorageDeviceExtension*)DeviceManager_GetExtension(TypeStorage);
  sprintf(optionsFilename, "%s/%s", storage->mountPoint, filename);
  SD_ReadFileToBuff(optionsFilename, buff, sizeof(buff));

  printf("settings data: %s\n", buff);

  ParseDataString();
}

void SettingsData_SetBrightness(_u8 brightness) {
  data.brightness = brightness;
}

_u8 SettingsData_GetBrightness() { return data.brightness; }

void SettingsData_SetVolume(_u8 volume) { data.volume = volume; }
_u8 SettingsData_GetVolume() { return data.volume; }

void SettingsData_SetPowerSave(bool enable) { data.powerSave = enable; }
_u8 SettingsData_GetPowerSave() { return data.powerSave; }

void SettingsData_SetSleepIn(_u8 sleepIn) { data.sleepIn = sleepIn; }
_u8 SettingsData_GetSleepIn() { return data.sleepIn; }
