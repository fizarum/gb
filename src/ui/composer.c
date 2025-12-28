#include "composer.h"

#include <stdio.h>
#include <stdlib.h>

#include "esp_log.h"
#include "views/box.h"

typedef struct Composer {
  _u16 width;
  _u16 height;

  Tree_t* uiTree;
} Composer;

static Composer composer;

static void OnDrawItem(TreeNode_t* node);
static void OnRecomposeItem(TreeNode_t* node, Direction_t parentDirection,
                            _u16 left, _u16 top, _u16 right, _u16 bottom);
static void OnClearItem(TreeNode_t* node);

static inline View_t* GetViewFromNode(TreeNode_t* node) {
  return (View_t*)TreeNode_GetData(node);
}

void Composer_Init(const _u16 width, const _u16 height) {
  composer.width = width;
  composer.height = height;
  composer.uiTree = Tree_Create();

  _u16 id = Composer_GetRootId();
  if (id != TREE_INDEX_NONE) {
    View_t* container = VBox_Create();
    if (container != NULL) {
      Box_t* box = (Box_t*)View_GetCustomView(container);
      bool set = Tree_SetNodeData(composer.uiTree, id, container);
      if (set == true) {
        ESP_LOGI(
            "Composer", "root box [id: %d] orientation: [%s] is created", id,
            Box_GetDirection(box) == Horizontal ? "horizontal" : "vertical");
      } else {
        ESP_LOGW("Composer", "can't create root box!");
      }

      View_SetId(container, id);
      // set max sizes for box
      Box_SetMaxSize(box, width, height);
    }
  }
}

_u16 Composer_GetRootId() {
  TreeNode_t* root = Tree_GetRoot(composer.uiTree);

  return root != NULL ? TreeNode_GetId(root) : TREE_INDEX_NONE;
}

_u16 Composer_AddBox(_u16 branchId, View_t* view) {
  if (view == NULL) {
    return TREE_INDEX_NONE;
  }

  TreeNode_t* branch = Tree_FindNode(composer.uiTree, branchId);
  if (branch == NULL || TreeNode_IsLeaf(branch) == true) {
    return TREE_INDEX_NONE;
  }

  _u16 id = Tree_AddNode(composer.uiTree, branch, false);
  if (id == TREE_INDEX_NONE) {
    return TREE_INDEX_NONE;
  }

  View_SetId(view, id);

  bool viewSet = Tree_SetNodeData(composer.uiTree, id, view);

  if (viewSet == true) {
    // set max width and height for child box
    View_t* parentView = GetViewFromNode(branch);
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

_u16 Composer_AddVBox(_u16 parentId) {
  return Composer_AddBox(parentId, VBox_Create());
}

_u16 Composer_AddHBox(_u16 parentId) {
  return Composer_AddBox(parentId, HBox_Create());
}

_u16 Composer_AddView(_u16 parentId, View_t* view) {
  if (view == NULL) {
    return TREE_INDEX_NONE;
  }

  TreeNode_t* branch = Tree_FindNode(composer.uiTree, parentId);
  if (branch == NULL || TreeNode_IsLeaf(branch) == true) {
    return TREE_INDEX_NONE;
  }

  _u16 id = Tree_AddNode(composer.uiTree, branch, true);
  if (id == TREE_INDEX_NONE) {
    return TREE_INDEX_NONE;
  }

  View_SetId(view, id);

  bool viewSet = Tree_SetNodeData(composer.uiTree, id, view);
  return viewSet == true ? id : TREE_INDEX_NONE;
}

View_t* Composer_FindView(const _u16 viewId) {
  TreeNode_t* node = Tree_FindNode(composer.uiTree, viewId);
  return GetViewFromNode(node);
}

void Composer_Recompose() {
  TreeNode_t* root = Tree_GetRoot(composer.uiTree);
  View_t* rootView = GetViewFromNode(root);
  Box_t* rootBox = (Box_t*)View_GetCustomView(rootView);
  Direction_t rootDirection = Box_GetDirection(rootBox);

  OnRecomposeItem(root, rootDirection, 0, 0, composer.width, composer.height);
}

void Composer_Draw() { Tree_Foreach(composer.uiTree, OnDrawItem); }

void Composer_Clear() {
  Tree_Destroy(composer.uiTree, OnClearItem);
  composer.uiTree = NULL;
}

// private part

static void OnDrawItem(TreeNode_t* node) {
  View_t* view = GetViewFromNode(node);
  if (view == NULL || View_IsBox(view)) {
    return;
  }

  View_Draw(view);
}

static void OnRecomposeItem(TreeNode_t* node, Direction_t parentDirection,
                            _u16 left, _u16 top, _u16 right, _u16 bottom) {
  View_t* view = GetViewFromNode(node);
  if (view == NULL) {
    ESP_LOGW("Composer", "view is null, skip step");
    return;
  }
  View_SetPosition(view, left, top);

  // get actual view size
  _u16 width = View_GetWidth(view);
  _u16 height = View_GetHeight(view);

  Padding_t* padding = View_GetPadding(view);

  if (View_IsBox(view)) {
    Box_t* box = (Box_t*)View_GetCustomView(view);
    Direction_t direction = Box_GetDirection(box);
    Array_t* children = TreeNode_GetChildren(node);

    for (_u8 index = 0; index < ArraySize(children); index++) {
      TreeNode_t* child = ArrayValueAt(children, index);
      if (direction == Vertical) {
        // TODO: recheck padding here
        OnRecomposeItem(child, direction,
                        // left
                        left + padding->left,
                        //  top
                        top + height,
                        // right
                        right - padding->right,
                        // bottom
                        bottom);
      } else {
        // TODO: recheck padding here
        OnRecomposeItem(child, direction,
                        // left
                        left + width,
                        // top
                        top + padding->top,
                        // right
                        right,
                        // bottom
                        bottom);
      }

      View_t* childView = GetViewFromNode(child);
      _u16 childWidth = childView != NULL ? View_GetWidth(childView) : 0;
      _u16 childHeight = childView != NULL ? View_GetHeight(childView) : 0;
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
  } else {
    SizePolicy* widthSizePolicy = View_GetWidthPolicy(view);
    SizePolicy* heightSizePolicy = View_GetHeightPolicy(view);

    if (widthSizePolicy->custom == 1) {
      if (widthSizePolicy->type == MatchParent) {
        width = right > left ? right - left : 0;
        View_SetWidth(view, width);
      }
    } else {
      View_SetWidth(view, widthSizePolicy->value);
    }

    if (heightSizePolicy->custom == 1) {
      if (heightSizePolicy->type == MatchParent) {
        height = bottom > top ? bottom - top : 0;
        View_SetHeight(view, height);
      }
    } else {
      View_SetHeight(view, heightSizePolicy->value);
    }
  }
}

static void OnClearItem(TreeNode_t* node) {
  View_t* view = GetViewFromNode(node);
  if (view != NULL) {
    View_Destroy(view);
  }
}