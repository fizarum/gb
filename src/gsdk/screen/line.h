#ifndef GAME_SDK_DRT_LINE_H
#define GAME_SDK_DRT_LINE_H

/**
 * @brief Resolution of screen can vary from 8x6 to 160x120 depending on pixel
 * size. For flexibility of current solution we have count that we always
 * working on the biggest available resolution: 160x120. Based on this info we
 * need some way to mark "durty" pixels for redraw in next draw call.
 * The simplest way is to make an array of numbers, each bit of which keeps
 * "durty" pixel data. If we have uint8_t which have 8 bits we can represent 8
 * pixels, thus 195 (or b1100 0011 or 0xC3) can refer to first, second, seventh
 * and eighth pixels as "durty" onces. Because we have limits in big integers,
 * we can take uint32_t as such pixel representer. But 160 (width) or even 120
 * (height) are too big for uint32_t.
 * So, to minimize pixels wasting we can select next way: each line has 5 of
 * uint32_t numbers to represent one line of 160 pixels. And we need 120 such
 * numbers.
 *
 * Briefly, entire screen is: array<Line, 120> - 120 lines, where Line is:
 * array of 5 numbers of uint32_t
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Structure representing durty region tracker (DRT) line of max 160
 * pixels (bytes of each Segment)
 */
typedef struct DRTLine_t DRTLine_t;

DRTLine_t *DRTLineCreate();
void DRTLineDestroy(DRTLine_t *line);

/**
 * @brief Set all elements of line to 1 (means its durty)
 */
void DRTLineSet(DRTLine_t *line);

/**
 * @brief Set all elements of line to 0 (means its clear)
 */
void DRTLineReset(DRTLine_t *line);

void DRTLineSetPixel(DRTLine_t *line, const uint8_t position);
bool DRTLineIsPixelSet(const DRTLine_t *line, const uint8_t position);
bool DRTLineHasAnyPixelSet(const DRTLine_t *line);

#ifdef __cplusplus
}
#endif

#endif  // GAME_SDK_DRT_LINE_H