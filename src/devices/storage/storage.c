#include "storage.h"

#include <specifications/storage_data.h>
#include <stddef.h>

#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
// temp
#include "storage_utils.h"

#define SD_CS 16
#define SD_CLK 7
#define SD_MOSI 15
#define SD_MISO 6

static DeviceSpecification_t specs = {
    .name = "SD Card",
    .type = TypeStorage,
};

static StorageDeviceData_t deviceData = {
    .mountPoint = "/sdcard",
};

static bool onInit(void) {
  esp_vfs_fat_sdmmc_mount_config_t mountConfig = {
      .format_if_mount_failed = false,
      .max_files = 5,
      .allocation_unit_size = 16 * 1024,
  };

  // By default, SD card frequency is initialized to SDMMC_FREQ_DEFAULT (20MHz)
  // For setting a specific frequency, use host.max_freq_khz (range 400kHz -
  // 20MHz for SDSPI) Example: for fixed frequency of 10MHz, use
  // host.max_freq_khz = 10000;
  sdmmc_host_t host = SDSPI_HOST_DEFAULT();
  host.slot = SPI3_HOST;

  spi_bus_config_t busConfig = {
      .mosi_io_num = SD_MOSI,
      .miso_io_num = SD_MISO,
      .sclk_io_num = SD_CLK,
      .quadwp_io_num = -1,
      .quadhd_io_num = -1,
      .max_transfer_sz = 4000,
  };

  esp_err_t ret = spi_bus_initialize(host.slot, &busConfig, SPI_DMA_CH_AUTO);
  if (ret != ESP_OK) {
    ESP_LOGE(specs.name, "Failed to initialize bus.");
    return false;
  }

  // This initializes the slot without card detect (CD) and write protect (WP)
  // signals. Modify slot_config.gpio_cd and slot_config.gpio_wp if your board
  // has these signals.
  sdspi_device_config_t slotConfig = SDSPI_DEVICE_CONFIG_DEFAULT();
  slotConfig.gpio_cs = SD_CS;
  slotConfig.host_id = host.slot;

  ESP_LOGI(specs.name, "Mounting filesystem");
  ret = esp_vfs_fat_sdspi_mount(deviceData.mountPoint, &host, &slotConfig,
                                &mountConfig, &(deviceData.card));

  if (ret != ESP_OK) {
    if (ret == ESP_FAIL) {
      ESP_LOGE(specs.name,
               "Failed to mount filesystem. "
               "If you want the card to be formatted, set the "
               "CONFIG_EXAMPLE_FORMAT_IF_MOUNT_FAILED menuconfig option.");
    } else {
      ESP_LOGE(specs.name,
               "Failed to initialize the card (%s). "
               "Make sure SD card lines have pull-up resistors in place.",
               esp_err_to_name(ret));
    }
    return false;
  }

  return true;
}

static void onEnable(bool enable) {
  //..
}

static void onUpdate(void) {
  //..
}

static void* getData(void) { return &deviceData; }

DeviceSpecification_t* StorageSpecification() {
  specs.onInit = &onInit;
  specs.onEnable = &onEnable;
  specs.onUpdate = &onUpdate;
  specs.getData = &getData;
  return &specs;
}