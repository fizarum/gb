#ifndef GAME_SDK_MENU_ITEM_H
#define GAME_SDK_MENU_ITEM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "../types/types.h"

typedef struct MenuItem MenuItem;
typedef void (*OnMenuItemClick)(void);

MenuItem* MenuItemCreate(_u16 id, const char* const name,
                         OnMenuItemClick callback);
void MenuItemDestroy(MenuItem* item);

void MenuItemOnSelected(MenuItem* item);
void MenuItemOnUnselected(MenuItem* item);

bool MenuItemIsSelected(const MenuItem* item);
const char* MenuItemGetName(const MenuItem* item);

#ifdef __cplusplus
}
#endif

#endif  // GAME_SDK_MENU_ITEM_H