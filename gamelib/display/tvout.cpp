/*
 * tvout.c
 *
 *  Created on: Jan 2, 2015
 *      Author: Kevin
 */

#include "tvout.h"
#include "../compat/SDL_DisplayInterface.h"
#include "../compat/compat_main.h"

#include <stdbool.h>

// DOUBLE BUFFERS
uint8_t framebuffer_A[FB_WIDTH * FB_HEIGHT / 8];
uint8_t framebuffer_B[FB_WIDTH * FB_HEIGHT / 8];

// EXTERNAL BUFFER POINTER (API)
uint8_t * TVOut_Framebuffer;

// INTERNAL BUFFER POINTER
uint8_t * framebuffer;

// DOUBLE BUFFER ENABLE
bool enableDoubleBuffer;

void TVOut_EnableDoubleBuffer(bool dbe)
{
    enableDoubleBuffer = dbe;

    if (dbe)
    {
        if (framebuffer == TVOut_Framebuffer)
        {
            if (TVOut_Framebuffer == framebuffer_A)
            {
                framebuffer = framebuffer_B;
            }
            else
            {
                framebuffer = framebuffer_A;

            }
        }
    }
    else
    {
        framebuffer = TVOut_Framebuffer;
    }
}

// BUFFER SWAP FLAG
bool swapBuffers;

bool TVOut_Swap()
{
    if (!swapBuffers && enableDoubleBuffer)
        return (swapBuffers = true);
    else
        return false;
}

// BUFFER CLEAR FLAG
bool clearBufferOnSwap;
bool clearBuffer;

void TVOut_ClearBufferOnSwap(bool cb)
{
    clearBufferOnSwap = cb;
}

// FRAME SYNCHRONIZATION
bool frameWaitSemaphore;

void TVOut_WaitFrames(uint32_t fc)
{
//    if (fc == 0)
//        return;
//    while (fc > 0)
//    {
//        while (frameWaitSemaphore)
//            ;//SysCtlSleep();
//        frameWaitSemaphore = true;
//        fc--;
//    }
}

void TVOut_Init()
{
    TVOut_Framebuffer = framebuffer_A;
    framebuffer = framebuffer_B;

    frameWaitSemaphore = false;
}



void Timer0IntHandler(void)
{


    if (swapBuffers && enableDoubleBuffer)
    {
        // Swap the double-buffer
        if (framebuffer == framebuffer_A)
        {
            framebuffer = framebuffer_B;
            TVOut_Framebuffer = framebuffer_A;
        }
        else
        {
            TVOut_Framebuffer = framebuffer_B;
            framebuffer = framebuffer_A;
        }
        swapBuffers = false;
        clearBuffer = 0;
    }

    // Reset the frame wait semaphore
    if (frameWaitSemaphore)
        frameWaitSemaphore = false;





}

