#include "progress.h"

#include <stdlib.h>

#include "../gfx/gfx.h"

typedef struct Progress_t {
  _u8 value;
  _u8 maxValue;

  View_t *view;
} Progress_t;

static void RecalculateSize(Progress_t *progress);
static void Draw(View_t *view, const uint16_t left, const uint16_t top,
                 const uint16_t right, const uint16_t bottom);
static void Destroy(void *progressArg);

View_t *Progress_Create(_u8 value, _u8 maxValue) {
  Progress_t *progress = (Progress_t *)malloc(sizeof(Progress_t));

  if (progress == NULL) {
    return NULL;
  }

  progress->value = value;
  progress->maxValue = maxValue;
  SizePolicy_t widthPolicy = {.type = MatchParent, .weight = 0};
  SizePolicy_t heightpolicy = {.type = WrapContent, .weight = 0};
  progress->view =
      View_Create(progress, &Draw, &Destroy, NULL, widthPolicy, heightpolicy);

  RecalculateSize(progress);

  return progress->view;
}

// private part
static const _u8 height = 14;
static void RecalculateSize(Progress_t *progress) {
  View_SetWidth(progress->view, 1);
  View_SetHeight(progress->view, height);
}

static void Draw(View_t *view, const uint16_t left, const uint16_t top,
                 const uint16_t right, const uint16_t bottom) {
  Progress_t *progress = (Progress_t *)View_GetCustomView(view);
  if (progress->value > progress->maxValue) {
    return;
  }
  _u16 width = right - left;
  _u16 height = bottom - top;

  _u16 pixelsPerPercent = width / progress->maxValue;
  _u16 widthOfFilledPart = pixelsPerPercent * progress->value;

  if (progress->value < progress->maxValue) {
    // top line
    GFX_DrawHLine(left, top, width, 1, GFXGetFontColor());
    // bottom line
    GFX_DrawHLine(left, bottom, width, 1, GFXGetFontColor());
    // right vertical line
    GFX_DrawVLine(right, top, height, 1, GFXGetFontColor());
  }

  // progress
  GFXDrawFilledRect(left, left + widthOfFilledPart, top, bottom,
                    GFXGetFontColor());
}

static void Destroy(void *progressArg) {
  if (progressArg == NULL) {
    return;
  }

  Progress_t *progress = (Progress_t *)progressArg;

  free(progress);
}