#ifndef GAME_SDK_SCREEN_CONFIG_H
#define GAME_SDK_SCREEN_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <types.h>

void ScreenConfig_Set(const _u16 width, const _u16 height, const _u8 scale);

_u16 ScreenConfig_GetRealWidth();
_u16 ScreenConfig_GetRealHeight();
_u16 ScreenConfig_GetWidth();
_u16 ScreenConfig_GetHeight();
_u8 ScreenConfig_GetScale();

#ifdef __cplusplus
}
#endif

#endif  // GAME_SDK_SCREEN_CONFIG_H