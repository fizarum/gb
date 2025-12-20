#include "demo_app.h"

#include <specifications/input_data.h>
#include <ui/gfx/gfx.h>

#include "../../devices/joystick/joystick.h"
#include "../../gsdk/game.h"
#include "../../gsdk/screen/screen_config.h"
#include "../apps_utils.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "resources.h"

static AppSpecification_t specs = {
    .name = "Demo GSDK",
    .onPause = &App_StubFunction,
    .onResume = &App_StubFunction,
    .onUpdate = &App_StubFunction,
};

static TaskHandle_t updateTaskHandler = NULL;

static void _AssignMenuSprites();
static void _AssignSprites();
static TileMap* _AssignTileMap();
static void _GameUpdate(void* arg);

static Game* game;
static ObjectId playerId;
static SpriteId grassId;
static SpriteId sandId;
static SpriteId coastId;
static SpriteId waterId;
static SpriteId water2Id;
static SpriteId fishId;

static void onAppStart() {
  ESP_LOGI(specs.name, "on app start...");
  // TODO: make this part as external option
  GFX_SetApplicationColorMode(RGB565);

  ScreenConfig_Set(320, 240, 2);
  game = Game_Create();

  PrepareSprites();
  _AssignSprites();
  _AssignMenuSprites();
  TileMap* tileMap = _AssignTileMap();
  Game_SetTileMap(tileMap);
  // lower number for priority - lower priority in system
  xTaskCreate(&_GameUpdate, "gameUpdateTask", 2048, NULL, 6,
              &updateTaskHandler);
  // Game_Start();
}

// TODO: refactor to minimize functions
static void _GameUpdate(void* arg) {
  Scene* scene = Game_GetScene();
  while (1) {
    // TODO: move game's logic update here

    // TODO: test animation
    Scene_MoveSpriteBy(scene, fishId, -1, 0);
    vTaskDelay(pdMS_TO_TICKS(20));
  }
}

static void onAppStop() {
  if (updateTaskHandler != NULL) {
    vTaskDelete(updateTaskHandler);
  }
  // TODO: make this call as bart of any application
  GFX_ResetApplicationColorMode();
  Game_Destroy(game);
}

static void onAppUpdate(void) {
  Game_Update();
  GFX_Redraw();
}

static void handleKey(const void* keyData) {
  InputDeviceData* data = (InputDeviceData*)keyData;
  Scene* scene = Game_GetScene();

  if (IsButtonLeftPressed(data)) {
    Scene_MoveGameObjectBy(scene, playerId, -1, 0);
  } else if (IsButtonRightPressed(data)) {
    Scene_MoveGameObjectBy(scene, playerId, 1, 0);
  } else if (IsButtonUpPressed(data)) {
    Scene_MoveGameObjectBy(scene, playerId, 0, -1);
  } else if (IsButtonDownPressed(data)) {
    Scene_MoveGameObjectBy(scene, playerId, 0, 1);
  } else if (IsButtonStartReleased(data)) {
    Game_TogglePauseResume();
  }
}

static void onAppRedraw(RedrawType_t redrawType) {
  Game_Resume();
  GFX_Redraw();
}

AppSpecification_t* DemoAppSpecification() {
  specs.handleInput = &handleKey;
  specs.onStart = &onAppStart;
  specs.onRedraw = &onAppRedraw;
  specs.onStop = &onAppStop;
  specs.onUpdate = &onAppUpdate;

  return &specs;
}

