#ifndef APPS_UTILS_H
#define APPS_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <gfx/gfx.h>
#include <palette.h>
#include <types.h>

#define TITLE_X_POS 30
#define TITLE_Y_POS 7

static void AppDrawBackgroundAndTitle(const char *title,
                                      const _u16 backgroundColor) {
  GFXFillScreen(backgroundColor);
  GFXDrawString(title, TITLE_X_POS, TITLE_Y_POS);
  // top status line
  GFXDrawFilledRect(20, 300, 20, 21, GFXGetFontColor());
}

#ifdef __cplusplus
}
#endif

#endif  // APPS_UTILS_H