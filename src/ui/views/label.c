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
  SizePolicy_t sizePolicy = {.type = WrapContent, .weight = 0};
  label->view = View_Create(label, &Label_Draw, &Label_Destroy, NULL,
                            sizePolicy, sizePolicy);

  Label_RecalculateSize(label);

  return label->view;
}

// private part

static void Label_Draw(View_t* view, const uint16_t left, const uint16_t top,
                       const uint16_t right, const uint16_t bottom) {
  Label_t* label = (Label_t*)View_GetCustomView(view);
  GFX_DrawString(label->text, left, top, GFX_GetFont());
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
  uint16_t textLength = strlen(label->text);

  uint8_t symbolWidth = Font_GetWidth(label->font);
  uint8_t symbolHeight = Font_GetHeight(label->font);

  uint16_t width = textLength * symbolWidth;
  uint8_t height = symbolHeight;

  View_SetWidth(label->view, width);
  View_SetHeight(label->view, height);
}