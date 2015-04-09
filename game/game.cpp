#include "game.h"

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

    // Swap the buffer (present the drawing to the screen)
    TVOut_Swap();

    // Wait for the frame to complete drawing to the screen
    TVOut_WaitFrames(1);
}
