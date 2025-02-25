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
  GFX_FillScreen(backgroundColor);
  GFX_DrawString(title, TITLE_X_POS, TITLE_Y_POS, GFX_GetFont());
  // top status line
  GFX_DrawFilledRect(20, 20, 300, 22, GFX_GetFontColor());
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
                     GFX_GetCanvasHeight() - 1, GFX_GetBackgroundColor());

  _u16 leftPos = left + FOCUS_SMALL_PADDING;
  _u16 topPos = top - FOCUS_MIDDLE_PADDING;
  _u16 bottomPos = top + focusHeight - FOCUS_MIDDLE_PADDING;

  GFX_DrawFilledRect(leftPos,                          // left
                     topPos,                           // top
                     leftPos + FOCUS_INDICATOR_WIDTH,  // right
                     bottomPos,                        // bottom
                     GFX_GetFontColor());
}

static inline void App_DrawOnOffButton(const _u16 left, const _u16 top,
                                       bool isOn) {
  const char *text = isOn == true ? "ON" : "OFF";
  if (isOn) {
    GFX_DrawFilledRect(left, top, left + ON_OFF_INDICATOR_SIZE,
                       top + ON_OFF_INDICATOR_SIZE, GFX_GetFontColor());
  } else {
    GFX_DrawRect(left, top, left + ON_OFF_INDICATOR_SIZE,
                 top + ON_OFF_INDICATOR_SIZE, 1, GFX_GetFontColor());
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

// battery part
static char percentageStr[8];
static const _u8 batteryWidgetWidth = 30;
static const _u8 batteryWidgetHeight = 10;
static _u8 filledPart = 0;
static inline void DrawBattery(bool charging, _u8 level, _u16 x, _u16 y,
                               Font_t *font) {
  GFX_DrawRect(x, y, x + batteryWidgetWidth, y + batteryWidgetHeight, 1,
               GFX_GetFontColor());
  if (charging == true) {
    GFX_DrawString("++", x, y, font);
  } else {
    filledPart = (((float)batteryWidgetWidth) / 100.0) * level;

    GFX_DrawFilledRect(x, y, x + filledPart, y + batteryWidgetHeight,
                       GFX_GetFontColor());
  }
}

// stub for app's unused callbacks
static void App_StubFunction(void) {};

#ifdef __cplusplus
}
#endif

#endif  // APPS_UTILS_H