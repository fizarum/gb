#include "composer.h"

#include <stdio.h>
#include <stdlib.h>

#include "esp_log.h"
#include "views/box.h"

typedef struct Composer_t {
  uint16_t width;
  uint16_t height;

  Tree_t* root;
} Composer_t;

static void Composer_OnDrawItems(TreeNode_t* node);
static void OnRecomposeItem(TreeNode_t* node, Direction_t parentDirection,
                            _u16 left, _u16 top, _u16 right, _u16 bottom);

Composer_t* Composer_Create(const uint16_t screenWidth,
                            const uint16_t screenHeight) {
  Composer_t* composer = (Composer_t*)malloc(sizeof(Composer_t));

  if (composer == NULL) {
    return NULL;
  }

  composer->width = screenWidth;
  composer->height = screenHeight;
  composer->root = Tree_Create();
  _u16 id = Composer_GetRootId(composer);

  if (id != TREE_INDEX_NONE) {
    View_t* container = VBox_Create();
    if (container != NULL) {
      Box_t* box = (Box_t*)View_GetCustomView(container);
      bool set = Tree_SetNodeData(composer->root, id, container);
      if (set == true) {
        ESP_LOGI(
            "Composer", "root box [id: %d] orientation: [%s] is created", id,
            Box_GetDirection(box) == Horizontal ? "horizontal" : "vertical");
      } else {
        ESP_LOGW("Composer", "can't create root box!");
      }

      View_SetId(container, id);
      // set max sizes for box
      Box_SetMaxSize(box, screenWidth, screenHeight);
    }
  }

  return composer;
}

uint16_t Composer_GetRootId(const Composer_t* composer) {
  TreeNode_t* root = Tree_GetRoot(composer->root);

  return root != NULL ? TreeNode_GetId(root) : TREE_INDEX_NONE;
}

_u16 Composer_AddBox(Composer_t* composer, uint16_t branchId, View_t* view) {
  if (view == NULL) {
    return TREE_INDEX_NONE;
  }

  TreeNode_t* branch = Tree_FindNode(composer->root, branchId);
  if (branch == NULL || TreeNode_IsLeaf(branch) == true) {
    return TREE_INDEX_NONE;
  }

  uint16_t id = Tree_AddNode(composer->root, branch, false);
  if (id == TREE_INDEX_NONE) {
    return TREE_INDEX_NONE;
  }

  View_SetId(view, id);

  bool viewSet = Tree_SetNodeData(composer->root, id, view);

  if (viewSet == true) {
    // set max width and height for child box
    View_t* parentView = TreeNode_GetData(branch);
    Box_t* parentBox = (Box_t*)View_GetCustomView(parentView);
    Box_t* box = View_GetCustomView(view);

    // If parent have vertical direction, its max capacity by height is:
    // <parent's max height> - <parent's actual height>
    // and max capacity by width is: <parent's actual width>.
    //
    // For horizontal its the same but with opposite sides
    _u16 maxWidth = View_GetWidth(parentView);
    _u16 maxHeight = View_GetHeight(parentView);

    if (Box_IsDirectionVertical(parentBox)) {
      maxHeight = Box_GetFreeHeightSpace(parentBox);
      // in cases when parent box is empty and its actual width and/or height ==
      // 0 we need to take its max width/height
      if (maxWidth == 0) {
        maxWidth = Box_GetMaxWidth(parentBox);
      }
    } else {
      maxWidth = Box_GetFreeWidthSpace(parentBox);
      // in cases when parent box is empty and its actual width and/or height ==
      // 0 we need to take its max width/height
      if (maxHeight == 0) {
        maxHeight = Box_GetMaxHeight(parentBox);
      }
    }

    Box_SetMaxSize(box, maxWidth, maxHeight);
    _u16 xPos, yPos = 0;
    if (Box_IsDirectionVertical(parentBox)) {
      xPos = View_GetXPosition(parentView);
      yPos = View_GetYPosition(parentView) + View_GetHeight(parentView);
    } else {
      xPos = View_GetXPosition(parentView) + View_GetWidth(parentView);
      yPos = View_GetYPosition(parentView);
    }

    View_SetPosition(view, xPos, yPos);

    ESP_LOGI("Composer", "attached box [%d] to [%d] max w: %d | max h: %d", id,
             branchId, Box_GetMaxWidth(box), Box_GetMaxHeight(box));
  } else {
    ESP_LOGW("Composer", "can not attach box [%d] to [%d]", id, branchId);
  }
  return viewSet == true ? id : TREE_INDEX_NONE;
}

_u16 Composer_AddVBox(Composer_t* composer, _u16 branchId) {
  View_t* box = VBox_Create();
  return Composer_AddBox(composer, branchId, box);
}

_u16 Composer_AddHBox(Composer_t* composer, _u16 branchId) {
  View_t* box = HBox_Create();
  return Composer_AddBox(composer, branchId, box);
}

