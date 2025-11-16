#include "switch_button.h"

#include <stdlib.h>
#include <string.h>

#include "../gfx/gfx.h"

typedef struct SwitchButton_t {
  // TODO: recheck why this flag doesn't change
  //  bool isOn;
  Font_t* font;

  View_t* view;
} SwitchButton_t;

static const char* onLabel = "ON";
static const char* offLabel = "OFF";
// TODO: used this external flag because of some strange bug which ignores
// changes in SwitchButton_t->isOn
static bool isOn = false;
static _u16 padding = 20;
static _u16 maxTextLengthInPixels = 0;

static void Draw(View_t* view, const _u16 left, const _u16 top,
                 const _u16 right, const _u16 bottom);
static void HandleInput(View_t* view, InputEvent* event);
static void Destroy(void* switchButtonArg);
static void RecalculateSize(SwitchButton_t* switchButton);

View_t* SwitchButton_Create(_u8 on, Font_t* font) {
  SwitchButton_t* button = (SwitchButton_t*)malloc(sizeof(SwitchButton_t));

  if (button == NULL) {
    return NULL;
  }

  SizePolicy_t widthPolicy = {.type = WrapContent, .weight = 0};
  SizePolicy_t heightpolicy = {.type = WrapContent, .weight = 0};
  button->font = font;
  button->view = View_Create(button, false, &Draw, &HandleInput, &Destroy, NULL,
                             widthPolicy, heightpolicy);

  RecalculateSize(button);
  isOn = on;

  return button->view;
}

void SwitchButton_Toggle(SwitchButton_t* button) {
  isOn = !isOn;
  View_SetUpdated((View_t*)button);
}

bool SwitchBuitton_IsOn(const SwitchButton_t* button) { return isOn; }

// private part
static const _u8 indicatorSize = 10;

static void Draw(View_t* view, const _u16 left, const _u16 top,
                 const _u16 right, const _u16 bottom) {
  GFX_DrawFilledRect(left, top, left + padding + maxTextLengthInPixels, bottom,
                     GFX_GetTheme()->backgroundColor);

  SwitchButton_t* button = (SwitchButton_t*)View_GetCustomView(view);
  const char* text = isOn ? onLabel : offLabel;

  if (isOn) {
    GFX_DrawFilledRect(left, top, left + indicatorSize, top + indicatorSize,
                       GFX_GetFontColor());
  } else {
    GFX_DrawRect(left, top, left + indicatorSize, top + indicatorSize, 1,
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