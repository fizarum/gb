#include "switch_button.h"

#include <stdlib.h>
#include <string.h>

#include "../gfx/gfx.h"

typedef struct SwitchButton_t {
  bool isOn;
  Font_t *font;

  View_t *view;
} SwitchButton_t;

static const char *onLabel = "ON";
static const char *offLabel = "OFF";

static void Draw(View_t *view, const _u16 left, const _u16 top,
                 const _u16 right, const _u16 bottom);
static void Destroy(void *switchButtonArg);
static void RecalculateSize(SwitchButton_t *switchButton);

View_t *SwitchButton_Create(bool on, Font_t *font) {
  SwitchButton_t *button = (SwitchButton_t *)malloc(sizeof(SwitchButton_t));

  if (button == NULL) {
    return NULL;
  }

  SizePolicy_t widthPolicy = {.type = WrapContent, .weight = 0};
  SizePolicy_t heightpolicy = {.type = WrapContent, .weight = 0};

  button->isOn = on;
  button->font = font;
  button->view = View_Create(button, false, &Draw, &Destroy, NULL, widthPolicy,
                             heightpolicy);

  RecalculateSize(button);

  return button->view;
}

void SwitchButton_Toggle(SwitchButton_t *button) {
  button->isOn = !(button->isOn);
}

bool SwitchBuitton_IsOn(const SwitchButton_t *button) { return button->isOn; }

// private part
static const _u8 indicatorSize = 10;

static void Draw(View_t *view, const _u16 left, const _u16 top,
                 const _u16 right, const _u16 bottom) {
  SwitchButton_t *button = (SwitchButton_t *)View_GetCustomView(view);
  const char *text = button->isOn == true ? onLabel : offLabel;

  if (button->isOn) {
    GFX_DrawFilledRect(left, top, left + indicatorSize, top + indicatorSize,
                       GFX_GetFontColor());
  } else {
    GFX_DrawRect(left, top, left + indicatorSize, top + indicatorSize, 1,
                 GFX_GetFontColor());
  }
  GFX_DrawString(text, left + 20, top, GFX_GetFont());
}

static void Destroy(void *switchButtonArg) {
  if (switchButtonArg == NULL) {
    return;
  }
  SwitchButton_t *button = (SwitchButton_t *)switchButtonArg;

  free(button);
}

static void RecalculateSize(SwitchButton_t *switchButton) {
  const char *text = switchButton->isOn == true ? onLabel : offLabel;
  _u16 textLength = strlen(text);

  _u8 symbolWidth = Font_GetWidth(switchButton->font);
  _u8 height = Font_GetHeight(switchButton->font);
  _u16 width = textLength * symbolWidth;

  View_SetWidth(switchButton->view, width);
  View_SetHeight(switchButton->view, height);
}