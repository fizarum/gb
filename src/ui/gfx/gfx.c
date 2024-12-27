#include "gfx.h"

#include <ctype.h>
#include <palette.h>
#include <string.h>

#include "../resources/symbols_res.h"

// check is bit set, starting from most significant bit
// 8-bit version
#define GFX_IS_BIT_SET8(source, position) (source & (0x80 >> position))

static Font_t *activeFont = NULL;
static _u16 backgroundColor;
static GFX_Callback drawCallback;
static _u16 canvasWidth = 0;
static _u16 canvasHeight = 0;

void GFX_Init(const _u16 width, const _u16 height, GFX_Callback callback) {
  canvasWidth = width;
  canvasHeight = height;
  drawCallback = callback;
  SymbolsResInit();
}

void GFX_SetFont(Font_t *font) { activeFont = font; }
Font_t *GFX_GetFont() { return activeFont; }

void GFX_SetBackground(_u16 color) { backgroundColor = color; }

_u16 GFX_GetCanwasWidth() { return canvasWidth; }
_u16 GFX_GetCanvasHeight() { return canvasHeight; }

/**
 * @brief Draws symbol
 * @return pixels (by width) drawn for provided symbol
 */
_u8 GFX_DrawSymbol(SymbolData_t *symbol, _u16 xPos, _u16 yPos,
                   const Font_t *font) {
  if (font == NULL || font->scale == 0) {
    return 0;
  }

  _u8 symbolWidth = font->width;
  _u8 symbolHeight = font->height;
  _u8 scale = font->scale;
  _u16 color = font->color;

  _u16 scaledX = 0;
  _u16 scaledY = 0;
  _u8 line = 0;

  for (_u8 y = 0; y < symbolHeight; y++) {
    line = symbol->data[y];

    // if blank line - skip current step
    if (line == 0) continue;

    for (_u8 x = 0; x < symbolWidth; x++) {
      if (GFX_IS_BIT_SET8(line, x)) {
        scaledX = xPos + x * scale;
        scaledY = yPos + y * scale;

        drawCallback(scaledX, scaledY, scaledX + scale - 1, scaledY + scale - 1,
                     color);
      }
    }
  }
  return GFX_FontGetWidth();
}

/**
 * @brief Draws char
 * @return pixels (by width) drawn for provided char
 */
_u8 GFX_DrawChar(_u8 asciiSymbol, _u16 xPos, _u16 yPos, const Font_t *font) {
  SymbolData_t *symbol = SymbolsGet(asciiSymbol);
  if (symbol == NULL) {
    return 0;
  }

  return GFX_DrawSymbol(symbol, xPos, yPos, font);
}

/**
 * @brief Draws string
 * @return pixels (by width) drawn for provided string
 */
_u16 GFX_DrawString(const char *string, _u16 xPos, _u16 yPos,
                    const Font_t *font) {
  if (string == NULL) {
    return 0;
  }
  _u16 x = xPos;
  _u16 length = strlen(string);
  _u8 letterInUpperCase;

  // TODO: move it to argument
  _u8 letterWidth = GFX_FontGetWidth();

  for (_u16 index = 0; index < length; index++) {
    letterInUpperCase = toupper(string[index]);
    GFX_DrawChar(letterInUpperCase, x, yPos, font);
    x += letterWidth;
    if (x >= canvasWidth) {
      // TODO: add logic for word/letter wrap, ellipsize, etc.
      return index * letterWidth;
    }
  }

  return letterWidth * length;
}

void GFX_DrawFilledRect(const _u16 left, const _u16 right, const _u16 top,
                        const _u16 bottom, const _u16 color) {
  drawCallback(left, top, right, bottom, color);
}

void GFX_DrawRect(const _u16 left, const _u16 top, const _u16 right,
                  const _u16 bottom, const _u8 lineWidth, const _u16 color) {
  // top line
  drawCallback(left, top, right, top + lineWidth, color);
  // bottom line
  drawCallback(left, bottom - lineWidth, right, bottom, color);
  // left
  drawCallback(left, top, left + lineWidth, bottom, color);
  // right
  drawCallback(right - lineWidth, top, right, bottom, color);
}

void GFX_DrawHLine(const _u16 left, const _u16 top, const _u16 length,
                   const _u8 lineWidth, const _u16 color) {
  drawCallback(left, top, left + length, top + lineWidth, color);
}

void GFX_DrawVLine(const _u16 left, const _u16 top, const _u16 length,
                   const _u8 lineWidth, const _u16 color) {
  drawCallback(left, top, left + lineWidth, top + length, color);
}

void GFX_FillScreen(const _u16 color) {
  drawCallback(0, 0, canvasWidth - 1, canvasHeight - 1, color);
}

_u8 GFX_FontGetWidth() {
  if (activeFont == NULL) {
    return 0;
  }

  //_activeFont->height is taken because for font smaller than 8x8
  // height will contain actual size, when width is still 8 pt
  // for example, for 5x5 font its 8x5 (width x height).
  // Its known issue which will be fixed later
  return activeFont->height * activeFont->scale + activeFont->spacing;
}

_u16 GFX_GetFontColor() {
  if (activeFont == NULL) {
    return 0;
  }

  return activeFont->color;
}

_u16 GFX_GetBackgroundColor() { return backgroundColor; }
