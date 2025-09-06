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

static Palette_t palette;
static GameState_t state;
static LayerType_t layerChanged;
static Scene_t* scene;
static Menu_t* menu;
static TileMap_t* _tileMap;

typedef struct Game {
  SpritesHolder_t* spritesHolder;
  ObjectsHolder_t* objectsHolder;

} Game;

static void _Game_SetupDefaultPalette();
void _OnRegionRedrawRequested(_u16 l, _u16 t, _u16 r, _u16 b);

static Game* _game;

Game* Game_Create() {
  Game* game = (Game*)malloc(sizeof(Game));
  if (game == NULL) return NULL;

  palette.background = COLOR_BLACK;

  game->spritesHolder = SpritesHolder_Create();
  game->objectsHolder = ObjectsHolder_Create();

  _Game_SetupDefaultPalette();

  menu = MenuCreate();
  state = STOPPED;
  layerChanged = LAYER_NONE;
  _tileMap = NULL;

  scene =
      Scene_Create(game->spritesHolder, game->objectsHolder, &(layerChanged));

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

void Game_SetTileMap(TileMap_t* tileMap) { _tileMap = tileMap; }

void Game_SetMenuItems(MenuItem_t** items, _u8 itemsCount) {
  // TODO: complete
}

void Game_Pause() {
  state = PAUSED;
  layerChanged = LAYER_UI;

  printf("game state: PAUSED\n");
  Scene_Pause(scene);
}

void Game_Resume() {
  state = RUNNING;
  layerChanged = LAYER_UI;

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

Menu_t* Game_GetMenu() { return menu; }

Scene_t* Game_GetScene() { return scene; }

static Color _color;
static Color buffer[320] = {COLOR_MAGENTA};

void _OnRegionRedrawRequested(_u16 l, _u16 t, _u16 r, _u16 b) {
  _u16 lineLength = r - l + 1;
  _u16 lines = b - t + 1;

  _u16 pixelIndex = 0;
  _u16 lineIndex = 0;

  for (_u16 y = t; y <= b; y++) {
    pixelIndex = 0;
    _color = COLOR_MAGENTA;

    for (_u16 x = l; x <= r; x++) {
      if (state == RUNNING) {
        _color = CalculatePixel(&palette, x, y, _game->spritesHolder, _tileMap,
                                layerChanged, 0);
      } else {
        _color = CalculatePixelForMenu(&palette, x, y, menu, 0);
      }

      buffer[pixelIndex] = _color;
      pixelIndex++;
    }

    _u32 index = GetIndexIn2DSpace(l, y, ScreenConfig_GetRealWidth());
    GFX_DrawPixelsInBuffer(index, buffer, lineLength);
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