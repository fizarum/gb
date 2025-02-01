#include "vspacer.h"

#include <stdlib.h>

typedef struct VSpacer_t {
  View_t *view;
} VSpacer_t;

static void Destroy(void *spacerArg);
static void RecalculateSize(VSpacer_t *spacer);

View_t *VSpacer_Create(const _u16 height) {
  VSpacer_t *spacer = (VSpacer_t *)malloc(sizeof(VSpacer_t));
  if (spacer == NULL) {
    return NULL;
  }

  SizePolicy_t heightPolicy = {.value = height};
  SizePolicy_t widthPolicy = {.value = 0};

  spacer->view = View_Create(spacer, false, NULL, &Destroy, NULL, widthPolicy,
                             heightPolicy);

  RecalculateSize(spacer);

  return spacer->view;
}

static void Destroy(void *spacerArg) {
  if (spacerArg == NULL) {
    return;
  }

  VSpacer_t *spacer = (VSpacer_t *)spacerArg;

  free(spacer);
}

static void RecalculateSize(VSpacer_t *spacer) {
  View_SetWidth(spacer->view, 0);
  SizePolicy_t *policy = View_GetHeightPolicy(spacer->view);
  View_SetHeight(spacer->view, policy->value);
}