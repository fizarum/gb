#include "screen_config.h"

#include <stdlib.h>

static _u16 realWidth;
static _u16 realHeight;
static _u16 screenWidth;
static _u16 screenHeight;

static _u8 screenScale;

void ScreenConfig_Set(const _u16 width, const _u16 height, const _u8 scale) {
  realWidth = width;
  realHeight = height;
  screenScale = scale;
  screenWidth = width / scale;
  screenHeight = height / scale;
}

_u16 ScreenConfig_GetRealWidth() { return realWidth; }
_u16 ScreenConfig_GetRealHeight() { return realHeight; }
_u16 ScreenConfig_GetWidth() { return screenWidth; }
_u16 ScreenConfig_GetHeight() { return screenHeight; }
_u8 ScreenConfig_GetScale() { return screenScale; }