#include "box.h"

#include <stdlib.h>

#include "node.h"

static const uint8_t Box_MaxNodes = 5;

typedef struct Box_t {
  Node_t *node;
  Direction_t direction;
  Array_t *nodes;
} Box_t;

static Box_t *Box_Create(const uint16_t id, const Direction_t direction,
                         uint16_t x, uint16_t y);

Box_t *HBox_Create(const uint16_t id, uint16_t x, uint16_t y) {
  return Box_Create(id, Horizontal, x, y);
}

Box_t *VBox_Create(const uint16_t id, uint16_t x, uint16_t y) {
  return Box_Create(id, Vertical, x, y);
}

bool Box_AddNode(Box_t *box, Node_t *node) {
  if (ArraySize(box->nodes) >= Box_MaxNodes) {
    return false;
  }

  uint16_t widgetHeight = Node_GetHeight(node);
  uint16_t widgetWidth = Node_GetWidth(node);
  uint16_t oldBoxWidth = Node_GetWidth(box->node);
  uint16_t oldBoxHeight = Node_GetHeight(box->node);

  if (box->direction == Horizontal) {
    Node_SetWidth(box->node, oldBoxWidth + widgetWidth);

    // update widget position in box
    Node_SetPosition(node, oldBoxWidth, Node_GetYPosition(box->node));
    //  we need to update overal box height according to most tall widget
    if (oldBoxHeight < widgetHeight) {
      Node_SetHeight(box->node, widgetHeight);
    }
  } else {
    Node_SetHeight(box->node, oldBoxHeight + widgetHeight);
    // update widget position in box
    Node_SetPosition(node, Node_GetXPosition(box->node), oldBoxHeight);
    // we need to update overal box width according to most wide widget
    if (oldBoxWidth < widgetWidth) {
      Node_SetWidth(box->node, widgetHeight);
    }
  }

  return ArrayAdd(box->nodes, node);
}

// part of private implementation

static Box_t *Box_Create(const uint16_t id, const Direction_t direction,
                         uint16_t x, uint16_t y) {
  Box_t *box = (Box_t *)malloc(sizeof(Box_t));

  if (box == NULL) {
    return NULL;
  }

  box->node = Node_Create(id);
  Node_SetPosition(box->node, x, y);

  box->nodes = ArrayCreate(Box_MaxNodes);
  box->direction = direction;

  return box;
}