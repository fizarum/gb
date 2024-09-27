#ifndef STORAGE_UTILS_H
#define STORAGE_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <types.h>

#include "esp_vfs_fat.h"

#define FILE_ITEM_NAME_MAX_LEN 30

// similar to dirent but with some optimizations
typedef struct FileItem_t {
  bool initialized;
  _u16 number;
  _u8 type;
  char name[FILE_ITEM_NAME_MAX_LEN];
} FileItem_t;

typedef struct dirent DirectoryItem;
typedef void(DirectoryItemIterator)(_u16 index, DirectoryItem* item);
/**
 * @brief General callback for reading file operations
 * @param data - read data
 * @param length - size of data portions
 * @returns true - to proceed reading, false - to stop reading
 */
typedef bool(OnDataGetCallback)(const char* data, _u8 length);

uint32_t StorageGetTotalSizeInMBs(const char* mountPoint);
uint32_t StorageGetUsedSizeInMBs(const char* mountPoint);

esp_err_t SDCalculateFilesInDirectory(const char* path, _u16* filesCount);
esp_err_t SDShowDirectory(const char* path, _u16 startPos, _u16 endPos,
                          long* finishedAt, DirectoryItemIterator iterator);
esp_err_t SDReadFile(const char* path, OnDataGetCallback callback);

void FileItemFromDirent(FileItem_t* dst, DirectoryItem* src);

#ifdef __cplusplus
}
#endif

#endif  // STORAGE_UTILS_H