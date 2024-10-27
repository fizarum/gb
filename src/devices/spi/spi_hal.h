#ifndef SPI_HAL_H
#define SPI_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <driver/spi_master.h>
#include <esp_err.h>
#include <hal/spi_types.h>

esp_err_t SPI_Init(spi_device_handle_t *handle, spi_host_device_t host,
                   const uint8_t mosi, const uint8_t sclk, const uint8_t cs);

#ifdef __cplusplus
}
#endif

#endif  // SPI_HAL_H