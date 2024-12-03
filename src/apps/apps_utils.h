#ifndef APPS_UTILS_H
#define APPS_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <palette.h>
#include <types.h>

#include "../ui/gfx/gfx.h"
#include "esp_log.h"

#define TITLE_X_POS 30
#define TITLE_Y_POS 7

#define PROGRESS_BAR_MIN_HEIGH 14

// on/off button
#define ON_OFF_INDICATOR_SIZE 10

// focus
#define FOCUS_AREA_WIDTH 20
#define FOCUS_INDICATOR_WIDTH 6
#define FOCUS_SMALL_PADDING 2
#define FOCUS_MIDDLE_PADDING 4

static inline void App_DrawBackgroundAndTitle(const char *title,
                                              const _u16 backgroundColor) {
  GFXFillScreen(backgroundColor);
  GFX_DrawString(title, TITLE_X_POS, TITLE_Y_POS, GFX_GetFont());
  // top status line
  GFXDrawFilledRect(20, 300, 20, 21, GFXGetFontColor());
}

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
    GFX_DrawHLine(left, top, hLength, 1, GFXGetFontColor());
    // bottom line
    GFX_DrawHLine(left, bottom, hLength, 1, GFXGetFontColor());
    // right vertical line
    GFX_DrawVLine(right, top, vLength, 1, GFXGetFontColor());
  }
  // progress
  GFXDrawFilledRect(left, left + width, top, bottom, GFXGetFontColor());
}

static inline void App_DrawFocusIndicator(const _u16 left, const _u16 top,
                                          const _u8 focusHeight) {
  GFXDrawFilledRect(left, left + FOCUS_AREA_WIDTH, 0, GFX_GetCanvasHeight() - 1,
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

static inline void App_DrawOnOffButton(const _u16 left, const _u16 top,
                                       bool isOn) {
  const char *text = isOn == true ? "ON" : "OFF";
  if (isOn) {
    GFXDrawFilledRect(left, left + ON_OFF_INDICATOR_SIZE, top,
                      top + ON_OFF_INDICATOR_SIZE, GFXGetFontColor());
  } else {
    GFX_DrawRect(left, top, left + ON_OFF_INDICATOR_SIZE,
                 top + ON_OFF_INDICATOR_SIZE, 1, GFXGetFontColor());
  }

  GFX_DrawString(text, left + 20, top, GFX_GetFont());
}

static inline void App_DrawHorizontalPicker(const _u16 left, const _u16 top,
                                            const char *text, _u8 textLength) {
  _u16 textLengthInPx = GFX_FontGetWidth() * textLength;
  _u8 spacing = GFX_FontGetWidth();

  // draw left arrow
  _u16 leftPos = left;
  GFX_DrawChar('<', leftPos, top, GFX_GetFont());

  // middle text
  leftPos += spacing * 2;
  GFX_DrawString(text, leftPos, top, GFX_GetFont());

  // draw right arrow
  leftPos += textLengthInPx + spacing;
  GFX_DrawChar('>', leftPos, top, GFX_GetFont());
}

// stub for app's unused callbacks
static void App_StubFunction(void) {};

#ifdef __cplusplus
}
#endif

#endif  // APPS_UTILS_H