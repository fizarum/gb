#include "line.h"

#include <stdlib.h>

#include "../utils/utils.h"

#define DRT_LINE_OFFSET_1 32
#define DRT_LINE_OFFSET_2 64
#define DRT_LINE_OFFSET_3 96
#define DRT_LINE_OFFSET_4 128

typedef struct DRTLine_t {
  uint32_t first;
  uint32_t second;
  uint32_t third;
  uint32_t fourth;
  uint32_t fifth;
} DRTLine_t;

DRTLine_t *DRTLineCreate() {
  DRTLine_t *line = (DRTLine_t *)malloc(sizeof(DRTLine_t));
  DRTLineReset(line);
  return line;
}

void DRTLineDestroy(DRTLine_t *line) { free(line); }

void DRTLineSet(DRTLine_t *line) {
  line->first = 0xffffffff;
  line->second = 0xffffffff;
  line->third = 0xffffffff;
  line->fourth = 0xffffffff;
  line->fifth = 0xffffffff;
}

void DRTLineReset(DRTLine_t *line) {
  line->first = 0;
  line->second = 0;
  line->third = 0;
  line->fourth = 0;
  line->fifth = 0;
}

void DRTLineSetPixel(DRTLine_t *line, const uint8_t position) {
  if (position < DRT_LINE_OFFSET_1) {
    line->first = setBit32(line->first, position);
    return;
  }
  if (position < DRT_LINE_OFFSET_2) {
    line->second = setBit32(line->second, position);
    return;
  }
  if (position < DRT_LINE_OFFSET_3) {
    line->third = setBit32(line->third, position);
    return;
  }
  if (position < DRT_LINE_OFFSET_4) {
    line->fourth = setBit32(line->fourth, position);
    return;
  }

  line->fifth = setBit32(line->fifth, position);
}

bool DRTLineIsPixelSet(const DRTLine_t *line, const uint8_t position) {
  if (position < DRT_LINE_OFFSET_1) return isBitSet32(line->first, position);
  if (position < DRT_LINE_OFFSET_2) return isBitSet32(line->second, position);
  if (position < DRT_LINE_OFFSET_3) return isBitSet32(line->third, position);
  if (position < DRT_LINE_OFFSET_4) return isBitSet32(line->fourth, position);
  return isBitSet32(line->fifth, position);
}

bool DRTLineHasAnyPixelSet(const DRTLine_t *line) {
  return line->first > 0 || line->second > 0 || line->third > 0 ||
         line->fourth > 0 || line->fifth > 0;
}