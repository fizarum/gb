#include "joystick_utils.h"

bool IsButtonMenuPressed(const InputDeviceData_t *data) {
  return IsButtonPressed(data, btnMenu);
}

bool IsButtonMenuReleased(const InputDeviceData_t *data) {
  return IsButtonReleased(data, btnMenu);
}

bool IsButtonMenuLongPressed(const InputDeviceData_t *data) {
  return IsButtonLongPressed(data, btnMenu);
}

bool IsButtonStartPressed(const InputDeviceData_t *data) {
  return IsButtonPressed(data, btnStart);
}

bool IsButtonStartReleased(const InputDeviceData_t *data) {
  return IsButtonReleased(data, btnStart);
}

bool IsButtonStartLongPressed(const InputDeviceData_t *data) {
  return IsButtonLongPressed(data, btnStart);
}

bool IsButtonSelectPressed(const InputDeviceData_t *data) {
  return IsButtonPressed(data, btnSelect);
}

bool IsButtonSelectReleased(const InputDeviceData_t *data) {
  return IsButtonReleased(data, btnSelect);
}

bool IsButtonSelectLongPressed(const InputDeviceData_t *data) {
  return IsButtonLongPressed(data, btnSelect);
}

bool IsButtonDownPressed(const InputDeviceData_t *data) {
  return IsButtonPressed(data, btnDown);
}

bool IsButtonDownReleased(const InputDeviceData_t *data) {
  return IsButtonReleased(data, btnDown);
}

bool IsButtonDownLongPressed(const InputDeviceData_t *data) {
  return IsButtonLongPressed(data, btnDown);
}

bool IsButtonRightPressed(const InputDeviceData_t *data) {
  return IsButtonPressed(data, btnRight);
}

bool IsButtonRightReleased(const InputDeviceData_t *data) {
  return IsButtonReleased(data, btnRight);
}

bool IsButtonRightLongPressed(const InputDeviceData_t *data) {
  return IsButtonLongPressed(data, btnRight);
}

bool IsButtonLeftPressed(const InputDeviceData_t *data) {
  return IsButtonPressed(data, btnLeft);
}

bool IsButtonLeftReleased(const InputDeviceData_t *data) {
  return IsButtonReleased(data, btnLeft);
}

bool IsButtonLeftLongPressed(const InputDeviceData_t *data) {
  return IsButtonLongPressed(data, btnLeft);
}

bool IsButtonUpPressed(const InputDeviceData_t *data) {
  return IsButtonPressed(data, btnUp);
}

bool IsButtonUpReleased(const InputDeviceData_t *data) {
  return IsButtonReleased(data, btnUp);
}

bool IsButtonUpLongPressed(const InputDeviceData_t *data) {
  return IsButtonLongPressed(data, btnUp);
}

bool IsButtonLTPressed(const InputDeviceData_t *data) {
  return IsButtonPressed(data, leftTrigger);
}

bool IsButtonLTReleased(const InputDeviceData_t *data) {
  return IsButtonReleased(data, leftTrigger);
}

bool IsButtonLTLongPressed(const InputDeviceData_t *data) {
  return IsButtonLongPressed(data, leftTrigger);
}

bool IsButtonRTPressed(const InputDeviceData_t *data) {
  return IsButtonPressed(data, rightTrigger);
}

bool IsButtonRTReleased(const InputDeviceData_t *data) {
  return IsButtonReleased(data, rightTrigger);
}

bool IsButtonRTLongPressed(const InputDeviceData_t *data) {
  return IsButtonLongPressed(data, rightTrigger);
}

bool IsButtonXPressed(const InputDeviceData_t *data) {
  return IsButtonPressed(data, btnX);
}

bool IsButtonXReleased(const InputDeviceData_t *data) {
  return IsButtonReleased(data, btnX);
}

bool IsButtonXLongPressed(const InputDeviceData_t *data) {
  return IsButtonLongPressed(data, btnX);
}

bool IsButtonAPressed(const InputDeviceData_t *data) {
  return IsButtonPressed(data, btnA);
}

bool IsButtonAReleased(const InputDeviceData_t *data) {
  return IsButtonReleased(data, btnA);
}

bool IsButtonALongPressed(const InputDeviceData_t *data) {
  return IsButtonLongPressed(data, btnA);
}

bool IsButtonYPressed(const InputDeviceData_t *data) {
  return IsButtonPressed(data, btnY);
}

bool IsButtonYReleased(const InputDeviceData_t *data) {
  return IsButtonReleased(data, btnY);
}

bool IsButtonYLongPressed(const InputDeviceData_t *data) {
  return IsButtonLongPressed(data, btnY);
}

bool IsButtonBPressed(const InputDeviceData_t *data) {
  return IsButtonPressed(data, btnB);
}

bool IsButtonBReleased(const InputDeviceData_t *data) {
  return IsButtonReleased(data, btnB);
}

bool IsButtonBLongPressed(const InputDeviceData_t *data) {
  return IsButtonLongPressed(data, btnB);
}