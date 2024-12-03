#ifndef UI_COMPOSER_H
#define UI_COMPOSER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "tree.h"
#include "view.h"

typedef struct Composer_t Composer_t;

Composer_t* Composer_Create(const uint16_t screenWidth,
                            const uint16_t screenHeight);
// TODO: add Composer_Destroy(Composer_t *composer);

// TODO: add Composer_Clear(Composer_t *composer);

uint16_t Composer_GetRootId(const Composer_t* composer);

_u16 Composer_AddBox(Composer_t* composer, uint16_t branchId, View_t* view);

uint16_t Composer_AddView(Composer_t* composer, uint16_t branchId,
                          View_t* view);

void Composer_Draw(Composer_t* composer);

#ifdef __cplusplus
}
#endif

#endif  // UI_COMPOSER_H