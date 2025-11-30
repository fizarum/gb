#ifndef JOYSTICK_UTILS_H
#define JOYSTICK_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <device.h>
#include <specifications/input_data.h>
#include <types.h>

// button masks
static const _u16 rightTrigger = 1 << 0;
static const _u16 btnX = 1 << 1;
static const _u16 btnA = 1 << 2;
static const _u16 btnY = 1 << 3;
static const _u16 btnB = 1 << 4;
static const _u16 btnMenu = 1 << 8;
static const _u16 btnStart = 1 << 9;
static const _u16 btnSelect = 1 << 10;
static const _u16 btnDown = 1 << 11;
static const _u16 btnRight = 1 << 12;
static const _u16 btnLeft = 1 << 13;
static const _u16 btnUp = 1 << 14;
static const _u16 leftTrigger = 1 << 15;

typedef union KeysStatus_t {
  struct KeyMap {
    bool B : 1;
    bool Y : 1;
    bool A : 1;
    bool X : 1;
    bool rightTrigger : 1;
    bool leftTrigger : 1;
    bool Up : 1;
    bool Left : 1;
    bool Right : 1;
    bool Down : 1;
    bool Select : 1;
    bool Start : 1;
    bool Menu : 1;

    _u8 unused : 3;
  } keyMap;

  _u16 value;
} KeysStatus_t;

// menu
bool IsButtonMenuPressed(const InputDeviceData* data);
bool IsButtonMenuReleased(const InputDeviceData* data);
bool IsButtonMenuLongPressed(const InputDeviceData* data);

// start
bool IsButtonStartPressed(const InputDeviceData* data);
bool IsButtonStartReleased(const InputDeviceData* data);
bool IsButtonStartLongPressed(const InputDeviceData* data);

// select
bool IsButtonSelectPressed(const InputDeviceData* data);
bool IsButtonSelectReleased(const InputDeviceData* data);
bool IsButtonSelectLongPressed(const InputDeviceData* data);

// down
bool IsButtonDownPressed(const InputDeviceData* data);
bool IsButtonDownReleased(const InputDeviceData* data);
bool IsButtonDownLongPressed(const InputDeviceData* data);

// right
bool IsButtonRightPressed(const InputDeviceData* data);
bool IsButtonRightReleased(const InputDeviceData* data);
bool IsButtonRightLongPressed(const InputDeviceData* data);

// left
bool IsButtonLeftPressed(const InputDeviceData* data);
bool IsButtonLeftReleased(const InputDeviceData* data);
bool IsButtonLeftLongPressed(const InputDeviceData* data);

// up
bool IsButtonUpPressed(const InputDeviceData* data);
bool IsButtonUpReleased(const InputDeviceData* data);
bool IsButtonUpLongPressed(const InputDeviceData* data);

// leftTrigger
bool IsButtonLTPressed(const InputDeviceData* data);
bool IsButtonLTReleased(const InputDeviceData* data);
bool IsButtonLTLongPressed(const InputDeviceData* data);

// rightTrigger
bool IsButtonRTPressed(const InputDeviceData* data);
bool IsButtonRTReleased(const InputDeviceData* data);
bool IsButtonRTLongPressed(const InputDeviceData* data);

// btnX
bool IsButtonXPressed(const InputDeviceData* data);
bool IsButtonXReleased(const InputDeviceData* data);
bool IsButtonXLongPressed(const InputDeviceData* data);

// btnA
bool IsButtonAPressed(const InputDeviceData* data);
bool IsButtonAReleased(const InputDeviceData* data);
bool IsButtonALongPressed(const InputDeviceData* data);

// btnY
bool IsButtonYPressed(const InputDeviceData* data);
bool IsButtonYReleased(const InputDeviceData* data);
bool IsButtonYLongPressed(const InputDeviceData* data);

// btnB
bool IsButtonBPressed(const InputDeviceData* data);
bool IsButtonBReleased(const InputDeviceData* data);
bool IsButtonBLongPressed(const InputDeviceData* data);

#ifdef __cplusplus
}
#endif

#endif  // JOYSTICK_UTILS_H