#ifndef UI_SWITCH_H
#define UI_SWITCH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../typography/font.h"
#include "../view.h"

typedef struct SwitchButton_t SwitchButton_t;

View_t* SwitchButton_Create(_u8 on, Font_t* font);
void SwitchButton_Toggle(SwitchButton_t* button);
bool SwitchBuitton_IsOn(const SwitchButton_t* button);

#ifdef __cplusplus
}
#endif

#endif  // UI_SWITCH_H