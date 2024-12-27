#ifndef SYMBOLS_DEFINITIONS_H
#define SYMBOLS_DEFINITIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "symbol_data.h"

// #
_u8 symbNumData[] = {0x0A, 0x1F, 0x0A, 0x1F, 0x0A};
// !
_u8 symbExclData[] = {0x04, 0x04, 0x04, 0x00, 0x04};
// "
_u8 symbQuotData[] = {0x0A, 0x0A, 0x00, 0x00, 0x00};
// $
_u8 symbDollarData[] = {0x0F, 0x14, 0x0E, 0x05, 0x1E};
// %
_u8 symbPercntData[] = {0x11, 0x02, 0x04, 0x08, 0x11};
// &
_u8 symbAmpData[] = {0x0C, 0x10, 0x0D, 0x12, 0x0D};
// '\'
_u8 symbBsolData[] = {0x10, 0x08, 0x04, 0x02, 0x01};
// '
_u8 symbAposData[] = {0x04, 0x04, 0x00, 0x00, 0x00};
// (
_u8 symbLparenData[] = {0x06, 0x08, 0x08, 0x08, 0x06};
// )
_u8 symbRparenData[] = {0x0C, 0x02, 0x02, 0x02, 0x0C};
// *
_u8 symbAstData[] = {0x0A, 0x04, 0x0A, 0x00, 0x00};
// +
_u8 symbPlusData[] = {0x04, 0x04, 0x1F, 0x04, 0x04};
// -
_u8 symbMinusData[] = {0x00, 0x00, 0x1F, 0x00, 0x00};
// .
_u8 symbDotData[] = {0x00, 0x00, 0x00, 0x00, 0x04};
// /
_u8 symbSolData[] = {0x01, 0x02, 0x04, 0x08, 0x10};
// :
_u8 symbColonData[] = {0x00, 0x04, 0x00, 0x04, 0x00};
// ;
_u8 symbSemiData[] = {0x00, 0x04, 0x00, 0x04, 0x04};
// <
_u8 symbLtData[] = {0x03, 0x0C, 0x10, 0x0C, 0x03};
// =
_u8 symbEqualsData[] = {0x00, 0x1F, 0x00, 0x1F, 0x00};
// >
_u8 symbGtData[] = {0x18, 0x06, 0x01, 0x06, 0x18};
// ?
_u8 symbQuestData[] = {0x0E, 0x02, 0x04, 0x00, 0x04};
// @
_u8 symbCommatData[] = {0x0E, 0x11, 0x15, 0x13, 0x08};
// [
_u8 symbLsqbData[] = {0x0E, 0x08, 0x08, 0x08, 0x0E};
// ]
_u8 symbRsqbData[] = {0x0E, 0x02, 0x02, 0x02, 0x0E};
// ^
_u8 symbHatData[] = {0x04, 0x0A, 0x00, 0x00, 0x00};
// _
_u8 symbLowbarData[] = {0x00, 0x00, 0x00, 0x00, 0x1F};
// `
_u8 symbGraveData[] = {0x08, 0x04, 0x00, 0x00, 0x00};
// {
_u8 symbLcubData[] = {0x0E, 0x08, 0x18, 0x08, 0x0E};
// |
_u8 symbVerbarData[] = {0x04, 0x04, 0x04, 0x04, 0x04};
// }
_u8 symbRcubData[] = {0x0E, 0x02, 0x03, 0x02, 0x0E};
// ~
_u8 symbTildeData[] = {0x00, 0x08, 0x15, 0x02, 0x00};

SymbolData_t symbNum = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 35,
};

SymbolData_t symbExcl = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 33,
};

SymbolData_t symbQuot = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 34,
};

SymbolData_t symbDollar = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 36,
};

SymbolData_t symbPercnt = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 37,
};

SymbolData_t symbAmp = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 38,
};

SymbolData_t symbBsol = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 92,
};

SymbolData_t symbApos = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 39,
};

