#include <apps_manager.h>
#include <device_manager.h>
#include <mcp23017.h>
#include <stddef.h>

#include "apps/fm/fm_app.h"
#include "apps/menu/menu_app.h"
#include "apps/settings/settings_app.h"
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

void app_main() {
  vTaskDelay(_100);

  inputDataQueue = xQueueCreate(inputDataSize, sizeof(InputDeviceData_t));

  xTaskCreate(driverTask, "driverTask", 4096, NULL, 11, &driverTaskHandle);
  xTaskCreate(systemTask, "systemTask", 4096, NULL, 10, &systemTaskHandle);
}

void systemTask(void* arg) {
  // added delay to let hadrware init properly(better log), can be removed
  vTaskDelay(pdMS_TO_TICKS(300));

  InputDeviceData_t inputDataToReceive;

  ESP_LOGI(SYS_TAG, "start!");
  appsManager = AppsManagerCreate();

  // menu
  _u16 appId = AppsManagerNextAppId(appsManager);
  AppSpecification_t* menuSpecs = MenuAppSpecification(appId, appsManager);
  App_t* menuApp = AppCreate(menuSpecs);

  // fm
  appId = AppsManagerNextAppId(appsManager);
  AppSpecification_t* fmSpecification = FileMangerAppSpecification(appId);
  App_t* fmApp = AppCreate(fmSpecification);
  AppsManagerAddApp(appsManager, fmApp);

  // settings
  appId = AppsManagerNextAppId(appsManager);
  AppSpecification_t* settingsSpecification = SettingsAppSpecification(appId);
  App_t* settingsApp = AppCreate(settingsSpecification);
  AppsManagerAddApp(appsManager, settingsApp);

  AppsManagerStart(appsManager, menuApp);

  while (1) {
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

  _u16 joystickId = DeviceManagerNextDeviceId(deviceManager);
  Device_t* joystick = DeviceCreate(joystickId, JoystickSpecification());
  _u16 addedDeviceId = DeviceManagerAdd(deviceManager, joystick);

  if (addedDeviceId != DEVICE_ID_NONE) {
    ESP_LOGI(DEV_TAG, "added new device: %d [%s]\n", addedDeviceId,
             DeviceGetName(joystick));
  }

  _u16 displayId = DeviceManagerNextDeviceId(deviceManager);
  Device_t* display = DeviceCreate(displayId, DislplaySpecification());
  addedDeviceId = DeviceManagerAdd(deviceManager, display);
  if (addedDeviceId != DEVICE_ID_NONE) {
    ESP_LOGI(DEV_TAG, "added new device: %d [%s]\n", addedDeviceId,
             DeviceGetName(display));
  }

  _u16 batteryId = DeviceManagerNextDeviceId(deviceManager);
  Device_t* battery = DeviceCreate(batteryId, BatterySpecification());
  addedDeviceId = DeviceManagerAdd(deviceManager, battery);
  if (addedDeviceId != DEVICE_ID_NONE) {
    ESP_LOGI(DEV_TAG, "added new device: %d [%s]\n", addedDeviceId,
             DeviceGetName(battery));
  }

  _u16 sdCardId = DeviceManagerNextDeviceId(deviceManager);
  Device_t* sdCard = DeviceCreate(sdCardId, StorageSpecification());
  addedDeviceId = DeviceManagerAdd(deviceManager, sdCard);
  if (addedDeviceId != DEVICE_ID_NONE) {
    ESP_LOGI(DEV_TAG, "added new device: %d [%s]\n", addedDeviceId,
             DeviceGetName(sdCard));
  }

  while (1) {
    DeviceManagerUpdate(deviceManager);

    Device_t* device = DeviceManagerGet(deviceManager, joystickId);
    const InputDeviceData_t* data = (InputDeviceData_t*)DeviceGetData(device);

    if (IsAnyButtonPressed(data) == true) {
      xQueueSend(inputDataQueue, data, _2);
    }

    vTaskDelay(_20);
  }
}
