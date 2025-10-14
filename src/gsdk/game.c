#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ui/gfx/gfx.h>

#include "scene/objects_holder.h"
#include "screen/pixel_calculator.h"
#include "screen/screen_config.h"
#include "types/game_state.h"
#include "utils/screen_utils.h"
#include "utils/utils.h"

static Palette palette;
static GameState state;
static Scene* scene;
static Menu* menu;
static TileMap* _tileMap;
static _u8 scale = 1;

typedef struct Game {
  SpritesHolder* spritesHolder;
  ObjectsHolder* objectsHolder;
} Game;

static void _Game_SetupDefaultPalette();
void _OnRegionRedrawRequested(_u16 l, _u16 t, _u16 r, _u16 b);

static Game* _game;

Game* Game_Create() {
  Game* game = (Game*)malloc(sizeof(Game));
  if (game == NULL) return NULL;

  palette.background = COLOR_BLACK;
  scale = ScreenConfig_GetScale();

  game->spritesHolder = SpritesHolder_Create();
  game->objectsHolder = ObjectsHolder_Create();

  _Game_SetupDefaultPalette();

  menu = MenuCreate();
  state = STOPPED;
  _tileMap = NULL;

  scene = Scene_Create(game->spritesHolder, game->objectsHolder);

  if (scene == NULL) {
    return NULL;
  }

  _game = game;

  return game;
}

void Game_Destroy(Game* game) {
  if (game == NULL) return;

  state = STOPPED;

  Scene_Destroy(scene);
  SpritesHolder_Destroy(game->spritesHolder);
  ObjectsHolder_Destroy(game->objectsHolder);

  free(game);
}

void Game_SetTileMap(TileMap* tileMap) { _tileMap = tileMap; }

void Game_SetMenuItems(MenuItem** items, _u8 itemsCount) {
  // TODO: complete
}

void Game_Pause() {
  state = PAUSED;

  printf("game state: PAUSED\n");
  Scene_Pause(scene);
}

void Game_Resume() {
  state = RUNNING;

  printf("game state: RUNNING\n");
  Scene_Resume(scene);
}

void Game_TogglePauseResume() {
  if (state == RUNNING) {
    Game_Pause();
  } else {
    Game_Resume();
  }
}

void Game_Start() { Game_Resume(); }

void Game_Update() {
  if (scene != NULL) {
    if (state == RUNNING) {
      Scene_Update(scene);
    }
    Scene_CleanupRegions(scene, &_OnRegionRedrawRequested);
  }
}

Menu* Game_GetMenu() { return menu; }

Scene* Game_GetScene() { return scene; }

static Color _color;

void _OnRegionRedrawRequested(_u16 l, _u16 t, _u16 r, _u16 b) {
  static _u16 xScaled, yScaled;

  for (_u16 y = t; y <= b; y++) {
    yScaled = y * scale;

    for (_u16 x = l; x <= r; x++) {
      xScaled = x * scale;

      if (state == RUNNING) {
        _color =
            CalculatePixel(&palette, x, y, _game->spritesHolder, _tileMap, 0);
      } else {
        _color = CalculatePixelForMenu(&palette, x, y, menu, 15);
      }

      GFX_DrawFilledRect(xScaled, yScaled, xScaled + scale, yScaled + scale,
                         _color);
    }
  }
}

static void _Game_SetupDefaultPalette() {
  Palette_SetColor(&palette, 0, COLOR_BLACK);
  Palette_SetColor(&palette, 1, COLOR_PEARL);
  Palette_SetColor(&palette, 2, COLOR_WATERMELON_RED);
  Palette_SetColor(&palette, 3, COLOR_PEWTER_BLUE);
  Palette_SetColor(&palette, 4, COLOR_PURPLE_TAUPE);
  Palette_SetColor(&palette, 5, COLOR_FOREST_GREEN);
  Palette_SetColor(&palette, 6, COLOR_INDIGO);
  Palette_SetColor(&palette, 7, COLOR_SUNRAY);
  Palette_SetColor(&palette, 8, COLOR_LIGHT_TAUPE);
  Palette_SetColor(&palette, 9, COLOR_FELDGRAU);
  Palette_SetColor(&palette, 10, COLOR_CEDAR_CHEST);
  Palette_SetColor(&palette, 11, COLOR_DARK_CHARCOAL);
  Palette_SetColor(&palette, 12, COLOR_SONIC_SILVER);
  Palette_SetColor(&palette, 13, COLOR_ASPARAGUS);
  Palette_SetColor(&palette, 14, COLOR_SEA_SERPENT);
  Palette_SetColor(&palette, 15, COLOR_GRAY);

  palette.background = COLOR_BLACK;
}