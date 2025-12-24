#include "storage_utils.h"

#include <dirent.h>

#include "esp_log.h"
#include "esp_vfs.h"
#define TAG "FS"

static const uint32_t bytesInMB = 1024 * 1024;

uint32_t Storage_GetTotalSizeInMBs(const char* mountPoint) {
  FATFS* fsinfo;
  uint32_t freClust;

  if (f_getfree(mountPoint, &freClust, &fsinfo) != 0) {
    return 0;
  }

  uint64_t size =
      ((uint64_t)(fsinfo->csize)) * (fsinfo->n_fatent - 2) * (fsinfo->ssize);

  return size / bytesInMB;
}

uint32_t Storage_GetUsedSizeInMBs(const char* mountPoint) {
  FATFS* fsinfo;
  DWORD freClust;
  if (f_getfree(mountPoint, &freClust, &fsinfo) != 0) {
    return 0;
  }

  uint64_t size = ((uint64_t)(fsinfo->csize)) *
                  ((fsinfo->n_fatent - 2) - (fsinfo->free_clst)) *
                  (fsinfo->ssize);

  return size / bytesInMB;
}

esp_err_t SD_CalculateFilesInDirectory(const char* path, _u16* filesCount) {
  DIR* dir = opendir(path);

  if (!dir) {
    ESP_LOGE(TAG, "Failed to open dir : %s", path);
    return ESP_FAIL;
  }

  *filesCount = 0;
  DirectoryItem* item;

  for (;;) {
    item = readdir(dir);
    if (item == NULL) {
      break;
    }
    *filesCount += 1;
  }
  closedir(dir);

  return ESP_OK;
}

esp_err_t SD_ShowDirectory(const char* path, _u16 startPos, _u16 endPos,
                           long* finishedAt, DirectoryItemIterator iterator) {
  if (iterator == NULL) {
    return ESP_OK;
  }

  if (finishedAt == NULL) {
    return ESP_ERR_INVALID_ARG;
  }

  if (startPos >= endPos) {
    return ESP_ERR_INVALID_ARG;
  }

  DIR* dir = opendir(path);

  if (!dir) {
    ESP_LOGE(TAG, "Failed to open dir : %s", path);
    return ESP_ERR_NOT_ALLOWED;
  }

  long minimalPos = telldir(dir);
  // in case when startPos is set incorrectly
  if (minimalPos > startPos) {
    seekdir(dir, minimalPos);
  } else {
    seekdir(dir, startPos);
  }

  DirectoryItem* item;
  for (_u16 index = startPos; index <= endPos; index++) {
    item = readdir(dir);
    if (item == NULL) {
      break;
    }
    iterator(index, item);
  }
  *finishedAt = telldir(dir);

  closedir(dir);

  return ESP_OK;
}

esp_err_t SD_ReadFile(const char* path, OnDataGetCallback callback) {
  ESP_LOGI(TAG, "Reading file %s", path);
#define LENGTH 32

  FILE* f = fopen(path, "r");
  if (f == NULL) {
    ESP_LOGE(TAG, "Failed to open file for reading");
    return ESP_FAIL;
  }
  static char line[LENGTH];
  bool proceed = false;

  while (fgets(line, LENGTH, f) != NULL) {
    proceed = callback(line, LENGTH);
    if (proceed == false) {
      break;
    }
  }

  fclose(f);

  return ESP_OK;
}

esp_err_t SD_ReadFileToBuff(const char* path, _u8* buff, _u16 length) {
  ESP_LOGI(TAG, "Reading file %s", path);
  FILE* f = fopen(path, "r");
  if (f == NULL) {
    ESP_LOGE(TAG, "Failed to open file for reading");
    return ESP_FAIL;
  }

  char* res = fgets(buff, length, f);
  if (res == NULL) {
    ESP_LOGE(TAG, "Failed to read from file");
    return ESP_FAIL;
  }

  fclose(f);

  return ESP_OK;
}

esp_err_t SD_WriteFile(const char* path, const _u8* data, const _u16 length) {
  ESP_LOGI(TAG, "Writing file %s", path);
  FILE* f = fopen(path, "w");

  if (f == NULL) {
    ESP_LOGE(TAG, "Failed to open file for reading");
    return ESP_FAIL;
  }

  int result = fputs(data, f);
  if (result == EOF) {
    ESP_LOGE(TAG, "Failed to write data!");
    return ESP_FAIL;
  }

  fclose(f);

  return ESP_OK;
}

void FileItemFromDirent(FileItem_t* dst, DirectoryItem* src) {
  if (dst == NULL || src == NULL) {
    return;
  }
  strncpy(dst->name, src->d_name, FILE_ITEM_NAME_MAX_LEN);
  dst->number = src->d_ino;
  dst->type = src->d_type;
}