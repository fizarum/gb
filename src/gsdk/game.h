#ifndef GAME_SDK_GAME_H
#define GAME_SDK_GAME_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../system/input/keycode.h"
#include "menu/menu.h"
#include "scene/scene.h"
#include "screen/screen_config.h"
#include "types/types.h"

typedef struct Game Game;
Game* Game_Create();
void Game_Destroy(Game* game);

void Game_SetTileMap(TileMap* tileMap);
void Game_SetMenuItems(MenuItem** items, _u8 itemsCount);

void Game_Pause();
void Game_Resume();
void Game_TogglePauseResume();

void Game_Start();
void Game_Update();

Menu* Game_GetMenu();
Scene* Game_GetScene();

#ifdef __cplusplus
}
#endif

#endif  // GAME_SDK_GAME_H