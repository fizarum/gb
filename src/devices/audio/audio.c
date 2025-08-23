#include "audio.h"

#include <driver/i2s_std.h>
#include <specifications/audio_data.h>

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

#define SAMPLE_RATE 44100
#define DATA_CHUNCK_SIZE 512

// #define I2S_BUFF_SIZE 1024

#define AUDIO_LRCLK 8
#define AUDIO_DOUT 17
#define AUDIO_BCLK 18

extern const _u8 beep1_start[] asm("_binary_cursor_pcm_start");
extern const _u8 beep1_end[] asm("_binary_cursor_pcm_end");

static i2s_chan_handle_t channelHandler;
static bool enabled = false;

static void i2s_example_write_task();

static DeviceSpecification_t specs = {
    .name = "Sound",
    .type = TypeAudio,
};

TaskHandle_t audioTaskHandler = NULL;
static QueueHandle_t audioQueue;

static AudioDeviceData_t data = {
    .data = NULL,
    .dataLength = 0,
    .volume = 0.0,
};

void playSystemSound(_u16 type) {
  if (type == 1) {
    i2s_example_write_task();
  }
}

// static void audioTask(void* arg);

static bool onInit(void) {
  audioQueue = xQueueCreate(DATA_CHUNCK_SIZE * 2, sizeof(_u16));

  i2s_chan_config_t channelConfig =
      I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_AUTO, I2S_ROLE_MASTER);

  esp_err_t result = i2s_new_channel(&channelConfig, &channelHandler, NULL);
  if (result != ESP_OK) {
    return false;
  }

  i2s_std_config_t config = {
      .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(SAMPLE_RATE),
      .slot_cfg = I2S_STD_MSB_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT,
                                                  I2S_SLOT_MODE_MONO),

      .gpio_cfg =
          {
              .mclk = I2S_GPIO_UNUSED,  // some codecs may require mclk signal,
                                        // this example doesn't need it
              .bclk = AUDIO_BCLK,
              .ws = AUDIO_LRCLK,
              .dout = AUDIO_DOUT,
              .din = GPIO_NUM_NC,
              .invert_flags =
                  {
                      .mclk_inv = false,
                      .bclk_inv = false,
                      .ws_inv = false,
                  },
          },
  };

  result = i2s_channel_init_std_mode(channelHandler, &config);

  return result == ESP_OK;
}

static bool onEnable(bool enable) {
  ESP_LOGI(specs.name, "enable: %d", enable);

  esp_err_t result;
  enabled = enable;
  if (enable) {
    result = i2s_channel_enable(channelHandler);
    // xTaskCreate(&audioTask, "audioTask", 2048, NULL, 12, &audioTaskHandler);
  } else {
    result = i2s_channel_disable(channelHandler);
    if (audioTaskHandler != NULL) {
      // vTaskDelete(audioTaskHandler);
    }
  }

  return result == ESP_OK;
}

static void onUpdate(void) {}

DeviceSpecification_t* AudioSpecification() {
  specs.data = &data;

  specs.onInit = &onInit;
  specs.onEnable = &onEnable;
  specs.onUpdate = &onUpdate;

  return &specs;
}

void audioTask(void* arg) {
  esp_err_t result;
  _u16 flag;

  while (1) {
    if (xQueueReceive(audioQueue, &flag, 0) == pdPASS &&
        (data.data != NULL, data.dataLength > 0)) {
      result = i2s_channel_write(channelHandler, data.data, data.dataLength,
                                 NULL, 1000);
      if (result != ESP_OK) {
        ESP_LOGE(specs.name, "can not write audio chunk, code: %d\n", result);
      }
    } else {
      vTaskDelay(1);
    }
  }
}

static _u16 buffer[DATA_CHUNCK_SIZE];

static void i2s_example_write_task() {
  size_t w_bytes = 0;
  uint32_t offset = 0;

  float volume = 0.5f;

  size_t beepSoundSize = beep1_end - beep1_start;
  _u32 chunks = beepSoundSize / DATA_CHUNCK_SIZE;
  if (beepSoundSize % DATA_CHUNCK_SIZE != 0) {
    chunks += 1;
  }

  for (_u32 index = 0; index < chunks; ++index) {
    if (channelHandler == NULL) {
      break;
    }

    if (i2s_channel_write(channelHandler, buffer, DATA_CHUNCK_SIZE * 2,
                          &w_bytes, 1000) == ESP_OK) {
      //..
    }

    if (offset > beepSoundSize) {
      break;
    }

    for (int i = 0; i < DATA_CHUNCK_SIZE; i++) {
      offset++;
      // reading only first (hight) 8 bit, details:
      //     https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/i2s.html#std-tx-mode
      buffer[i] = beep1_start[offset] << 3;
    }
  }
}