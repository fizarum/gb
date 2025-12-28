#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <types.h>

typedef enum SizePolicyType {
  // in this case ViewSizeChangedCallback should provide exact size
  ExactSize,

  // in this case ViewSizeChangedCallback should provide exact size
  WrapContent,

  // stretches view to max available space, ignores values from
  // ViewSizeChangedCallback
  MatchParent,

  // in this case view's size is calculated from all avail parent's size divided
  // on sum of all view weights. Weight value is taken from SizePolicy.weight.
  // Max value is 7.
  //
  // For example view1.weight = 1, view2.weight = 1 means each has 50% of
  // parent's size.
  // For view1.weight = 1, view2.weight = 2: view1 has 1/3 view2 has 2/3 of
  // parent's size
  Weight,
} SizePolicyType;

typedef union SizePolicy {
  struct {
    _u8 custom : 1;
    SizePolicyType type : 3;
    _u8 weight : 8;
    _u8 reserved : 4;
  };
  // in case when type == WrapContent this value represents size in pixels

  // can not have value bigger than INT16_MAX
  _u16 value;
} SizePolicy;

static SizePolicy sizePolicyMatchParent = {
    .custom = 1, .type = MatchParent, .weight = 0};

static SizePolicy sizePolicyWrapContent = {
    .custom = 1, .type = WrapContent, .weight = 0};

#ifdef __cplusplus
}
#endif