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
bool IsButtonMenuPressed(const InputDeviceData_t *data);
bool IsButtonMenuReleased(const InputDeviceData_t *data);
bool IsButtonMenuLongPressed(const InputDeviceData_t *data);

// start
bool IsButtonStartPressed(const InputDeviceData_t *data);
bool IsButtonStartReleased(const InputDeviceData_t *data);
bool IsButtonStartLongPressed(const InputDeviceData_t *data);

// select
bool IsButtonSelectPressed(const InputDeviceData_t *data);
bool IsButtonSelectReleased(const InputDeviceData_t *data);
bool IsButtonSelectLongPressed(const InputDeviceData_t *data);

// down
bool IsButtonDownPressed(const InputDeviceData_t *data);
bool IsButtonDownReleased(const InputDeviceData_t *data);
bool IsButtonDownLongPressed(const InputDeviceData_t *data);

// right
bool IsButtonRightPressed(const InputDeviceData_t *data);
bool IsButtonRightReleased(const InputDeviceData_t *data);
bool IsButtonRightLongPressed(const InputDeviceData_t *data);

// left
bool IsButtonLeftPressed(const InputDeviceData_t *data);
bool IsButtonLeftReleased(const InputDeviceData_t *data);
bool IsButtonLeftLongPressed(const InputDeviceData_t *data);

// up
bool IsButtonUpPressed(const InputDeviceData_t *data);
bool IsButtonUpReleased(const InputDeviceData_t *data);
bool IsButtonUpLongPressed(const InputDeviceData_t *data);

// leftTrigger
bool IsButtonLTPressed(const InputDeviceData_t *data);
bool IsButtonLTReleased(const InputDeviceData_t *data);
bool IsButtonLTLongPressed(const InputDeviceData_t *data);

// rightTrigger
bool IsButtonRTPressed(const InputDeviceData_t *data);
bool IsButtonRTReleased(const InputDeviceData_t *data);
bool IsButtonRTLongPressed(const InputDeviceData_t *data);

// btnX
bool IsButtonXPressed(const InputDeviceData_t *data);
bool IsButtonXReleased(const InputDeviceData_t *data);
bool IsButtonXLongPressed(const InputDeviceData_t *data);

// btnA
bool IsButtonAPressed(const InputDeviceData_t *data);
bool IsButtonAReleased(const InputDeviceData_t *data);
bool IsButtonALongPressed(const InputDeviceData_t *data);

// btnY
bool IsButtonYPressed(const InputDeviceData_t *data);
bool IsButtonYReleased(const InputDeviceData_t *data);
bool IsButtonYLongPressed(const InputDeviceData_t *data);

// btnB
bool IsButtonBPressed(const InputDeviceData_t *data);
bool IsButtonBReleased(const InputDeviceData_t *data);
bool IsButtonBLongPressed(const InputDeviceData_t *data);

#ifdef __cplusplus
}
#endif

#endif  // JOYSTICK_UTILS_H