#ifndef UI_VIEW_H
#define UI_VIEW_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <system/input/input_event.h>
#include <types.h>

typedef enum SizePolicyType_t {
  // in this case ViewSizeChangedCallback should provide exact size
  WrapContent,

  // stretches view to max available space, ignores values from
  // ViewSizeChangedCallback
  MatchParent,

  // in this case view's size is calculated from all avail parent's size divided
  // on sum of all view weights. Weight value is taken from SizePolicy_t.weight.
  // Max value is 7.
  //
  // For example view1.weight = 1, view2.weight = 1 means each has 50% of
  // parent's size.
  // For view1.weight = 1, view2.weight = 2: view1 has 1/3 view2 has 2/3 of
  // parent's size
  Weight,
} SizePolicyType_t;

typedef union SizePolicy_t {
  struct {
    _u8 weight : 8;
    SizePolicyType_t type : 8;
  };
  // in case when type == WrapContent this value represents size in pixels
  _u16 value;
} SizePolicy_t;

typedef struct Padding_t {
  _u16 left;
  _u16 right;
  _u16 top;
  _u16 bottom;
} Padding_t;

typedef struct View_t View_t;
typedef void (*DrawCallback)(View_t* view, const _u16 left, const _u16 top,
                             const _u16 width, const _u16 height);
typedef void (*ViewCallback)(void* customView);
typedef void (*ViewSizeChangedCallback)(View_t* view, const _u16 width,
                                        const _u16 height);
typedef void (*InputCallback)(View_t* view, InputEvent* event);

View_t* View_Create(void* customView, bool isBox,
                    // callbacks part
                    DrawCallback drawCallback,       // drawnig
                    InputCallback inputCallback,     // input
                    ViewCallback onDestroyCallback,  // detroy
                    ViewSizeChangedCallback onSizeChangedCallback,
                    // size policy part
                    const SizePolicy_t widthPolicy,
                    const SizePolicy_t heightPolicy);
void View_Destroy(View_t* node);

void View_Draw(View_t* view);

_u16 View_GetId(View_t* view);
void View_SetId(View_t* view, _u16 id);

bool View_IsBox(View_t* view);

Padding_t* View_GetPadding(View_t* view);

void View_SetPosition(View_t* view, _u16 x, _u16 y);

_u16 View_GetXPosition(const View_t* view);

/**
 * @brief Get x position excluding padding
 */
_u16 View_GetContentXPosition(const View_t* view);

_u16 View_GetYPosition(const View_t* view);

/**
 * @brief Get y position excluding padding
 */
_u16 View_GetContentYPosition(const View_t* view);

_u16 View_GetWidth(const View_t* view);

/**
 * @brief Get Width excluding padding
 */
_u16 View_GetContentWidth(const View_t* view);
void View_SetWidth(View_t* view, const _u16 width);

/**
 * @brief Get Height excluding padding
 */
_u16 View_GetContentHeight(const View_t* view);
_u16 View_GetHeight(const View_t* view);
void View_SetHeight(View_t* view, const _u16 height);

void View_SetPadding(View_t* view, const _u16 l, const _u16 t, const _u16 r,
                     const _u16 b);
void View_SetHPadding(View_t* view, const _u16 horizontal);
void View_SetVPadding(View_t* view, const _u16 vertical);

SizePolicy_t* View_GetWidthPolicy(View_t* view);
SizePolicy_t* View_GetHeightPolicy(View_t* view);

void* View_GetCustomView(const View_t* view);

/**
 * @brief Mark view as updated and to be redrawn
 */
void View_SetUpdated(View_t* view);
bool View_IsUpdated(const View_t* view);

void View_HandleInput(View_t* view, InputEvent* inputEvent);

#ifdef __cplusplus
}
#endif

#endif  // UI_VIEW_H