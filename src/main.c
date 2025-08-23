#include <apps_manager.h>
#include <broadcast/broadcast_manager.h>
#include <device_manager.h>
#include <specifications/battery_data.h>
#include <stddef.h>

#include "apps/demo/demo_app.h"
#include "apps/fm/fm_app.h"
#include "apps/info/info_app.h"
#include "apps/menu/menu_app.h"
#include "apps/settings/settings_app.h"
#include "devices/audio/audio.h"
#include "devices/battery/battery.h"
#include "devices/display/display.h"
#include "devices/joystick/joystick.h"
#include "devices/storage/storage.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

static const char* const SYS_TAG = "System";
static const char* const DEV_TAG = "Devices";

TaskHandle_t systemTaskHandle = NULL;
TaskHandle_t driverTaskHandle = NULL;

TaskHandle_t testHandler = NULL;
QueueHandle_t inputDataQueue;

AppsManager_t* appsManager = NULL;
DeviceManager_t* deviceManager = NULL;

TickType_t _500 = pdMS_TO_TICKS(500);
TickType_t _200 = pdMS_TO_TICKS(200);
TickType_t _100 = pdMS_TO_TICKS(100);
TickType_t _20 = pdMS_TO_TICKS(20);
TickType_t _10 = pdMS_TO_TICKS(10);
TickType_t _5 = pdMS_TO_TICKS(5);
TickType_t _2 = pdMS_TO_TICKS(2);
const _u8 inputDataSize = 2;

void systemTask(void* arg);
void driverTask(void* arg);
void testTask(void* arg);
static _u16 RegisterDevice(DeviceManager_t* deviceManager,
                           DeviceSpecification_t* deviceSpecification);

void app_main() {
  vTaskDelay(_100);

  inputDataQueue = xQueueCreate(inputDataSize, sizeof(InputDeviceData_t));

  xTaskCreate(driverTask, "driverTask", 4096, NULL, 11, &driverTaskHandle);
  xTaskCreate(systemTask, "systemTask", 4096, NULL, 10, &systemTaskHandle);
  xTaskCreate(testTask, "testTask", 4096, NULL, 10, &testHandler);
}

void testTask(void* arg) {
  while (1) {
    playSystemSound(1);
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}

void systemTask(void* arg) {
  // added delay to let hadrware init properly(better log), can be removed
  vTaskDelay(pdMS_TO_TICKS(300));

  InputDeviceData_t inputDataToReceive;

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

    if (xQueueReceive(inputDataQueue, &inputDataToReceive, _2) == pdPASS) {
      // handle menu button to close active app (except menu)
      if (IsButtonMenuReleased(&inputDataToReceive) == true) {
        AppsManagerStopActiveApp(appsManager);
      } else {
        AppsManagerHandleInput(appsManager, &inputDataToReceive);
      }
    }
    AppsManagerUpdate(appsManager);
    vTaskDelay(_10);
  }
}

void driverTask(void* arg) {
  deviceManager = DeviceManagerGetInstance();

  _u16 joystickId = RegisterDevice(deviceManager, JoystickSpecification());
  RegisterDevice(deviceManager, DislplaySpecification());
  _u16 batteryId = RegisterDevice(deviceManager, BatterySpecification());
  RegisterDevice(deviceManager, StorageSpecification());
  RegisterDevice(deviceManager, AudioSpecification());

  while (1) {
    DeviceManagerUpdate(deviceManager);

    Device_t* device = DeviceManagerGet(deviceManager, joystickId);
    const InputDeviceData_t* data = (InputDeviceData_t*)DeviceGetData(device);

    if (IsAnyButtonPressed(data) == true) {
      xQueueSend(inputDataQueue, data, _2);
    }

    Device_t* batteryDevice = DeviceManagerGet(deviceManager, batteryId);
    BatteryDeviceData_t* batteryData =
        (BatteryDeviceData_t*)DeviceGetData(batteryDevice);

    if (batteryData->charginStatusChanged == true) {
      batteryData->charginStatusChanged = false;

      BroadcastEvent_t batteryEvent = {
          .payload = batteryData->chargeLevelPercents,
      };

      if (batteryData->charging) {
        batteryEvent.type = ChargingOn;
      } else {
        batteryEvent.type = ChargingOff;
      }

      ESP_LOGW(DEV_TAG, "charging status changed, sending broadcast event: %lu",
               batteryEvent.value);

      BroadcastManager_SendEvent(batteryEvent.value);
    }

    vTaskDelay(_20);
  }
}

static _u16 RegisterDevice(DeviceManager_t* deviceManager,
                           DeviceSpecification_t* deviceSpecification) {
  _u16 id = DeviceManagerNextDeviceId(deviceManager);
  Device_t* device = DeviceCreate(id, deviceSpecification);
  _u16 addedDeviceId = DeviceManagerAdd(deviceManager, device);
  const char* name = DeviceGetName(device);

  if (addedDeviceId != DEVICE_ID_NONE) {
    ESP_LOGI(DEV_TAG, "added new device: %d [%s]\n", addedDeviceId, name);
  } else {
    ESP_LOGW(DEV_TAG, "cannot add device: [%s]\n", name);
  }
  return addedDeviceId;
}