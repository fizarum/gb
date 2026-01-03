#include "switch_button.h"

#include <stdlib.h>
#include <string.h>

#include "../gfx/gfx.h"

typedef struct SwitchButton_t {
  bool isOn;
  Font_t* font;

  SwitchChangedCallback callback;

  View_t* view;
} SwitchButton_t;

static const char* onLabel = "ON";
static const char* offLabel = "OFF";
static _u16 padding = 20;
static _u16 maxTextLengthInPixels = 0;

static void Draw(View_t* view, const _u16 left, const _u16 top,
                 const _u16 right, const _u16 bottom);
static void HandleInput(View_t* view, InputEvent* event);
static void Destroy(void* switchButtonArg);
static void RecalculateSize(SwitchButton_t* switchButton);

inline static SwitchButton_t* GetSwitchButton(View_t* view) {
  return (SwitchButton_t*)View_GetCustomView(view);
}

View_t* SwitchButton_Create(bool on, Font_t* font,
                            SwitchChangedCallback callback) {
  SwitchButton_t* button = (SwitchButton_t*)malloc(sizeof(SwitchButton_t));

  if (button == NULL) {
    return NULL;
  }

  button->font = font;
  button->view =
      View_Create(button, false, &Draw, &HandleInput, &Destroy, NULL,
                  sizePolicyWrapContent.value, sizePolicyWrapContent.value);

  button->isOn = on;
  button->callback = callback;

  RecalculateSize(button);

  return button->view;
}

void SwitchButton_Toggle(View_t* view) {
  SwitchButton_t* button = GetSwitchButton(view);
  button->isOn = !(button->isOn);
  if (button->callback != NULL) {
    button->callback(button, button->isOn);
  }
  View_SetUpdated(view);
}

void SwitchButton_SetIsOn(View_t* view, bool on) {
  SwitchButton_t* button = GetSwitchButton(view);
  button->isOn = on;
  if (button->callback != NULL) {
    button->callback(button, on);
  }
}

bool SwitchBuitton_IsOn(const View_t* view) {
  return GetSwitchButton(view)->isOn;
}

// private part
static const _u8 indicatorSize = 10;

static void Draw(View_t* view, const _u16 left, const _u16 top,
                 const _u16 right, const _u16 bottom) {
  GFX_DrawFilledRect(left, top, left + padding + maxTextLengthInPixels, bottom,
                     GFX_GetTheme()->backgroundColor);

  SwitchButton_t* button = GetSwitchButton(view);
  const char* text = button->isOn ? onLabel : offLabel;

  if (button->isOn) {
    GFX_DrawFilledRect(left, top, left + indicatorSize, top + indicatorSize,
                       GFX_GetFontColor());
  } else {
    GFX_DrawRect(left, top, left + indicatorSize, top + indicatorSize, 2,
                 GFX_GetFontColor());
  }

  GFX_DrawString(text, left + padding, top, GFX_GetFont());
}

static void HandleInput(View_t* view, InputEvent* event) {
  if (event == NULL || event->type != Released) {
    return;
  }

  SwitchButton_t* button = (SwitchButton_t*)view;

  switch (event->keycode) {
    case KEY_RIGHT:
    case KEY_LEFT:
      SwitchButton_Toggle(button);
      break;

    default:
      break;
  }
}

static void Destroy(void* switchButtonArg) {
  if (switchButtonArg == NULL) {
    return;
  }
  SwitchButton_t* button = (SwitchButton_t*)switchButtonArg;

  free(button);
}

static void RecalculateSize(SwitchButton_t* switchButton) {
  // we need to calculate max possible size obtained by switch button
  // thus, "off" text is taken (longer than "on")
  _u16 maxTextLength = strlen(offLabel);

  _u8 symbolWidth = Font_GetWidth(switchButton->font);
  _u8 height = Font_GetHeight(switchButton->font);
  // + 1 dp (depended/scaled pixel) added as an extra padding to make sure
  // eveerything is clear on redraw
  _u8 extraPadding = switchButton->font->scale;
  maxTextLengthInPixels = maxTextLength * symbolWidth + extraPadding;

  View_SetWidth(switchButton->view, maxTextLengthInPixels);
  View_SetHeight(switchButton->view, height);
}