#ifndef UI_VIEW_H
#define UI_VIEW_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <types.h>

typedef enum SizePolicyType_t {
  // in this case ViewSizeChangedCallback should provide exact size
  WrapContent,

  // stretches view to max available space, ignores values from
  // ViewSizeChangedCallback
  MatchParent,

  // in this case view's size is calculated from all avail parent's size divided
  // on sum of all view weights. Weight value is taken from SizePolicy_t.weight
  //
  // For example view1.weight = 1, view2.weight = 1 means each has 50% of
  // parent's size.
  // For view1.weight = 1, view2.weight = 2: view1 has 1/3 view2 has 2/3 of
  // parent's size
  Weight,
} SizePolicyType_t;

typedef union SizePolicy_t {
  struct {
    SizePolicyType_t type : 8;
    _u8 weight : 8;
  };
  _u16 value;
} SizePolicy_t;

typedef struct View_t View_t;
typedef void (*DrawCallback)(View_t* view, const _u16 left, const _u16 top,
                             const _u16 width, const _u16 height);
typedef void (*ViewCallback)(void* customView);
typedef void (*ViewSizeChangedCallback)(View_t* view, const _u16 width,
                                        const _u16 height);

View_t* View_Create(void* customView, bool isBox, DrawCallback drawCallback,
                    ViewCallback onDestroyCallback,
                    ViewSizeChangedCallback onSizeChangedCallback,
                    const SizePolicy_t widthPolicy,
                    const SizePolicy_t heightPolicy);
void View_Destroy(View_t* node);

void View_Draw(View_t* view);

uint16_t View_GetId(View_t* view);
void View_SetId(View_t* view, _u16 id);

bool View_IsBox(View_t* view);

void View_SetPosition(View_t* node, _u16 x, _u16 y);
uint16_t View_GetXPosition(const View_t* node);
uint16_t View_GetYPosition(const View_t* node);

uint16_t View_GetWidth(const View_t* node);
uint16_t View_GetHeight(const View_t* node);

SizePolicy_t View_GetWidthPolicy(const View_t* view);
SizePolicy_t View_GetHeightPolicy(const View_t* view);

void View_SetWidth(View_t* node, const _u16 width);
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