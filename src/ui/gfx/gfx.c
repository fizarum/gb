#include "gfx.h"

#include <ctype.h>
#include <palette.h>
#include <string.h>

#include "../resources/symbols_res.h"
#include "gfx_utils.h"

// check is bit set, starting from most significant bit
// 8-bit version
#define GFX_IS_BIT_SET8(source, position) (source & (0x80 >> position))

static Font_t *activeFont = NULL;
static _u16 backgroundColor;
static GFX_CanvasUpdated canvasCallback;
static _u16 canvasWidth = 0;
static _u16 canvasHeight = 0;
static _u32 canvasSize = 0;
static _u16 *canvas;

void GFX_Init(const _u16 width, const _u16 height, GFX_CanvasUpdated callback) {
  canvasWidth = width;
  canvasHeight = height;
  canvasSize = width * height;
  canvasCallback = callback;

  canvas = calloc(canvasSize, sizeof(_u16));
  for (_u32 index = 0; index < canvasSize; index++) {
    canvas[index] = 0;
  }
  SymbolsResInit();
}

void GFX_Redraw() { canvasCallback(); }

_u16 *GFX_GetCanvas() { return canvas; }

void GFX_SetFont(Font_t *font) { activeFont = font; }
Font_t *GFX_GetFont() { return activeFont; }

void GFX_SetBackgroundColor(_u16 color) { backgroundColor = color; }

_u16 GFX_GetCanwasWidth() { return canvasWidth; }
_u16 GFX_GetCanvasHeight() { return canvasHeight; }
_u32 GFX_CanvasSize() { return canvasSize; }

/**
 * @brief Draws symbol
 * @return pixels (by width) drawn for provided symbol
 */
_u8 GFX_DrawSymbol(SymbolData_t *symbol, const _u16 left, const _u16 top,
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
        scaledX = left + x * scale;
        scaledY = top + y * scale;

        if (scale > 1) {
          // draw rectangle as scaled pixel
          GFX_DrawFilledRect(scaledX, scaledY, scaledX + scale, scaledY + scale,
                             color);
        } else {
          // draw one pixel
          canvas[scaledY * canvasWidth + scaledX] = color;
        }
      }
    }
  }
  return GFX_FontGetWidth();
}

/**
 * @brief Draws char
 * @return pixels (by width) drawn for provided char
 */
_u8 GFX_DrawChar(_u8 asciiSymbol, const _u16 left, const _u16 top,
                 const Font_t *font) {
  SymbolData_t *symbol = SymbolsGet(asciiSymbol);
  if (symbol == NULL) {
    return 0;
  }

  return GFX_DrawSymbol(symbol, left, top, font);
}

/**
 * @brief Draws string
 * @return pixels (by width) drawn for provided string
 */
_u16 GFX_DrawString(const char *string, const _u16 left, const _u16 top,
                    const Font_t *font) {
  if (string == NULL) {
    return 0;
  }
  _u16 x = left;
  _u16 length = strlen(string);
  _u8 letterInUpperCase;

  // TODO: move it to argument
  _u8 letterWidth = GFX_FontGetWidth();

  for (_u16 index = 0; index < length; index++) {
    letterInUpperCase = toupper(string[index]);
    GFX_DrawChar(letterInUpperCase, x, top, font);
    x += letterWidth;
    if (x >= canvasWidth) {
      // TODO: add logic for word/letter wrap, ellipsize, etc.
      return index * letterWidth;
    }
  }

  return letterWidth * length;
}
static _u32 start, end;

void GFX_DrawPixel(const _u16 left, const _u16 top, _u16 color) {
  if (left >= canvasWidth || top > canvasHeight) {
    return;
  }
  start = GFX_GetIndexIn2DSpace(left, top, canvasWidth);
  canvas[start] = color;
}

void GFX_DrawPixels(const _u16 left, const _u16 top, _u16 *colors,
                    _u8 colorsCount) {
  if (left >= canvasWidth || top > canvasHeight) {
    return;
  }

  start = GFX_GetIndexIn2DSpace(left, top, canvasWidth);
  for (_u8 index = 0; index < colorsCount; ++index) {
    if (start + index >= canvasSize) {
      return;
    }
    canvas[start + index] = colors[index];
  }
}

void GFX_DrawPixelsInBuffer(const _u32 start, _u16 *colors, _u8 colorsCount) {
  if (start >= canvasSize) {
    return;
  }

  if (start + colorsCount >= canvasSize) {
    return;
  }

  for (_u8 index = 0; index < colorsCount; ++index) {
    canvas[start + index] = colors[index];
  }
}

// TODO: rework to left, top, right, bottom sequence
void GFX_DrawFilledRect(const _u16 left, const _u16 top, const _u16 right,
                        const _u16 bottom, const _u16 color) {
  for (_u32 index = top; index < bottom; index++) {
    GFX_DrawHLine(left, index, right - left, 1, color);
  }
}

void GFX_DrawRect(const _u16 left, const _u16 top, const _u16 right,
                  const _u16 bottom, const _u8 lineWidth, const _u16 color) {
  // top line
  GFX_DrawHLine(left, top, right - left, lineWidth, color);
  // bottom line
  GFX_DrawHLine(left, bottom, right - left, lineWidth, color);
  // left
  GFX_DrawVLine(left, top, bottom - top, lineWidth, color);
  // right
  GFX_DrawVLine(right, top, bottom - top, lineWidth, color);
}

void GFX_DrawHLine(const _u16 left, const _u16 top, const _u16 length,
                   const _u8 lineWidth, const _u16 color) {
  _u16 normalizedLength = length;
  if (length + left >= canvasWidth) {
    normalizedLength = canvasWidth - left - 1;
  }
  // calculate start and end in one line
  start = top * canvasWidth + left;
  end = (start + normalizedLength);

  // iterate from top to bottom with iterations = lineWidth
  for (_u8 lineIndex = 0; lineIndex < lineWidth; lineIndex++) {
    for (_u32 index = start; index < end; index++) {
      canvas[index] = color;
    }
    start += canvasWidth;
    end += canvasWidth;
  }
}

void GFX_DrawVLine(const _u16 left, const _u16 top, const _u16 length,
                   const _u8 lineWidth, const _u16 color) {
  _u16 bottom = top + length;
  if (bottom >= canvasHeight) {
    bottom = canvasHeight - 1;
  }
  for (_u16 index = top; index <= bottom; index++) {
    // we're using drawing horizontal line because of lineWidth
    GFX_DrawHLine(left, index, lineWidth, 1, color);
  }
}

void GFX_FillScreen(const _u16 color) {
  for (_u32 index = 0; index < canvasSize; index++) {
    canvas[index] = color;
  }
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
