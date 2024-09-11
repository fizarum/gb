#ifndef STORAGE_UTILS_H
#define STORAGE_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "esp_vfs_fat.h"

uint32_t StorageGetTotalSizeInMBs(const char* mountPoint);
uint32_t StorageGetUsedSizeInMBs(const char* mountPoint);

#ifdef __cplusplus
}
#endif

#endif  // STORAGE_UTILS_H