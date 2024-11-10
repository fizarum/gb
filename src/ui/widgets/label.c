#include "label.h"

#include <stdlib.h>
#include <string.h>

#include "../gfx/gfx.h"

typedef struct Label_t {
  Node_t* node;
  char* text;
  Font_t* font;
} Label_t;

static void Label_RecalculateNode(Label_t* label);

Label_t* Label_Create(const uint16_t id, char* text, Font_t* font) {
  Label_t* label = (Label_t*)malloc(sizeof(Label_t));
  if (label == NULL) {
    return NULL;
  }

  label->node = Node_Create(id);
  label->text = text;
  label->font = font;

  Label_RecalculateNode(label);

  return label;
}

void Label_Destroy(Label_t* label) {
  if (label == NULL) {
    return;
  }

  Node_Destroy(label->node);
  free(label);
}

Node_t* Label_GetNode(Label_t* label) { return label->node; }

void Label_Draw(const Label_t* label) {
  GFX_DrawString(label->text, Node_GetXPosition(label->node),
                 Node_GetYPosition(label->node), GFX_GetFont());
}

// part of private code
static void Label_RecalculateNode(Label_t* label) {
  uint16_t textLength = strlen(label->text);

  uint8_t symbolWidth = Font_GetWidth(label->font);
  uint8_t symbolHeight = Font_GetHeight(label->font);

  uint16_t width = textLength * symbolWidth;
  uint8_t height = textLength * symbolHeight;

  Node_SetWidth(label->node, width);
  Node_SetHeight(label->node, height);
}