#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <types.h>

#include "input_type.h"
#include "keycode.h"

typedef union {
  struct {
    KeyCode keycode : 8;
    InputType type : 4;
  };
  _u16 value;

} InputEvent;

#ifdef __cplusplus
}
#endif