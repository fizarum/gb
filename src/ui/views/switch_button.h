#ifndef UI_SWITCH_H
#define UI_SWITCH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../typography/font.h"
#include "../view.h"

typedef struct SwitchButton_t SwitchButton_t;
typedef void (*SwitchChangedCallback)(SwitchButton_t* button, _u8 on);

View_t* SwitchButton_Create(_u8 on, Font_t* font,
                            SwitchChangedCallback callback);
void SwitchButton_Toggle(View_t* button);
void SwitchButton_SetIsOn(View_t* view, _u8 on);
bool SwitchBuitton_IsOn(const View_t* button);

#ifdef __cplusplus
}
#endif

#endif  // UI_SWITCH_H