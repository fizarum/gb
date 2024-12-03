#ifndef UI_VIEW_H
#define UI_VIEW_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <types.h>

typedef struct View_t View_t;
typedef void (*DrawCallback)(View_t* view, const _u16 left, const _u16 top,
                             const _u16 width, const _u16 height);
typedef void (*ViewCallback)(void* customView);
typedef void (*ViewSizeChangedCallback)(View_t* view, const _u16 width,
                                        const _u16 height);

View_t* View_Create(void* customView, DrawCallback drawCallback,
                    ViewCallback onDestroyCallback,
                    ViewSizeChangedCallback onSizeChangedCallback);
void View_Destroy(View_t* node);

void View_Draw(View_t* view);

uint16_t View_GetId(View_t* view);
void View_SetId(View_t* view, _u16 id);

void View_SetPosition(View_t* node, _u16 x, _u16 y);
uint16_t View_GetXPosition(const View_t* node);
uint16_t View_GetYPosition(const View_t* node);

uint16_t View_GetWidth(const View_t* node);
void View_SetWidth(View_t* node, const _u16 width);

uint16_t View_GetHeight(const View_t* node);
void View_SetHeight(View_t* node, const _u16 height);

void* View_GetCustomView(const View_t* view);

/**
 * @brief Mark view as updated and to be redrawn
 */
void View_SetUpdated(View_t* view);
bool View_IsUpdated(const View_t* view);

#ifdef __cplusplus
}
#endif

#endif  // UI_VIEW_H