#include "game.h"

// Don't worry about this; see the Text demo for more info
#include "../bitmaps/font5x7.h"

// BITMAPS DEMO

// Include the smiley bitmap from the bitmaps directory
#include "../bitmaps/smiley.h"

// Make a Bitmap_t from the raw data
// The arguments are:
// Bitmap_t(raw data,       <-- The uint8_t[] containing the image data
//          width,          <-- The width of the data in pixels
//          height)         <-- The height of the data in pixels
Bitmap_t smiley_bitmap(smiley, 64, 64);

// Make some source and destination rectangles for the scaling demo
// The arguments are:
// Rect_t(upper-left hand corner x,
//          upper-left hand corner y,
//          rectangle width,
//          rectangle height);
Rect_t
    src_rect(0, 0, 64, 64),
    dest_rect(0, 0, 64, 64),
    dest_rect_scaled(0, FB_HEIGHT - 72 - 16, 72, 72);


// +----------------------+
// | IGNORE THE FOLLOWING |
// +----------------------+

// Create a font to use for the labels
Bitmap_t font5x7_bitmap(font5x7, 256*5, 7);
uint8_t mapping(uint8_t char_code){return char_code;}
Font_t font5x7_font(&font5x7_bitmap, 5, 7, 5, 1, mapping);

// +----------------------+
// | IGNORE THE PRECEDING |
// +----------------------+

/* TODO:    Your code goes here
 *
 *          Declare any global variables here
 */

void Game_init()
{
    // Enable double-buffering
    TVOut_EnableDoubleBuffer(true);

    // Enable buffer clearing on double buffer swap
    TVOut_ClearBufferOnSwap(true);

    /* TODO:    Your code goes here
     *
     *          Any code that needs to run once
     *          to set up your game goes here
     */
}

// Modes to use in the 2x4 draw demos
const Bitmap_mode_t modes[] =
{
    MODE_BLEND, MODE_INVERT, MODE_MASK, MODE_OVERWRITE,
    MODE_BLEND_INVERT, MODE_INVERT_INVERT, MODE_MASK_INVERT, MODE_OVERWRITE_INVERT
};

const char mode_strings[][16] =
{
    "B 32x32", "I 32x32", "M 32x32", "OW 32x32",
    "B_I 32x32", "I_I 32x32", "M_I 32x32", "OW_I 32x32",
};

void Game_loop()
{
    /* TODO:    Your code goes here
     *
     *          Any code that modifies the screen
     *          which needs to run once per frame
     *          goes here
     */

    // Draw the bitmap without scaling in the upper-left hand corner
    screen.bitmap(&smiley_bitmap, &src_rect, &dest_rect, MODE_OVERWRITE);

    // Throw a label on there
    screen.text(font5x7_font, dest_rect.lowerLeft() + Point_t(0,2), "OW 64x64", MODE_OVERWRITE_INVERT);

    // Draw the bitmap with scaling in the lower-left hand corner
    screen.bitmap_scaled(&smiley_bitmap, &src_rect, &dest_rect_scaled, MODE_OVERWRITE);

    // Throw a label on there
    screen.text(font5x7_font, dest_rect_scaled.lowerLeft() + Point_t(0,2), "OW sc. 72x72", MODE_OVERWRITE_INVERT);

    // For 2 rows
    for(int j = 0; j < 2; j++)
        // For 4 columns
        for(int i = 0; i < 4; i++)
        {
            // The relative position of the top rectangle (drawn first), 50% scaling
            Rect_t overlap_top_rect(0,0,32,32);
            // The relative position of the bottom rectangle (drawn second), 50% scaling
            Rect_t overlap_bottom_rect(16,16,32,32);

            // The temporary destination rectangle, used to store the
            // absolute position before drawing
            Rect_t overlap_dest_rect;

            // The absolute offset; relative positions are added
            // to this to produce the output positions
            Point_t offset((i + 1) * (64 + 10), 10 + (10 + 64) * j);

            // Compute the absolute destination rectangles and draw the bitmap scaled to 50%
            overlap_dest_rect = overlap_top_rect + offset;
            screen.bitmap_scaled(&smiley_bitmap, &src_rect, &overlap_dest_rect, MODE_OVERWRITE);

            overlap_dest_rect = overlap_bottom_rect + offset;
            screen.bitmap_scaled(&smiley_bitmap, &src_rect, &overlap_dest_rect, modes[i + 4*j]);

            // Throw a label on there
            screen.text(font5x7_font, overlap_dest_rect.lowerLeft() + Point_t(0,2), mode_strings[i + 4*j], MODE_OVERWRITE_INVERT);
        }

    // Swap the buffer (present the drawing to the screen)
    TVOut_Swap();

    // Wait for the frame to complete drawing to the screen
    TVOut_WaitFrames(1);
}
