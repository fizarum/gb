#ifndef GAME_SDK_COLOR_H
#define GAME_SDK_COLOR_H

#include "types.h"

/**
 * @brief color in RGB565 format
 */
typedef _u16 Color;

static const Color ColorTransparent = UINT16_MAX;

/**
 * @brief index of color in pallette, can have values in range: 0..15
 */
typedef _u8 ColorIndex;
typedef ColorIndex _ci;

/**
 * @brief pair of ColorIndex values first 4 bits represents value of first
 * ColorIndex the second 4 - second ColorIndex.
 * Can have value in range: 0..255
 */
typedef _u8 ColorIndexes;

#endif  // GAME_SDK_COLOR_H