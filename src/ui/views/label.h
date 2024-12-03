#ifndef UI_LABEL_H
#define UI_LABEL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../typography/font.h"
#include "../view.h"

typedef struct Label_t Label_t;

View_t* Label_Create(char* text, Font_t* font);

#ifdef __cplusplus
}
#endif

#endif  // UI_LABEL_H