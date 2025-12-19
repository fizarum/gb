#ifndef SYMBOL_DATA_H
#define SYMBOL_DATA_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include <types.h>

/**
 * @brief Container for symbol bitmap
 */
typedef struct SymbolData_t {
  _u8* data;
  _u16 dataLength;
  _u8 asciiCode;
} SymbolData_t;

#ifdef __cplusplus
}
#endif

#endif  // SYMBOL_DATA_H