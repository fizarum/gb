#include "spi_hal.h"

esp_err_t SPI_Init(spi_device_handle_t *handle, spi_host_device_t host,
                   const uint8_t mosi, const uint8_t sclk, const uint8_t cs) {
  spi_bus_config_t busConfig = {
      .sclk_io_num = sclk,
      .mosi_io_num = mosi,
      .miso_io_num = -1,
      .quadwp_io_num = -1,
      .quadhd_io_num = -1,
      // transfer 80 lines of pixels (assume pixel is RGB565)
      // at most in one SPI transaction
      .max_transfer_sz = 320 * 80 * sizeof(uint16_t),
  };

  esp_err_t result = spi_bus_initialize(host, &busConfig, SPI_DMA_CH_AUTO);
  if (result != ESP_OK) {
    return result;
  }

  spi_device_interface_config_t devConfig = {
      .clock_speed_hz = SPI_MASTER_FREQ_40M,
      .spics_io_num = cs,
      .queue_size = 7,
      .flags = SPI_DEVICE_NO_DUMMY,
  };

  return spi_bus_add_device(host, &devConfig, handle);
}