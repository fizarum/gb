#ifndef UI_BOX_H
#define UI_BOX_H

#ifdef __cplusplus
extern "C" {
#endif

#include <array.h>

#include "../direction.h"
#include "../view.h"

typedef struct Box_t Box_t;

View_t *HBox_Create();
View_t *VBox_Create();

View_t *Box_GetView(Box_t *box);
Direction_t Box_GetDirection(const Box_t *box);
bool Box_IsDirectionVertical(const Box_t *box);

void Box_SetMaxSize(Box_t *box, const _u16 width, const _u16 height);
_u16 Box_GetMaxWidth(Box_t *box);
_u16 Box_GetMaxHeight(Box_t *box);

// returns available (free) space in box
_u16 Box_GetFreeWidthSpace(Box_t *box);
_u16 Box_GetFreeHeightSpace(Box_t *box);

void Box_RecalculateSizeForNewView(Box_t *box, View_t *childView);

#ifdef __cplusplus
}
#endif

#endif  // UI_BOX_H