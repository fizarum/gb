#ifndef LETTER_DEFINITIONS_H
#define LETTER_DEFINITIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "symbol_data.h"

_u8 symbAData[] = {0x1C, 0x22, 0x22, 0x3E, 0x22};
_u8 symbBData[] = {0x3C, 0x22, 0x3C, 0x22, 0x3C};
_u8 symbCData[] = {0x1e, 0x20, 0x20, 0x20, 0x1e};
_u8 symbDData[] = {0x3c, 0x22, 0x22, 0x22, 0x3c};
_u8 symbEData[] = {0x3e, 0x20, 0x3c, 0x20, 0x3e};
_u8 symbFData[] = {0x3e, 0x20, 0x3c, 0x20, 0x20};
_u8 symbGData[] = {0x1e, 0x20, 0x2e, 0x22, 0x1e};
_u8 symbHData[] = {0x22, 0x22, 0x3e, 0x22, 0x22};
_u8 symbIData[] = {0x3e, 0x8, 0x8, 0x8, 0x3e};
_u8 symbJData[] = {0x2, 0x2, 0x22, 0x22, 0x1c};
_u8 symbKData[] = {0x22, 0x24, 0x38, 0x24, 0x22};
_u8 symbLData[] = {0x20, 0x20, 0x20, 0x20, 0x3e};
_u8 symbMData[] = {0x22, 0x36, 0x2a, 0x2a, 0x22};
_u8 symbNData[] = {0x22, 0x32, 0x2a, 0x26, 0x22};
_u8 symbOData[] = {0x1c, 0x22, 0x22, 0x22, 0x1c};
_u8 symbPData[] = {0x3c, 0x22, 0x3c, 0x20, 0x20};
_u8 symbQData[] = {0x1c, 0x22, 0x22, 0x24, 0x1a};
_u8 symbRData[] = {0x3c, 0x22, 0x3c, 0x24, 0x22};
_u8 symbSData[] = {0x1e, 0x20, 0x1c, 0x2, 0x3c};
_u8 symbTData[] = {0x3e, 0x8, 0x8, 0x8, 0x8};
_u8 symbUData[] = {0x22, 0x22, 0x22, 0x22, 0x1c};
_u8 symbVData[] = {0x22, 0x22, 0x22, 0x14, 0x8};
_u8 symbWData[] = {0x22, 0x2a, 0x2a, 0x2a, 0x1c};
_u8 symbXData[] = {0x22, 0x14, 0x8, 0x14, 0x22};
_u8 symbYData[] = {0x22, 0x22, 0x1c, 0x8, 0x8};
_u8 symbZData[] = {0x3e, 0x4, 0x8, 0x10, 0x3e};

SymbolData_t symbA = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 65,
};

SymbolData_t symbB = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 66,
};

SymbolData_t symbC = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 67,
};

SymbolData_t symbD = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 68,
};

SymbolData_t symbE = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 69,
};

SymbolData_t symbF = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 70,
};

SymbolData_t symbG = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 71,
};

SymbolData_t symbH = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 72,
};

SymbolData_t symbI = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 73,
};

SymbolData_t symbJ = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 74,
};

SymbolData_t symbK = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 75,
};

SymbolData_t symbL = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 76,
};

SymbolData_t symbM = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 77,
};

SymbolData_t symbN = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 78,
};

SymbolData_t symbO = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 79,
};

SymbolData_t symbP = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 80,
};

SymbolData_t symbQ = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 81,
};

SymbolData_t symbR = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 82,
};

SymbolData_t symbS = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 83,
};

SymbolData_t symbT = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 84,
};

SymbolData_t symbU = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 85,
};

SymbolData_t symbV = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 86,
};

SymbolData_t symbW = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 87,
};

SymbolData_t symbX = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 88,
};

SymbolData_t symbY = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 89,
};

SymbolData_t symbZ = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 90,
};

void LetterAssignDefinitions() {
  symbA.data = symbAData;
  symbA.dataLength = 5;

  symbB.data = symbBData;
  symbB.dataLength = 5;

  symbC.data = symbCData;
  symbC.dataLength = 5;

  symbD.data = symbDData;
  symbD.dataLength = 5;

  symbE.data = symbEData;
  symbE.dataLength = 5;

  symbF.data = symbFData;
  symbF.dataLength = 5;

  symbG.data = symbGData;
  symbG.dataLength = 5;

  symbH.data = symbHData;
  symbH.dataLength = 5;

  symbI.data = symbIData;
  symbI.dataLength = 5;

  symbJ.data = symbJData;
  symbJ.dataLength = 5;

  symbK.data = symbKData;
  symbK.dataLength = 5;

  symbL.data = symbLData;
  symbL.dataLength = 5;

  symbM.data = symbMData;
  symbM.dataLength = 5;

  symbN.data = symbNData;
  symbN.dataLength = 5;

  symbO.data = symbOData;
  symbO.dataLength = 5;

  symbP.data = symbPData;
  symbP.dataLength = 5;

  symbQ.data = symbQData;
  symbQ.dataLength = 5;

  symbR.data = symbRData;
  symbR.dataLength = 5;

  symbS.data = symbSData;
  symbS.dataLength = 5;

  symbT.data = symbTData;
  symbT.dataLength = 5;

  symbU.data = symbUData;
  symbU.dataLength = 5;

  symbV.data = symbVData;
  symbV.dataLength = 5;

  symbW.data = symbWData;
  symbW.dataLength = 5;

  symbX.data = symbXData;
  symbX.dataLength = 5;

  symbY.data = symbYData;
  symbY.dataLength = 5;

  symbZ.data = symbZData;
  symbZ.dataLength = 5;
}

#ifdef __cplusplus
}
#endif

#endif  // LETTER_DEFINITIONS_H