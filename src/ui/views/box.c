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

static View_t *Box_Create(_u16 x, _u16 y, const Direction_t direction);
static void Box_Destroy(void *boxArg);

View_t *HBox_Create(_u16 x, _u16 y) { return Box_Create(x, y, Horizontal); }

View_t *VBox_Create(_u16 x, _u16 y) { return Box_Create(x, y, Vertical); }

View_t *Box_GetView(Box_t *box) {
  if (box == NULL) {
    return NULL;
  }
  return box->view;
}

Direction_t Box_GetDirection(const Box_t *box) { return box->direction; }

void Box_SetMaxSize(Box_t *box, const _u16 width, const _u16 height) {
  box->maxWidth = width;
  box->maxHeight = height;
}

_u16 Box_GetMaxWidth(Box_t *box) { return box->maxWidth; }

_u16 Box_GetMaxHeight(Box_t *box) { return box->maxHeight; }

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

  if (box->direction == Horizontal) {
    // update view position to the end of box's width
    View_SetPosition(childView, boxViewWidth, boxViewTopPos);

    // we need to update overal box height according to most tall view
    if (boxViewHeight < childViewHeight) {
      View_SetHeight(boxView, childViewHeight);
    }
    _u16 newBoxWidth = boxViewWidth + childViewWidth;

    if (newBoxWidth > box->maxWidth) {
      childViewWidth -= (newBoxWidth - box->maxWidth);
      // child view exceeds box max size, resize child
      View_SetWidth(childView, childViewWidth);
    }

    // and stretch box with new view
    View_SetWidth(boxView, boxViewWidth + childViewWidth);
  } else {
    // update view position in box: left bottom
    View_SetPosition(childView, boxViewLeftPos, boxViewTopPos + boxViewHeight);

    // we need to update overal box width according to most wide widget
    if (boxViewWidth < childViewWidth) {
      View_SetWidth(boxView, childViewWidth);
    }

    _u16 newBoxHeight = boxViewHeight + childViewHeight;

    if (newBoxHeight > box->maxHeight) {
      childViewHeight = box->maxHeight - boxViewHeight;
      // child view exceeds box max size, resize child
      View_SetHeight(childView, childViewHeight);
    }

    // and stretch box with new view
    View_SetHeight(boxView, boxViewHeight + childViewHeight);
  }
  ESP_LOGI("Box",
           "recalculated size for %s box [%d], width: %d | height: %d (max "
           "width: %d | max height: %d)",
           box->direction == Horizontal ? "horizontal" : "vertical",
           View_GetId(boxView), View_GetWidth(boxView), View_GetHeight(boxView),
           Box_GetMaxWidth(box), Box_GetMaxHeight(box));
}

// private part

static void Box_Draw(View_t *view, const _u16 left, const _u16 top,
                     const _u16 right, const _u16 bottom) {
  if (view == NULL) {
    ESP_LOGW("Box", "box is empty, skip draw call");
  } else {
    ESP_LOGI("Box", "box draw called");
  }
}

static View_t *Box_Create(_u16 x, _u16 y, const Direction_t direction) {
  Box_t *box = (Box_t *)malloc(sizeof(Box_t));
  if (box == NULL) {
    return NULL;
  }

  box->direction = direction;
  box->view = View_Create(box, &Box_Draw, &Box_Destroy, NULL);
  View_SetPosition(box->view, x, y);

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