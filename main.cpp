#include <SDL2/SDL.h>
#include "SDL_DisplayInterface.h"
#include "bitbuf_screen.h"
#include "hellojustin.h"

SDL_DisplayInterface* displayInterface;

void draw();

#define FRAMEBUFFER_WIDTH (376)
#define FRAMEBUFFER_HEIGHT (229)
#define FRAMEBUFFER_BYTE_WIDTH (FRAMEBUFFER_WIDTH / 8)

static uint8_t framebuffer[FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT / 8] = {0};

Screen screen(framebuffer, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);

const uint8_t man_walk[] =
{
    0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
    0b11111100, 0b01111111, 0b11111100, 0b01111111, 0b11111100, 0b01111111, 0b11111100, 0b01111111, 0b11111100, 0b01111111, 0b11111100, 0b01111111, 0b11111100, 0b01111111, 0b11111100, 0b01111111,
    0b11111100, 0b01111111, 0b11111100, 0b01111111, 0b11111100, 0b01111111, 0b11111100, 0b01111111, 0b11111100, 0b01111111, 0b11111100, 0b01111111, 0b11111100, 0b01111111, 0b11111100, 0b01111111,
    0b11111100, 0b01111111, 0b11111100, 0b01111111, 0b11111100, 0b01111111, 0b11111100, 0b01111111, 0b11111100, 0b01111111, 0b11111100, 0b01111111, 0b11111100, 0b01111111, 0b11111100, 0b01111111,
    0b11111100, 0b01111111, 0b11111100, 0b01111111, 0b11111100, 0b01111111, 0b11111100, 0b01111111, 0b11111100, 0b01111111, 0b11111100, 0b01111111, 0b11111100, 0b01111111, 0b11111100, 0b01111111,
    0b11111110, 0b11111111, 0b11111110, 0b11111111, 0b11111110, 0b11111111, 0b11111110, 0b11111111, 0b11111110, 0b11111111, 0b11111110, 0b11111111, 0b11111110, 0b11111111, 0b11111110, 0b11111111,
    0b11111100, 0b11111111, 0b11111100, 0b11111111, 0b11111100, 0b11111111, 0b11111100, 0b11111111, 0b11111100, 0b11111111, 0b11111100, 0b11111111, 0b11111100, 0b11111111, 0b11111100, 0b01101111,
    0b11110001, 0b01101111, 0b11111101, 0b01111111, 0b11111100, 0b11111111, 0b11111100, 0b01111111, 0b11111000, 0b11111111, 0b11111000, 0b01111111, 0b11111001, 0b01111111, 0b11100001, 0b10011111,
    0b11101101, 0b10011111, 0b11111001, 0b10111111, 0b11111001, 0b01111111, 0b11111100, 0b01111111, 0b11111000, 0b01111111, 0b11110001, 0b01111111, 0b11110101, 0b10111111, 0b11111101, 0b11111111,
    0b11111011, 0b11111111, 0b11110011, 0b11001111, 0b11111001, 0b01111111, 0b11111100, 0b01111111, 0b11110011, 0b00111111, 0b11101011, 0b10011111, 0b11101011, 0b11001111, 0b11111011, 0b11111111,
    0b11111101, 0b11111111, 0b11111101, 0b11111111, 0b11111001, 0b10011111, 0b11111100, 0b00111111, 0b11111101, 0b11111111, 0b11111100, 0b11111111, 0b11111100, 0b11111111, 0b11111000, 0b11111111,
    0b11111100, 0b11111111, 0b11111101, 0b11111111, 0b11111100, 0b11111111, 0b11111100, 0b11111111, 0b11111010, 0b11111111, 0b11111011, 0b01111111, 0b11111011, 0b01111111, 0b11111011, 0b01111111,
    0b11111101, 0b01111111, 0b11110100, 0b11111111, 0b11110100, 0b11111111, 0b11111101, 0b01111111, 0b11111101, 0b01111111, 0b11111101, 0b01111111, 0b11111011, 0b01111111, 0b11111011, 0b01111111,
    0b11100011, 0b10111011, 0b11101000, 0b01111111, 0b11101000, 0b11111111, 0b11111000, 0b01111111, 0b11111000, 0b11111111, 0b11110000, 0b01111111, 0b11000111, 0b01111111, 0b11100111, 0b10101111,
    0b11011111, 0b11010111, 0b11101111, 0b01101111, 0b11101110, 0b11111111, 0b11111011, 0b11111111, 0b11110101, 0b11111111, 0b11100110, 0b11111111, 0b11011111, 0b00111111, 0b11001111, 0b10011111,
    0b11100111, 0b11101111, 0b11110111, 0b10011111, 0b11101110, 0b00111111, 0b11111000, 0b11111111, 0b11110001, 0b11111111, 0b11110011, 0b11111111, 0b11001111, 0b11111111, 0b11100111, 0b11111111,

};

Bitmap_t man_walk_bmp;
Bitmap_t hellojustin_bmp;

int main(int argc, char* argv[])
{
    SDL_Window* window;
    displayInterface = SDL_DisplayInterface::createWindow(FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, window, 4);

    SDL_SetHintWithPriority(SDL_HINT_RENDER_VSYNC, "1", SDL_HINT_OVERRIDE);

    /*
     * TODO:
     * Your code goes here
     */

    SDL_Event gameEvent;

    screen.clear(BLACK);

    man_walk_bmp.data = man_walk;
    man_walk_bmp.w = 128;
    man_walk_bmp.h = 16;

    hellojustin_bmp.data = hellojustin;
    hellojustin_bmp.w = 128;
    hellojustin_bmp.h = 128;

    while(true)
    {
        SDL_PumpEvents();

        if(SDL_PollEvent(&gameEvent))
        {
            if(gameEvent.type == SDL_QUIT || (gameEvent.type == SDL_KEYDOWN && gameEvent.key.keysym.scancode == SDL_SCANCODE_ESCAPE))
                break;
        }

        //if(gameEvent.window.event && )

        /*
         * TODO:
         * Your code goes here
         */

        draw();
    }

    delete displayInterface;

    SDL_Quit();


    return 0;
}

void draw()
{
    screen.clear(BLACK);

    static int man_frame = 0;
    static int frame_count = 0;

    frame_count++;

    if(frame_count == 16)
    {
        frame_count = 0;
        man_frame++;
        man_frame = man_frame % 8;
    }



    screen.circle(screen.getWidth()/2, screen.getHeight()/2, 20, WHITE, NONE);

    Rect_t srcRect = {16*man_frame,0,16,16};
    Point_t destPoint = {screen.getWidth()/2 - 8, screen.getHeight()/2 - 8};

    screen.bitmap(&man_walk_bmp, &srcRect, &destPoint, MODE_BLEND_INVERT);

    screen.bitmap(0,0, hellojustin, 128, 128, MODE_BLEND);

    /*
     * TODO:
     * Your code goes here
     */

    displayInterface->fillFromBitmap(framebuffer);
    displayInterface->flip();


}
