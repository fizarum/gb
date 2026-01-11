#include "label.h"

#include <stdlib.h>
#include <string.h>

#include "../gfx/gfx.h"

typedef struct Label_t {
  char* text;
  Font_t* font;

  View_t* view;
} Label_t;

static void Label_RecalculateSize(Label_t* label);
static void Label_Draw(View_t* view, const uint16_t left, const uint16_t top,
                       const uint16_t right, const uint16_t bottom);
static void Label_Destroy(void* labelArg);

View_t* Label_Create(char* text, Font_t* font) {
  Label_t* label = (Label_t*)malloc(sizeof(Label_t));
  if (label == NULL) {
    return NULL;
  }

  label->text = text;
  label->font = font;
  label->view =
      View_Create(label, false, &Label_Draw, NULL, &Label_Destroy, NULL,
                  sizePolicyWrapContent.value, sizePolicyWrapContent.value);

  Label_RecalculateSize(label);

  return label->view;
}

// private part

static void Label_Draw(View_t* view, const uint16_t left, const uint16_t top,
                       const uint16_t right, const uint16_t bottom) {
  Label_t* label = (Label_t*)View_GetCustomView(view);
  Font_t* font = label->font;
  GFX_DrawString(label->text, left, top + font->spacing, font);
}

static void Label_Destroy(void* labelArg) {
  if (labelArg == NULL) {
    return;
  }

  Label_t* label = (Label_t*)labelArg;

  free(label);
}

// TODO: rework to GetWith() & GetHeight callbacks to view's
// ctor like Label_Draw() and Label_Destroy()
static void Label_RecalculateSize(Label_t* label) {
  _u16 textLength = strlen(label->text);

  _u8 symbolWidth = Font_GetWidth(label->font);
  _u8 height = Font_GetHeight(label->font);

  _u16 width = textLength * symbolWidth;

  View_SetWidth(label->view, width);
  View_SetHeight(label->view, height);
}