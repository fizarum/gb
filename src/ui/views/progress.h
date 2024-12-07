#ifndef UI_PROGRESS_H
#define UI_PROGRESS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../view.h"

typedef struct Progress_t Progress_t;

View_t *Progress_Create(_u8 progress, _u8 maxValue);

#ifdef __cplusplus
}
#endif

#endif  // UI_PROGRESS_H