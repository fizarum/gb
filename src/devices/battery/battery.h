#ifndef BATTERY_H
#define BATTERY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <device.h>
#include <specifications/battery_data.h>

DeviceSpecification* BatterySpecification();

#ifdef __cplusplus
}
#endif

#endif  // BATTERY_H