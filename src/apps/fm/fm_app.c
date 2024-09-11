#include "fm_app.h"

#include <gfx/gfx.h>
#include <palette.h>
#include <stddef.h>

#include "../../devices/joystick/joystick.h"
#include "esp_log.h"

void (*requestToStop)(const _u16 appId) = NULL;

static AppSpecification_t specs = {
    .name = "File Manager",
};

static void handleKey(const void* keyData) {
  // InputDeviceData_t* data = (InputDeviceData_t*)keyData;
  // ESP_LOGI(specs.name, "provided keydata: %u", data->keymap);
  // if (IsButtonMenuReleased(data) == true) {
  //   if (requestToStop != NULL) {
  //     requestToStop(specs.id);
  //   }
  // }
}

static void onAppLoading(void) { ESP_LOGI(specs.name, "on app loading..."); }

static void onAppUpdate(void) {}

static void onAppPause(void) { ESP_LOGI(specs.name, "on app pause..."); }

static void onAppResume(void) {
  ESP_LOGI(specs.name, "on app resume...");
  GFXFillScreen(COLOR_DARK_CHARCOAL);
  GFXDrawString(specs.name, 30, 7);
}

static void onAppStop(void) { ESP_LOGI(specs.name, "on app stop..."); }

AppSpecification_t* FileMangerAppSpecification(const _u16 appId) {
  specs.id = appId;
  specs.handleInput = &handleKey;
  specs.onInit = &onAppLoading;
  specs.onStart = &onAppResume;
  specs.onPause = &onAppPause;
  specs.onResume = &onAppResume;
  specs.onUpdate = &onAppUpdate;
  specs.onStop = &onAppStop;

  return &specs;
}