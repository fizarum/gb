#include "menu_app.h"

#include <gfx/gfx.h>
#include <palette.h>
#include <specifications/input_data.h>
#include <stddef.h>

#include "../../devices/joystick/joystick.h"
#include "esp_log.h"
#include "esp_timer.h"

#define BACKGROUND_COLOR COLOR_DARKCYAN
#define ACCENT_COLOR COLOR_ORANGE

void SelectNextApp();
void SelectPreviousApp();

AppsManager_t* _appsManager = NULL;
App_t* selectedApp = NULL;

_u8 selectedAppIndex = UINT8_MAX;
Array_t* allApps = NULL;

static bool redraw = false;
volatile bool paused = false;

static AppSpecification_t specs = {
    .name = "Menu",
};

static void handleKey(const void* keyData) {
  if (paused == true) return;

  InputDeviceData_t* data = (InputDeviceData_t*)keyData;
  ESP_LOGI(specs.name, "[specs] handle input: %d", data->keymap);

  if (IsButtonLeftReleased(data)) {
    SelectPreviousApp();
  } else if (IsButtonRightReleased(data)) {
    SelectNextApp();
  } else if (IsButtonYReleased(data)) {
    AppsManagerStartAppWithId(_appsManager, AppGetId(selectedApp));
  }
}

static void onAppLoading(void) { ESP_LOGI(specs.name, "on app loading..."); }

static void onAppUpdate(void) {
  if (redraw == false) {
    return;
  }
  redraw = false;

  const char* appName = AppGetName(selectedApp);

  GFXDrawFilledRect(40, 190, 120, 130, BACKGROUND_COLOR);
  GFXDrawString(appName, 40, 120);
  ESP_LOGI(specs.name, "selected app: %s", appName);
}

static void onAppPause(void) {
  paused = true;
  ESP_LOGI(specs.name, "on app pause...");
}

int64_t start, period;
static void onAppResume(void) {
  ESP_LOGI(specs.name, "on app resume...");
  if (_appsManager != NULL) {
    allApps = AppsManagerGetAllApps(_appsManager);
  }

  GFXFillScreen(BACKGROUND_COLOR);

  // time placeholder
  GFXDrawString("23:59", 30, 7);

  // battery placeholder
  GFXDrawFilledRect(270, 290, 5, 15, ACCENT_COLOR);

  // top status line
  GFXDrawFilledRect(20, 300, 20, 21, ACCENT_COLOR);

  // content
  // GFXDrawString("Hello world", 40, 120);
  // start = esp_timer_get_time();
  // GFXDrawString("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 2, 30);
  // GFXDrawString("0123456789", 2, 50);
  // GFXDrawString("#!\"$\%&\\'()*+-./:;<=>?@[]^_", 2, 65);
  // GFXDrawString("`{|}~", 2, 80);

  // period = esp_timer_get_time() - start;
  // ESP_LOGI(specs.name, "elapsed time: %lld uSeconds", period);

  // left arrow
  GFXDrawFilledRect(15, 15, 110, 130, ACCENT_COLOR);
  GFXDrawFilledRect(23, 24, 100, 140, ACCENT_COLOR);

  // right arrow
  GFXDrawFilledRect(304, 304, 110, 130, ACCENT_COLOR);
  GFXDrawFilledRect(296, 297, 100, 140, ACCENT_COLOR);

  // bottom status line
  GFXDrawFilledRect(20, 300, 220, 221, ACCENT_COLOR);

  if (selectedAppIndex == UINT8_MAX) {
    SelectNextApp();
  } else {
    redraw = true;
  }

  paused = false;
}

static void onAppStop(void) { ESP_LOGI(specs.name, "on app stop..."); }

AppSpecification_t* MenuAppSpecification(const _u16 appId,
                                         AppsManager_t* appsManager) {
  specs.id = appId;
  specs.handleInput = &handleKey;
  specs.onInit = &onAppLoading;
  specs.onStart = &onAppResume;
  specs.onPause = &onAppPause;
  specs.onResume = &onAppResume;
  specs.onUpdate = &onAppUpdate;

  specs.onStop = &onAppStop;

  _appsManager = appsManager;
  return &specs;
}

void SelectNextApp() {
  selectedAppIndex++;

  if (selectedAppIndex >= ArraySize(allApps)) {
    selectedAppIndex = 0;
  }

  selectedApp = ArrayValueAt(allApps, selectedAppIndex);
  redraw = true;
}

void SelectPreviousApp() {
  selectedAppIndex--;

  if (selectedAppIndex == UINT8_MAX) {
    selectedAppIndex = ArrayLastIndex(allApps);
  }
  selectedApp = ArrayValueAt(allApps, selectedAppIndex);
  redraw = true;
}