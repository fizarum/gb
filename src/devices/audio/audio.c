#include "audio.h"

#include <driver/i2s_std.h>
#include <specifications/audio_data.h>

#include "audio_state.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "system_sound.h"

#define SAMPLE_RATE 44100
#define DATA_CHUNCK_SIZE 512

#define AUDIO_LRCLK 8
#define AUDIO_DOUT 17
#define AUDIO_BCLK 18

typedef struct SystemSoundInfo {
  _u8* start;
  _u8* end;
  size_t length;
} SystemSoundInfo;

static DeviceSpecification_t specs = {
    .name = "Sound",
    .type = TypeAudio,
};

static AudioDeviceData_t data = {
    .volume = 1.0,
};

static i2s_chan_handle_t channelHandler;
static SystemSoundInfo systemSound = {
    .start = NULL,
    .end = NULL,
    .length = 0,
};
static AudioState audioState;

TaskHandle_t audioTaskHandler = NULL;
static QueueHandle_t audioQueue;

static void processAudio(void* arg);

void playSystemSound(_u16 type) {
  if (audioState == Play) {
    return;
  }

  if (audioState == Stop || audioState == Enabled) {
    audioState = Play;
  }
  switch (type) {
    case 1:
      systemSound.start = (_u8*)sfx2_start;
      systemSound.end = (_u8*)sfx2_end;
      systemSound.length = sfx2_end - sfx2_start;
      break;

    case 2:
      systemSound.start = (_u8*)o_start;
      systemSound.end = (_u8*)o_end;
      systemSound.length = o_end - o_start;
      break;

    default:
      systemSound.start = NULL;
      systemSound.end = NULL;
      systemSound.length = 0;
      break;
  }
}

static bool onInit(void) {
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

  if (result == ESP_OK) {
    audioQueue = xQueueCreate(DATA_CHUNCK_SIZE * 2, sizeof(_u16));
  }

  return result == ESP_OK;
}

static bool onEnable(bool enable) {
  esp_err_t result;

  if (enable) {
    // TODO: read volume value from settings
    data.volume = 2.0;

    audioState = Enabled;
    xTaskCreate(&processAudio, "processAudio", 2048, NULL, 12,
                &audioTaskHandler);
  } else {
    if (audioTaskHandler != NULL) {
      vTaskDelete(audioTaskHandler);
    }
    audioState = Disabled;
  }
  ESP_LOGI(specs.name, "enable: %d", enable);
  return enable;
}

static void onUpdate(void) {}

DeviceSpecification_t* AudioSpecification() {
  specs.data = &data;

  specs.onInit = &onInit;
  specs.onEnable = &onEnable;
  specs.onUpdate = &onUpdate;

  return &specs;
}

static _u16 buffer[DATA_CHUNCK_SIZE * 2];

static void processAudio(void* arg) {
  static size_t w_bytes = 0;
  static uint32_t offset = 0;
  static _u32 chunks = 0;
  static _u16 chunkIndex = 0;
  static _u32 index = 0;

  while (1) {
    if (audioState == Play && systemSound.length > 0) {
      chunks = systemSound.length / DATA_CHUNCK_SIZE;
      if (systemSound.length % DATA_CHUNCK_SIZE != 0) {
        chunks += 1;
      }
      i2s_channel_enable(channelHandler);
      for (index = 0; index < chunks; ++index) {
        for (chunkIndex = 0; chunkIndex < DATA_CHUNCK_SIZE; ++chunkIndex) {
          offset++;
          // reading only first (hight) 8 bit, details:
          //     https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/i2s.html#std-tx-mode
          // buffer[i] = (beep1_start[offset] << 3) * (data.volume);
          buffer[chunkIndex] =
              (*(systemSound.start + offset) << 3);  //* (data.volume);
        }

        if (i2s_channel_write(channelHandler, buffer, DATA_CHUNCK_SIZE * 2,
                              &w_bytes, 1000) != ESP_OK) {
          ESP_LOGE(specs.name, "can not process audio chunk\n");
        }
      }
      i2s_channel_disable(channelHandler);
      audioState = Stop;

      w_bytes = 0;
      offset = 0;
      chunks = 0;
      for (index = 0; index < DATA_CHUNCK_SIZE * 2; ++index) {
        buffer[index] = 0;
      }
      vTaskDelay(pdMS_TO_TICKS(1));
    } else {
      vTaskDelay(pdMS_TO_TICKS(100));
    }
  }
}