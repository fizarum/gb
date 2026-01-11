#ifndef APPS_UTILS_H
#define APPS_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <palette.h>
#include <stdio.h>
#include <types.h>

#include "../ui/gfx/gfx.h"
#include "esp_log.h"
#include "menu/menu_resources.h"

#define TITLE_X_POS 30
#define TITLE_Y_POS 7

static const char* const osVersion = "0.0.1";

#define PROGRESS_BAR_MIN_HEIGH 14

// on/off button
#define ON_OFF_INDICATOR_SIZE 10

// focus
#define FOCUS_AREA_WIDTH 20
#define FOCUS_INDICATOR_WIDTH 6
#define FOCUS_SMALL_PADDING 2
#define FOCUS_MIDDLE_PADDING 4

static inline void App_DrawProgress(const _u16 left, const _u16 top,
                                    const _u16 right, const _u16 bottom,
                                    const _u8 progress) {
  if (left >= right) {
    return;
  }

  if (top + PROGRESS_BAR_MIN_HEIGH < bottom) {
    return;
  }

  if (progress > 100) {
    return;
  }

  _u16 hLength = right - left;
  _u16 vLength = bottom - top;

  _u16 pixelsPerPercent = (right - left) / 100;
  _u16 width = pixelsPerPercent * progress;

  if (progress < 100) {
    // top line
    GFX_DrawHLine(left, top, hLength, 1, GFX_GetFontColor());
    // bottom line
    GFX_DrawHLine(left, bottom, hLength, 1, GFX_GetFontColor());
    // right vertical line
    GFX_DrawVLine(right, top, vLength, 1, GFX_GetFontColor());
  }
  // progress
  GFX_DrawFilledRect(left, top, left + width, bottom, GFX_GetFontColor());
}

static inline void App_DrawFocusIndicator(const _u16 left, const _u16 top,
                                          const _u8 focusHeight) {
  GFX_DrawFilledRect(left, 0, left + FOCUS_AREA_WIDTH,
                     GFX_GetCanvasHeight() - 1,
                     GFX_GetTheme()->backgroundColor);

  _u16 leftPos = left + FOCUS_SMALL_PADDING;
  _u16 topPos = top - FOCUS_MIDDLE_PADDING;
  _u16 bottomPos = top + focusHeight - FOCUS_MIDDLE_PADDING;

  GFX_DrawFilledRect(leftPos,                          // left
                     topPos,                           // top
                     leftPos + FOCUS_INDICATOR_WIDTH,  // right
                     bottomPos,                        // bottom
                     GFX_GetFontColor());
}

// battery part
static inline void DrawBattery(bool charging, _u8 level, _u16 x, _u16 y) {
  GFX_DrawImageIndexes(x, y, 16, 16, batteryLeftPart, true);
  GFX_DrawImageIndexes(x + 16, y, 16, 16, batteryRightPart, true);
  if (charging) {
    GFX_DrawImageIndexes(x + 8, y, 16, 16, chargingIcon, false);
  } else {
    // calculate battery staus based on [level] param
  }
}

static inline void DrawTextWithIcon(const char* text, const _u8* const icon,
                                    _u16 x, _u16 y, Font_t* font) {
  GFX_DrawImageIndexes(x, y, 16, 16, icon, false);
  // "y+3" is a difference between height of icon (16 px) and font (10) divided
  // by 2
  GFX_DrawString(text, x + 20, y + 3, font);
}

// stub for app's unused callbacks
static void App_StubFunction(void) {};

#ifdef __cplusplus
}
#endif

#endif  // APPS_UTILS_H