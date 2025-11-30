#include "joystick_utils.h"

bool IsButtonMenuPressed(const InputDeviceData* data) {
  return IsButtonPressed(data, btnMenu);
}

bool IsButtonMenuReleased(const InputDeviceData* data) {
  return IsButtonReleased(data, btnMenu);
}

bool IsButtonMenuLongPressed(const InputDeviceData* data) {
  return IsButtonLongPressed(data, btnMenu);
}

bool IsButtonStartPressed(const InputDeviceData* data) {
  return IsButtonPressed(data, btnStart);
}

bool IsButtonStartReleased(const InputDeviceData* data) {
  return IsButtonReleased(data, btnStart);
}

bool IsButtonStartLongPressed(const InputDeviceData* data) {
  return IsButtonLongPressed(data, btnStart);
}

bool IsButtonSelectPressed(const InputDeviceData* data) {
  return IsButtonPressed(data, btnSelect);
}

bool IsButtonSelectReleased(const InputDeviceData* data) {
  return IsButtonReleased(data, btnSelect);
}

bool IsButtonSelectLongPressed(const InputDeviceData* data) {
  return IsButtonLongPressed(data, btnSelect);
}

bool IsButtonDownPressed(const InputDeviceData* data) {
  return IsButtonPressed(data, btnDown);
}

bool IsButtonDownReleased(const InputDeviceData* data) {
  return IsButtonReleased(data, btnDown);
}

bool IsButtonDownLongPressed(const InputDeviceData* data) {
  return IsButtonLongPressed(data, btnDown);
}

bool IsButtonRightPressed(const InputDeviceData* data) {
  return IsButtonPressed(data, btnRight);
}

bool IsButtonRightReleased(const InputDeviceData* data) {
  return IsButtonReleased(data, btnRight);
}

bool IsButtonRightLongPressed(const InputDeviceData* data) {
  return IsButtonLongPressed(data, btnRight);
}

bool IsButtonLeftPressed(const InputDeviceData* data) {
  return IsButtonPressed(data, btnLeft);
}

bool IsButtonLeftReleased(const InputDeviceData* data) {
  return IsButtonReleased(data, btnLeft);
}

bool IsButtonLeftLongPressed(const InputDeviceData* data) {
  return IsButtonLongPressed(data, btnLeft);
}

bool IsButtonUpPressed(const InputDeviceData* data) {
  return IsButtonPressed(data, btnUp);
}

bool IsButtonUpReleased(const InputDeviceData* data) {
  return IsButtonReleased(data, btnUp);
}

bool IsButtonUpLongPressed(const InputDeviceData* data) {
  return IsButtonLongPressed(data, btnUp);
}

bool IsButtonLTPressed(const InputDeviceData* data) {
  return IsButtonPressed(data, leftTrigger);
}

bool IsButtonLTReleased(const InputDeviceData* data) {
  return IsButtonReleased(data, leftTrigger);
}

bool IsButtonLTLongPressed(const InputDeviceData* data) {
  return IsButtonLongPressed(data, leftTrigger);
}

bool IsButtonRTPressed(const InputDeviceData* data) {
  return IsButtonPressed(data, rightTrigger);
}

bool IsButtonRTReleased(const InputDeviceData* data) {
  return IsButtonReleased(data, rightTrigger);
}

bool IsButtonRTLongPressed(const InputDeviceData* data) {
  return IsButtonLongPressed(data, rightTrigger);
}

bool IsButtonXPressed(const InputDeviceData* data) {
  return IsButtonPressed(data, btnX);
}

bool IsButtonXReleased(const InputDeviceData* data) {
  return IsButtonReleased(data, btnX);
}

bool IsButtonXLongPressed(const InputDeviceData* data) {
  return IsButtonLongPressed(data, btnX);
}

bool IsButtonAPressed(const InputDeviceData* data) {
  return IsButtonPressed(data, btnA);
}

bool IsButtonAReleased(const InputDeviceData* data) {
  return IsButtonReleased(data, btnA);
}

bool IsButtonALongPressed(const InputDeviceData* data) {
  return IsButtonLongPressed(data, btnA);
}

bool IsButtonYPressed(const InputDeviceData* data) {
  return IsButtonPressed(data, btnY);
}

bool IsButtonYReleased(const InputDeviceData* data) {
  return IsButtonReleased(data, btnY);
}

bool IsButtonYLongPressed(const InputDeviceData* data) {
  return IsButtonLongPressed(data, btnY);
}

bool IsButtonBPressed(const InputDeviceData* data) {
  return IsButtonPressed(data, btnB);
}

bool IsButtonBReleased(const InputDeviceData* data) {
  return IsButtonReleased(data, btnB);
}

bool IsButtonBLongPressed(const InputDeviceData* data) {
  return IsButtonLongPressed(data, btnB);
}