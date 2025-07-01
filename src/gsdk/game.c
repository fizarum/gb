#include "game.h"

#include <stdio.h>
#include <stdlib.h>

#include "scene/objects_holder.h"
#include "screen/pixel_calculator.h"
#include "screen/tracker.h"
#include "types/game_state.h"
#include "utils/screen_utils.h"
#include "utils/utils.h"

static Tracker_t* tracker;
static Palette_t palette;
static GameState_t state;
static LayerType_t layerChanged;
static Scene_t* scene;
static Menu_t* menu;
static TileMap_t* _tileMap;
static ScreenConfig* _config;

static _u8 cellsInScreenWidth;

typedef struct Game {
  SpritesHolder_t* spritesHolder;
  ObjectsHolder_t* objectsHolder;

} Game;

static void _Game_SetupDefaultPalette();
void _TrackerCallback(_u16 cellIndex, _u8 cellData);

OnPixelsUpdateCallback _drawCallback2;
OnPixelsBufferUpdateCallback _drawCallback;
static Game* _game;

Game* Game_Create(ScreenConfig* config, OnPixelsBufferUpdateCallback callback) {
  Game* game = (Game*)malloc(sizeof(Game));
  if (game == NULL) return NULL;

  _config = config;
  cellsInScreenWidth = _config->width / CELL_SIZE;

  palette.background = COLOR_BLACK;

  tracker = TrackerCreate(config);

  game->spritesHolder = SpritesHolderCreate();
  game->objectsHolder = ObjectsHolderCreate();

  _Game_SetupDefaultPalette();

  menu = MenuCreate(config);
  state = STOPPED;
  layerChanged = LAYER_NONE;
  _tileMap = NULL;

  scene = SceneCreate(game->spritesHolder, game->objectsHolder, tracker,
                      &(layerChanged), config);

  if (scene == NULL) {
    return NULL;
  }

  _game = game;
  _drawCallback = callback;

  return game;
}

void Game_Destroy(Game* game) {
  if (game == NULL) return;

  state = STOPPED;

  SceneDestroy(scene);
  TrackerDestroy(tracker);
  SpritesHolderDestroy(game->spritesHolder);
  ObjectsHolderDestroy(game->objectsHolder);

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
  TrackerSetAll(tracker);
}

void Game_Resume() {
  state = RUNNING;
  layerChanged = LAYER_UI;

  printf("game state: RUNNING\n");
  TrackerSetAll(tracker);
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
  if (state == RUNNING && scene != NULL) {
    SceneUpdate(scene);
  }
  if (state != STOPPED) {
    TrackerGetRegion(tracker, &_TrackerCallback);
    TrackerClear(tracker);
  }
}

Menu_t* Game_GetMenu() { return menu; }

Scene_t* Game_GetScene() { return scene; }

static Color _color;
static Color _colorsPortion[CELL_SIZE];

void _TrackerCallback(_u16 cellIndex, _u8 cellData) {
  _u8 startX = (cellIndex % cellsInScreenWidth) * CELL_SIZE;

  _u16 y = cellIndex / cellsInScreenWidth;
  _u16 x = startX;

  for (_u8 bitIndex = 0; bitIndex < CELL_SIZE; bitIndex++) {
    if (isBitSet(cellData, bitIndex)) {
      if (state == RUNNING) {
        _color = CalculatePixel(&palette, x, y, _game->spritesHolder, _tileMap,
                                layerChanged, 0);
      } else {
        _color = CalculatePixelForMenu(&palette, x, y, menu, 0);
      }
    }
    _colorsPortion[bitIndex] = _color;
    x++;
  }

  _u32 index = GetIndexIn2DSpace(startX, y, _config->width);
  _drawCallback(index, _colorsPortion, CELL_SIZE);
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