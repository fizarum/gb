#include "settings_app.h"

#include <string.h>

#include "../../devices/joystick/joystick.h"
#include "../../ui/composer.h"
#include "../../ui/views/box.h"
#include "../../ui/views/label.h"
#include "../../ui/views/list_focus.h"
#include "../../ui/views/toolbar.h"
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

static _u8 focusedItemIndex = 0;
static _u8 startVPadding = 40;

const static _u8 maxItems = 4;

static Composer_t* composer;
static View_t* focusIndicator;

static void ShowInfo();

static void handleKey(const void* keyData) {
  InputDeviceData_t* data = (InputDeviceData_t*)keyData;
  if (IsButtonUpReleased(data) == true) {
    focusedItemIndex--;
    if (focusedItemIndex >= maxItems) {
      focusedItemIndex = maxItems - 1;
    }
    ListFocus_SelectItemIndex(focusIndicator, focusedItemIndex);
    specs.redrawNeeded = RedrawPartial;
  } else if (IsButtonDownReleased(data) == true) {
    focusedItemIndex++;
    if (focusedItemIndex >= maxItems) {
      focusedItemIndex = 0;
    }
    specs.redrawNeeded = RedrawPartial;
    ListFocus_SelectItemIndex(focusIndicator, focusedItemIndex);
  }
}

static void onAppStart() {
  composer = Composer_Create(GFX_GetCanwasWidth(), GFX_GetCanvasHeight());
  GFX_SetBackground(specs.background);

  _u16 rootId = Composer_GetRootId(composer);
  if (rootId == TREE_INDEX_NONE) {
    return;
  }

  // toolbar
  View_t* toolbar = Toolbar_Create(specs.name, GFX_GetFont());
  Composer_AddView(composer, rootId, toolbar);
  _u16 topPadding = View_GetHeight(toolbar);

  // content box
  View_t* box = HBox_Create(0, topPadding);
  _u16 boxId = Composer_AddBox(composer, rootId, box);
  // ESP_LOGI(specs.name, "created box, id: %d", boxId);

  focusIndicator = ListFocus_Create(4);
  _u16 focusId = Composer_AddView(composer, boxId, focusIndicator);
  // ESP_LOGI(specs.name, "created focus, id: %d", focusId);

  View_t* label = Label_Create("Hi world,", GFX_GetFont());
  _u16 viewId = Composer_AddView(composer, boxId, label);
  // ESP_LOGI(specs.name, "created label, id: %d", viewId);

  View_t* label2 = Label_Create("Its me", GFX_GetFont());
  _u16 view2Id = Composer_AddView(composer, boxId, label2);
  // ESP_LOGI(specs.name, "created label2, id: %d", view2Id);
}

static void onAppRedraw(RedrawType_t redrawType) {
  if (redrawType == RedrawFull) {
    GFXFillScreen(specs.background);
  }
  Composer_Draw(composer);
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