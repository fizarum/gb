#include "symbols_res.h"

#include <map.h>

#include "letter_definitions.h"
#include "number_definitions.h"
#include "symbol_definitions.h"

Map_t *allSymbols;

void SymbolsInit() {
  SymbolAssignDefinitions();

  // save all into map
  MapSet(allSymbols, symbNum.asciiCode, &symbNum);
  MapSet(allSymbols, symbExcl.asciiCode, &symbExcl);
  MapSet(allSymbols, symbQuot.asciiCode, &symbQuot);
  MapSet(allSymbols, symbDollar.asciiCode, &symbDollar);
  MapSet(allSymbols, symbPercnt.asciiCode, &symbPercnt);
  MapSet(allSymbols, symbAmp.asciiCode, &symbAmp);
  MapSet(allSymbols, symbBsol.asciiCode, &symbBsol);
  MapSet(allSymbols, symbApos.asciiCode, &symbApos);
  MapSet(allSymbols, symbLparen.asciiCode, &symbLparen);
  MapSet(allSymbols, symbRparen.asciiCode, &symbRparen);
  MapSet(allSymbols, symbAst.asciiCode, &symbAst);
  MapSet(allSymbols, symbPlus.asciiCode, &symbPlus);
  MapSet(allSymbols, symbMinus.asciiCode, &symbMinus);
  MapSet(allSymbols, symbDot.asciiCode, &symbDot);
  MapSet(allSymbols, symbSol.asciiCode, &symbSol);
  MapSet(allSymbols, symbColon.asciiCode, &symbColon);
  MapSet(allSymbols, symbSemi.asciiCode, &symbSemi);
  MapSet(allSymbols, symbLt.asciiCode, &symbLt);
  MapSet(allSymbols, symbEquals.asciiCode, &symbEquals);
  MapSet(allSymbols, symbGt.asciiCode, &symbGt);
  MapSet(allSymbols, symbQuest.asciiCode, &symbQuest);
  MapSet(allSymbols, symbCommat.asciiCode, &symbCommat);
  MapSet(allSymbols, symbLsqb.asciiCode, &symbLsqb);
  MapSet(allSymbols, symbRsqb.asciiCode, &symbRsqb);
  MapSet(allSymbols, symbHat.asciiCode, &symbHat);
  MapSet(allSymbols, symbLowbar.asciiCode, &symbLowbar);
  MapSet(allSymbols, symbGrave.asciiCode, &symbGrave);
  MapSet(allSymbols, symbLcub.asciiCode, &symbLcub);
  MapSet(allSymbols, symbVerbar.asciiCode, &symbVerbar);
  MapSet(allSymbols, symbRcub.asciiCode, &symbRcub);
  MapSet(allSymbols, symbTilde.asciiCode, &symbTilde);
}

void SymbolsResInit() {
  allSymbols = MapCreate(UINT8_MAX);

  // letters
  LetterAssignDefinitions();

  // save all into map
  MapSet(allSymbols, symbA.asciiCode, &symbA);
  MapSet(allSymbols, symbB.asciiCode, &symbB);
  MapSet(allSymbols, symbC.asciiCode, &symbC);
  MapSet(allSymbols, symbD.asciiCode, &symbD);
  MapSet(allSymbols, symbE.asciiCode, &symbE);
  MapSet(allSymbols, symbF.asciiCode, &symbF);
  MapSet(allSymbols, symbG.asciiCode, &symbG);
  MapSet(allSymbols, symbH.asciiCode, &symbH);
  MapSet(allSymbols, symbI.asciiCode, &symbI);
  MapSet(allSymbols, symbJ.asciiCode, &symbJ);
  MapSet(allSymbols, symbK.asciiCode, &symbK);
  MapSet(allSymbols, symbL.asciiCode, &symbL);
  MapSet(allSymbols, symbM.asciiCode, &symbM);
  MapSet(allSymbols, symbN.asciiCode, &symbN);
  MapSet(allSymbols, symbO.asciiCode, &symbO);
  MapSet(allSymbols, symbP.asciiCode, &symbP);
  MapSet(allSymbols, symbQ.asciiCode, &symbQ);
  MapSet(allSymbols, symbR.asciiCode, &symbR);
  MapSet(allSymbols, symbS.asciiCode, &symbS);
  MapSet(allSymbols, symbT.asciiCode, &symbT);
  MapSet(allSymbols, symbU.asciiCode, &symbU);
  MapSet(allSymbols, symbV.asciiCode, &symbV);
  MapSet(allSymbols, symbW.asciiCode, &symbW);
  MapSet(allSymbols, symbX.asciiCode, &symbX);
  MapSet(allSymbols, symbY.asciiCode, &symbY);
  MapSet(allSymbols, symbZ.asciiCode, &symbZ);

  // numbers
  NumberAssignDefinitions();

  // save all into map
  MapSet(allSymbols, symb1.asciiCode, &symb1);
  MapSet(allSymbols, symb2.asciiCode, &symb2);
  MapSet(allSymbols, symb3.asciiCode, &symb3);
  MapSet(allSymbols, symb4.asciiCode, &symb4);
  MapSet(allSymbols, symb5.asciiCode, &symb5);
  MapSet(allSymbols, symb6.asciiCode, &symb6);
  MapSet(allSymbols, symb7.asciiCode, &symb7);
  MapSet(allSymbols, symb8.asciiCode, &symb8);
  MapSet(allSymbols, symb9.asciiCode, &symb9);
  MapSet(allSymbols, symb0.asciiCode, &symb0);

  SymbolsInit();
}

SymbolData_t *SymbolsGet(_u8 c) {
  return (SymbolData_t *)(MapGet(allSymbols, c));
}