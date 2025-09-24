#include "menu_item.h"

#include <stdlib.h>

typedef struct MenuItem {
  _u16 id;
  const char* name;
  bool selected;
  OnMenuItemClick callback;
} MenuItem;

MenuItem* MenuItemCreate(_u16 id, const char* const name,
                         OnMenuItemClick callback) {
  MenuItem* item = (MenuItem*)malloc(sizeof(MenuItem));

  if (item == NULL) return NULL;

  item->id = id;
  item->name = name;
  item->callback = callback;
  item->selected = false;

  return item;
}

void MenuItemDestroy(MenuItem* item) {
  if (item == NULL) return;

  free(item);
}

void MenuItemOnSelected(MenuItem* item) { item->selected = true; }

void MenuItemOnUnselected(MenuItem* item) { item->selected = false; }

bool MenuItemIsSelected(const MenuItem* item) { return item->selected; }

const char* MenuItemGetName(const MenuItem* item) { return item->name; }