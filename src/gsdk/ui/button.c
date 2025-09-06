#include "button.h"

#include <stdlib.h>

#include "../collections/array.h"

typedef struct Button_t {
  ClickListener clickListener;
  bool selected;
} Button_t;

Button_t* ButtonCreate(ClickListener clickListener) {
  Button_t* button = malloc(sizeof(Button_t));

  if (button == NULL) return NULL;

  button->clickListener = clickListener;
  button->selected = false;

  return button;
}

void ButtonDestroy(Button_t* button) { free(button); }

void ButtonChangeSelection(Button_t* button, bool selected) {
  button->selected = selected;
}