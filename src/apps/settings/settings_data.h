#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <types.h>

/**
 * @brief settings saved in format:
 * 1 <brightness>: 1..10
 * 2 <volume>: 0(off)..10
 * 3 <power save>: 0(off)..1(on)
 * 4 <sleep in>: 1,2,5,10
 * ....
 * every setting is stored as pair of uint8_t in format:
 * '<option>:<value>', For brightness: 3 its: `1:3`
 *
 * each value in file is separated by ' ', so brightness 5 and volume 2 is:
 * `1:5 2:2`
 */
typedef struct SettingsData SettingsData;

void SettingsData_Save();
void SettingsData_Load();

void SettingsData_SetBrightness(_u8 brightness);
_u8 SettingsData_GetBrightness();

void SettingsData_SetVolume(_u8 volume);
_u8 SettingsData_GetVolume();

void SettingsData_SetPowerSave(bool enable);
_u8 SettingsData_GetPowerSave();

void SettingsData_SetSleepIn(_u8 sleepIn);
_u8 SettingsData_GetSleepIn();

#ifdef __cplusplus
}
#endif