void _AssignSprites() {
  Scene* scene = Game_GetScene();

  grassId = Scene_AddSprite(scene, &grass, LAYER_TILEMAP);
  waterId = Scene_AddSprite(scene, &water, LAYER_TILEMAP);
  water2Id = Scene_AddSprite(scene, &water2, LAYER_TILEMAP);
  sandId = Scene_AddSprite(scene, &sand, LAYER_TILEMAP);
  coastId = Scene_AddSprite(scene, &coast, LAYER_TILEMAP);

  playerId = Scene_AddGameObject(scene, &player, LAYER_MID, true, true, false);

  fishId = Scene_AddSprite(scene, &fish, LAYER_MID);

  SpriteId animatedFire = Scene_AddSprite(scene, &fire, LAYER_NEAR);
  Scene_ChangeSpriteAnimationSpeed(animatedFire, ANIMATION_SPEED_SUPER_FAST);
  Scene_MoveSpriteTo(scene, animatedFire, 100, 40);

  // stop animation
  // Scene_ChangeSpriteAnimationSpeed(animatedFire, ANIMATION_SPEED_NONE);

  // tree sprite, 8x8 pixels
  ObjectId treeObjectId =
      Scene_AddGameObject(scene, &tree, LAYER_MID, true, true, false);
  Scene_MoveGameObjectTo(scene, treeObjectId, 30, 30);

  // another tree, used as background sprite
  SpriteId treeBackId = Scene_AddSprite(scene, &tree, LAYER_FAR);
  Scene_MoveSpriteTo(scene, treeBackId, 10, 35);

  // tree used as foreground sprite
  SpriteId treeNearId = Scene_AddSprite(scene, &tree, LAYER_NEAR);
  Scene_MoveSpriteTo(scene, treeNearId, 80, 10);

  Scene_MoveSpriteTo(scene, fishId, 160, 100);
}

TileMap* _AssignTileMap() {
  SpriteId tiles[] = {
      // line 1
      grassId,
      grassId,
      grassId,
      grassId,
      grassId,
      grassId,
      grassId,
      grassId,
      grassId,
      grassId,
      // line 2
      grassId,
      grassId,
      grassId,
      grassId,
      grassId,
      grassId,
      grassId,
      grassId,
      grassId,
      grassId,

      // line 3
      grassId,
      grassId,
      grassId,
      grassId,
      grassId,
      grassId,
      grassId,
      grassId,
      grassId,
      grassId,
      // TODO: add sandId

      // line 4
      grassId,
      grassId,
      grassId,
      grassId,
      grassId,
      grassId,
      grassId,
      grassId,
      grassId,
      grassId,

      // coast line
      coastId,
      coastId,
      coastId,
      coastId,
      coastId,
      coastId,
      coastId,
      coastId,
      coastId,
      coastId,

      // water
      // line 1
      waterId,
      waterId,
      waterId,
      waterId,
      waterId,
      waterId,
      waterId,
      waterId,
      waterId,
      waterId,

      // line 2
      water2Id,
      water2Id,
      water2Id,
      water2Id,
      water2Id,
      water2Id,
      water2Id,
      water2Id,
      water2Id,
      water2Id,

      // line 3
      water2Id,
      water2Id,
      water2Id,
      water2Id,
      water2Id,
      water2Id,
      water2Id,
      water2Id,
      water2Id,
      water2Id,

      // line 4
      water2Id,
      water2Id,
      water2Id,
      water2Id,
      water2Id,
      water2Id,
      water2Id,
      water2Id,
      water2Id,
      water2Id,
  };

  _u16 tilesCount = sizeof(tiles) / sizeof(SpriteId);
  Scene* scene = Game_GetScene();
  return Scene_SetupTileMap(scene, tiles, tilesCount, 10);
}

void _AssignMenuSprites() {
  Menu* menu = Game_GetMenu();
  // MenuSetupButtonSprites(menu, SpritesRepo_Set(&btn_left),
  //                        SpritesRepo_Set(&btn_mid),
  //                        SpritesRepo_Set(&btn_right));

  MenuSetupButtonSprites(menu, &btn_left, &btn_mid, &btn_right);

  const SpriteData* pauseData[] = {&letter_p, &letter_a, &letter_u,
                                   &letter_s, &letter_e, &letter_d};
  _u8 lettersCount = sizeof(pauseData) / sizeof(SpriteData*);
  MenuCreateHCenterLabel(menu, pauseData, lettersCount, 40);

  const SpriteData* exitData[] = {&letter_e, &letter_x, &letter_i, &letter_t};
  lettersCount = sizeof(exitData) / sizeof(SpriteData*);
  MenuCreateHCenterButton(menu, exitData, lettersCount, 80);

  const SpriteData* resumeData[] = {&letter_r, &letter_e, &letter_s,
                                    &letter_u, &letter_m, &letter_e};
  lettersCount = sizeof(resumeData) / sizeof(SpriteData*);
  MenuCreateHCenterButton(menu, resumeData, lettersCount, 100);
}