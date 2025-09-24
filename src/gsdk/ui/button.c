#include "button.h"

#include <stdlib.h>

#include "../collections/array.h"

typedef struct Button {
  ClickListener clickListener;
  bool selected;
} Button;

Button* ButtonCreate(ClickListener clickListener) {
  Button* button = malloc(sizeof(Button));

  if (button == NULL) return NULL;

  button->clickListener = clickListener;
  button->selected = false;

  return button;
}

void ButtonDestroy(Button* button) { free(button); }

void ButtonChangeSelection(Button* button, bool selected) {
  button->selected = selected;
}