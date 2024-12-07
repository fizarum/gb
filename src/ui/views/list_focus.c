#include "list_focus.h"

#include <stddef.h>
#include <stdlib.h>

#include "../gfx/gfx.h"
#include "esp_log.h"

typedef struct ListFocus_t {
  _u8 items;
  _u8 selectedItem;
  _u16 itemHeight;

  View_t *view;
} ListFocus_t;

static void ListFocus_RecalculateSize(ListFocus_t *listFocus);
static void ListFocus_Draw(View_t *view, const _u16 left, const _u16 top,
                           const _u16 right, const _u16 bottom);
static void ListFocus_Destroy(void *listFocusArg);
static void OnResize(View_t *view, const _u16 width, const _u16 height);

View_t *ListFocus_Create(_u8 items) {
  ListFocus_t *listFocus = (ListFocus_t *)malloc(sizeof(ListFocus_t));
  if (listFocus == NULL) {
    return NULL;
  }

  listFocus->items = items;
  listFocus->selectedItem = 0;

  SizePolicy_t widthSizePolicy = {.type = WrapContent, .weight = 0};
  SizePolicy_t heightSizePolicy = {.type = MatchParent, .weight = 0};

  listFocus->view = View_Create(listFocus, &ListFocus_Draw, &ListFocus_Destroy,
                                &OnResize, widthSizePolicy, heightSizePolicy);

  ListFocus_RecalculateSize(listFocus);

  return listFocus->view;
}

// private part
static const _u8 width = 20;
static const _u8 mediumPadding = 5;
static const _u8 indicatorWidth = 6;

static void ListFocus_RecalculateSize(ListFocus_t *listFocus) {
  View_SetWidth(listFocus->view, width);
  View_SetHeight(listFocus->view, 1);
}

void ListFocus_SelectItemIndex(View_t *view, _u8 index) {
  ListFocus_t *listFocus = (ListFocus_t *)View_GetCustomView(view);
  if (index < listFocus->items) {
    listFocus->selectedItem = index;
    View_SetUpdated(view);
  }
}

// private part

static void ListFocus_Draw(View_t *view, const _u16 left, const _u16 top,
                           const _u16 right, const _u16 bottom) {
  GFXDrawFilledRect(left, right, top, bottom, GFX_GetBackgroundColor());

  ListFocus_t *focus = (ListFocus_t *)View_GetCustomView(view);

  // indicator
  _u16 topPadding = focus->itemHeight * focus->selectedItem;

  _u16 leftPos = left + mediumPadding;
  _u16 topPos = top + topPadding;
  _u16 bottomPos = topPos + focus->itemHeight;

  GFXDrawFilledRect(leftPos,
                    /*right*/ leftPos + indicatorWidth,
                    /*top*/ topPos,
                    /*bottom*/ bottomPos, GFXGetFontColor());
}

static void ListFocus_Destroy(void *listFocusArg) {
  if (listFocusArg == NULL) {
    return;
  }

  ListFocus_t *listFocus = (ListFocus_t *)listFocusArg;

  free(listFocus);
}

static void OnResize(View_t *view, const _u16 width, const _u16 height) {
  if (height == 0) {
    return;
  }

  ListFocus_t *focus = (ListFocus_t *)View_GetCustomView(view);
  focus->itemHeight = height / (focus->items);
  ESP_LOGI("Focus", "[resize] view height: %d item height: %d", height,
           focus->itemHeight);
  View_SetUpdated(view);
}