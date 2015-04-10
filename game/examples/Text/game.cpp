#include "game.h"

// FONT DEMO

// Include the 5x7 font
#include "../bitmaps/font5x7.h"

// Create a bitmap for the 5x7 font
// this bitmap is a wide strip having all the characters
// in the font side by side

// The 5x7 font sheet has characters for all ASCII codes (8-bit integer = 256 characters)
// and each character is 5 pixels wide and 7 pixels tall
// The arguments to the Bitmap_t constructor are:
// Bitmap_t( bitmap data,       <-- The uint8_t[] of the bitmap data. Generate this with bitmapgen.py
//           bitmap width,      <-- Width of the bitmap in pixels (256 characters * 5 pixels per character)
//           bitmap height)     <-- Height of the bitmap in pixels (7 pixels tall)
Bitmap_t font5x7_bitmap(font5x7, 5 * 256, 7);

// Create a mapping for the font; all characters are
// located at positions corresponding to their ASCII
// values, so this mapping is direct
uint8_t mapping(uint8_t char_code)
{
    return char_code;
}

// Create a font from the bitmap
// The arguments are:
// Font_t( pointer to bitmap,   <-- a pointer to the font's bitmap
//         character width,     <-- width of a character, in pixels
//         character height,    <-- height of a character, in pixels
//         character stride,    <-- number of pixels between characters in the bitmap (usually equal to the character width)
//         kerning space,       <-- number of blank pixels to leave between characters when printing
//         mapping)             <-- a mapping of ASCII character values to positions in the bitmap
// The font is 5*7 pixels per character
Font_t font5x7_font(&font5x7_bitmap, 5, 7, 5, 1, mapping);

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

int8_t yDisplacementTable[] = {
0,1,2,3,4,5,4,3,2,1,0,-1,-2,-3,-4,-5,-4,-3,-2,-1
};

uint32_t t = 0;
Point_t charDisplacement(0, 0);

Point_t& modpos(uint32_t char_idx)
{
    charDisplacement.y = yDisplacementTable[(t/1024 + char_idx*3)%20];
    t++;
    return charDisplacement;
}

void Game_loop()
{
    /* TODO:    Your code goes here
     *
     *          Any code that modifies the screen
     *          which needs to run once per frame
     *          goes here
     */

    screen.text(font5x7_font,                                   // <-- the font to use
                Point_t(10, 10),                                // <-- where to put the upper-left hand corner of the text
                "Test message!\n"                               // <-- the string to print
                "You can also print things on multiple\n"
                "lines by using the \\n character sequence\n"
                "in your strings (newline).",
                MODE_BLEND_INVERT                               // <-- the drawing mode (Invert the source bitmap, then blend)
                );

    screen.text_plus_offset(font5x7_font,                       // <-- the font to use
                Point_t(10, 64),                                // <-- where to put the upper-left hand corner of the text
                "Wavy text!\n"                                  // <-- the string to print
                "You can also print wavy text and apply other effects\n"
                "by calling screen.text_plus_offset and passing a\n"
                "position modification function as an argument!\n"
                "WHEEEE!",
                modpos,
                MODE_BLEND_INVERT                               // <-- the drawing mode (Invert the source bitmap, then blend)
                );

    // Swap the buffer (present the drawing to the screen)
    TVOut_Swap();

    // Wait for the frame to complete drawing to the screen
    TVOut_WaitFrames(1);
}
