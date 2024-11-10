#include "settings_app.h"

#include <string.h>

#include "../../devices/joystick/joystick.h"
#include "../apps_utils.h"
#include "esp_log.h"

static AppSpecification_t specs = {
    .name = "Settings",
    .background = COLOR_INDIGO,
    .onPause = &App_StubFunction,
    .onResume = &App_StubFunction,
    .onUpdate = &App_StubFunction,
    .onStop = &App_StubFunction,
};

static _u8 focusedFileIndex = 0;
static _u8 startVPadding = 40;
static _u8 vSpacing = 50;

const static _u8 maxItems = 4;

static void ShowInfo();

static void handleKey(const void* keyData) {
  InputDeviceData_t* data = (InputDeviceData_t*)keyData;
  if (IsButtonUpReleased(data) == true) {
    focusedFileIndex--;
    if (focusedFileIndex >= maxItems) {
      focusedFileIndex = maxItems - 1;
    }
    specs.redrawNeeded = RedrawPartial;
  } else if (IsButtonDownReleased(data) == true) {
    focusedFileIndex++;
    if (focusedFileIndex >= maxItems) {
      focusedFileIndex = 0;
    }
    specs.redrawNeeded = RedrawPartial;
  }
}

static void onAppStart() { GFX_SetBackground(specs.background); }

static void onAppRedraw(RedrawType_t redrawType) {
  if (redrawType == RedrawFull) {
    ShowInfo();
  }

  // draw focus indicator
  App_DrawFocusIndicator(3,                                         // left
                         vSpacing + (vSpacing * focusedFileIndex),  // top
                         26);
}

AppSpecification_t* SettingsAppSpecification(const _u16 appId) {
  specs.id = appId;
  specs.handleInput = &handleKey;
  specs.onStart = &onAppStart;
  specs.onRedraw = &onAppRedraw;

  return &specs;
};

static void ShowInfo() {
  uint8_t yPos = startVPadding;
  uint8_t xPos = 30;
  uint8_t itemHeight = 34;
  _u8 padding = 20;
  _u8 progressBarHeight = 14;

  App_DrawBackgroundAndTitle(specs.name, specs.background);

  GFX_DrawString("brightness:", xPos, yPos, GFX_GetFont());
  yPos += padding;
  App_DrawProgress(xPos,                      // left
                   yPos,                      // top
                   300,                       // right
                   yPos + progressBarHeight,  // bottom
                   20                         // progress
  );

  yPos += itemHeight;
  GFX_DrawString("volume:", xPos, yPos, GFX_GetFont());
  yPos += padding;
  App_DrawProgress(xPos,                      // left
                   yPos,                      // top
                   300,                       // right
                   yPos + progressBarHeight,  // bottom
                   40                         // progress
  );

  yPos += itemHeight;
  GFX_DrawString("power save:", xPos, yPos, GFX_GetFont());
  App_DrawOnOffButton(xPos + 140, yPos, true);
  yPos += padding;

  yPos += itemHeight;
  GFX_DrawString("sleep in:", xPos, yPos, GFX_GetFont());
  const char* text = "30 sec";

  App_DrawHorizontalPicker(xPos + 140, yPos, text, strlen(text));
  yPos += padding;
}