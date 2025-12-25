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
bool IsButtonMenuPressed(const InputDeviceExtension* data);
bool IsButtonMenuReleased(const InputDeviceExtension* data);
bool IsButtonMenuLongPressed(const InputDeviceExtension* data);

// start
bool IsButtonStartPressed(const InputDeviceExtension* data);
bool IsButtonStartReleased(const InputDeviceExtension* data);
bool IsButtonStartLongPressed(const InputDeviceExtension* data);

// select
bool IsButtonSelectPressed(const InputDeviceExtension* data);
bool IsButtonSelectReleased(const InputDeviceExtension* data);
bool IsButtonSelectLongPressed(const InputDeviceExtension* data);

// down
bool IsButtonDownPressed(const InputDeviceExtension* data);
bool IsButtonDownReleased(const InputDeviceExtension* data);
bool IsButtonDownLongPressed(const InputDeviceExtension* data);

// right
bool IsButtonRightPressed(const InputDeviceExtension* data);
bool IsButtonRightReleased(const InputDeviceExtension* data);
bool IsButtonRightLongPressed(const InputDeviceExtension* data);

// left
bool IsButtonLeftPressed(const InputDeviceExtension* data);
bool IsButtonLeftReleased(const InputDeviceExtension* data);
bool IsButtonLeftLongPressed(const InputDeviceExtension* data);

// up
bool IsButtonUpPressed(const InputDeviceExtension* data);
bool IsButtonUpReleased(const InputDeviceExtension* data);
bool IsButtonUpLongPressed(const InputDeviceExtension* data);

// leftTrigger
bool IsButtonLTPressed(const InputDeviceExtension* data);
bool IsButtonLTReleased(const InputDeviceExtension* data);
bool IsButtonLTLongPressed(const InputDeviceExtension* data);

// rightTrigger
bool IsButtonRTPressed(const InputDeviceExtension* data);
bool IsButtonRTReleased(const InputDeviceExtension* data);
bool IsButtonRTLongPressed(const InputDeviceExtension* data);

// btnX
bool IsButtonXPressed(const InputDeviceExtension* data);
bool IsButtonXReleased(const InputDeviceExtension* data);
bool IsButtonXLongPressed(const InputDeviceExtension* data);

// btnA
bool IsButtonAPressed(const InputDeviceExtension* data);
bool IsButtonAReleased(const InputDeviceExtension* data);
bool IsButtonALongPressed(const InputDeviceExtension* data);

// btnY
bool IsButtonYPressed(const InputDeviceExtension* data);
bool IsButtonYReleased(const InputDeviceExtension* data);
bool IsButtonYLongPressed(const InputDeviceExtension* data);

// btnB
bool IsButtonBPressed(const InputDeviceExtension* data);
bool IsButtonBReleased(const InputDeviceExtension* data);
bool IsButtonBLongPressed(const InputDeviceExtension* data);

#ifdef __cplusplus
}
#endif

#endif  // JOYSTICK_UTILS_H