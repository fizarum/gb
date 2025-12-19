#ifndef CHIP_HAL_H
#define CHIP_HAL_H

#include "esp_chip_info.h"

static const char* Chip_GetModel(void) {
  esp_chip_info_t chipInfo;
  esp_chip_info(&chipInfo);

  switch (chipInfo.model) {
    case CHIP_ESP32:
      return "ESP32";

    case CHIP_ESP32S2:
      return "ESP32 S2";

    case CHIP_ESP32S3:
      return "ESP32 S3";

    case CHIP_ESP32C3:
      return "ESP32 C3";

    case CHIP_ESP32C2:
      return "ESP32 C2";

    case CHIP_ESP32C6:
      return "ESP32 C6";

    case CHIP_ESP32H2:
      return "ESP32 H2";

    case CHIP_ESP32P4:
      return "ESP32 P4";

    default:
      return "N/A";
  }
}

#endif  // CHIP_HAL_H
