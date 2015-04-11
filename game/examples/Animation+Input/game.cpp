#include "game.h"

// Include the littleman bitmap (sprite sheet)
#include "../bitmaps/littleman_fixed.h"

// Create a bitmap from the raw data
// The arguments are:
// Bitmap_t(raw data,       <-- The uint8_t[] containing the image data
//          width,          <-- The width of the data in pixels
//          height)         <-- The height of the data in pixels
Bitmap_t littleman_bitmap(littleman_fixed, 16*8, 64);

// Enum to select the row from the sprite sheet
// Top row (row 0) is the right walk animation
// ...
// Bottom row (row 3) is the left jump animation
enum Walk_type
{
    WALK_RIGHT = 0,
    JUMP_RIGHT = 1,
    WALK_LEFT = 2,
    JUMP_LEFT = 3
};

// Get the source rectangle for the sprite
// sheet for a specified animation and frame
// Walk_type wt     <-- the animation
// uint8_t t        <-- the frame (out of 8)
Rect_t getSourceRectAnim(Walk_type wt, uint8_t t)
{
    return Rect_t(
               // Each frame in the sheet is 16x16;
               // To select the frame, multiply the frame
               // number by 16
               16*t,
               // The animation is selected the same
               // way, but in y instead of x
               16*((uint8_t)wt),
               // Width, Height
               16, 16);
}

// Create a struct to store the buttons states
Remote_buttons_t remote_status;

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

void Game_loop()
{
    /* TODO:    Your code goes here
     *
     *          Any code that modifies the screen
     *          which needs to run once per frame
     *          goes here
     */

    // Get the status of the remote
    Remote_read(&remote_status);

    static Rect_t destRect(0,0,16,16);
    static Walk_type wtype = WALK_LEFT;
    static uint8_t time = 0;


    bool moved = false;
    if(remote_status.buttons.right)
    {
        destRect += Point_t(1,0);
        wtype = WALK_RIGHT;
        moved = true;
    }
    if(remote_status.buttons.left)
    {
        destRect -= Point_t(1,0);
        wtype = WALK_LEFT;
        moved = true;
    }
    if(remote_status.buttons.up)
    {
        destRect -= Point_t(0,1);
        moved = true;
    }
    if(remote_status.buttons.down)
    {
        destRect += Point_t(0,1);
        moved = true;
    }

    if(moved)
    {
        time++;
        time %= 8;
    }
    else
    {
        time = 0;
    }

    Rect_t srcRect = getSourceRectAnim(wtype, time);

    screen.bitmap(&littleman_bitmap, &srcRect, &destRect, MODE_OVERWRITE_INVERT);

    // Swap the buffer (present the drawing to the screen)
    TVOut_Swap();

    // Wait for the frame to complete drawing to the screen
    TVOut_WaitFrames(1);
}
