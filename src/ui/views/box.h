#ifndef UI_BOX_H
#define UI_BOX_H

#ifdef __cplusplus
extern "C" {
#endif

#include <array.h>

#include "../view.h"

typedef enum Direction_t {
  Horizontal,
  Vertical,
} Direction_t;

typedef struct Box_t Box_t;

View_t *HBox_Create(_u16 x, _u16 y);
View_t *VBox_Create(_u16 x, _u16 y);

View_t *Box_GetView(Box_t *box);
Direction_t Box_GetDirection(const Box_t *box);

void Box_SetMaxSize(Box_t *box, const _u16 width, const _u16 height);
_u16 Box_GetMaxWidth(Box_t *box);
_u16 Box_GetMaxHeight(Box_t *box);

void Box_RecalculateSizeForNewView(Box_t *box, View_t *childView);

#ifdef __cplusplus
}
#endif

#endif  // UI_BOX_H