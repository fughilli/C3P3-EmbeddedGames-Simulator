/*
 * tvout_constants.h
 *
 *  Created on: Jan 2, 2015
 *      Author: Kevin
 */

#ifndef TVOUT_CONSTANTS_H_
#define TVOUT_CONSTANTS_H_

//#define PAL
#define NTSC

#define ASPECT_16_9
//#define ASPECT_4_3

#if defined(PAL)
#define SCANLINE_TIME_US 64
#define BROAD_SYNC_GAP_NS 4700
#define SHORT_SYNC_GAP_NS 2350
#define HSYNC_GAP_NS 4700
#define BACK_PORCH_GAP_NS 5700
#define FRONT_PORCH_GAP_NS 2350 	//1650
#define US_MAGNITUDE 1000000
#define NUM_BROAD_SYNC_LINES 5
#define NUM_SHORT_SYNC_LINES 5
#define NUM_END_SHORT_SYNC_LINES 6
#define NUM_DATA_LINES 304
#define VIDEO_LINE_OFFSET 46
#define NUM_VIDEO_LINES 229
#elif defined(NTSC)
// +----------------------------------+
// | TIMING CONSTANTS, IN NANOSECONDS |
// +----------------------------------+

// Time for one scan line
#define SCANLINE_TIME_NS (64000)
// Low pulse width in the broad sync section
#define BROAD_SYNC_GAP_NS (4700)
// Low pulse width in the short sync section
#define SHORT_SYNC_GAP_NS (2350)

// Low pulse width for a horizontal sync pulse
#define HSYNC_GAP_NS (4700)

// Medium pulse width for back porch
#define BACK_PORCH_GAP_NS (5700) // 5700
// Medium pulse width for front porch
#define FRONT_PORCH_GAP_NS (1650 + 200) //1650

// Magnitude of a second relative to a nanosecond (10e9)
#define NS_MAGNITUDE (1000000000)

// +--------------------+
// | LINE NUM CONSTANTS |
// +--------------------+

// Number of broad sync pulses (5, 2.5 lines)
#define NUM_BROAD_SYNC_LINES (5)
// Number of short sync pulses (5, 2.5 lines)
#define NUM_SHORT_SYNC_LINES (5)

// Number of short sync pulses at the end of the frame (6, 3 lines)
#define NUM_END_SHORT_SYNC_LINES (6)

// Number of scanlines where data can be encoded
#define NUM_DATA_LINES (251)

// Offset to the first line of video
#define VIDEO_LINE_OFFSET (10)			//46

// Number of video lines
#define NUM_VIDEO_LINES (238)			//179

// Vertical offset of the framebuffer with respect to the first video line (before this, alternating lines are white and black)
#define FB_LINE_OFFSET (9)

#endif

// Total lines in a frame
#define TOTAL_LINES (((NUM_BROAD_SYNC_LINES + NUM_SHORT_SYNC_LINES + NUM_END_SHORT_SYNC_LINES)/2) + NUM_DATA_LINES)

// +-----------------------------------+
// | HORIZONTAL RESOLUTION CALCULATION |
// +-----------------------------------+

// Period of the active video section in one scanline
#define ACTIVE_VIDEO_LINE_TIME (SCANLINE_TIME_NS - (HSYNC_GAP_NS + FRONT_PORCH_GAP_NS + BACK_PORCH_GAP_NS))

// SSI module frequency
#if defined(ASPECT_16_9)
#define TV_SSI_FREQ (7272727)
#elif defined(ASPECT_4_3)
#define TV_SSI_FREQ (6153846)
#endif

// Calculated horizontal resolution
#define HORIZ_RESOLUTION ((unsigned int)((double)(TV_SSI_FREQ)*ACTIVE_VIDEO_LINE_TIME/1000000000))

// Next multiple of 8 of horizontal resolution
#define HORIZ_RESOLUTION_NMO_8 ((HORIZ_RESOLUTION%8)?((HORIZ_RESOLUTION&~(8-1))+8):(HORIZ_RESOLUTION))

#endif /* TVOUT_CONSTANTS_H_ */

