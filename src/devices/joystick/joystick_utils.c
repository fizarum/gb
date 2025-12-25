#include "joystick_utils.h"

bool IsButtonMenuPressed(const InputDeviceExtension* data) {
  return IsButtonPressed(data, btnMenu);
}

bool IsButtonMenuReleased(const InputDeviceExtension* data) {
  return IsButtonReleased(data, btnMenu);
}

bool IsButtonMenuLongPressed(const InputDeviceExtension* data) {
  return IsButtonLongPressed(data, btnMenu);
}

bool IsButtonStartPressed(const InputDeviceExtension* data) {
  return IsButtonPressed(data, btnStart);
}

bool IsButtonStartReleased(const InputDeviceExtension* data) {
  return IsButtonReleased(data, btnStart);
}

bool IsButtonStartLongPressed(const InputDeviceExtension* data) {
  return IsButtonLongPressed(data, btnStart);
}

bool IsButtonSelectPressed(const InputDeviceExtension* data) {
  return IsButtonPressed(data, btnSelect);
}

bool IsButtonSelectReleased(const InputDeviceExtension* data) {
  return IsButtonReleased(data, btnSelect);
}

bool IsButtonSelectLongPressed(const InputDeviceExtension* data) {
  return IsButtonLongPressed(data, btnSelect);
}

bool IsButtonDownPressed(const InputDeviceExtension* data) {
  return IsButtonPressed(data, btnDown);
}

bool IsButtonDownReleased(const InputDeviceExtension* data) {
  return IsButtonReleased(data, btnDown);
}

bool IsButtonDownLongPressed(const InputDeviceExtension* data) {
  return IsButtonLongPressed(data, btnDown);
}

bool IsButtonRightPressed(const InputDeviceExtension* data) {
  return IsButtonPressed(data, btnRight);
}

bool IsButtonRightReleased(const InputDeviceExtension* data) {
  return IsButtonReleased(data, btnRight);
}

bool IsButtonRightLongPressed(const InputDeviceExtension* data) {
  return IsButtonLongPressed(data, btnRight);
}

bool IsButtonLeftPressed(const InputDeviceExtension* data) {
  return IsButtonPressed(data, btnLeft);
}

bool IsButtonLeftReleased(const InputDeviceExtension* data) {
  return IsButtonReleased(data, btnLeft);
}

bool IsButtonLeftLongPressed(const InputDeviceExtension* data) {
  return IsButtonLongPressed(data, btnLeft);
}

bool IsButtonUpPressed(const InputDeviceExtension* data) {
  return IsButtonPressed(data, btnUp);
}

bool IsButtonUpReleased(const InputDeviceExtension* data) {
  return IsButtonReleased(data, btnUp);
}

bool IsButtonUpLongPressed(const InputDeviceExtension* data) {
  return IsButtonLongPressed(data, btnUp);
}

bool IsButtonLTPressed(const InputDeviceExtension* data) {
  return IsButtonPressed(data, leftTrigger);
}

bool IsButtonLTReleased(const InputDeviceExtension* data) {
  return IsButtonReleased(data, leftTrigger);
}

bool IsButtonLTLongPressed(const InputDeviceExtension* data) {
  return IsButtonLongPressed(data, leftTrigger);
}

bool IsButtonRTPressed(const InputDeviceExtension* data) {
  return IsButtonPressed(data, rightTrigger);
}

bool IsButtonRTReleased(const InputDeviceExtension* data) {
  return IsButtonReleased(data, rightTrigger);
}

bool IsButtonRTLongPressed(const InputDeviceExtension* data) {
  return IsButtonLongPressed(data, rightTrigger);
}

bool IsButtonXPressed(const InputDeviceExtension* data) {
  return IsButtonPressed(data, btnX);
}

bool IsButtonXReleased(const InputDeviceExtension* data) {
  return IsButtonReleased(data, btnX);
}

bool IsButtonXLongPressed(const InputDeviceExtension* data) {
  return IsButtonLongPressed(data, btnX);
}

bool IsButtonAPressed(const InputDeviceExtension* data) {
  return IsButtonPressed(data, btnA);
}

bool IsButtonAReleased(const InputDeviceExtension* data) {
  return IsButtonReleased(data, btnA);
}

bool IsButtonALongPressed(const InputDeviceExtension* data) {
  return IsButtonLongPressed(data, btnA);
}

bool IsButtonYPressed(const InputDeviceExtension* data) {
  return IsButtonPressed(data, btnY);
}

bool IsButtonYReleased(const InputDeviceExtension* data) {
  return IsButtonReleased(data, btnY);
}

bool IsButtonYLongPressed(const InputDeviceExtension* data) {
  return IsButtonLongPressed(data, btnY);
}

bool IsButtonBPressed(const InputDeviceExtension* data) {
  return IsButtonPressed(data, btnB);
}

bool IsButtonBReleased(const InputDeviceExtension* data) {
  return IsButtonReleased(data, btnB);
}

bool IsButtonBLongPressed(const InputDeviceExtension* data) {
  return IsButtonLongPressed(data, btnB);
}