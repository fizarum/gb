#ifndef GAME_SDK_MENU_ITEM_H
#define GAME_SDK_MENU_ITEM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "../types/types.h"

typedef struct MenuItem_t MenuItem_t;
typedef void (*OnMenuItemClick)(void);

MenuItem_t* MenuItemCreate(_u16 id, const char* const name,
                           OnMenuItemClick callback);
void MenuItemDestroy(MenuItem_t* item);

void MenuItemOnSelected(MenuItem_t* item);
void MenuItemOnUnselected(MenuItem_t* item);

bool MenuItemIsSelected(const MenuItem_t* item);
const char* MenuItemGetName(const MenuItem_t* item);

#ifdef __cplusplus
}
#endif

#endif  // GAME_SDK_MENU_ITEM_H