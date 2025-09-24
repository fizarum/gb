#ifndef GAME_SDK_MENU_H
#define GAME_SDK_MENU_H

#ifdef __cplusplus
extern "C" {
#endif
#include "../scene/layer_type.h"
#include "../screen/screen_config.h"
#include "../sprite/sprite_data.h"
#include "../types/types.h"
#include "menu_item.h"

typedef struct Menu Menu;

Menu* MenuCreate();
void MenuDestroy(Menu* menu);

void MenuSelectNextItem(Menu* menu);
void MenuSelectPreviousItem(Menu* menu);

SpriteId MenuAddSprite(Menu* menu, const SpriteData* data, const _u16 x,
                       const _u16 y, const LayerType layer);

_ci MenuGetColorIndex(Menu* menu, const _u16 x, const _u16 y, _ci fallback);

void MenuSetupButtonSprites(Menu* menu, const SpriteData* leftPartSprite,
                            const SpriteData* middlePartSprite,
                            const SpriteData* rightPartSprite);
void MenuCreateHCenterButton(Menu* menu, const SpriteData** letterSprites,
                             const _u8 lettersCount, const _u16 y);

void MenuCreateLabel(Menu* menu, const SpriteData** letterSprites,
                     const _u8 lettersCount, const _u16 x, const _u16 y);
void MenuCreateHCenterLabel(Menu* menu, const SpriteData** letterSprites,
                            const _u8 lettersCount, const _u16 y);
#ifdef __cplusplus
}
#endif

#endif  // GAME_SDK_MENU_H