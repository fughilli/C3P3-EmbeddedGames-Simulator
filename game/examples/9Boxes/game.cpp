#include "game.h"

#include "../gamelib/fancygraphics/boxsprite.h"
#include "../bitmaps/border_fancy.h"
#include "../bitmaps/border_bubble.h"
#include "../bitmaps/font5x7.h"

/* TODO:    Your code goes here
 *
 *          Declare any global variables here
 */

Bitmap_t nine_box_border_bitmap(border_fancy, 24, 24);
BoxSprite_t nine_box_border_bspr(&nine_box_border_bitmap, 9);

Bitmap_t font_bitmap(font5x7, 256*5, 7);
uint8_t mapping(uint8_t charcode)
{
    return charcode;
}
Font_t font_font(&font_bitmap, 5, 7, 5, 1, mapping);

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

void Game_loop()
{
    screen.clear(WHITE);
    screen.textBox(
            "POK\xE9MON",
            font_font,
            nine_box_border_bspr,
            Point_t(10,10),
            5,
            MODE_MASK,
            MODE_MASK);
    /* TODO:    Your code goes here
     *
     *          Any code that modifies the screen
     *          which needs to run once per frame
     *          goes here
     */

    // Swap the buffer (present the drawing to the screen)
    TVOut_Swap();

    // Wait for the frame to complete drawing to the screen
    TVOut_WaitFrames(1);
}
