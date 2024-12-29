#include "option_picker.h"

#include <stdlib.h>
#include <string.h>

#include "../gfx/gfx.h"

typedef struct OptionPicker_t {
  Array_t* options;
  _u16 selecedOptionIndex;

  Font_t* font;

  View_t* view;
} OptionPicker_t;

static void Draw(View_t* view, const _u16 left, const _u16 top,
                 const _u16 right, const _u16 bottom);
static void Destroy(void* optionPickerArg);
static void RecalculateSize(OptionPicker_t* picker);

View_t* OptionPicker_Create(Array_t* options, Font_t* font) {
  OptionPicker_t* picker = (OptionPicker_t*)malloc(sizeof(OptionPicker_t));

  if (picker == NULL) {
    return NULL;
  }

  SizePolicy_t widthPolicy = {.type = WrapContent, .weight = 0};
  SizePolicy_t heightpolicy = {.type = WrapContent, .weight = 0};

  picker->options = options;
  picker->selecedOptionIndex = 0;
  picker->font = font;
  picker->view = View_Create(picker, false, &Draw, &Destroy, NULL, widthPolicy,
                             heightpolicy);

  RecalculateSize(picker);

  return picker->view;
}

// private part
inline OptionPicker_t* GetPicker(View_t* view) {
  return (OptionPicker_t*)View_GetCustomView(view);
}

static void Draw(View_t* view, const _u16 left, const _u16 top,
                 const _u16 right, const _u16 bottom) {
  OptionPicker_t* picker = GetPicker(view);
  const char* text =
      (const char*)ArrayValueAt(picker->options, picker->selecedOptionIndex);
  _u8 textLength = strlen(text);
  _u8 symbolWidth = GFX_FontGetWidth();

  // draw left arrow
  _u16 leftPos = left;
  GFX_DrawChar('<', leftPos, top, GFX_GetFont());

  // middle text
  leftPos += symbolWidth * 2;
  GFX_DrawString(text, leftPos, top, GFX_GetFont());

  // draw right arrow
  leftPos += symbolWidth * textLength + symbolWidth;
  GFX_DrawChar('>', leftPos, top, GFX_GetFont());
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