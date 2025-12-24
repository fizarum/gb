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

uint32_t Storage_GetTotalSizeInMBs(const char* mountPoint);
uint32_t Storage_GetUsedSizeInMBs(const char* mountPoint);

esp_err_t SD_CalculateFilesInDirectory(const char* path, _u16* filesCount);
esp_err_t SD_ShowDirectory(const char* path, _u16 startPos, _u16 endPos,
                           long* finishedAt, DirectoryItemIterator iterator);
esp_err_t SD_ReadFile(const char* path, OnDataGetCallback callback);
esp_err_t SD_ReadFileToBuff(const char* path, _u8* buff, _u16 length);
esp_err_t SD_WriteFile(const char* path, const _u8* data, const _u16 length);

void FileItemFromDirent(FileItem_t* dst, DirectoryItem* src);

#ifdef __cplusplus
}
#endif

#endif  // STORAGE_UTILS_H