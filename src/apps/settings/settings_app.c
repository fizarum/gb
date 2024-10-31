#include "settings_app.h"

#include "../apps_utils.h"
#include "esp_log.h"

#define BACKGROUND_COLOR COLOR_INDIGO

static AppSpecification_t specs = {
    .name = "Settings",
    .onStart = &App_StubFunction,
    .onPause = &App_StubFunction,
    .onResume = &App_StubFunction,
    .onUpdate = &App_StubFunction,
    .onStop = &App_StubFunction,
};

static void ShowInfo();

static void handleKey(const void* keyData) {}

static void onAppRedraw(RedrawType_t redrawType) {
  if (redrawType == RedrawFull) {
    ShowInfo();
  }
}

AppSpecification_t* SettingsAppSpecification(const _u16 appId) {
  specs.id = appId;
  specs.handleInput = &handleKey;
  specs.onRedraw = &onAppRedraw;

  return &specs;
};

static void ShowInfo() {
  uint8_t yPos = 40;
  uint8_t firstColumnXPos = 30;
  uint8_t itemHeight = 34;
  _u8 padding = 20;
  _u8 progressBarHeight = 14;

  App_DrawBackgroundAndTitle(specs.name, BACKGROUND_COLOR);

  GFXDrawString("brightness:", firstColumnXPos, yPos);
  yPos += padding;
  App_DrawProgress(firstColumnXPos,           // left
                   yPos,                      // top
                   300,                       // right
                   yPos + progressBarHeight,  // bottom
                   20                         // progress
  );

  yPos += itemHeight;
  GFXDrawString("volume:", firstColumnXPos, yPos);
  yPos += padding;
  App_DrawProgress(firstColumnXPos,           // left
                   yPos,                      // top
                   300,                       // right
                   yPos + progressBarHeight,  // bottom
                   40                         // progress
  );
  // GFXDrawString(buff, secondColumnXPos, yPos);

  yPos += itemHeight;
  GFXDrawString("power save:", firstColumnXPos, yPos);
  yPos += padding;

  yPos += itemHeight;
  GFXDrawString("sleep in:", firstColumnXPos, yPos);
  yPos += padding;
}