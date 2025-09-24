#include "menu.h"

#include <stdlib.h>

#include "../collections/array.h"
#include "../scene/layer_type.h"
#include "../sprite/sprite.h"
#include "../ui/button.h"

#define SPRITES_BACKGROUND_MAX_COUNT 50
#define SPRITES_FOREGROUND_MAX_COUNT 50
#define BUTTONS_MAX_COUNT 10

typedef struct Menu {
  // TODO: complete
  Array_t* buttons;
  Array_t* backgroundSprites;
  Array_t* foregroundSprites;

  _u8 totalItems;
  _u8 selectedItem;

  SpriteData* btnLeft;
  SpriteData* btnMid;
  SpriteData* btnRight;
} Menu;

_u8 _MenuGetHCenterForText(const Menu* menu, const _u8 letterSize,
                           const _u8 lettersCount);

Menu* MenuCreate() {
  Menu* menu = (Menu*)malloc(sizeof(Menu));

  if (menu == NULL) return NULL;

  menu->selectedItem = 0;
  menu->totalItems = 0;
  menu->buttons = ArrayCreate(BUTTONS_MAX_COUNT);
  menu->backgroundSprites = ArrayCreate(SPRITES_BACKGROUND_MAX_COUNT);
  menu->foregroundSprites = ArrayCreate(SPRITES_FOREGROUND_MAX_COUNT);

  menu->btnLeft = NULL;
  menu->btnMid = NULL;
  menu->btnRight = NULL;

  return menu;
}

void MenuDestroy(Menu* menu) {
  if (menu == NULL) return;

  ArrayDestroy(menu->backgroundSprites);
  ArrayDestroy(menu->foregroundSprites);

  free(menu);
}

void MenuSelectNextItem(Menu* menu) {
  menu->selectedItem++;
  if (menu->selectedItem >= menu->totalItems) {
    menu->selectedItem = 0;
  }
}

void MenuSelectPreviousItem(Menu* menu) {
  if (menu->selectedItem == 0) {
    menu->selectedItem = menu->totalItems - 1;
    return;
  }
  menu->selectedItem--;
}

SpriteId MenuAddSprite(Menu* menu, const SpriteData* data, const _u16 x,
                       const _u16 y, const LayerType layer) {
  Sprite* sprite = Sprite_Create(data, layer);

  if (x != 0 || y != 0) {
    Sprite_MoveTo(sprite, x, y);
  }

  Array_t* sprites = (Sprite_GetLayer(sprite) == LAYER_MENU_NEAR)
                         ? menu->foregroundSprites
                         : menu->backgroundSprites;
  if (ArrayAdd(sprites, sprite) == true) {
    return (SpriteId)sprite;
  }
  return OBJECT_ID_NA;
}

_ci MenuGetColorIndex(Menu* menu, const _u16 x, const _u16 y, _ci fallback) {
  Array_t* sprites = menu->foregroundSprites;
  Sprite* sprite = NULL;
  _ci color = fallback;

  for (_u16 index = 0; index < ArraySize(sprites); index++) {
    sprite = (Sprite*)ArrayValueAt(sprites, index);
    if (Sprite_ContainsPoint(sprite, x, y) == true) {
      color = Sprite_GetColorIndex(sprite, x, y, fallback);
      if (color != fallback) {
        return color;
      }
    }
  }

  // if no sprite found, check background sprites
  sprites = menu->backgroundSprites;
  for (_u16 index = 0; index < ArraySize(sprites); index++) {
    sprite = (Sprite*)ArrayValueAt(sprites, index);
    if (Sprite_ContainsPoint(sprite, x, y) == true) {
      return Sprite_GetColorIndex(sprite, x, y, fallback);
    }
  }
  return fallback;
}

void MenuSetupButtonSprites(Menu* menu, const SpriteData* leftPartSprite,
                            const SpriteData* middlePartSprite,
                            const SpriteData* rightPartSprite) {
  menu->btnLeft = (SpriteData*)leftPartSprite;
  menu->btnMid = (SpriteData*)middlePartSprite;
  menu->btnRight = (SpriteData*)rightPartSprite;
}

void MenuCreateHCenterButton(Menu* menu, const SpriteData** letterSprites,
                             const _u8 lettersCount, const _u16 y) {
  const _u8 letterSize = letterSprites[0]->width;

  _u8 xPos = _MenuGetHCenterForText(menu, letterSize, lettersCount);

  MenuAddSprite(menu, menu->btnLeft, xPos, y, LAYER_MENU_FAR);
  for (_u8 index = 0; index < lettersCount; index++) {
    const SpriteData* spriteData = letterSprites[index];
    MenuAddSprite(menu, spriteData, xPos, y, LAYER_MENU_NEAR);
    if (index < lettersCount - 1) {
      MenuAddSprite(menu, menu->btnMid, xPos, y, LAYER_MENU_FAR);
      xPos += letterSize;
    }
  }
  MenuAddSprite(menu, menu->btnRight, xPos, y, LAYER_MENU_FAR);
  // TODO: add callback
  Button* button = ButtonCreate(NULL);
  ArrayAdd(menu->buttons, button);
}

void MenuCreateLabel(Menu* menu, const SpriteData** letterSprites,
                     const _u8 lettersCount, const _u16 x, const _u16 y) {
  const _u8 letterSize = letterSprites[0]->width;
  _u8 xPos = x;

  for (_u8 index = 0; index < lettersCount; index++) {
    const SpriteData* spriteData = letterSprites[index];
    MenuAddSprite(menu, spriteData, xPos, y, LAYER_MENU_NEAR);
    xPos += letterSize;
  }
}

void MenuCreateHCenterLabel(Menu* menu, const SpriteData** letterSprites,
                            const _u8 lettersCount, const _u16 y) {
  const _u8 letterSize = letterSprites[0]->width;

  _u8 xPos = _MenuGetHCenterForText(menu, letterSize, lettersCount);

  for (_u8 index = 0; index < lettersCount; index++) {
    const SpriteData* spriteData = letterSprites[index];
    MenuAddSprite(menu, spriteData, xPos, y, LAYER_MENU_NEAR);
    xPos += letterSize;
  }
}

_u8 _MenuGetHCenterForText(const Menu* menu, const _u8 letterSize,
                           const _u8 lettersCount) {
  _u16 stringLength = letterSize * lettersCount;
  return ScreenConfig_GetRealWidth() / 2 - stringLength / 2;
}
