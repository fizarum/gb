#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <types.h>

#include "input_type.h"
#include "keycode.h"

typedef union {
  struct {
    KeyCode keycode : 12;
    // TODO: rework this part. we can't keep buttons state in 4 bits about few
    // buttons at same time.
    InputType type : 4;
  };
  _u16 value;

} InputEvent;

#ifdef __cplusplus
}
#endif