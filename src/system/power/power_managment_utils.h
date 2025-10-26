#pragma once

#include <types.h>

#include "esp_log.h"
#include "esp_private/esp_clk.h"
#include "soc/rtc.h"

static const char* TAG = "power";

void esp_timer_impl_update_apb_freq(
    uint32_t apb_ticks_per_us);  // private in IDF

/**
 * @param cpu_freq_mhz can be 240, 160, 80, 40, 20, 10
 * @return true is cpu frequency set, false otherwise
 */
static bool setCPUFrequencyMhz(uint32_t cpu_freq_mhz) {
  rtc_cpu_freq_config_t conf, cconf;
  uint32_t capb, apb;

  // Get current CPU clock configuration
  rtc_clk_cpu_freq_get_config(&cconf);

  // return if frequency has not changed
  if (cconf.freq_mhz == cpu_freq_mhz) {
    return true;
  }

  // Get configuration for the new CPU frequency
  if (!rtc_clk_cpu_freq_mhz_to_config(cpu_freq_mhz, &conf)) {
    ESP_LOGE(TAG, "CPU clock could not be set to %u MHz", cpu_freq_mhz);
    return false;
  }
  // Current APB
  capb = APB_CLK_FREQ;
  // New APB
  apb = APB_CLK_FREQ;
  // Make the frequency change
  rtc_clk_cpu_freq_set_config_fast(&conf);

  if (capb != apb) {
    // Update APB Freq REG
    rtc_clk_apb_freq_update(apb);
    // Update esp_timer divisor
    esp_timer_impl_update_apb_freq(apb / MHZ);
  }
  ESP_LOGI(TAG, "CPU clock is set to %u MHz", cpu_freq_mhz);
  return true;
}
