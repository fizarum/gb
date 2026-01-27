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

static void handleKey(const void* keyData);
static void onAppStart();
static void onAppResume();
static void onAppRedraw(RedrawType_t redrawType);
static void onBroadcastEvent(BroadcastEvent_t event);

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
    .handleInput = handleKey,
    .onStart = onAppStart,
    .onResume = onAppResume,
    .onRedraw = onAppRedraw,
    .onBroadcastEvent = onBroadcastEvent,
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
  const _u16 displayHCenter = GFX_GetCanvasWidth() / 2;

  const char* appName = AppGetName(selectedApp);
  Font_t* font = GFX_GetTheme()->fontNormal;

  // just added one GFX_FontGetWidth() to make it centered
  const _u16 textLength = (strlen(appName) + 1) * Font_GetWidth(font);
  const _u16 textStart = displayHCenter - textLength / 2;

  switch (redrawType) {
    case RedrawFull: {
      DrawScreen();

      GFX_DrawString(appName, textStart, 120, font);

      DrawBattery(isCharging, percentage, 250, 7);

      ESP_LOGI(specs.name, "selected app: %s", appName);
      break;
    }
    case RedrawPartial: {
      GFX_DrawFilledRect(26, 120, 294, 150, GFX_GetTheme()->backgroundColor);

      GFX_DrawString(appName, textStart, 120, font);

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

  if (selectedAppIndex == UINT8_MAX) {
    SelectNextApp();
  } else {
    specs.redrawNeeded = RedrawPartial;
  }
}

static void onBroadcastEvent(BroadcastEvent_t event) {
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

  Font_t* font = GFX_GetTheme()->fontNormal;
  // time placeholder
  GFX_DrawString("23:59", 30, 7, font);

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

  _u8 lineWidth = 2;
  _u16 color = GFX_GetTheme()->primaryColor;

  // left arrow
  GFX_DrawLine(15, 110, 15, 130, lineWidth, color);
  GFX_DrawLine(23, 100, 23, 140, lineWidth, color);

  // right arrow
  GFX_DrawLine(296, 100, 296, 140, lineWidth, color);
  GFX_DrawLine(304, 110, 304, 130, lineWidth, color);

  // nav icon
  DrawTextWithIcon("move", leftRightNavIcon, 20, 220, font);

  // command icon
  DrawTextWithIcon("start", xButton, 180, 220, font);
}