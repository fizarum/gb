#include "settings_data.h"

#include <device.h>
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
  StorageDeviceData* storageData =
      (StorageDeviceData*)DeviceManager_GetData(TypeStorage);
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
      SettingsData_SetPowerSave(value);
      break;
    case 4:
      SettingsData_SetSleepIn(value);
      break;

    default:
      break;
  }
}

static void ParseDataString() {
  // brightness
  char* rawSetting = strtok(buff, " ");
  ApplySetting(rawSetting);

  // volume
  rawSetting = strtok(NULL, " ");
  ApplySetting(rawSetting);

  // power save
  rawSetting = strtok(NULL, " ");
  ApplySetting(rawSetting);

  // sleep in
  rawSetting = strtok(NULL, " ");
  ApplySetting(rawSetting);
}

void SettingsData_Load() {
  StorageDeviceData* storageData =
      (StorageDeviceData*)DeviceManager_GetData(TypeStorage);
  sprintf(optionsFilename, "%s/%s", storageData->mountPoint, filename);
  SD_ReadFileToBuff(optionsFilename, buff, sizeof(buff));

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
