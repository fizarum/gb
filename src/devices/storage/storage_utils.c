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

esp_err_t SDShowDirectory(const char* path, _u16* filesCount,
                          DirectoryItemIterator iterator) {
  if (iterator == NULL) {
    return ESP_OK;
  }

  DIR* dir = opendir(path);

  if (!dir) {
    ESP_LOGE(TAG, "Failed to open dir : %s", path);
    return ESP_FAIL;
  }

  *filesCount = 0;

  struct dirent* item;
  for (;;) {
    item = readdir(dir);
    if (item == NULL) {
      break;
    }
    *filesCount += 1;
    iterator(item);
  }
  closedir(dir);

  return ESP_OK;
}

// todo: test implementation - callback needed!
esp_err_t SDReadFile(const char* path) {
  ESP_LOGI(TAG, "Reading file %s", path);
  FILE* f = fopen(path, "r");
  if (f == NULL) {
    ESP_LOGE(TAG, "Failed to open file for reading");
    return ESP_FAIL;
  }
  char line[100];
  fgets(line, sizeof(line), f);
  fclose(f);

  // strip newline
  char* pos = strchr(line, '\n');
  if (pos) {
    *pos = '\0';
  }
  ESP_LOGI(TAG, "Read from file: '%s'", line);

  return ESP_OK;
}