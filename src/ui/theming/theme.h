#ifndef UI_THEME
#define UI_THEME

#ifdef __cplusplus
extern "C" {
#endif

#include <types.h>

typedef struct Theme {
  _u16 primaryColor;
  _u16 secondaryColor;
  _u16 tertiaryColor;

  _u16 backgroundColor;

} Theme;

#ifdef __cplusplus
}
#endif

#endif  // UI_THEME