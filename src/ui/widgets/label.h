#ifndef UI_LABEL_H
#define UI_LABEL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../node.h"
#include "../typography/font.h"

typedef struct Label_t Label_t;

Label_t* Label_Create(const uint16_t id, char* text, Font_t* font);
void Label_Destroy(Label_t* label);

Node_t* Label_GetNode(Label_t* label);

void Label_Draw(const Label_t* label);

#ifdef __cplusplus
}
#endif

#endif  // UI_LABEL_H