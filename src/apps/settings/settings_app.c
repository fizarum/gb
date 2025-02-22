#include "settings_app.h"

#include <string.h>

#include "../../devices/joystick/joystick.h"
#include "../../ui/composer.h"
#include "../../ui/views/box.h"
#include "../../ui/views/label.h"
#include "../../ui/views/list_focus.h"
#include "../../ui/views/option_picker.h"
#include "../../ui/views/progress.h"
#include "../../ui/views/switch_button.h"
#include "../../ui/views/toolbar.h"
#include "../../ui/views/vspacer.h"
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

const static _u8 maxItems = 4;

static Composer_t* composer;
static View_t* listFocus;

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

static _u8 settingsItem = 4;
static Array_t* sleepOptions;
static Array_t* brightnessOptions;
static Array_t* volumeOptions;

static inline View_t* CreateSleepOptionsPicker() {
  sleepOptions = ArrayCreate(6);
  ArrayAdd(sleepOptions, "5 sec");
  ArrayAdd(sleepOptions, "10 sec");
  ArrayAdd(sleepOptions, "30 sec");
  ArrayAdd(sleepOptions, "1 min");
  ArrayAdd(sleepOptions, "5 min");
  ArrayAdd(sleepOptions, "15 min");
  ArrayAdd(sleepOptions, "never");

  return OptionPicker_Create(sleepOptions, GFX_GetFont());
}

static inline View_t* CreateBrightnessOptionsPicker() {
  brightnessOptions = ArrayCreate(10);
  ArrayAdd(brightnessOptions, "10%");
  ArrayAdd(brightnessOptions, "20%");
  ArrayAdd(brightnessOptions, "30%");
  ArrayAdd(brightnessOptions, "40%");
  ArrayAdd(brightnessOptions, "50%");
  ArrayAdd(brightnessOptions, "60%");
  ArrayAdd(brightnessOptions, "70%");
  ArrayAdd(brightnessOptions, "80%");
  ArrayAdd(brightnessOptions, "90%");
  ArrayAdd(brightnessOptions, "100%");

  return OptionPicker_Create(brightnessOptions, GFX_GetFont());
}

static inline View_t* CreateVolumeOptionsPicker() {
  volumeOptions = ArrayCreate(7);
  ArrayAdd(volumeOptions, "OFF");
  ArrayAdd(volumeOptions, "10%");
  ArrayAdd(volumeOptions, "30%");
  ArrayAdd(volumeOptions, "50%");
  ArrayAdd(volumeOptions, "70%");
  ArrayAdd(volumeOptions, "90%");
  ArrayAdd(volumeOptions, "100%");

  return OptionPicker_Create(volumeOptions, GFX_GetFont());
}
// TODO: move to dynamic calculation
static const _u8 settingItemHeight = 52;
static const _u8 padding = 20;

static void onAppStart() {
  composer = Composer_Create(GFX_GetCanwasWidth(), GFX_GetCanvasHeight());
  GFX_SetBackgroundColor(specs.background);

  _u16 rootId = Composer_GetRootId(composer);
  if (rootId == TREE_INDEX_NONE) {
    return;
  }

  // toolbar
  View_t* toolbar = Toolbar_Create(specs.name, GFX_GetFont());
  Composer_AddView(composer, rootId, toolbar);

  // main box
  _u16 contentBoxId = Composer_AddHBox(composer, rootId);
  listFocus = ListFocus_Create(settingsItem);
  Composer_AddView(composer, contentBoxId, listFocus);

  _u16 settingsBoxId = Composer_AddVBox(composer, contentBoxId);

  // 1. brightness
  _u16 settingsItem1 = Composer_AddVBox(composer, settingsBoxId);
  Composer_AddView(composer, settingsItem1, VSpacer_Create(padding));
  _u16 brItemBoxId = Composer_AddHBox(composer, settingsItem1);
  View_t* brLabel = Label_Create("Brightness:", GFX_GetFont());
  View_t* brProgress = CreateBrightnessOptionsPicker();
  Composer_AddView(composer, brItemBoxId, brLabel);
  Composer_AddView(composer, brItemBoxId, brProgress);
  Composer_AddView(composer, brItemBoxId, VSpacer_Create(settingItemHeight));

  // 2. volume
  _u16 volumeBoxId = Composer_AddHBox(composer, settingsBoxId);
  View_t* vlLabel = Label_Create("volume:", GFX_GetFont());
  View_t* vlProgress = CreateVolumeOptionsPicker();
  Composer_AddView(composer, volumeBoxId, vlLabel);
  Composer_AddView(composer, volumeBoxId, vlProgress);
  Composer_AddView(composer, volumeBoxId, VSpacer_Create(settingItemHeight));

  // 3. power save mode
  _u16 powerSaveBoxId = Composer_AddHBox(composer, settingsBoxId);
  View_t* psLabel = Label_Create("power save: ", GFX_GetFont());
  View_t* psSwitch = SwitchButton_Create(false, GFX_GetFont());
  Composer_AddView(composer, powerSaveBoxId, psLabel);
  Composer_AddView(composer, powerSaveBoxId, psSwitch);
  Composer_AddView(composer, powerSaveBoxId, VSpacer_Create(settingItemHeight));

  // 4. "sleep in" option
  _u16 sleepOptionBoxId = Composer_AddHBox(composer, settingsBoxId);
  View_t* slLabel = Label_Create("sleep in: ", GFX_GetFont());
  View_t* slOptionPicker = CreateSleepOptionsPicker();
  Composer_AddView(composer, sleepOptionBoxId, slLabel);
  Composer_AddView(composer, sleepOptionBoxId, slOptionPicker);
  Composer_AddView(composer, sleepOptionBoxId,
                   VSpacer_Create(settingItemHeight));

  Composer_Recompose(composer);
}

static void onAppRedraw(RedrawType_t redrawType) {
  if (redrawType == RedrawFull) {
    GFX_FillScreen(specs.background);
  }
  Composer_Draw(composer);
  GFX_Redraw();
}

AppSpecification_t* SettingsAppSpecification(const _u16 appId) {
  specs.id = appId;
  specs.handleInput = &handleKey;
  specs.onStart = &onAppStart;
  specs.onRedraw = &onAppRedraw;

  return &specs;
};

static void OnStop() {
  Composer_Clear(composer);
  ArrayDestroy(sleepOptions);
  ArrayDestroy(brightnessOptions);
  ArrayDestroy(volumeOptions);
}