#include "node.h"

#include <stdlib.h>

/**
 * @brief Basic structure for ui component
 */
typedef struct Node_t {
  uint16_t id;

  uint16_t x;
  uint16_t y;

  uint16_t width;
  uint16_t height;
} Node_t;

Node_t *Node_Create(const uint16_t id) {
  Node_t *node = (Node_t *)malloc(sizeof(Node_t));
  if (node == NULL) {
    return NULL;
  }

  node->id = id;
  node->x = 0;
  node->y = 0;

  node->width = 0;
  node->height = 0;

  return node;
}

void Node_Destroy(Node_t *node) {
  if (node == NULL) {
    return;
  }

  free(node);
}

void Node_SetPosition(Node_t *node, uint16_t x, uint16_t y) {
  node->x = x;
  node->y = y;
}

uint16_t Node_GetWidth(const Node_t *node) { return node->width; }
uint16_t Node_GetHeight(const Node_t *node) { return node->height; }

void Node_SetWidth(Node_t *node, const uint16_t width) { node->width = width; }

void Node_SetHeight(Node_t *node, const uint16_t height) {
  node->height = height;
}

uint16_t Node_GetXPosition(const Node_t *node) { return node->x; }
uint16_t Node_GetYPosition(const Node_t *node) { return node->y; }
