#ifndef UI_LIST_FOCUS_H
#define UI_LIST_FOCUS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../view.h"

typedef struct ListFocus_t ListFocus_t;

View_t *ListFocus_Create(_u8 items);
void ListFocus_SelectItemIndex(View_t *view, _u8 index);
#ifdef __cplusplus
}
#endif

#endif  // UI_LIST_FOCUS_H