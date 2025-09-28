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
static TileMap* _AssignTileMap();

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
  TileMap* tileMap = _AssignTileMap();
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
  Scene* scene = Game_GetScene();
  
  SpriteId waterSid = Scene_AddSprite(scene, &water, LAYER_FAR);

  grassId = Scene_AddSprite(scene, &grass, LAYER_TILEMAP);
  waterId = Scene_AddSprite(scene, &water, LAYER_TILEMAP);
  water2Id = Scene_AddSprite(scene, &water2, LAYER_TILEMAP);
  sandId = Scene_AddSprite(scene, &sand, LAYER_TILEMAP);
  coastId = Scene_AddSprite(scene, &coast, LAYER_TILEMAP);
  playerId = Scene_AddGameObject(scene, &player, LAYER_MID, true, true, false);

  SpriteId animatedFire = Scene_AddSprite(scene, &fire, LAYER_NEAR);
  Scene_ChangeSpriteAnimationSpeed(animatedFire, ANIMATION_SPEED_SUPER_FAST);
  Scene_MoveSpriteTo(scene, animatedFire, 100, 40);

  // stop animation
  // Scene_ChangeSpriteAnimationSpeed(animatedFire, ANIMATION_SPEED_NONE);

  Scene_MoveSpriteTo(scene, waterSid, 100, 100);

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
}

TileMap* _AssignTileMap() {
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
  return Scene_SetupTileMap(Game_GetScene(), locationBackground, tilesCount,
                            20);
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