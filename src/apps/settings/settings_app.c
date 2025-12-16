#include "settings_app.h"

#include <device_manager.h>
#include <extensions/display_extensions.h>
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
    .onPause = &App_StubFunction,
    .onResume = &App_StubFunction,
    .onUpdate = &App_StubFunction,
    .onStop = &OnStop,
};

static Composer_t* composer;
static View_t* listFocus;
static View_t* brightnessPicker;
static View_t* volumePicker;
static View_t* powerSaveSwitch;
static View_t* sleepInOptionPicker;
static InputEvent inputEvent;

Device* displayDevice;

static View_t* selectOptionViewByIndex(_u8 index) {
  switch (index) {
    case 0:
      return brightnessPicker;
    case 1:
      return volumePicker;
    case 2:
      return powerSaveSwitch;
    case 3:
      return sleepInOptionPicker;
    default:
      return NULL;
  }
}

static void handleKey(const void* keyData) {
  InputDeviceData* data = (InputDeviceData*)keyData;
  static _u8 selectedIndex = 0;
  inputEvent.keycode = KEY_NONE;
  inputEvent.type = Cancelled;

  if (IsButtonUpReleased(data) == true) {
    inputEvent.keycode = KEY_UP;
    inputEvent.type = Released;
    View_HandleInput(listFocus, &inputEvent);

    specs.redrawNeeded = RedrawPartial;
  } else if (IsButtonDownReleased(data) == true) {
    inputEvent.keycode = KEY_DOWN;
    inputEvent.type = Released;
    View_HandleInput(listFocus, &inputEvent);

    specs.redrawNeeded = RedrawPartial;
  } else if (IsButtonRightReleased(data) == true) {
    inputEvent.keycode = KEY_RIGHT;
    inputEvent.type = Released;

    selectedIndex = ListFocus_GetSelectedItemIndex(listFocus);
    View_t* view = selectOptionViewByIndex(selectedIndex);
    View_HandleInput(view, &inputEvent);

    specs.redrawNeeded = RedrawPartial;
  } else if (IsButtonLeftReleased(data) == true) {
    inputEvent.keycode = KEY_LEFT;
    inputEvent.type = Released;

    selectedIndex = ListFocus_GetSelectedItemIndex(listFocus);
    View_t* view = selectOptionViewByIndex(selectedIndex);
    View_HandleInput(view, &inputEvent);

    specs.redrawNeeded = RedrawPartial;
  }
}

static _u8 settingsItem = 4;
static Array_t* sleepOptions;
static Array_t* brightnessOptions;
static Array_t* volumeOptions;

static void OnSleepTimeoutChanged(OptionPicker_t* picker, void* option) {
  const _u8 value = (_u8)option;
  ESP_LOGI("Sleep timeout", "option changed to: %d !", value);
}

static inline View_t* CreateSleepOptionsPicker() {
  sleepOptions = ArrayCreate(4);
  ArrayAdd(sleepOptions, 1);
  ArrayAdd(sleepOptions, 2);
  ArrayAdd(sleepOptions, 5);
  ArrayAdd(sleepOptions, 10);

  return OptionPicker_Create(sleepOptions, GFX_GetFont(), OnSleepTimeoutChanged,
                             NULL);
}

static void OnBrightnessChanged(OptionPicker_t* picker, void* option) {
  const _u8 value = (_u8)option;
  ESP_LOGI("Brightness", "option changed to: %d !", value);
  DeviceSpecification* specs = Device_GetSpecification(displayDevice);
  const _u8 brightness = value * 10;

  if (specs != NULL) {
    ((DisplayExtension*)specs->extension)->changeBrightness(brightness);
  }
}

static bool OnBrightnessItemMap(void* option, char* buff) {
  _u8 value = (_u8)option;
  snprintf(buff, 7, "%d", value);
  return true;
}

static inline View_t* CreateBrightnessOptionsPicker() {
  brightnessOptions = ArrayCreate(10);
  ArrayAdd(brightnessOptions, 1);
  ArrayAdd(brightnessOptions, 2);
  ArrayAdd(brightnessOptions, 3);
  ArrayAdd(brightnessOptions, 4);
  ArrayAdd(brightnessOptions, 5);
  ArrayAdd(brightnessOptions, 6);
  ArrayAdd(brightnessOptions, 7);
  ArrayAdd(brightnessOptions, 8);
  ArrayAdd(brightnessOptions, 9);
  ArrayAdd(brightnessOptions, 10);

  return OptionPicker_Create(brightnessOptions, GFX_GetFont(),
                             OnBrightnessChanged, OnBrightnessItemMap);
}

