#ifndef APPS_UTILS_H
#define APPS_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <gfx/gfx.h>
#include <palette.h>
#include <types.h>

#include "esp_log.h"

// todo: move to outer layer
#define FS_DISPLAY_WIDTH 320
#define FS_DISPLAY_HEIGHT 240

#define TITLE_X_POS 30
#define TITLE_Y_POS 7

#define PROGRESS_BAR_MIN_HEIGH 14
#define FOCUS_AREA_WIDTH 20
#define FOCUS_INDICATOR_WIDTH 6
#define FOCUS_SMALL_PADDING 2
#define FOCUS_MIDDLE_PADDING 4

static void App_DrawBackgroundAndTitle(const char *title,
                                       const _u16 backgroundColor) {
  GFXFillScreen(backgroundColor);
  GFXDrawString(title, TITLE_X_POS, TITLE_Y_POS);
  // top status line
  GFXDrawFilledRect(20, 300, 20, 21, GFXGetFontColor());
}

static void App_DrawProgress(const _u16 left, const _u16 top, const _u16 right,
                             const _u16 bottom, const _u8 progress) {
  if (left >= right) {
    return;
  }

  if (top + PROGRESS_BAR_MIN_HEIGH < bottom) {
    return;
  }

  if (progress > 100) {
    return;
  }

  _u16 pixelsPerPercent = (right - left) / 100;
  _u16 width = pixelsPerPercent * progress;

  GFXDrawFilledRect(left, left + width, top, bottom, GFXGetFontColor());
}

static void App_DrawFocusIndicator(const _u16 left, const _u16 top,
                                   const _u8 focusHeight) {
  GFXDrawFilledRect(left, left + FOCUS_AREA_WIDTH, 0, FS_DISPLAY_HEIGHT - 1,
                    GFX_GetBackgroundColor());

  _u16 leftPos = left + FOCUS_SMALL_PADDING;
  _u16 topPos = top - FOCUS_MIDDLE_PADDING;
  _u16 bottomPos = top + focusHeight - FOCUS_MIDDLE_PADDING;

  GFXDrawFilledRect(leftPos,                          // left
                    leftPos + FOCUS_INDICATOR_WIDTH,  // right
                    topPos,                           // top
                    bottomPos,                        // bottom
                    GFXGetFontColor());
}

// stub for app's unused callbacks
static void App_StubFunction(void) {};

#ifdef __cplusplus
}
#endif

#endif  // APPS_UTILS_H