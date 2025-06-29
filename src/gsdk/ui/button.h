#ifndef GAME_SDK_BUTTON_H
#define GAME_SDK_BUTTON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "../types/types.h"

typedef struct Button_t Button_t;

typedef void (*ClickListener)(void);

Button_t* ButtonCreate(ClickListener clickListener);
void ButtonDestroy(Button_t* button);

void ButtonChangeSelection(Button_t* button, bool selected);

#ifdef __cplusplus
}
#endif

#endif  // GAME_SDK_BUTTON_H
