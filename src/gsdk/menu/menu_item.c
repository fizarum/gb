#include "menu_item.h"

#include <stdlib.h>

typedef struct MenuItem_t {
  _u16 id;
  const char* name;
  bool selected;
  OnMenuItemClick callback;
} MenuItem_t;

MenuItem_t* MenuItemCreate(_u16 id, const char* const name,
                           OnMenuItemClick callback) {
  MenuItem_t* item = (MenuItem_t*)malloc(sizeof(MenuItem_t));

  if (item == NULL) return NULL;

  item->id = id;
  item->name = name;
  item->callback = callback;
  item->selected = false;

  return item;
}

void MenuItemDestroy(MenuItem_t* item) {
  if (item == NULL) return;

  free(item);
}

void MenuItemOnSelected(MenuItem_t* item) { item->selected = true; }

void MenuItemOnUnselected(MenuItem_t* item) { item->selected = false; }

bool MenuItemIsSelected(const MenuItem_t* item) { return item->selected; }

const char* MenuItemGetName(const MenuItem_t* item) { return item->name; }