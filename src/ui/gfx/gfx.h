#ifndef GFX_H
#define GFX_H

#ifdef __cplusplus
extern "C" {
#endif

#include <resources/symbol_data.h>
#include <types.h>

#include "../typography/font.h"

typedef void (*GFX_Callback)(const _u16 left, const _u16 top, const _u16 right,
                             const _u16 bottom, _u16 color);

void GFXInit(const _u16 width, const _u16 height, GFX_Callback callback);

// symbols drawing
void GFXSetFont(Font_t *font);
Font_t *GFX_GetFont();
void GFX_SetBackground(_u16 color);

_u16 GFX_GetCanwasWidth();
_u16 GFX_GetCanvasHeight();

_u8 GFX_DrawSymbol(SymbolData_t *symbol, _u16 xPos, _u16 yPos,
                   const Font_t *font);
_u8 GFX_DrawChar(_u8 asciiSymbol, _u16 xPos, _u16 yPos, const Font_t *font);
_u16 GFX_DrawString(const char *string, _u16 xPos, _u16 yPos,
                    const Font_t *font);

// objects drawing
void GFXDrawFilledRect(const _u16 left, const _u16 right, const _u16 top,
                       const _u16 bottom, const _u16 color);
void GFX_DrawRect(const _u16 left, const _u16 top, const _u16 right,
                  const _u16 bottom, const _u8 lineWidth, const _u16 color);

void GFX_DrawHLine(const _u16 left, const _u16 top, const _u16 length,
                   const _u8 lineWidth, const _u16 color);

void GFX_DrawVLine(const _u16 left, const _u16 top, const _u16 length,
                   const _u8 lineWidth, const _u16 color);

// utils
void GFXFillScreen(const _u16 color);

_u8 GFX_FontGetWidth();
_u16 GFXGetFontColor();

_u16 GFX_GetBackgroundColor();

#ifdef __cplusplus
}
#endif
#endif  // GFX_H