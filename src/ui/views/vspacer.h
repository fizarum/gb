#ifndef UI_VERTICAL_SPACER_H
#define UI_VERTICAL_SPACER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../view.h"

typedef struct VSpacer_t VSpacer_t;

View_t *VSpacer_Create(const _u16 height);

#ifdef __cplusplus
}
#endif

#endif  // UI_VERTICAL_SPACER_H