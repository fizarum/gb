#include "toolbar.h"

#include <stdlib.h>

#include "../gfx/gfx.h"

typedef struct Toolbar_t {
  char *title;
  Font_t *font;

  View_t *view;
} Toolbar_t;

static void Toolbar_RecalculateSize(Toolbar_t *toolbar);
static void Toolbar_Draw(View_t *view, const _u16 left, const _u16 top,
                         const _u16 right, const _u16 bottom);
static void Toolbar_Destroy(void *toolbarArg);

View_t *Toolbar_Create(char *text, Font_t *font) {
  Toolbar_t *toolbar = (Toolbar_t *)malloc(sizeof(Toolbar_t));
  if (toolbar == NULL) {
    return NULL;
  }

  toolbar->title = text;
  toolbar->font = font;
  toolbar->view = View_Create(toolbar, &Toolbar_Draw, &Toolbar_Destroy, NULL);

  Toolbar_RecalculateSize(toolbar);

  return toolbar->view;
}

static const _u8 titleTopPadding = 7;
static const _u8 titleStartPadding = 30;
static const _u8 titleBottomPadding = 3;
static const _u8 lineSidePadding = 20;

static void Toolbar_Draw(View_t *view, const _u16 left, const _u16 top,
                         const _u16 right, const _u16 bottom) {
  Toolbar_t *toolbar = (Toolbar_t *)View_GetCustomView(view);
  Font_t *activeFont = GFX_GetFont();
  _u16 yPosForNextView = top + titleTopPadding;

  GFX_DrawString(toolbar->title, left + titleStartPadding, yPosForNextView,
                 activeFont);
  yPosForNextView += Font_GetHeight(activeFont);
  // add padding between title and line decoration
  yPosForNextView += titleBottomPadding;

  GFXDrawFilledRect(left + lineSidePadding, right - lineSidePadding,
                    yPosForNextView, yPosForNextView + 1, GFXGetFontColor());
}

// private part
static void Toolbar_RecalculateSize(Toolbar_t *toolbar) {
  View_SetWidth(toolbar->view, GFX_GetCanwasWidth());
  View_SetHeight(toolbar->view, 30);
}

static void Toolbar_Destroy(void *toolbarArg) {
  if (toolbarArg == NULL) {
    return;
  }
  Toolbar_t *toolbar = (Toolbar_t *)toolbarArg;

  free(toolbar);
}