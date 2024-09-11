#ifndef BATTERY_ADC_UTILS_H
#define BATTERY_ADC_UTILS_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "hal/adc_types.h"

bool BatteryADCInit(const adc_unit_t unit, const adc_channel_t channel);
int BatteryADCRead(const adc_unit_t unit, const adc_channel_t channel,
                   const bool calibrated);

#ifdef __cplusplus
}
#endif
#endif  // BATTERY_ADC_UTILS_H