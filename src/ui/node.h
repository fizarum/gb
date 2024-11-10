#ifndef UI_NODE_H
#define UI_NODE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct Node_t Node_t;

Node_t *Node_Create(const uint16_t id);
void Node_Destroy(Node_t *node);

void Node_SetPosition(Node_t *node, uint16_t x, uint16_t y);

uint16_t Node_GetWidth(const Node_t *node);
uint16_t Node_GetHeight(const Node_t *node);

void Node_SetWidth(Node_t *node, const uint16_t width);
void Node_SetHeight(Node_t *node, const uint16_t height);

uint16_t Node_GetXPosition(const Node_t *node);
uint16_t Node_GetYPosition(const Node_t *node);

#ifdef __cplusplus
}
#endif

#endif  // UI_NODE_H