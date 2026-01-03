#ifndef AUDIO_H
#define AUDIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <device.h>
#include <specifications/audio_data.h>

DeviceSpecification* AudioSpecification();

void playSystemSound(_u16 type);

#ifdef __cplusplus
}
#endif

#endif  // AUDIO_H