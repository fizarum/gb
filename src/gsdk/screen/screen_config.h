#ifndef GAME_SDK_SCREEN_CONFIG_H
#define GAME_SDK_SCREEN_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <types.h>

typedef struct ScreenConfig ScreenConfig;

void ScreenConfig_Create(const _u16 width, const _u16 height);

_u16 ScreenConfig_GetRealWidth();
_u16 ScreenConfig_GetRealHeight();

#ifdef __cplusplus
}
#endif

#endif  // GAME_SDK_SCREEN_CONFIG_H