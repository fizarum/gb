#include <apps_manager.h>
#include <broadcast/broadcast_manager.h>
#include <device_manager.h>
#include <stddef.h>

#include "apps/demo/demo_app.h"
#include "apps/fm/fm_app.h"
#include "apps/info/info_app.h"
#include "apps/menu/menu_app.h"
#include "apps/settings/settings_app.h"
#include "apps/settings/settings_data.h"
#include "devices/audio/audio.h"
#include "devices/battery/battery.h"
#include "devices/display/display.h"
#include "devices/hal/gpio_hal.h"
#include "devices/joystick/joystick.h"
#include "devices/storage/storage.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "system/power/power_managment.h"

#define WAKEUP_PIN GPIO_NUM_9

static const char* const SYS_TAG = "System";
static const char* const DEV_TAG = "Devices";
// 300 millis
static const int64_t thresholdMicrosForPowerModeChange = 300000;

TaskHandle_t sysTaskHandle = NULL;
TaskHandle_t appsTaskHandle = NULL;
TaskHandle_t driverTaskHandle = NULL;

QueueHandle_t inputDataQueue;

AppsManager_t* appsManager = NULL;
DeviceManager* deviceManager = NULL;

TickType_t _500 = pdMS_TO_TICKS(500);
TickType_t _200 = pdMS_TO_TICKS(200);
TickType_t _100 = pdMS_TO_TICKS(100);
TickType_t _50 = pdMS_TO_TICKS(50);
TickType_t _20 = pdMS_TO_TICKS(20);
TickType_t _10 = pdMS_TO_TICKS(10);
TickType_t _5 = pdMS_TO_TICKS(5);
TickType_t _2 = pdMS_TO_TICKS(2);
const _u8 inputDataSize = 2;

volatile int64_t micros = 0;
volatile int64_t whenUpdatedPowerModeInMicros = 0;

volatile bool powerUpPressed = false;

void sysTask(void* arg);
void appsTask(void* arg);
void driverTask(void* arg);

static _u16 RegisterDevice(DeviceManager* deviceManager,
                           DeviceSpecification* deviceSpecification);
static void onPowerModeChangedCallback(const PowerMode mode,
                                       const ModeChangedBy changedBy);

static void LoadAndApplySettings();

static void IRAM_ATTR powerButtonHandler(void* args) {
  micros = esp_timer_get_time();
  if (micros - whenUpdatedPowerModeInMicros >
      thresholdMicrosForPowerModeChange) {
    powerUpPressed = true;
    whenUpdatedPowerModeInMicros = micros;
  }
}

void app_main() {
  vTaskDelay(_20);

  inputDataQueue = xQueueCreate(inputDataSize, sizeof(InputDeviceExtension));

  xTaskCreate(driverTask, "driverTask", 4096, NULL, 11, &driverTaskHandle);
  xTaskCreate(appsTask, "appsTask", 6144, NULL, 10, &appsTaskHandle);

  // Just to make sure that device manager is initialized properly
  vTaskDelay(_50);
  xTaskCreate(sysTask, "sysTask", 4096, NULL, 12, &sysTaskHandle);
}

void sysTask(void* arg) {
  GPIO_SetInterrupt(WAKEUP_PIN, GPIO_INTR_POSEDGE, powerButtonHandler);

  _u32 millis = SettingsData_GetSleepIn() * 60 * 1000;
  PowerManager_Init(WAKEUP_PIN, millis, &onPowerModeChangedCallback);

  while (1) {
    if (powerUpPressed) {
      powerUpPressed = false;

      if (PowerManager_IsSleeping()) {
        PowerManager_ResetPowerMode(ByButton);
        LoadAndApplySettings();
      } else {
        PowerManager_SetPowerMode(Nap, ByButton);
      }
    }
    PowerManager_Update();
    vTaskDelay(_100);
  }
}

