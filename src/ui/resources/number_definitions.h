#ifndef NUMBER_DEFINITIONS_H
#define NUMBER_DEFINITIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "symbol_data.h"

_u8 symb1Data[] = {0x0C, 0x14, 0x04, 0x04, 0x1F};
_u8 symb2Data[] = {0x1E, 0x01, 0x06, 0x08, 0x1F};
_u8 symb3Data[] = {0x1F, 0x01, 0x0F, 0x01, 0x1F};
_u8 symb4Data[] = {0x11, 0x11, 0x1F, 0x01, 0x01};
_u8 symb5Data[] = {0x1F, 0x10, 0x1F, 0x01, 0x1F};
_u8 symb6Data[] = {0x1F, 0x10, 0x1F, 0x11, 0x1F};
_u8 symb7Data[] = {0x1F, 0x01, 0x02, 0x04, 0x08};
_u8 symb8Data[] = {0x1F, 0x11, 0x1F, 0x11, 0x1F};
_u8 symb9Data[] = {0x1F, 0x11, 0x1F, 0x01, 0x1F};
_u8 symb0Data[] = {0x1F, 0x11, 0x11, 0x11, 0x1F};

SymbolData_t symb0 = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 48,
};

SymbolData_t symb1 = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 49,
};

SymbolData_t symb2 = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 50,
};

SymbolData_t symb3 = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 51,
};

SymbolData_t symb4 = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 52,
};

SymbolData_t symb5 = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 53,
};

SymbolData_t symb6 = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 54,
};

SymbolData_t symb7 = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 55,
};

SymbolData_t symb8 = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 56,
};

SymbolData_t symb9 = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 57,
};

void NumberAssignDefinitions() {
  symb1.data = symb1Data;
  symb1.dataLength = 5;

  symb2.data = symb2Data;
  symb2.dataLength = 5;

  symb3.data = symb3Data;
  symb3.dataLength = 5;

  symb4.data = symb4Data;
  symb4.dataLength = 5;

  symb5.data = symb5Data;
  symb5.dataLength = 5;

  symb6.data = symb6Data;
  symb6.dataLength = 5;

  symb7.data = symb7Data;
  symb7.dataLength = 5;

  symb8.data = symb8Data;
  symb8.dataLength = 5;

  symb9.data = symb9Data;
  symb9.dataLength = 5;

  symb0.data = symb0Data;
  symb0.dataLength = 5;
}

#ifdef __cplusplus
}
#endif

#endif  // NUMBER_DEFINITIONS_H