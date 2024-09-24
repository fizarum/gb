#include "display.h"

#include <gfx/gfx.h>
#include <ili9341.h>
#include <palette.h>
#include <specifications/display_data.h>
#include <stddef.h>

#include "backlight.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_log.h"

#define DISPLAY_WIDTH 240
#define DISPLAY_HEIGHT 320
#define CONFIG_OFFSETX 0
#define CONFIG_OFFSETY 0

#define DISPLAY_SCLK 12
#define DISPLAY_MOSI 11
#define DISPLAY_MISO 13
#define DISPLAY_CS 16
#define DISPLAY_DC 10
#define DISPLAY_RESET 5
#define DISPLAY_BL 1

static DeviceSpecification_t specs = {
    .name = "Display",
    .type = TypeDisplay,
};

ILI9341_t dev = {
    .width = DISPLAY_WIDTH,
    .height = DISPLAY_HEIGHT,
    .offsetx = CONFIG_OFFSETX,
    .offsety = CONFIG_OFFSETY,
    .rotation = Angle0,
    .fontRotaion = Angle0,
    .colorMode = ModeBGR,
    .dc = DISPLAY_DC,
};

Font_t font = {
    .height = 5,
    .width = 8,
    .color = COLOR_ORANGE,
    .scale = 2,
};

static DisplayDeviceData_t deviceData;
static _u8 brightness = 30;

static bool onInit(void) {
  spi_device_handle_t spiHandle;

  esp_err_t result =
      SPIInit(&spiHandle, SPI2_HOST, DISPLAY_MOSI, DISPLAY_SCLK, DISPLAY_CS);
  if (result != ESP_OK) {
    return false;
  }

  dev.handle = spiHandle;

  Ili9341Init(&dev, DISPLAY_RESET);

  GFXInit(&dev);

  deviceData.width = DISPLAY_WIDTH;
  deviceData.height = DISPLAY_HEIGHT;

  ESP_LOGI(specs.name, "on init: 1");

  return true;
}

static void onEnable(bool enable) {
  ESP_LOGI(specs.name, "enable: %d", enable);
  Ili9341PowerOn(&dev, enable);

  if (enable == true) {
    BacklightSetValue(DISPLAY_BL, brightness);
  } else {
    BacklightSetValue(DISPLAY_BL, 0);
    return;
  }

  GFXSetFont(&font);
}

static void onUpdate(void) {}

DeviceSpecification_t* DislplaySpecification() {
  specs.data = &deviceData;

  specs.onInit = &onInit;
  specs.onEnable = &onEnable;
  specs.onUpdate = &onUpdate;

  return &specs;
}