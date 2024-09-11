#include "storage_utils.h"

static const uint32_t bytesInMB = 1024 * 1024;

uint32_t StorageGetTotalSizeInMBs(const char* mountPoint) {
  FATFS* fsinfo;
  uint32_t freClust;

  if (f_getfree(mountPoint, &freClust, &fsinfo) != 0) {
    return false;
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