#ifndef GFX_H
#define GFX_H

#ifdef __cplusplus
extern "C" {
#endif

#include <types.h>

#include "../resources/symbol_data.h"
#include "../typography/font.h"

typedef void (*GFX_CanvasUpdated)();

void GFX_Init(const _u16 width, const _u16 height, GFX_CanvasUpdated callback);
void GFX_Redraw();

_u16* GFX_GetCanvas();

void GFX_SetFont(Font_t* font);
Font_t* GFX_GetFont();

void GFX_SetBackgroundColor(_u16 color);

_u16 GFX_GetCanwasWidth();
_u16 GFX_GetCanvasHeight();
_u32 GFX_CanvasSize();

_u8 GFX_DrawSymbol(SymbolData_t* symbol, const _u16 left, const _u16 top,
                   const Font_t* font);
_u8 GFX_DrawChar(_u8 asciiSymbol, const _u16 left, const _u16 top,
                 const Font_t* font);
_u16 GFX_DrawString(const char* string, const _u16 left, const _u16 top,
                    const Font_t* font);

// draw one pixel on x,y coords
void GFX_DrawPixel(const _u16 x, const _u16 y, _u16 color);

// draw array of colors starting on x,y coords
void GFX_DrawPixels(const _u16 x, const _u16 y, _u16* colors, _u8 colorsCount);

// draw array of colors in linear colors array
void GFX_DrawPixelsInBuffer(const _u32 start, _u16* colors, _u16 colorsCount);

void GFX_DrawFilledRect(const _u16 left, const _u16 top, const _u16 right,
                        const _u16 bottom, const _u16 color);
void GFX_DrawRect(const _u16 left, const _u16 top, const _u16 right,
                  const _u16 bottom, const _u8 lineWidth, const _u16 color);

void GFX_DrawHLine(const _u16 left, const _u16 top, const _u16 length,
                   const _u8 lineWidth, const _u16 color);

void GFX_DrawVLine(const _u16 left, const _u16 top, const _u16 length,
                   const _u8 lineWidth, const _u16 color);

// utils
void GFX_FillScreen(const _u16 color);

_u8 GFX_FontGetWidth();
_u16 GFX_GetFontColor();

_u16 GFX_GetBackgroundColor();

#ifdef __cplusplus
}
#endif
#endif  // GFX_H