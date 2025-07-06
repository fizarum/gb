#include "demo_app.h"

#include <specifications/input_data.h>
#include <ui/gfx/gfx.h>

#include "../../devices/joystick/joystick.h"
#include "../../gsdk/game.h"
#include "../../gsdk/screen/screen_config.h"
#include "../apps_utils.h"
#include "resources.h"

static AppSpecification_t specs = {
    .name = "Demo GSDK",
    .background = COLOR_BLACK,
    .onPause = &App_StubFunction,
    .onResume = &App_StubFunction,
    .onUpdate = &App_StubFunction,
};

static void _AssignMenuSprites();
static void _AssignSprites();
static TileMap_t* _AssignTileMap();

static Game* game;
static ObjectId playerId;
static SpriteId grassId;
static SpriteId sandId;
static SpriteId coastId;
static SpriteId waterId;
static SpriteId water2Id;

static void onAppStart() {
  ESP_LOGI(specs.name, "on app start...");
  GFX_SetBackgroundColor(specs.background);
  ScreenConfig_Create(320, 240);
  game = Game_Create();

  PrepareSprites();
  _AssignSprites();
  _AssignMenuSprites();
  TileMap_t* tileMap = _AssignTileMap();
  Game_SetTileMap(tileMap);

  // Game_Start();
}

static void onAppStop() { Game_Destroy(game); }

static void onAppUpdate(void) {
  Game_Update();
  GFX_Redraw();
}

static void handleKey(const void* keyData) {
  InputDeviceData_t* data = (InputDeviceData_t*)keyData;
  Scene_t* scene = Game_GetScene();

  if (IsButtonLeftPressed(data)) {
    SceneMoveGameObjectBy(scene, playerId, -1, 0);
  } else if (IsButtonRightPressed(data)) {
    SceneMoveGameObjectBy(scene, playerId, 1, 0);
  } else if (IsButtonUpPressed(data)) {
    SceneMoveGameObjectBy(scene, playerId, 0, -1);
  } else if (IsButtonDownPressed(data)) {
    SceneMoveGameObjectBy(scene, playerId, 0, 1);
  } else if (IsButtonStartReleased(data)) {
    Game_TogglePauseResume();
  }
}

static void onAppRedraw(RedrawType_t redrawType) {
  if (redrawType == RedrawFull) {
    App_DrawBackgroundAndTitle(specs.name, specs.background);
  }
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
  Scene_t* scene = Game_GetScene();

  const SpriteData_t* playerSd = &player;
  const SpriteData_t* sandSd = &sand;
  const SpriteData_t* coastSd = &coast;

  SpriteId waterSid = SceneAddSprite2(scene, &water, LAYER_FAR);

  grassId = SceneAddSprite2(scene, &grass, LAYER_TILEMAP);
  waterId = SceneAddSprite2(scene, &water, LAYER_TILEMAP);
  water2Id = SceneAddSprite2(scene, &water2, LAYER_TILEMAP);
  sandId = SceneAddSprite2(scene, sandSd, LAYER_TILEMAP);
  coastId = SceneAddSprite2(scene, coastSd, LAYER_TILEMAP);
  playerId = SceneAddGameObject(scene, playerSd, LAYER_MID, true, true, false);

  SpriteId animatedFire = SceneAddSprite2(scene, &fire, LAYER_NEAR);
  SceneChangeSpriteAnimationSpeed(animatedFire, ANIMATION_SPEED_SUPER_FAST);
  SceneMoveSprite2To(scene, animatedFire, 100, 40);

  // stop animation
  // SceneChangeSpriteAnimationSpeed(animatedFire, ANIMATION_SPEED_NONE);

  SceneMoveSprite2To(scene, waterSid, 100, 100);

  // tree sprite, 8x8 pixels
  ObjectId treeObjectId =
      SceneAddGameObject(scene, &tree, LAYER_MID, true, true, false);
  SceneMoveGameObjectTo(scene, treeObjectId, 30, 30);

  // another tree, used as background sprite
  SpriteId treeBackId = SceneAddSprite2(scene, &tree, LAYER_FAR);
  SceneMoveSprite2To(scene, treeBackId, 10, 35);

  // tree used as foreground sprite
  SpriteId treeNearId = SceneAddSprite2(scene, &tree, LAYER_NEAR);
  SceneMoveSprite2To(scene, treeNearId, 80, 10);
}

TileMap_t* _AssignTileMap() {
  SpriteId locationBackground[] = {
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

      // line 5
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

      // line 6
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

      // line 7
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

      // line 8
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

  _u16 tilesCount = sizeof(locationBackground) / sizeof(SpriteId);
  return SceneSetupTileMap(Game_GetScene(), locationBackground, tilesCount, 20);
}

void _AssignMenuSprites() {
  Menu_t* menu = Game_GetMenu();
  // MenuSetupButtonSprites(menu, SpritesRepo_Set(&btn_left),
  //                        SpritesRepo_Set(&btn_mid),
  //                        SpritesRepo_Set(&btn_right));

  MenuSetupButtonSprites(menu, &btn_left, &btn_mid, &btn_right);

  const SpriteData_t* pauseData[] = {&letter_p, &letter_a, &letter_u,
                                     &letter_s, &letter_e, &letter_d};
  _u8 lettersCount = sizeof(pauseData) / sizeof(SpriteData_t*);
  MenuCreateHCenterLabel(menu, pauseData, lettersCount, 40);

  const SpriteData_t* exitData[] = {&letter_e, &letter_x, &letter_i, &letter_t};
  lettersCount = sizeof(exitData) / sizeof(SpriteData_t*);
  MenuCreateHCenterButton(menu, exitData, lettersCount, 80);

  const SpriteData_t* resumeData[] = {&letter_r, &letter_e, &letter_s,
                                      &letter_u, &letter_m, &letter_e};
  lettersCount = sizeof(resumeData) / sizeof(SpriteData_t*);
  MenuCreateHCenterButton(menu, resumeData, lettersCount, 100);
}