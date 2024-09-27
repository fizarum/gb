#include "storage_utils.h"

#include <dirent.h>

#include "esp_log.h"
#include "esp_vfs.h"
#define TAG "FS"

static const uint32_t bytesInMB = 1024 * 1024;

uint32_t StorageGetTotalSizeInMBs(const char* mountPoint) {
  FATFS* fsinfo;
  uint32_t freClust;

  if (f_getfree(mountPoint, &freClust, &fsinfo) != 0) {
    return 0;
  }

  uint64_t size =
      ((uint64_t)(fsinfo->csize)) * (fsinfo->n_fatent - 2) * (fsinfo->ssize);

  return size / bytesInMB;
}

uint32_t StorageGetUsedSizeInMBs(const char* mountPoint) {
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

esp_err_t SDCalculateFilesInDirectory(const char* path, _u16* filesCount) {
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

esp_err_t SDShowDirectory(const char* path, _u16 startPos, _u16 endPos,
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

// todo: test implementation - callback needed!
esp_err_t SDReadFile(const char* path, OnDataGetCallback callback) {
  ESP_LOGI(TAG, "Reading file %s", path);
#define LENGTH 16

  FILE* f = fopen(path, "r");
  if (f == NULL) {
    ESP_LOGE(TAG, "Failed to open file for reading");
    return ESP_FAIL;
  }
  // static const _u8 length = 16;
  static char line[LENGTH];
  // char* result = NULL;
  bool proceed = false;

  while (fgets(line, LENGTH, f) != NULL) {
    proceed = callback(line, LENGTH);
    if (proceed == false) {
      break;
    }
  }

  // result = fgets(line, sizeof(line), f);

  fclose(f);

  // strip newline
  // char* pos = strchr(line, '\n');
  // if (pos) {
  //   *pos = '\0';
  // }
  // ESP_LOGI(TAG, "Read from file: '%s'", line);

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