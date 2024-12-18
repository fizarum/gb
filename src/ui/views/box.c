#include "box.h"

#include <stdio.h>
#include <stdlib.h>

#include "../view.h"
#include "esp_log.h"

typedef struct Box_t {
  Direction_t direction;

  _u16 maxWidth;
  _u16 maxHeight;

  View_t *view;
} Box_t;

static View_t *Box_Create(const Direction_t direction);
static void Box_Destroy(void *boxArg);

View_t *HBox_Create() { return Box_Create(Horizontal); }
View_t *VBox_Create() { return Box_Create(Vertical); }

View_t *Box_GetView(Box_t *box) {
  if (box == NULL) {
    return NULL;
  }
  return box->view;
}

Direction_t Box_GetDirection(const Box_t *box) { return box->direction; }

bool Box_IsDirectionVertical(const Box_t *box) {
  return box->direction == Vertical;
}

void Box_SetMaxSize(Box_t *box, const _u16 width, const _u16 height) {
  box->maxWidth = width;
  box->maxHeight = height;
}

_u16 Box_GetMaxWidth(Box_t *box) { return box->maxWidth; }

_u16 Box_GetMaxHeight(Box_t *box) { return box->maxHeight; }

_u16 Box_GetFreeWidthSpace(Box_t *box) {
  _u16 width = View_GetWidth(box->view);
  _u16 maxWidth = box->maxWidth;
  return width < maxWidth ? maxWidth - width : 0;
}

_u16 Box_GetFreeHeightSpace(Box_t *box) {
  _u16 height = View_GetHeight(box->view);
  _u16 maxHeight = box->maxHeight;
  return height < maxHeight ? maxHeight - height : 0;
}

void Box_RecalculateSizeForNewView(Box_t *box, View_t *childView) {
  _u16 childViewWidth = View_GetWidth(childView);
  _u16 childViewHeight = View_GetHeight(childView);

  View_t *boxView = box->view;
  _u16 boxViewWidth = View_GetWidth(boxView);
  _u16 boxViewHeight = View_GetHeight(boxView);
  _u16 boxViewTopPos = View_GetYPosition(boxView);
  _u16 boxViewLeftPos = View_GetXPosition(boxView);

  // normalize child view sizes according to max sizes of hosting box
  if (childViewWidth > box->maxWidth) {
    childViewWidth = box->maxWidth;
    View_SetWidth(childView, childViewWidth);
  }

  if (childViewHeight > box->maxHeight) {
    childViewHeight = box->maxHeight;
    View_SetHeight(childView, childViewHeight);
  }

  SizePolicyType_t widthPolicy = View_GetWidthPolicy(childView).type;
  SizePolicyType_t heightPolicy = View_GetHeightPolicy(childView).type;

  if (widthPolicy == MatchParent) {
    childViewWidth =
        box->maxWidth - boxViewWidth - 2;  // 2 x 1 px of line width
    View_SetWidth(childView, childViewWidth);
    View_SetWidth(boxView, box->maxWidth);
  } else if (widthPolicy == WrapContent) {
    _u16 freeWidthSpaceInBox = Box_GetFreeWidthSpace(box);

    // if box's width with view is bigger that its max width
    if (childViewWidth > freeWidthSpaceInBox) {
      childViewWidth = freeWidthSpaceInBox - 2;  // 2 x 1 px of line width
      // child view exceeds box max size, resize child
      View_SetWidth(boxView, box->maxWidth);
    } else {
      // stretch box with new view
      View_SetWidth(boxView, boxViewWidth + childViewWidth);
    }
    View_SetWidth(childView, childViewWidth);
  } else {
    // not handled yet
  }

  if (heightPolicy == MatchParent) {
    childViewHeight =
        box->maxHeight - boxViewHeight - 2;  // 2 x 1 px of line width
    View_SetHeight(childView, childViewHeight);
    View_SetHeight(boxView, box->maxHeight);
  } else if (heightPolicy == WrapContent) {
    _u16 freeHeightSpaceInBox = Box_GetFreeHeightSpace(box);

    // if box's width with view is bigger that its max width
    if (childViewHeight > freeHeightSpaceInBox) {
      // child view exceeds box max size, resize child
      childViewHeight = freeHeightSpaceInBox - 2;  // 2 x 1 px of line width
      View_SetHeight(boxView, box->maxHeight);
    } else {
      // stretch box with new view
      View_SetHeight(boxView, boxViewHeight + childViewHeight);
    }
    View_SetHeight(childView, childViewHeight);
  } else {
    // not handled yet
  }

  if (box->direction == Horizontal) {
    // update view position to the end of box's width
    View_SetPosition(childView, boxViewWidth, boxViewTopPos);

    // we need to update overal box height according to most tall view
    if (boxViewHeight < childViewHeight) {
      View_SetHeight(boxView, childViewHeight);
    }
  } else {
    // update view position in box: left bottom
    View_SetPosition(childView, boxViewLeftPos, boxViewTopPos + boxViewHeight);

    // we need to update overal box width according to most wide widget
    if (boxViewWidth < childViewWidth) {
      View_SetWidth(boxView, childViewWidth);
    }
  }
  ESP_LOGI("Box",
           "recalculated size for %s box [%d], width: %d | height: %d (max "
           "width: %d | max height: %d)",
           box->direction == Horizontal ? "horizontal" : "vertical",
           View_GetId(boxView), View_GetWidth(boxView), View_GetHeight(boxView),
           Box_GetMaxWidth(box), Box_GetMaxHeight(box));
}

// private part

static View_t *Box_Create(const Direction_t direction) {
  Box_t *box = (Box_t *)malloc(sizeof(Box_t));
  if (box == NULL) {
    return NULL;
  }

  box->direction = direction;
  SizePolicy_t sizePolicy = {.type = MatchParent, .weight = 0};

  box->view =
      View_Create(box, true, NULL, &Box_Destroy, NULL, sizePolicy, sizePolicy);

  return box->view;
}

static void Box_Destroy(void *boxArg) {
  if (boxArg == NULL) {
    return;
  }

  Box_t *box = (Box_t *)boxArg;
  // TODO: notify tree that children should be destroyed as well
  free(box);
}