static void OnVolumeChanged(OptionPicker_t* picker, void* option) {
  const _u8 value = (_u8)option;
  ESP_LOGI("Volume", "option changed to: %d !", value);
}

static bool OnVolumeItemMap(void* option, char* buff) {
  _u8 value = (_u8)option;
  if (value == 0) {
    sprintf(buff, "OFF");
  } else {
    sprintf(buff, "%d", value);
  }

  return true;
}

static inline View_t* CreateVolumeOptionsPicker() {
  volumeOptions = ArrayCreate(7);
  ArrayAdd(volumeOptions, 0);
  ArrayAdd(volumeOptions, 1);
  ArrayAdd(volumeOptions, 3);
  ArrayAdd(volumeOptions, 5);
  ArrayAdd(volumeOptions, 7);
  ArrayAdd(volumeOptions, 9);
  ArrayAdd(volumeOptions, 10);

  return OptionPicker_Create(volumeOptions, GFX_GetFont(), OnVolumeChanged,
                             OnVolumeItemMap);
}

// TODO: move to dynamic calculation
static const _u8 settingItemHeight = 52;
static const _u8 padding = 20;

static void onAppStart() {
  composer = Composer_Create(GFX_GetCanwasWidth(), GFX_GetCanvasHeight());

  _u16 rootId = Composer_GetRootId(composer);
  if (rootId == TREE_INDEX_NONE) {
    return;
  }

  DeviceManager* deviceManger = DeviceManagerGetInstance();
  displayDevice = DeviceManagerGetByType(deviceManger, TypeDisplay);

  // toolbar
  View_t* toolbar = Toolbar_Create(specs.name, GFX_GetFont());
  Composer_AddView(composer, rootId, toolbar);

  // main box
  _u16 contentBoxId = Composer_AddHBox(composer, rootId);
  // list focus (left)
  listFocus = ListFocus_Create(settingsItem);
  Composer_AddView(composer, contentBoxId, listFocus);

  _u16 settingsBoxId = Composer_AddVBox(composer, contentBoxId);

  // 1. brightness
  _u16 settingsItem1 = Composer_AddVBox(composer, settingsBoxId);
  Composer_AddView(composer, settingsItem1, VSpacer_Create(padding));
  _u16 brItemBoxId = Composer_AddHBox(composer, settingsItem1);
  View_t* brLabel = Label_Create("Brightness:", GFX_GetFont());
  brightnessPicker = CreateBrightnessOptionsPicker();
  Composer_AddView(composer, brItemBoxId, brLabel);
  Composer_AddView(composer, brItemBoxId, brightnessPicker);
  Composer_AddView(composer, brItemBoxId, VSpacer_Create(settingItemHeight));

  // 2. volume
  _u16 volumeBoxId = Composer_AddHBox(composer, settingsBoxId);
  View_t* vlLabel = Label_Create("volume:", GFX_GetFont());
  volumePicker = CreateVolumeOptionsPicker();
  Composer_AddView(composer, volumeBoxId, vlLabel);
  Composer_AddView(composer, volumeBoxId, volumePicker);
  Composer_AddView(composer, volumeBoxId, VSpacer_Create(settingItemHeight));

  // 3. power save mode
  _u16 powerSaveBoxId = Composer_AddHBox(composer, settingsBoxId);
  View_t* psLabel = Label_Create("power save: ", GFX_GetFont());
  powerSaveSwitch = SwitchButton_Create(false, GFX_GetFont());
  Composer_AddView(composer, powerSaveBoxId, psLabel);
  Composer_AddView(composer, powerSaveBoxId, powerSaveSwitch);
  Composer_AddView(composer, powerSaveBoxId, VSpacer_Create(settingItemHeight));

  // 4. "sleep in" option
  _u16 sleepOptionBoxId = Composer_AddHBox(composer, settingsBoxId);
  View_t* slLabel = Label_Create("sleep in: ", GFX_GetFont());
  sleepInOptionPicker = CreateSleepOptionsPicker();
  Composer_AddView(composer, sleepOptionBoxId, slLabel);
  Composer_AddView(composer, sleepOptionBoxId, sleepInOptionPicker);
  // TODO: redraw this label as well
  View_t* minutesLabel = Label_Create(" min", GFX_GetFont());
  Composer_AddView(composer, sleepOptionBoxId, minutesLabel);
  Composer_AddView(composer, sleepOptionBoxId,
                   VSpacer_Create(settingItemHeight));

  Composer_Recompose(composer);
}

static void onAppRedraw(RedrawType_t redrawType) {
  if (redrawType == RedrawFull) {
    GFX_FillScreen(GFX_GetTheme()->backgroundColor);
  }
  Composer_Draw(composer);
  GFX_Redraw();
}

AppSpecification_t* SettingsAppSpecification() {
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