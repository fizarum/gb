#ifndef SYMBOLS_RES_H
#define SYMBOLS_RES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "symbol_data.h"

void SymbolsResInit();
SymbolData_t *SymbolsGet(_u8 c);

#ifdef __cplusplus
}
#endif

#endif  // SYMBOLS_RES_H