#include "battery_adc_utils.h"

#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"

#define ADC_ATTEN ADC_ATTEN_DB_12

#define TAG "BatteryADC"

static bool doCalibration(adc_unit_t unit, adc_channel_t channel,
                          adc_atten_t atten, adc_cali_handle_t* outHandle);

static adc_oneshot_chan_cfg_t config = {
    .atten = ADC_ATTEN,
    .bitwidth = ADC_BITWIDTH_DEFAULT,
};

static adc_oneshot_unit_init_cfg_t initConfig;
static adc_cali_handle_t caliHandle = NULL;

static int adcRaw[10];
static int voltage[10];
static adc_oneshot_unit_handle_t adcHandle;

bool BatteryADCInit(const adc_unit_t unit, const adc_channel_t channel) {
  initConfig.unit_id = unit;
  ESP_ERROR_CHECK(adc_oneshot_new_unit(&initConfig, &adcHandle));
  ESP_ERROR_CHECK(adc_oneshot_config_channel(adcHandle, channel, &config));
  return doCalibration(initConfig.unit_id, channel, config.atten, &caliHandle);
}

int BatteryADCRead(const adc_unit_t unit, const adc_channel_t channel,
                   const bool calibrated) {
  ESP_ERROR_CHECK(adc_oneshot_read(adcHandle, channel, adcRaw));
  // ESP_LOGI(TAG, "ADC%d Channel[%d] Raw Data: %d", initConfig.unit_id + 1,
  //          channel, adcRaw[0]);

  if (calibrated == true) {
    ESP_ERROR_CHECK(adc_cali_raw_to_voltage(caliHandle, adcRaw[0], voltage));
    // ESP_LOGI(TAG, "ADC%d Channel[%d] Cali Voltage: %d mV",
    //          initConfig.unit_id + 1, channel, voltage[0]);
  }

  return voltage[0];
}

// private part
static bool doCalibration(adc_unit_t unit, adc_channel_t channel,
                          adc_atten_t atten, adc_cali_handle_t* outHandle) {
  adc_cali_handle_t handle = NULL;
  esp_err_t ret = ESP_FAIL;
  bool calibrated = false;

#if ADC_CALI_SCHEME_CURVE_FITTING_SUPPORTED
  if (!calibrated) {
    ESP_LOGI(TAG, "calibration scheme version is %s", "Curve Fitting");
    adc_cali_curve_fitting_config_t caliConfig = {
        .unit_id = unit,
        .chan = channel,
        .atten = atten,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    ret = adc_cali_create_scheme_curve_fitting(&caliConfig, &handle);
    if (ret == ESP_OK) {
      calibrated = true;
    }
  }
#endif

#if ADC_CALI_SCHEME_LINE_FITTING_SUPPORTED
  if (!calibrated) {
    ESP_LOGI(TAG, "calibration scheme version is %s", "Line Fitting");
    adc_cali_line_fitting_config_t caliConfig = {
        .unit_id = unit,
        .atten = atten,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    ret = adc_cali_create_scheme_line_fitting(&caliConfig, &handle);
    if (ret == ESP_OK) {
      calibrated = true;
    }
  }
#endif

  *outHandle = handle;
  if (ret == ESP_OK) {
    ESP_LOGI(TAG, "Calibration Success");
  } else if (ret == ESP_ERR_NOT_SUPPORTED || !calibrated) {
    ESP_LOGW(TAG, "eFuse not burnt, skip software calibration");
  } else {
    ESP_LOGE(TAG, "Invalid arg or no memory");
  }

  return calibrated;
}
