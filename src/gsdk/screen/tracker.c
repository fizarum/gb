#include "tracker.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/screen_utils.h"
#include "../utils/utils.h"

typedef struct Tracker_t {
  _u8* canvas;
  _u8 canvasWidth;
  _u16 canvasSize;

  ScreenConfig* config;

  bool isAtLeastOnePixelDurty;

} Tracker_t;

Tracker_t* TrackerCreate(ScreenConfig* config) {
  Tracker_t* tracker = (Tracker_t*)malloc(sizeof(Tracker_t));
  if (tracker == NULL) return NULL;

  tracker->config = config;

  tracker->canvasWidth = config->width / CELL_SIZE;

  printf("screen width: %d\n", config->width);
  printf("screen width [in cells]: %d\n", tracker->canvasWidth);

  _u16 totalCells = tracker->canvasWidth * config->height;
  printf("total cells: %d\n", totalCells);

  tracker->canvas = malloc(totalCells);
  tracker->canvasSize = totalCells;
  tracker->isAtLeastOnePixelDurty = false;

  return tracker;
}

void TrackerDestroy(Tracker_t* tracker) {
  if (tracker == NULL) {
    return;
  }

  free(tracker->canvas);
  free(tracker);
}

void TrackerClear(Tracker_t* tracker) {
  memset(tracker->canvas, 0, tracker->canvasSize);

  tracker->isAtLeastOnePixelDurty = false;
}

void TrackerSetRegion(Tracker_t* tracker, const _u16 l, const _u16 t,
                      const _u16 r, const _u16 b) {
  if (l > r || t > b) {
    return;
  }

  if (t >= tracker->config->height || l >= tracker->config->width) {
    return;
  }

  // get left cell on a line
  _u8 leftCell = l / CELL_SIZE;
  // get right cell on a line
  _u8 rightCell = r / CELL_SIZE;

  _u8 startIndexInCellOfDurtyRegion = l % CELL_SIZE;
  _u8 endIndexInCellOfDurtyRegion = r % CELL_SIZE;
  printf("left cell: %d | right cell: %d\n", leftCell, rightCell);

  _u16 index = 0;
  _u8 temp = 0;

  // TODO: write top left position of durty region, so we can start iterate from
  // it in TrackerGetRegion to minimize iteration steps
  for (_u16 y = t; y <= b; y++) {
    _u16 yOffset = GetIndexIn2DSpace(0, y, tracker->canvasWidth);

    for (_u8 x = leftCell; x <= rightCell; x++) {
      index = yOffset + x;
      temp = 0xff;

      if (x == leftCell) {
        temp >> startIndexInCellOfDurtyRegion;
      }

      if (x == rightCell) {
        temp << endIndexInCellOfDurtyRegion;
      }

      tracker->canvas[index] = temp;
    }
  }
  tracker->isAtLeastOnePixelDurty = true;
}

void TrackerGetRegion(const Tracker_t* tracker, TrackerCallback callback) {
  if (tracker->isAtLeastOnePixelDurty == false) return;

  _u8 data = 0;

  for (_u16 index = 0; index < tracker->canvasSize; index++) {
    data = tracker->canvas[index];
    if (data > 0) {
      callback(index, data);
    }
  }
}

void TrackerSetAll(Tracker_t* tracker) {
  memset(tracker->canvas, 0xff, tracker->canvasSize);

  tracker->isAtLeastOnePixelDurty = true;
}