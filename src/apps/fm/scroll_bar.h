#ifndef SCROLL_BAR_H
#define SCROLL_BAR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <palette.h>
#include <types.h>

#include "../../ui/gfx/gfx.h"

#define INDICATOR_MINIMAL_HEIGHT 10
#define INDICATOR_WIDTH 10
#define INDICATOR_PADDING 5

static void DrawScrollBar(_u16 page, _u16 pages, _u16 screenWidth,
                          _u16 screenHeight, _u16 color, _u16 backgroundColor) {
  const _u16 right = screenWidth - INDICATOR_PADDING;
  const _u16 left = right - INDICATOR_WIDTH;

  _u16 sectorHeight = screenHeight / pages;
  if (sectorHeight < INDICATOR_MINIMAL_HEIGHT) {
    sectorHeight = INDICATOR_MINIMAL_HEIGHT;
  }
  _u16 top = page * sectorHeight;
  _u16 bottom = top + sectorHeight;

  if (top > 0) {
    GFX_DrawFilledRect(left, right, 0, top - 1, backgroundColor);
  }
  GFX_DrawFilledRect(left, right, top, bottom, color);

  if (bottom < screenHeight - 1) {
    GFX_DrawFilledRect(left, right, bottom, screenHeight - 1, backgroundColor);
  }
}

#ifdef __cplusplus
}
#endif

#endif  // SCROLL_BAR_H