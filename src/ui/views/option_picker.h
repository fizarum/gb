#ifndef UI_OPTION_PICKER_H
#define UI_OPTION_PICKER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <array.h>

#include "../typography/font.h"
#include "../view.h"

typedef struct OptionPicker_t OptionPicker_t;
typedef void (*OptionChangedCallback)(OptionPicker_t* picker, void* option);

View_t* OptionPicker_Create(Array_t* options, Font_t* font,
                            OptionChangedCallback callback);

#ifdef __cplusplus
}
#endif

#endif  // UI_OPTION_PICKER_H