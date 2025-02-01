#include "view.h"

#include <array.h>
#include <stdlib.h>
#include <string.h>

#define VIEW_ID_NONE UINT16_MAX

/**
 * @brief Basic structure for ui component
 */
typedef struct View_t {
  _u16 id;

  bool isBox;

  SizePolicy_t widthPolicy;
  SizePolicy_t heightPolicy;
  _u16 width;
  _u16 height;

  _u16 x;
  _u16 y;

  // indicates if view should be redrawn next View_Draw() call
  bool needsRedraw;

  void* customView;

  DrawCallback drawCallback;
  ViewCallback destroyCallback;
  ViewSizeChangedCallback onSizeChangedCallback;
} View_t;

View_t* View_Create(void* customView, bool isBox, DrawCallback drawCallback,
                    ViewCallback onDestroyCallback,
                    ViewSizeChangedCallback onSizeChangedCallback,
                    const SizePolicy_t widthPolicy,
                    const SizePolicy_t heightPolicy) {
  View_t* view = (View_t*)malloc(sizeof(View_t));
  if (view == NULL) {
    return NULL;
  }

  view->id = VIEW_ID_NONE;
  view->customView = customView;
  view->drawCallback = drawCallback;
  view->destroyCallback = onDestroyCallback;
  view->onSizeChangedCallback = onSizeChangedCallback;
  view->isBox = isBox;
  view->x = 0;
  view->y = 0;
  view->width = 0;
  view->height = 0;
  view->needsRedraw = true;
  view->widthPolicy = widthPolicy;
  view->heightPolicy = heightPolicy;

  return view;
}

void View_Destroy(View_t* view) {
  if (view == NULL) {
    return;
  }

  if (view->destroyCallback != NULL) {
    view->destroyCallback(view->customView);
  }

  free(view);
}

void View_Draw(View_t* view) {
  if (view == NULL || view->drawCallback == NULL) {
    return;
  }

  if (view->needsRedraw == false) {
    return;
  }

  view->needsRedraw = false;

  view->drawCallback(view,
                     /*left*/ view->x,
                     /*top*/ view->y,
                     /*right*/ view->x + view->width,
                     /*bottom*/ view->y + view->height);
}

_u16 View_GetId(View_t* view) { return view->id; }
void View_SetId(View_t* view, _u16 id) { view->id = id; }

bool View_IsBox(View_t* view) { return view->isBox; }

void View_SetPosition(View_t* view, _u16 x, _u16 y) {
  view->x = x;
  view->y = y;
}

_u16 View_GetXPosition(const View_t* view) { return view->x; }
_u16 View_GetYPosition(const View_t* view) { return view->y; }

_u16 View_GetWidth(const View_t* view) { return view->width; }
_u16 View_GetHeight(const View_t* view) { return view->height; }

SizePolicy_t* View_GetWidthPolicy(View_t* view) { return &(view->widthPolicy); }
SizePolicy_t* View_GetHeightPolicy(View_t* view) {
  return &(view->heightPolicy);
}

void View_SetWidth(View_t* view, const _u16 width) {
  if (view->width == width) {
    return;
  }

  view->width = width;

  if (view->onSizeChangedCallback != NULL) {
    view->onSizeChangedCallback(view, view->width, view->height);
  }
}

void View_SetHeight(View_t* view, const _u16 height) {
  if (view->height == height) {
    return;
  }

  view->height = height;

  if (view->onSizeChangedCallback != NULL) {
    view->onSizeChangedCallback(view, view->width, view->height);
  }
}

void* View_GetCustomView(const View_t* view) { return view->customView; }

void View_SetUpdated(View_t* view) { view->needsRedraw = true; }
bool View_IsUpdated(const View_t* view) { return view->needsRedraw; }