void appsTask(void* arg) {
  // added delay to let hadrware init properly(better log), can be removed
  vTaskDelay(pdMS_TO_TICKS(300));

  InputDeviceExtension inputDataToReceive;

  ESP_LOGI(SYS_TAG, "start!");
  BroadcastManager_Init();
  appsManager = AppsManagerCreate();

  // menu
  AppSpecification_t* menuSpecs = MenuAppSpecification(appsManager);
  App_t* menuApp = AppsMangerSetLauncher(appsManager, menuSpecs);

  BroadcastManager_AddListener(ChargingOn, menuApp);
  BroadcastManager_AddListener(ChargingOff, menuApp);

  // fm
  AppsMangerAddAppSpecs(appsManager, FileMangerAppSpecification());

  // settings
  AppsMangerAddAppSpecs(appsManager, SettingsAppSpecification());

  // info
  AppsMangerAddAppSpecs(appsManager, InfoAppSpecification());

  // demo app
  AppsMangerAddAppSpecs(appsManager, DemoAppSpecification());

  AppsManagerStart(appsManager, NULL);

  while (1) {
    BroadcastManager_Update();
    if (PowerManager_IsAwake()) {
      if (xQueueReceive(inputDataQueue, &inputDataToReceive, _200) == pdPASS) {
        // handle menu button to close active app (except menu)
        if (IsButtonMenuReleased(&inputDataToReceive) == true) {
          AppsManagerStopActiveApp(appsManager);
        } else {
          AppsManagerHandleInput(appsManager, &inputDataToReceive);
        }
        // TODO: add this filtering as internal logic of input device
        if (inputDataToReceive.keymap & 0xffdf) {
          PowerManager_ResetSleepTimer();
        }
      }
      AppsManagerUpdate(appsManager);
      vTaskDelay(_5);
    } else {
      vTaskDelay(_200);
    }
  }
}

void driverTask(void* arg) {
  deviceManager = DeviceManagerGetInstance();

  RegisterDevice(deviceManager, JoystickSpecification());
  RegisterDevice(deviceManager, DislplaySpecification());
  RegisterDevice(deviceManager, BatterySpecification());
  RegisterDevice(deviceManager, StorageSpecification());
  RegisterDevice(deviceManager, AudioSpecification());
  LoadAndApplySettings();

  while (1) {
    DeviceManagerUpdate(deviceManager);

    // battery broadcast example
    BatteryDeviceExtension* batteryExtension =
        (BatteryDeviceExtension*)DeviceManager_GetExtension(TypePower);

    if (batteryExtension->charginStatusChanged == true) {
      batteryExtension->charginStatusChanged = false;

      BroadcastEvent_t event = {
          .payload = batteryExtension->chargeLevelPercents,
      };

      if (batteryExtension->charging) {
        event.type = ChargingOn;
      } else {
        event.type = ChargingOff;
      }

      ESP_LOGW(DEV_TAG, "charging status changed, sending broadcast event: %lu",
               event.value);

      // TODO: move broadcast to a separate function
      BroadcastManager_SendEvent(event.value);
    }

    if (PowerManager_IsAwake()) {
      InputDeviceExtension* keyboardExtension =
          (InputDeviceExtension*)DeviceManager_GetExtension(TypeInput);

      if (IsAnyButtonPressed(keyboardExtension) == true) {
        xQueueSend(inputDataQueue, keyboardExtension, _2);
      }

      vTaskDelay(_5);
    } else {
      vTaskDelay(_200);
    }
  }
}

static _u16 RegisterDevice(DeviceManager* deviceManager,
                           DeviceSpecification* deviceSpecification) {
  _u16 id = DeviceManagerNextDeviceId(deviceManager);
  Device* device = DeviceCreate(id, deviceSpecification);
  _u16 addedDeviceId = DeviceManagerAdd(deviceManager, device);
  const char* name = DeviceGetName(device);

  if (addedDeviceId != DEVICE_ID_NONE) {
    ESP_LOGI(DEV_TAG, "added new device: %d [%s]\n", addedDeviceId, name);
  } else {
    ESP_LOGW(DEV_TAG, "cannot add device: [%s]\n", name);
  }
  return addedDeviceId;
}

static void onPowerModeChangedCallback(const PowerMode mode,
                                       const ModeChangedBy changedBy) {
  ESP_LOGI(SYS_TAG, "on power mode change: %d, reason: %d", mode, changedBy);
}

static void LoadAndApplySettings() {
  // load system settings
  SettingsData_Load();

  // and apply them
  DisplayDeviceExtension* displayExtension =
      (DisplayDeviceExtension*)DeviceManager_GetExtension(TypeDisplay);
  if (displayExtension != NULL) {
    _u8 value = SettingsData_GetBrightness() * 10;
    displayExtension->changeBrightness(value);
  }

  AudioDeviceExtension* audioExtension =
      (AudioDeviceExtension*)DeviceManager_GetExtension(TypeAudio);
  if (audioExtension != NULL) {
    _u8 volume = SettingsData_GetVolume();
    audioExtension->changeVolume(volume);
  }

  PowerMode powerMode = SettingsData_GetPowerSave() == 0 ? Normal : SavePower;
  PowerManager_SetPowerMode(powerMode, BySystem);

  _u32 millis = SettingsData_GetSleepIn() * 60 * 1000;
  PowerManager_SetSleepTimer(millis);
  // TODO: complete for other settings
}