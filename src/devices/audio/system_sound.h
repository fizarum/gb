#ifndef SYSTEM_SOUND_H
#define SYSTEM_SOUND_H

#ifdef __cplusplus
extern "C" {
#endif

#include <types.h>

extern const _u8 sfx1_start[] asm("_binary_sfx1_pcm_start");
extern const _u8 sfx1_end[] asm("_binary_sfx1_pcm_end");

extern const _u8 sfx2_start[] asm("_binary_sfx2_pcm_start");
extern const _u8 sfx2_end[] asm("_binary_sfx2_pcm_end");

#ifdef __cplusplus
}
#endif

#endif  // SYSTEM_SOUND_H