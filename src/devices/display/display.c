#include "display.h"

#include <gfx/gfx.h>
#include <ili9341.h>
#include <palette.h>
#include <specifications/display_data.h>
#include <stddef.h>
#include <string.h>

#include "../gpio/gpio_hal.h"
// #include "../spi/spi_hal.h"
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

#define DC_C 0
#define DC_D 1

#define SPI_BYTE_BUFF_MAX_SIZE 1024
static _u8 buffer[SPI_BYTE_BUFF_MAX_SIZE];

static bool transmitCommand(const _u8 command);
static bool transmitData(const _u8* data, const size_t length);
static bool transmitDataTimes(const _u16 value, const _u16 times);
static bool transmit(const _u8* data, const size_t length);

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
    .res = DISPLAY_RESET,
    .transmitCommand = &transmitCommand,
    .transmitData = &transmitData,
    .transmitDataTimes = &transmitDataTimes,
};

Font_t font = {
    .height = 5,
    .width = 8,
    .color = COLOR_ORANGE,
    .scale = 2,
};

typedef struct UpdateTransaction_t {
  _u16 left;
  _u16 top;
  _u16 right;
  _u16 bottom;
  _u16 color;
} UpdateTransaction_t;

TaskHandle_t displayTaskHandle = NULL;
QueueHandle_t displayDataQueue;
UpdateTransaction_t updateTransaction;

spi_device_handle_t spiHandle;
static DisplayDeviceData_t deviceData;
static _u8 brightness = 30;

void drawingTask(void* arg);
static void drawCallback(const _u16 left, const _u16 top, const _u16 right,
                         const _u16 bottom, _u16 color);

static bool onInit(void) {
  // bool result =
  //     SPI_Init(&spiHandle, SPI2_HOST, DISPLAY_MOSI, DISPLAY_SCLK,
  //     DISPLAY_CS);
  // if (result == false) {
  //   return false;
  // }

  esp_err_t result =
      SPIInit(&spiHandle, SPI2_HOST, DISPLAY_MOSI, DISPLAY_SCLK, DISPLAY_CS);
  if (result != ESP_OK) {
    return false;
  }

  GPIO_Config(dev.dc, GPIO_MODE_OUTPUT);
  GPIO_Config(dev.res, GPIO_MODE_OUTPUT);
  GPIO_Set(dev.dc, 0);
  GPIO_Set(dev.res, 0);

  if (dev.res >= 0) {
    vTaskDelay(pdMS_TO_TICKS(1));
    GPIO_Set(dev.res, 1);
  }

  Ili9341Init(&dev);

  deviceData.width = dev.width;
  deviceData.height = dev.height;

  ESP_LOGI(specs.name, "on init: 1");

  displayDataQueue = xQueueCreate(4000, sizeof(UpdateTransaction_t));

  GFXInit(deviceData.width, deviceData.height, &drawCallback);

  return true;
}

static void onEnable(bool enable) {
  ESP_LOGI(specs.name, "enable: %d", enable);
  Ili9341PowerOn(&dev, enable);

  if (enable == true) {
    BacklightSetValue(DISPLAY_BL, brightness);

    xTaskCreate(&drawingTask, "drawingTask", 8096, NULL, 12,
                &displayTaskHandle);
  } else {
    BacklightSetValue(DISPLAY_BL, 0);
    if (displayTaskHandle != NULL) {
      vTaskDelete(displayTaskHandle);
    }
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

void drawingTask(void* arg) {
  while (1) {
    if (xQueueReceive(displayDataQueue, &updateTransaction, 0) == pdPASS) {
      Ili9341DrawPixelTimes(&dev, updateTransaction.left,
                            updateTransaction.right, updateTransaction.top,
                            updateTransaction.bottom, updateTransaction.color);
    } else {
      vTaskDelay(1);
    }
  }
}

static void drawCallback(const _u16 left, const _u16 top, const _u16 right,
                         const _u16 bottom, _u16 color) {
  updateTransaction.left = left;
  updateTransaction.top = top;
  updateTransaction.right = right;
  updateTransaction.bottom = bottom;
  updateTransaction.color = color;

  xQueueSend(displayDataQueue, &updateTransaction, 0);
}

static spi_transaction_t transaction;

static bool transmitCommand(const _u8 command) {
  gpio_set_level(dev.dc, DC_C);

  buffer[0] = command;

  return transmit(buffer, 1);
}

static bool transmitData(const _u8* data, const size_t length) {
  gpio_set_level(dev.dc, DC_D);

  memcpy(buffer, data, length);

  return transmit(buffer, length);
}

static bool transmitDataTimes(const _u16 value, const _u16 times) {
  gpio_set_level(dev.dc, DC_D);

  _u16 bytesCount = times * 2;

  if (bytesCount >= SPI_BYTE_BUFF_MAX_SIZE) {
    return false;
  }

  _u16 index = 0;

  for (_u16 i = 0; i < times; i++) {
    buffer[index++] = (value >> 8) & 0xff;
    buffer[index++] = value & 0xff;
  }

  return transmit(buffer, bytesCount);
}

static IRAM_ATTR bool transmit(const _u8* data, const size_t length) {
  memset(&transaction, 0, sizeof(transaction));

  transaction.length = length * 8;
  transaction.tx_buffer = data;

  return spi_device_transmit(spiHandle, &transaction) == ESP_OK;
}