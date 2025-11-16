#include "list_focus.h"

#include <stddef.h>
#include <stdlib.h>

#include "../gfx/gfx.h"
#include "esp_log.h"

typedef struct ListFocus_t {
  _u8 items;
  _u8 selectedItemIndex;
  _u16 itemHeight;

  View_t* view;
} ListFocus_t;

static void RecalculateSize(ListFocus_t* listFocus);
static void Draw(View_t* view, const _u16 left, const _u16 top,
                 const _u16 right, const _u16 bottom);
static void HandleInput(View_t* view, InputEvent* event);
static void Destroy(void* listFocusArg);
static void OnResize(View_t* view, const _u16 width, const _u16 height);

View_t* ListFocus_Create(_u8 items) {
  ListFocus_t* listFocus = (ListFocus_t*)malloc(sizeof(ListFocus_t));
  if (listFocus == NULL) {
    return NULL;
  }

  listFocus->items = items;
  listFocus->selectedItemIndex = 0;

  SizePolicy_t widthSizePolicy = {.type = WrapContent, .weight = 0};
  SizePolicy_t heightSizePolicy = {.type = MatchParent, .weight = 0};

  listFocus->view = View_Create(listFocus, false, &Draw, &HandleInput, &Destroy,
                                &OnResize, widthSizePolicy, heightSizePolicy);

  RecalculateSize(listFocus);

  return listFocus->view;
}

// private part
static const _u8 width = 20;
static const _u8 mediumPadding = 5;
static const _u8 indicatorWidth = 6;

static void RecalculateSize(ListFocus_t* listFocus) {
  View_SetWidth(listFocus->view, width);
  View_SetHeight(listFocus->view, 1);
}

void ListFocus_SelectItemIndex(View_t* view, _u8 index) {
  ListFocus_t* listFocus = (ListFocus_t*)View_GetCustomView(view);
  if (index < listFocus->items) {
    listFocus->selectedItemIndex = index;
    View_SetUpdated(view);
  }
}

_u8 ListFocus_GetSelectedItemIndex(View_t* view) {
  ListFocus_t* listFocus = (ListFocus_t*)View_GetCustomView(view);
  return listFocus->selectedItemIndex;
}

// private part

static void Draw(View_t* view, const _u16 left, const _u16 top,
                 const _u16 right, const _u16 bottom) {
  GFX_DrawFilledRect(left, top, right, bottom, GFX_GetTheme()->backgroundColor);

  ListFocus_t* focus = (ListFocus_t*)View_GetCustomView(view);

  // indicator
  _u16 topPadding = focus->itemHeight * focus->selectedItemIndex;

  _u16 leftPos = left + mediumPadding;
  _u16 topPos = top + topPadding;
  _u16 bottomPos = topPos + focus->itemHeight;

  GFX_DrawFilledRect(leftPos,
                     /*top*/ topPos,
                     /*right*/ leftPos + indicatorWidth,
                     /*bottom*/ bottomPos, GFX_GetFontColor());
}

static void HandleInput(View_t* view, InputEvent* event) {
  // handling release events only
  if (event == NULL || event->type != Released) {
    return;
  }

  ListFocus_t* focusView = (ListFocus_t*)View_GetCustomView(view);

  switch (event->keycode) {
    case KEY_UP:
      focusView->selectedItemIndex--;
      if (focusView->selectedItemIndex >= focusView->items) {
        focusView->selectedItemIndex = focusView->items - 1;
      }
      View_SetUpdated(view);
      break;

    case KEY_DOWN:
      focusView->selectedItemIndex++;
      if (focusView->selectedItemIndex >= focusView->items) {
        focusView->selectedItemIndex = 0;
      }
      View_SetUpdated(view);
      break;

    default:
      ESP_LOGW("Focus", "handle input type: %d not yet implemented!",
               event->keycode);
      break;
  }
}

static void Destroy(void* listFocusArg) {
  if (listFocusArg == NULL) {
    return;
  }

  ListFocus_t* listFocus = (ListFocus_t*)listFocusArg;

  free(listFocus);
}

static void OnResize(View_t* view, const _u16 width, const _u16 height) {
  if (height == 0) {
    return;
  }

  ListFocus_t* focus = (ListFocus_t*)View_GetCustomView(view);
  focus->itemHeight = height / (focus->items);
  ESP_LOGI("Focus", "[resize] view height: %d item height: %d", height,
           focus->itemHeight);
  View_SetUpdated(view);
}