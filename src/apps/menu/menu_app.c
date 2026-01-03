#include "menu_app.h"

#include <palette.h>
#include <specifications/input_data.h>
#include <stddef.h>
#include <string.h>

#include "../../devices/joystick/joystick.h"
#include "../../ui/gfx/gfx.h"
#include "../apps_utils.h"
#include "devices/audio/audio.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "menu_resources.h"

void SelectNextApp();
void SelectPreviousApp();
void DrawScreen();

AppsManager_t* _appsManager = NULL;
App_t* selectedApp = NULL;
// TODO: move to battery widget file in UI
static const _u8 _batteryWidgetWidth = 30;
static const _u8 _batteryWidgetHeight = 10;

_u8 selectedAppIndex = UINT8_MAX;
Array_t* allApps = NULL;
bool isCharging = false;
_u16 percentage = 0;

static AppSpecification_t specs = {
    .name = "Menu",
    .onPause = &App_StubFunction,
    .onStop = &App_StubFunction,
    .onUpdate = &App_StubFunction,
};

static Theme theme = {
    .primaryColor = COLOR_SUNRAY,
    .backgroundColor = COLOR_DARKCYAN,
};

static void onAppStart() {
  ESP_LOGI(specs.name, "on app started...");
  GFX_SetTheme(&theme);

  if (_appsManager != NULL) {
    allApps = AppsManagerGetAllApps(_appsManager);
  }
  SelectNextApp();
}

static void handleKey(const void* keyData) {
  if (keyData == NULL) {
    ESP_LOGW(specs.name, "can't handle keyData, its null!");
  }

  InputDeviceExtension* data = (InputDeviceExtension*)keyData;
  // ESP_LOGI(specs.name, "[specs] handle input: %d", data->keymap);

  if (IsButtonLeftReleased(data)) {
    SelectPreviousApp();
    playSystemSound(2);
  } else if (IsButtonRightReleased(data)) {
    SelectNextApp();
    playSystemSound(2);
  } else if (IsButtonYReleased(data)) {
    AppsManagerStartAppWithId(_appsManager, AppGetId(selectedApp));
  }
}

static void onAppRedraw(RedrawType_t redrawType) {
  const _u16 displayHCenter = GFX_GetCanwasWidth() / 2;

  const char* appName = AppGetName(selectedApp);

  // just added one GFX_FontGetWidth() to make it centered
  const _u16 textLength = (strlen(appName) + 1) * GFX_FontGetWidth();
  const _u16 textStart = displayHCenter - textLength / 2;

  switch (redrawType) {
    case RedrawFull: {
      DrawScreen();

      GFX_DrawString(appName, textStart, 120, GFX_GetFont());

      DrawBattery(isCharging, percentage, 250, 7);

      ESP_LOGI(specs.name, "selected app: %s", appName);
      break;
    }
    case RedrawPartial: {
      GFX_DrawFilledRect(40, 120, 240, 130, GFX_GetTheme()->backgroundColor);

      GFX_DrawString(appName, textStart, 120, GFX_GetFont());

      ESP_LOGI(specs.name, "selected app: %s", appName);
      break;
    }
    // test part - redrawing only battery status (when device is on battery
    // only)
    case RedrawCustom: {
      DrawBattery(isCharging, percentage, 250, 7);
      break;
    }
    default:
      break;
  }

  GFX_Redraw();
}

static void onAppResume(void) {
  ESP_LOGI(specs.name, "on app resume...");
  GFX_SetTheme(&theme);

  if (selectedAppIndex == UINT8_MAX) {
    SelectNextApp();
  } else {
    specs.redrawNeeded = RedrawPartial;
  }
}

void onBroadcastEvent(BroadcastEvent_t event) {
  ESP_LOGI(specs.name, "received broadcast event: [%d]\n", event.type);
  switch (event.type) {
    case ChargingOn:
      isCharging = true;
      // TODO: temp solution
      percentage = (_u8)event.payload;
      specs.redrawNeeded = RedrawCustom;
      break;

    case ChargingOff:
      isCharging = false;
      percentage = (_u8)event.payload;
      // TODO: temp solution
      specs.redrawNeeded = RedrawCustom;
      break;

    default:
      specs.redrawNeeded = RedrawNone;
      break;
  }
}

AppSpecification_t* MenuAppSpecification(AppsManager_t* appsManager) {
  specs.handleInput = &handleKey;
  specs.onStart = &onAppStart;
  specs.onResume = &onAppResume;
  specs.onRedraw = &onAppRedraw;
  specs.onBroadcastEvent = &onBroadcastEvent;

  _appsManager = appsManager;
  return &specs;
}

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
  GFX_FillScreen(GFX_GetTheme()->backgroundColor);
  // time placeholder
  GFX_DrawString("23:59", 30, 7, GFX_GetFont());

  // battery placeholder
  // used DrawBattery() instead

  // test content
  // GFX_DrawString("Hello world", 40, 120);
  // start = esp_timer_get_time();
  // GFX_DrawString("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 2, 30);
  // GFX_DrawString("0123456789", 2, 50);
  // GFX_DrawString("#!\"$\%&\\'()*+-./:;<=>?@[]^_", 2, 65);
  // GFX_DrawString("`{|}~", 2, 80);

  // period = esp_timer_get_time() - start;
  // ESP_LOGI(specs.name, "elapsed time: %lld uSeconds", period);

  // left arrow
  GFX_DrawFilledRect(15, 110, 17, 130, GFX_GetTheme()->primaryColor);
  GFX_DrawFilledRect(23, 100, 25, 140, GFX_GetTheme()->primaryColor);

  // right arrow
  GFX_DrawFilledRect(304, 110, 306, 130, GFX_GetTheme()->primaryColor);
  GFX_DrawFilledRect(296, 100, 298, 140, GFX_GetTheme()->primaryColor);

  // bottom status line
  // GFX_DrawFilledRect(20, 220, 300, 222, GFX_GetTheme()->primaryColor);

  // nav icon
  DrawTextWithIcon("move", leftRightNavIcon, 20, 220);

  // command icon
  DrawTextWithIcon("start", xButton, 180, 220);
}