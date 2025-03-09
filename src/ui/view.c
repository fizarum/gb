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

  Padding_t padding;

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
  view->padding.left = 0;
  view->padding.right = 0;
  view->padding.top = 0;
  view->padding.bottom = 0;

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

Padding_t* View_GetPadding(View_t* view) { return &(view->padding); }

void View_SetPosition(View_t* view, _u16 x, _u16 y) {
  view->x = x;
  view->y = y;
}

_u16 View_GetXPosition(const View_t* view) { return view->x; }

_u16 View_GetContentXPosition(const View_t* view) {
  return view->x - view->padding.left;
}

_u16 View_GetYPosition(const View_t* view) { return view->y; }

_u16 View_GetContentYPosition(const View_t* view) {
  return view->y - view->padding.top;
}

_u16 View_GetWidth(const View_t* view) { return view->width; }

_u16 View_GetContentWidth(const View_t* view) {
  _u16 padding = view->padding.left + view->padding.right;
  return view->width > padding ? view->width - padding : 0;
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

_u16 View_GetContentHeight(const View_t* view) {
  _u16 padding = view->padding.top + view->padding.bottom;
  return view->height > padding ? view->height - padding : 0;
}

_u16 View_GetHeight(const View_t* view) { return view->height; }
void View_SetHeight(View_t* view, const _u16 height) {
  if (view->height == height) {
    return;
  }

  view->height = height;

  if (view->onSizeChangedCallback != NULL) {
    view->onSizeChangedCallback(view, view->width, view->height);
  }
}

void View_SetPadding(View_t* view, const _u16 l, const _u16 t, const _u16 r,
                     const _u16 b) {
  view->padding.left = l;
  view->padding.right = r;
  view->padding.top = t;
  view->padding.bottom = b;
}

void View_SetHPadding(View_t* view, const _u16 horizontal) {
  view->padding.left = horizontal;
  view->padding.right = horizontal;
}

void View_SetVPadding(View_t* view, const _u16 vertical) {
  view->padding.top = vertical;
  view->padding.bottom = vertical;
}

SizePolicy_t* View_GetWidthPolicy(View_t* view) { return &(view->widthPolicy); }
SizePolicy_t* View_GetHeightPolicy(View_t* view) {
  return &(view->heightPolicy);
}

void* View_GetCustomView(const View_t* view) { return view->customView; }

void View_SetUpdated(View_t* view) { view->needsRedraw = true; }
bool View_IsUpdated(const View_t* view) { return view->needsRedraw; }