uint16_t Composer_AddView(Composer_t* composer, uint16_t branchId,
                          View_t* view) {
  if (view == NULL) {
    return TREE_INDEX_NONE;
  }

  TreeNode_t* branch = Tree_FindNode(composer->root, branchId);
  if (branch == NULL || TreeNode_IsLeaf(branch) == true) {
    return TREE_INDEX_NONE;
  }

  uint16_t id = Tree_AddNode(composer->root, branch, true);
  if (id == TREE_INDEX_NONE) {
    return TREE_INDEX_NONE;
  }

  View_SetId(view, id);

  bool viewSet = Tree_SetNodeData(composer->root, id, view);
  View_t* branchView = (View_t*)TreeNode_GetData(branch);

  ESP_LOGI("Composer",
           "created view with id : %d [parent:%d] is set : %s, new x: %d | y: "
           "%d, new width: %d | height: %d",
           id, branchId, viewSet == true ? "true" : "false",
           View_GetXPosition(view), View_GetYPosition(view),
           View_GetWidth(view), View_GetHeight(view));
  return viewSet == true ? id : TREE_INDEX_NONE;
}

void Composer_Recompose(Composer_t* composer) {
  TreeNode_t* root = Tree_GetRoot(composer->root);
  View_t* rootView = (View_t*)TreeNode_GetData(root);
  Box_t* rootBox = (Box_t*)View_GetCustomView(rootView);
  Direction_t rootDirection = Box_GetDirection(rootBox);

  OnRecomposeItem(root, rootDirection, 0, 0, composer->width, composer->height);
}

void Composer_Draw(Composer_t* composer) {
  Tree_Foreach(composer->root, Composer_OnDrawItems);
}

// private part

static void Composer_OnDrawItems(TreeNode_t* node) {
  _u16 nodeId = TreeNode_GetId(node);
  // possible temporary solution
  if (nodeId == 0) {
    return;
  }

  View_t* view = (View_t*)(TreeNode_GetData(node));
  if (view == NULL) {
    return;
  }

  ESP_LOGI("Compose", "called draw for node: %d", nodeId);
  View_Draw(view);
}

static void OnRecomposeItem(TreeNode_t* node, Direction_t parentDirection,
                            _u16 left, _u16 top, _u16 right, _u16 bottom) {
  View_t* view = TreeNode_GetData(node);
  // update current view position
  if (View_IsBox(view) == true) {
    View_SetPosition(view, left, top);
  } else {
    View_SetPosition(view, left + 1, top + 1);
  }

  printf(
      "[Composer] recomposing step, left: %d | top: %d | right: %d | bottom: "
      "%d\n",
      left, top, right, bottom);

  // get actual view size
  _u16 width = View_GetWidth(view);
  _u16 height = View_GetHeight(view);

  if (View_IsBox(view)) {
    Box_t* box = (Box_t*)View_GetCustomView(view);
    Direction_t direction = Box_GetDirection(box);
    Array_t* children = TreeNode_GetChildren(node);
    if (children != NULL) {
      for (_u8 index = 0; index < ArraySize(children); index++) {
        TreeNode_t* child = ArrayValueAt(children, index);
        if (direction == Vertical) {
          OnRecomposeItem(child, direction, left, top + height, right, bottom);
        } else {
          OnRecomposeItem(child, direction, left + width, top, right, bottom);
        }

        View_t* childView = (View_t*)TreeNode_GetData(child);
        _u16 childWidth = View_GetWidth(childView);
        _u16 childHeight = View_GetHeight(childView);
        // resize parent with new child's size
        if (direction == Vertical) {
          height += childHeight;
          // if child is wider than box - resize box to fit requirements
          if (childWidth > width) {
            width = childWidth;
          }
        } else {
          width += childWidth;
          // if child is taller than box - resize box to fit requirements
          if (childHeight > height) {
            height = childHeight;
          }
        }
        View_SetWidth(view, width);
        View_SetHeight(view, height);
      }
      Box_SetMaxSize(box, right - left, bottom - top);
      printf("[Composer] box [%d] width: %d | height: %d x: %d | y: %d\n",
             View_GetId(view), View_GetWidth(view), View_GetHeight(view),
             View_GetXPosition(view), View_GetYPosition(view));
    }
  } else {
    // TODO: optimize to pointers
    SizePolicy_t widthSizePolicy = View_GetWidthPolicy(view);
    SizePolicy_t heightSizePolicy = View_GetHeightPolicy(view);

    if (widthSizePolicy.type == MatchParent) {
      // TODO: recheck if size < 2
      width = right - left - 2;
      View_SetWidth(view, width);
    }

    if (heightSizePolicy.type == MatchParent) {
      // TODO: recheck if size < 2
      height = bottom - top - 2;
      View_SetHeight(view, height);
    }
    printf("[Composer] view[%d] width: %d | height: %d x: %d | y: %d\n",
           View_GetId(view), View_GetWidth(view), View_GetHeight(view),
           View_GetXPosition(view), View_GetYPosition(view));
  }
}