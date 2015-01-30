#ifndef _TVOUT_H_
#define _TVOUT_H_

#include <stdint.h>
#include "tvout_constants.h"

#define FB_WIDTH (HORIZ_RESOLUTION_NMO_8)
#define FB_HEIGHT (NUM_VIDEO_LINES - FB_LINE_OFFSET)

extern uint8_t * TVOut_Framebuffer;

void TVOut_Init();
void TVOut_WaitFrames(uint32_t fc);
bool TVOut_Swap();
void TVOut_ClearBufferOnSwap(bool cb);
void TVOut_EnableDoubleBuffer(bool dbe);

#endif // _TVOUT_H_
