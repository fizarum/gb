#ifndef UI_COMPOSER_H
#define UI_COMPOSER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "tree.h"
#include "view.h"

typedef struct Composer Composer;

void Composer_Init(const _u16 width, const _u16 height);

_u16 Composer_GetRootId();

_u16 Composer_AddBox(_u16 parentId, View_t* view);
_u16 Composer_AddVBox(_u16 parentId);
_u16 Composer_AddHBox(_u16 parentId);
_u16 Composer_AddView(_u16 parentId, View_t* view);

View_t* Composer_FindView(const _u16 viewId);

void Composer_Recompose();
void Composer_Draw();
void Composer_Clear();

#ifdef __cplusplus
}
#endif

#endif  // UI_COMPOSER_H