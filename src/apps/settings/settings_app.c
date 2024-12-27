#include "settings_app.h"

#include <string.h>

#include "../../devices/joystick/joystick.h"
#include "../../ui/composer.h"
#include "../../ui/views/box.h"
#include "../../ui/views/label.h"
#include "../../ui/views/list_focus.h"
#include "../../ui/views/progress.h"
#include "../../ui/views/toolbar.h"
#include "../apps_utils.h"
#include "esp_log.h"

static void OnStop();

static AppSpecification_t specs = {
    .name = "Settings",
    .background = COLOR_INDIGO,
    .onPause = &App_StubFunction,
    .onResume = &App_StubFunction,
    .onUpdate = &App_StubFunction,
    .onStop = &OnStop,
};

static _u8 focusedItemIndex = 0;
static _u8 startVPadding = 40;

const static _u8 maxItems = 4;

static Composer_t* composer;
static View_t* listFocus;

static void ShowInfo();

static void handleKey(const void* keyData) {
  InputDeviceData_t* data = (InputDeviceData_t*)keyData;
  if (IsButtonUpReleased(data) == true) {
    focusedItemIndex--;
    if (focusedItemIndex >= maxItems) {
      focusedItemIndex = maxItems - 1;
    }
    ListFocus_SelectItemIndex(listFocus, focusedItemIndex);
    specs.redrawNeeded = RedrawPartial;
  } else if (IsButtonDownReleased(data) == true) {
    focusedItemIndex++;
    if (focusedItemIndex >= maxItems) {
      focusedItemIndex = 0;
    }
    specs.redrawNeeded = RedrawPartial;
    ListFocus_SelectItemIndex(listFocus, focusedItemIndex);
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

  // main box
  _u16 contentBoxId = Composer_AddHBox(composer, rootId);

  listFocus = ListFocus_Create(4);
  Composer_AddView(composer, contentBoxId, listFocus);

  _u16 settingsBoxId = Composer_AddVBox(composer, contentBoxId);

  _u16 brItemBoxId = Composer_AddVBox(composer, settingsBoxId);
  View_t* brLabel = Label_Create("Brightness:", GFX_GetFont());
  View_t* brProgress = Progress_Create(30, 100);
  Composer_AddView(composer, brItemBoxId, brLabel);
  Composer_AddView(composer, brItemBoxId, brProgress);

  _u16 testBoxId = Composer_AddVBox(composer, settingsBoxId);
  View_t* label = Label_Create("Hi world,", GFX_GetFont());
  _u16 viewId = Composer_AddView(composer, testBoxId, label);

  View_t* label2 = Label_Create("Its me", GFX_GetFont());
  _u16 view2Id = Composer_AddView(composer, testBoxId, label2);

  // TODO: fix adding second view to same box
  //  View_t* brightnessLabel = Label_Create("brightness:", GFX_GetFont());
  //  Composer_AddView(composer, settingsBoxId, brightnessLabel);

  Composer_Recompose(composer);
}

static void onAppRedraw(RedrawType_t redrawType) {
  if (redrawType == RedrawFull) {
    GFX_FillScreen(specs.background);
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

static void OnStop() { Composer_Clear(composer); }