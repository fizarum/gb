#ifndef UI_BOX_H
#define UI_BOX_H

#ifdef __cplusplus
extern "C" {
#endif

#include <array.h>

#include "node.h"

typedef enum Direction_t {
  Horizontal,
  Vertical,
} Direction_t;

typedef struct Box_t Box_t;

Box_t *HBox_Create(const uint16_t id, uint16_t x, uint16_t y);
Box_t *VBox_Create(const uint16_t id, uint16_t x, uint16_t y);

//..
bool Box_AddNode(Box_t *box, Node_t *node);

#ifdef __cplusplus
}
#endif

#endif  // UI_BOX_H