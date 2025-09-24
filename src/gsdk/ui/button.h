#ifndef GAME_SDK_BUTTON_H
#define GAME_SDK_BUTTON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "../types/types.h"

typedef struct Button Button;

typedef void (*ClickListener)(void);

Button* ButtonCreate(ClickListener clickListener);
void ButtonDestroy(Button* button);

void ButtonChangeSelection(Button* button, bool selected);

#ifdef __cplusplus
}
#endif

#endif  // GAME_SDK_BUTTON_H
