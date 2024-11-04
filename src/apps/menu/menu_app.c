#include "menu_app.h"

#include <gfx/gfx.h>
#include <palette.h>
#include <specifications/input_data.h>
#include <stddef.h>

#include "../../devices/joystick/joystick.h"
#include "../apps_utils.h"
#include "esp_log.h"
#include "esp_timer.h"

#define ACCENT_COLOR COLOR_ORANGE

void SelectNextApp();
void SelectPreviousApp();
void DrawScreen();

AppsManager_t* _appsManager = NULL;
App_t* selectedApp = NULL;

_u8 selectedAppIndex = UINT8_MAX;
Array_t* allApps = NULL;

static AppSpecification_t specs = {
    .name = "Menu",
    .background = COLOR_DARKCYAN,
    .onPause = &App_StubFunction,
    .onStop = &App_StubFunction,
    .onUpdate = &App_StubFunction,
};

static void onAppStart() {
  ESP_LOGI(specs.name, "on app started...");

  if (_appsManager != NULL) {
    allApps = AppsManagerGetAllApps(_appsManager);
  }
  SelectNextApp();
}

static void handleKey(const void* keyData) {
  if (keyData == NULL) {
    ESP_LOGW(specs.name, "can't handle keyData, its null!");
  }

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

static void onAppRedraw(RedrawType_t redrawType) {
  if (redrawType == RedrawFull) {
    DrawScreen();
  }

  const char* appName = AppGetName(selectedApp);

  GFXDrawFilledRect(40, 190, 120, 130, specs.background);
  GFXDrawString(appName, 40, 120);

  ESP_LOGI(specs.name, "selected app: %s", appName);
}

int64_t start, period;
static void onAppResume(void) {
  ESP_LOGI(specs.name, "on app resume...");

  if (selectedAppIndex == UINT8_MAX) {
    SelectNextApp();
  } else {
    specs.redrawNeeded = RedrawPartial;
  }
}

AppSpecification_t* MenuAppSpecification(const _u16 appId,
                                         AppsManager_t* appsManager) {
  specs.id = appId;
  specs.handleInput = &handleKey;
  specs.onStart = &onAppStart;
  specs.onResume = &onAppResume;
  specs.onRedraw = &onAppRedraw;

  _appsManager = appsManager;
  return &specs;
}

//
void NormalizeSelectedAppIndex() {
  if (selectedAppIndex == UINT8_MAX) {
    selectedAppIndex = ArrayLastIndex(allApps);
    return;
  }

  if (selectedAppIndex >= ArraySize(allApps)) {
    selectedAppIndex = 0;
  }
}

void SelectNextApp() {
  if (allApps == NULL) return;
  selectedAppIndex++;

  NormalizeSelectedAppIndex();

  selectedApp = ArrayValueAt(allApps, selectedAppIndex);
  specs.redrawNeeded = RedrawPartial;
}

void SelectPreviousApp() {
  if (allApps == NULL) return;
  selectedAppIndex--;

  NormalizeSelectedAppIndex();

  selectedApp = ArrayValueAt(allApps, selectedAppIndex);
  specs.redrawNeeded = RedrawPartial;
}

void DrawScreen() {
  GFXFillScreen(specs.background);
  // time placeholder
  GFXDrawString("23:59", 30, 7);

  // battery placeholder
  GFXDrawFilledRect(270, 290, 5, 15, ACCENT_COLOR);

  // top status line
  GFXDrawFilledRect(20, 300, 20, 21, ACCENT_COLOR);

  // test content
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
}