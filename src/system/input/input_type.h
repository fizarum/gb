#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  Press,
  LongPress,
  Released,
  // can be used as unknown (or cancelled by user) event
  Cancelled,
} InputType;

#ifdef __cplusplus
}
#endif