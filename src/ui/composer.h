#ifndef UI_COMPOSER_H
#define UI_COMPOSER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "tree.h"
#include "view.h"

typedef struct Composer_t Composer_t;

Composer_t* Composer_Create(const _u16 screenWidth, const _u16 screenHeight);
// TODO: add Composer_Destroy(Composer_t *composer);

// TODO: add Composer_Clear(Composer_t *composer);

uint16_t Composer_GetRootId(const Composer_t* composer);

_u16 Composer_AddBox(Composer_t* composer, _u16 branchId, View_t* view);
_u16 Composer_AddVBox(Composer_t* composer, _u16 branchId);
_u16 Composer_AddHBox(Composer_t* composer, _u16 branchId);

_u16 Composer_AddView(Composer_t* composer, _u16 branchId, View_t* view);

void Composer_Recompose(Composer_t* composer);

void Composer_Draw(Composer_t* composer);

#ifdef __cplusplus
}
#endif

#endif  // UI_COMPOSER_H