SymbolData_t symbLparen = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 40,
};

SymbolData_t symbRparen = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 41,
};

SymbolData_t symbAst = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 42,
};

SymbolData_t symbPlus = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 43,
};

SymbolData_t symbMinus = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 45,
};

SymbolData_t symbDot = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 46,
};

SymbolData_t symbSol = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 47,
};

SymbolData_t symbColon = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 58,
};

SymbolData_t symbSemi = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 59,
};

SymbolData_t symbLt = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 60,
};

SymbolData_t symbEquals = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 61,
};

SymbolData_t symbGt = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 62,
};

SymbolData_t symbQuest = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 63,
};

SymbolData_t symbCommat = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 64,
};

SymbolData_t symbLsqb = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 91,
};

SymbolData_t symbRsqb = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 93,
};

SymbolData_t symbHat = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 94,
};

SymbolData_t symbLowbar = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 95,
};

SymbolData_t symbGrave = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 96,
};

SymbolData_t symbLcub = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 123,
};

SymbolData_t symbVerbar = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 124,
};

SymbolData_t symbRcub = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 125,
};

SymbolData_t symbTilde = {
    .data = NULL,
    .dataLength = 0,
    .asciiCode = 126,
};

void SymbolAssignDefinitions() {
  symbNum.data = symbNumData;
  symbNum.dataLength = 5;

  symbExcl.data = symbExclData;
  symbExcl.dataLength = 5;

  symbQuot.data = symbQuotData;
  symbQuot.dataLength = 5;

  symbDollar.data = symbDollarData;
  symbDollar.dataLength = 5;

  symbPercnt.data = symbPercntData;
  symbPercnt.dataLength = 5;

  symbAmp.data = symbAmpData;
  symbAmp.dataLength = 5;

  symbBsol.data = symbBsolData;
  symbBsol.dataLength = 5;

  symbApos.data = symbAposData;
  symbApos.dataLength = 5;

  symbLparen.data = symbLparenData;
  symbLparen.dataLength = 5;

  symbRparen.data = symbRparenData;
  symbRparen.dataLength = 5;

  symbAst.data = symbAstData;
  symbAst.dataLength = 5;

  symbPlus.data = symbPlusData;
  symbPlus.dataLength = 5;

  symbMinus.data = symbMinusData;
  symbMinus.dataLength = 5;

  symbDot.data = symbDotData;
  symbDot.dataLength = 5;

  symbSol.data = symbSolData;
  symbSol.dataLength = 5;

  symbColon.data = symbColonData;
  symbColon.dataLength = 5;

  symbSemi.data = symbSemiData;
  symbSemi.dataLength = 5;

  symbLt.data = symbLtData;
  symbLt.dataLength = 5;

  symbEquals.data = symbEqualsData;
  symbEquals.dataLength = 5;

  symbGt.data = symbGtData;
  symbGt.dataLength = 5;

  symbQuest.data = symbQuestData;
  symbQuest.dataLength = 5;

  symbCommat.data = symbCommatData;
  symbCommat.dataLength = 5;

  symbLsqb.data = symbLsqbData;
  symbLsqb.dataLength = 5;

  symbRsqb.data = symbRsqbData;
  symbRsqb.dataLength = 5;

  symbHat.data = symbHatData;
  symbHat.dataLength = 5;

  symbLowbar.data = symbLowbarData;
  symbLowbar.dataLength = 5;

  symbGrave.data = symbGraveData;
  symbGrave.dataLength = 5;

  symbLcub.data = symbLcubData;
  symbLcub.dataLength = 5;

  symbVerbar.data = symbVerbarData;
  symbVerbar.dataLength = 5;

  symbRcub.data = symbRcubData;
  symbRcub.dataLength = 5;

  symbTilde.data = symbTildeData;
  symbTilde.dataLength = 5;
}

#ifdef __cplusplus
}
#endif

#endif  // SYMBOLS_DEFINITIONS_H