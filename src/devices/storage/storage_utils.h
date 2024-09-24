#ifndef STORAGE_UTILS_H
#define STORAGE_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <types.h>

#include "esp_vfs_fat.h"

typedef struct dirent DirectoryItem;
typedef void(DirectoryItemIterator)(DirectoryItem* item);

uint32_t StorageGetTotalSizeInMBs(const char* mountPoint);
uint32_t StorageGetUsedSizeInMBs(const char* mountPoint);

esp_err_t SDShowDirectory(const char* path, _u16* filesCount,
                          DirectoryItemIterator iterator);
esp_err_t SDReadFile(const char* path);

#ifdef __cplusplus
}
#endif

#endif  // STORAGE_UTILS_H