#include "progress.h"

#include <stdlib.h>

#include "../gfx/gfx.h"
#include "esp_log.h"

typedef struct Progress_t {
  _u8 value;
  _u8 maxValue;

  View_t* view;
} Progress_t;

static void RecalculateSize(Progress_t* progress);
static void Draw(View_t* view, const uint16_t left, const uint16_t top,
                 const uint16_t right, const uint16_t bottom);
static void HandleInput(View_t* view, InputEvent* event);
static void Destroy(void* progressArg);

View_t* Progress_Create(_u8 value, _u8 maxValue) {
  Progress_t* progress = (Progress_t*)malloc(sizeof(Progress_t));

  if (progress == NULL) {
    return NULL;
  }

  progress->value = value;
  progress->maxValue = maxValue;
  progress->view =
      View_Create(progress, false, &Draw, &HandleInput, &Destroy, NULL,
                  sizePolicyMatchParent.value, sizePolicyWrapContent.value);

  RecalculateSize(progress);

  return progress->view;
}

// private part
static const _u8 height = 14;
static void RecalculateSize(Progress_t* progress) {
  View_SetWidth(progress->view, 1);
  View_SetHeight(progress->view, height);
}

static void Draw(View_t* view, const uint16_t left, const uint16_t top,
                 const uint16_t right, const uint16_t bottom) {
  Progress_t* progress = (Progress_t*)View_GetCustomView(view);
  if (progress->value > progress->maxValue) {
    return;
  }

  // these values are shifted by line width (2 px)
  _u16 leftWithPadding = left + 2;
  _u16 rightWithPadding = right - 2;
  _u16 topWithPadding = top + 2;
  _u16 bottomWithPadding = bottom - 2;

  _u16 width = rightWithPadding - leftWithPadding;
  _u16 height = bottomWithPadding - topWithPadding;

  _u16 pixelsPerPercent = width / progress->maxValue;
  _u16 widthOfFilledPart = pixelsPerPercent * progress->value;

  if (progress->value < progress->maxValue) {
    GFX_DrawRect(leftWithPadding, topWithPadding, rightWithPadding,
                 bottomWithPadding, 1, GFX_GetFontColor());
    // progress
    GFX_DrawFilledRect(leftWithPadding + 2, topWithPadding + 2,
                       leftWithPadding + widthOfFilledPart,
                       bottomWithPadding - 2, GFX_GetFontColor());
  } else {
    // progress
    GFX_DrawFilledRect(leftWithPadding, topWithPadding,
                       leftWithPadding + widthOfFilledPart, bottomWithPadding,
                       GFX_GetFontColor());
  }
}

static void HandleInput(View_t* view, InputEvent* event) {
  ESP_LOGW("Progress", "handle input not yet implemented!");
}

static void Destroy(void* progressArg) {
  if (progressArg == NULL) {
    return;
  }

  Progress_t* progress = (Progress_t*)progressArg;

  free(progress);
}