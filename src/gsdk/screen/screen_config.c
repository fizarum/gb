#include "screen_config.h"

#include <stdlib.h>

typedef struct ScreenConfig {
  _u16 width;
  _u16 height;
} ScreenConfig;

static ScreenConfig *_config = NULL;

void ScreenConfig_Create(const _u16 width, const _u16 height) {
  _config = (ScreenConfig *)malloc(sizeof(ScreenConfig));

  assert(_config != NULL);

  _config->width = width;
  _config->height = height;
}

_u16 ScreenConfig_GetRealWidth() { return _config->width; }
_u16 ScreenConfig_GetRealHeight() { return _config->height; }