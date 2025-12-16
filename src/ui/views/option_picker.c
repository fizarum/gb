#include "option_picker.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../gfx/gfx.h"
#include "esp_log.h"

typedef struct OptionPicker_t {
  Array_t* options;
  _u8 optionsCount;
  _u16 selecedOptionIndex;

  Font_t* font;

  OptionChangedCallback callback;
  OptionMapItem mapItemCallback;

  View_t* view;
} OptionPicker_t;

char _buff[8];

static void Draw(View_t* view, const _u16 left, const _u16 top,
                 const _u16 right, const _u16 bottom);
static void HandleInput(View_t* view, InputEvent* event);
static void Destroy(void* optionPickerArg);
static void RecalculateSize(OptionPicker_t* picker);

View_t* OptionPicker_Create(Array_t* options, Font_t* font,
                            OptionChangedCallback callback,
                            OptionMapItem mapItemCallback) {
  OptionPicker_t* picker = (OptionPicker_t*)malloc(sizeof(OptionPicker_t));

  if (picker == NULL) {
    return NULL;
  }

  SizePolicy_t widthPolicy = {.type = WrapContent, .weight = 0};
  SizePolicy_t heightpolicy = {.type = WrapContent, .weight = 0};

  picker->options = options;
  picker->optionsCount = ArrayCapacity(options);
  picker->selecedOptionIndex = 0;
  picker->font = font;
  picker->callback = callback;
  picker->mapItemCallback = mapItemCallback;
  picker->view = View_Create(picker, false, &Draw, &HandleInput, &Destroy, NULL,
                             widthPolicy, heightpolicy);

  RecalculateSize(picker);

  return picker->view;
}

// private part
inline static OptionPicker_t* GetPicker(View_t* view) {
  return (OptionPicker_t*)View_GetCustomView(view);
}

static void Draw(View_t* view, const _u16 left, const _u16 top,
                 const _u16 right, const _u16 bottom) {
  OptionPicker_t* picker = GetPicker(view);

  const _u8 value = ArrayValueAt(picker->options, picker->selecedOptionIndex);
  if (picker->mapItemCallback != NULL &&
      picker->mapItemCallback(value, _buff)) {
  } else {
    snprintf(_buff, 7, "%d", value);
  }
  _u8 textLength = strlen(_buff);
  _u8 symbolWidth = GFX_FontGetWidth();

  // size can change based on text of option so its length is text length +
  // 4*size of symbols for '<', 'space',  'space' and '>'
  // also, when texts redaraw from longer text ("< 100% >") to smaller
  // ("< 10% >") we need to clear old duty space so + 2 symbolWidth to
  // total size for extra safety
  _u16 newRight = left + symbolWidth * textLength + 4 * symbolWidth +
                  // extra space for cleaning text from longer to shorter
                  3 * symbolWidth;
  // clear old value
  GFX_DrawFilledRect(left, top, newRight, bottom,
                     GFX_GetTheme()->backgroundColor);

  // draw left arrow
  _u16 leftPos = left;
  GFX_DrawChar('<', leftPos, top, GFX_GetFont());

  // middle text
  leftPos += symbolWidth * 2;
  GFX_DrawString(_buff, leftPos, top, GFX_GetFont());

  // draw right arrow
  leftPos += symbolWidth * textLength + symbolWidth;
  GFX_DrawChar('>', leftPos, top, GFX_GetFont());
}

static void HandleInput(View_t* view, InputEvent* event) {
  OptionPicker_t* picker = GetPicker(view);

  switch (event->keycode) {
    case KEY_LEFT:
      picker->selecedOptionIndex--;
      if (picker->selecedOptionIndex >= picker->optionsCount) {
        picker->selecedOptionIndex = picker->optionsCount - 1;
      }
      if (picker->callback != NULL) {
        void* data = ArrayValueAt(picker->options, picker->selecedOptionIndex);
        picker->callback(picker, data);
      }

      View_SetUpdated(view);
      break;

    case KEY_RIGHT:
      picker->selecedOptionIndex++;
      if (picker->selecedOptionIndex >= picker->optionsCount) {
        picker->selecedOptionIndex = 0;
      }
      if (picker->callback != NULL) {
        void* data = ArrayValueAt(picker->options, picker->selecedOptionIndex);
        picker->callback(picker, data);
      }

      View_SetUpdated(view);
      break;

    default:
      ESP_LOGW("OptionPicker", "handle input type: %d not yet implemented!",
               event->keycode);
      break;
  }
}

static void Destroy(void* optionPickerArg) {
  if (optionPickerArg == NULL) {
    return;
  }

  OptionPicker_t* picker = (OptionPicker_t*)optionPickerArg;
  free(picker);
}

// TODO: rework this value
static const _u8 width = 80;

static void RecalculateSize(OptionPicker_t* picker) {
  _u8 height = Font_GetHeight(picker->font);

  View_SetWidth(picker->view, width);
  View_SetHeight(picker->view, height);
}