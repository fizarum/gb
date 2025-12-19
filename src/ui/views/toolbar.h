#ifndef UI_TOOLBAR_H
#define UI_TOOLBAR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../typography/font.h"
#include "../view.h"

typedef struct Toolbar_t Toolbar_t;

View_t* Toolbar_Create(char* title, Font_t* font);

#ifdef __cplusplus
}
#endif

#endif  // UI_TOOLBAR_H