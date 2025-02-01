#ifndef UI_FONT_H
#define UI_FONT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @brief General font description, should be used
 * with SymbolData_t to define font size
 */
typedef struct Font_t {
  uint8_t width;
  uint8_t height;
  uint8_t scale;
  uint8_t spacing;
  uint16_t color;
} Font_t;

static uint8_t Font_GetWidth(Font_t *font) {
  //  height is taken because for font smaller than 8x8
  //  height will contain actual size, when width is still 8 pt
  //  for example, for 5x5 font its 8x5 (width x height).
  //  Its known issue which will be fixed later
  return font->height * font->scale + font->spacing;
}

static uint8_t Font_GetHeight(Font_t *font) {
  // we have to apply spacing * 2 for correct top and bottom spacing
  return font->height * font->scale + font->spacing * 2;
}

#ifdef __cplusplus
}
#endif

#endif  // UI_FONT_H