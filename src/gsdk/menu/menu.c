#include "menu.h"

#include <stdlib.h>

#include "../collections/array.h"
#include "../scene/layer_type.h"
#include "../sprite/sprite.h"
#include "../ui/button.h"

#define SPRITES_BACKGROUND_MAX_COUNT 50
#define SPRITES_FOREGROUND_MAX_COUNT 50
#define BUTTONS_MAX_COUNT 10

typedef struct Menu_t {
  // TODO: complete
  Array_t* buttons;
  Array_t* backgroundSprites;
  Array_t* foregroundSprites;

  _u8 totalItems;
  _u8 selectedItem;

  ScreenConfig* config;

  SpriteData_t* btnLeft;
  SpriteData_t* btnMid;
  SpriteData_t* btnRight;
} Menu_t;

_u8 _MenuGetHCenterForText(const Menu_t* menu, const _u8 letterSize,
                           const _u8 lettersCount);

Menu_t* MenuCreate(ScreenConfig* config) {
  Menu_t* menu = (Menu_t*)malloc(sizeof(Menu_t));

  if (menu == NULL) return NULL;

  menu->selectedItem = 0;
  menu->totalItems = 0;
  menu->buttons = ArrayCreate(BUTTONS_MAX_COUNT);
  menu->backgroundSprites = ArrayCreate(SPRITES_BACKGROUND_MAX_COUNT);
  menu->foregroundSprites = ArrayCreate(SPRITES_FOREGROUND_MAX_COUNT);

  menu->config = config;

  menu->btnLeft = NULL;
  menu->btnMid = NULL;
  menu->btnRight = NULL;

  return menu;
}

void MenuDestroy(Menu_t* menu) {
  if (menu == NULL) return;

  ArrayDestroy(menu->backgroundSprites);
  ArrayDestroy(menu->foregroundSprites);

  free(menu);
}

void MenuSelectNextItem(Menu_t* menu) {
  menu->selectedItem++;
  if (menu->selectedItem >= menu->totalItems) {
    menu->selectedItem = 0;
  }
}

void MenuSelectPreviousItem(Menu_t* menu) {
  if (menu->selectedItem == 0) {
    menu->selectedItem = menu->totalItems - 1;
    return;
  }
  menu->selectedItem--;
}

SpriteId MenuAddSprite(Menu_t* menu, const SpriteData_t* data, const _u8 x,
                       const _u8 y, const LayerType_t layer) {
  Sprite_t* sprite = Sprite_Create(data, layer);

  if (x != 0 || y != 0) {
    SpriteMoveTo(sprite, x, y);
  }

  Array_t* sprites = (SpriteGetLayer(sprite) == LAYER_MENU_NEAR)
                         ? menu->foregroundSprites
                         : menu->backgroundSprites;
  if (ArrayAdd(sprites, sprite) == true) {
    return (SpriteId)sprite;
  }
  return OBJECT_ID_NA;
}

_ci MenuGetColorIndex(Menu_t* menu, const _u8 x, const _u8 y, _ci fallback) {
  Array_t* sprites = menu->foregroundSprites;
  Sprite_t* sprite = NULL;
  _ci color = fallback;

  for (_u16 index = 0; index < ArraySize(sprites); index++) {
    sprite = (Sprite_t*)ArrayValueAt(sprites, index);
    if (SpriteContainsPoint(sprite, x, y) == true) {
      color = SpriteGetColorIndex(sprite, x, y, fallback);
      if (color != fallback) {
        return color;
      }
    }
  }

  // if no sprite found, check background sprites
  sprites = menu->backgroundSprites;
  for (_u16 index = 0; index < ArraySize(sprites); index++) {
    sprite = (Sprite_t*)ArrayValueAt(sprites, index);
    if (SpriteContainsPoint(sprite, x, y) == true) {
      return SpriteGetColorIndex(sprite, x, y, fallback);
    }
  }
  return fallback;
}

void MenuSetupButtonSprites(Menu_t* menu, const SpriteData_t* leftPartSprite,
                            const SpriteData_t* middlePartSprite,
                            const SpriteData_t* rightPartSprite) {
  menu->btnLeft = leftPartSprite;
  menu->btnMid = middlePartSprite;
  menu->btnRight = rightPartSprite;
}

void MenuCreateHCenterButton(Menu_t* menu, const SpriteData_t** letterSprites,
                             const _u8 lettersCount, const _u8 y) {
  const _u8 letterSize = letterSprites[0]->width;

  _u8 xPos = _MenuGetHCenterForText(menu, letterSize, lettersCount);

  MenuAddSprite(menu, menu->btnLeft, xPos, y, LAYER_MENU_FAR);
  for (_u8 index = 0; index < lettersCount; index++) {
    const SpriteData_t* spriteData = letterSprites[index];
    MenuAddSprite(menu, spriteData, xPos, y, LAYER_MENU_NEAR);
    if (index < lettersCount - 1) {
      MenuAddSprite(menu, menu->btnMid, xPos, y, LAYER_MENU_FAR);
      xPos += letterSize;
    }
  }
  MenuAddSprite(menu, menu->btnRight, xPos, y, LAYER_MENU_FAR);
  // TODO: add callback
  Button_t* button = ButtonCreate(NULL);
  ArrayAdd(menu->buttons, button);
}

void MenuCreateLabel(Menu_t* menu, const SpriteData_t** letterSprites,
                     const _u8 lettersCount, const _u8 x, const _u8 y) {
  const _u8 letterSize = letterSprites[0]->width;
  _u8 xPos = x;

  for (_u8 index = 0; index < lettersCount; index++) {
    const SpriteData_t* spriteData = letterSprites[index];
    MenuAddSprite(menu, spriteData, xPos, y, LAYER_MENU_NEAR);
    xPos += letterSize;
  }
}

void MenuCreateHCenterLabel(Menu_t* menu, const SpriteData_t** letterSprites,
                            const _u8 lettersCount, const _u8 y) {
  const _u8 letterSize = letterSprites[0]->width;

  _u8 xPos = _MenuGetHCenterForText(menu, letterSize, lettersCount);

  for (_u8 index = 0; index < lettersCount; index++) {
    const SpriteData_t* spriteData = letterSprites[index];
    MenuAddSprite(menu, spriteData, xPos, y, LAYER_MENU_NEAR);
    xPos += letterSize;
  }
}

_u8 _MenuGetHCenterForText(const Menu_t* menu, const _u8 letterSize,
                           const _u8 lettersCount) {
  _u16 stringLength = letterSize * lettersCount;
  return menu->config->width / 2 - stringLength / 2;
}