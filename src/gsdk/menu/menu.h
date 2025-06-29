#ifndef GAME_SDK_MENU_H
#define GAME_SDK_MENU_H

#ifdef __cplusplus
extern "C" {
#endif
#include "../scene/layer_type.h"
#include "../sprite/sprite_data.h"
#include "../types/types.h"
#include "menu_item.h"

typedef struct Menu_t Menu_t;

Menu_t* MenuCreate(const _u16 screenWidth, const _u16 screenHeight);
void MenuDestroy(Menu_t* menu);

void MenuSelectNextItem(Menu_t* menu);
void MenuSelectPreviousItem(Menu_t* menu);

SpriteId MenuAddSprite(Menu_t* menu, const SpriteData_t* data, const _u8 x,
                       const _u8 y, const LayerType_t layer);

_ci MenuGetColorIndex(Menu_t* menu, const _u8 x, const _u8 y, _ci fallback);

void MenuSetupButtonSprites(Menu_t* menu, const SpriteData_t* leftPartSprite,
                            const SpriteData_t* middlePartSprite,
                            const SpriteData_t* rightPartSprite);
void MenuCreateHCenterButton(Menu_t* menu, const SpriteData_t** letterSprites,
                             const _u8 lettersCount, const _u8 y);

void MenuCreateLabel(Menu_t* menu, const SpriteData_t** letterSprites,
                     const _u8 lettersCount, const _u8 x, const _u8 y);
void MenuCreateHCenterLabel(Menu_t* menu, const SpriteData_t** letterSprites,
                            const _u8 lettersCount, const _u8 y);
#ifdef __cplusplus
}
#endif

#endif  // GAME_SDK_MENU_H