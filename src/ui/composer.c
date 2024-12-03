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
    View_t* container = VBox_Create(0, 0);
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
    _u16 maxWidth = 0;
    _u16 maxHeight = 0;
    if (Box_GetDirection(parentBox) == Vertical) {
      maxHeight = Box_GetMaxHeight(parentBox) - View_GetHeight(parentView);
      maxWidth = View_GetWidth(parentView);
    } else {
      maxWidth = Box_GetMaxWidth(parentBox) - View_GetWidth(parentView);
      maxHeight = View_GetHeight(parentView);
    }

    Box_SetMaxSize(box, maxWidth, maxHeight);

    ESP_LOGI("Composer", "attached box [%d] to [%d] max w: %d | max h: %d", id,
             branchId, Box_GetMaxWidth(box), Box_GetMaxHeight(box));
  } else {
    ESP_LOGW("Composer", "can not attach box [%d] to [%d]", id, branchId);
  }
  return viewSet == true ? id : TREE_INDEX_NONE;
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
  Box_t* box = (Box_t*)View_GetCustomView(branchView);

  Box_RecalculateSizeForNewView(box, view);

  ESP_LOGI("Composer",
           "created view with id : %d [parent:%d] is set : %s, new x: %d | y: "
           "%d, new width: %d | height: %d",
           id, branchId, viewSet == true ? "true" : "false",
           View_GetXPosition(view), View_GetYPosition(view),
           View_GetWidth(view), View_GetHeight(view));
  return viewSet == true ? id : TREE_INDEX_NONE;
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