#include <SDL2/SDL.h>
#include "gamelib/compat/SDL_DisplayInterface.h"
#include "gamelib/display/screen.h"
#include "bitmaps/hellojustin.h"
#include "gamelib/input/remote.h"
#include "bitmaps/littleman_fixed.h"
#include "gamelib/compat/compat_main.h"
#include "gamelib/display/tvout.h"

SDL_DisplayInterface* compat_disp_interface;

Remote_buttons_t sim_hooked_buttons;

void draw();

//#define FRAMEBUFFER_WIDTH (376)
//#define FRAMEBUFFER_HEIGHT (229)
//#define FRAMEBUFFER_BYTE_WIDTH (FRAMEBUFFER_WIDTH / 8)

//static uint8_t framebuffer[FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT / 8] = {0};

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

int contx = 0, conty = 0;

int main(int argc, char* argv[])
{
    TVOut_Init();
    SDL_Window* window;
    compat_disp_interface = SDL_DisplayInterface::createWindow(FB_WIDTH, FB_HEIGHT, window, 4);

    //SDL_SetHintWithPriority(SDL_HINT_RENDER_VSYNC, "1", SDL_HINT_OVERRIDE);

    /*
     * TODO:
     * Your code goes here
     */

    SDL_Event gameEvent;

    screen.clear(BLACK);

    man_walk_bmp.data = littleman_fixed;
    man_walk_bmp.w = 128;
    man_walk_bmp.h = 64;

    hellojustin_bmp.data = hellojustin;
    hellojustin_bmp.w = 128;
    hellojustin_bmp.h = 128;

    contx = screen.getWidth()/2;
    conty = screen.getHeight()/2;

    while(true)
    {
        SDL_PumpEvents();

        if(SDL_PollEvent(&gameEvent))
        {
            if(gameEvent.type == SDL_QUIT || (gameEvent.type == SDL_KEYDOWN && gameEvent.key.keysym.scancode == SDL_SCANCODE_ESCAPE))
                break;

            if(gameEvent.type == SDL_KEYDOWN || gameEvent.type == SDL_KEYUP)
            {
                uint8_t buttonflag = 0;
                if(gameEvent.type == SDL_KEYDOWN)
                    buttonflag = 1;
                switch(gameEvent.key.keysym.scancode)
                {
                case SDL_SCANCODE_W:
                    sim_hooked_buttons.buttons.up = buttonflag;
                    break;
                case SDL_SCANCODE_A:
                    sim_hooked_buttons.buttons.left = buttonflag;
                    break;
                case SDL_SCANCODE_S:
                    sim_hooked_buttons.buttons.down = buttonflag;
                    break;
                case SDL_SCANCODE_D:
                    sim_hooked_buttons.buttons.right = buttonflag;
                    break;
                case SDL_SCANCODE_H:
                    sim_hooked_buttons.buttons.a = buttonflag;
                    break;
                case SDL_SCANCODE_U:
                    sim_hooked_buttons.buttons.b = buttonflag;
                    break;
                default:
                    break;
                }
            }
        }

        //if(gameEvent.window.event && )

        /*
         * TODO:
         * Your code goes here
         */

        draw();
    }

    delete compat_disp_interface;

    SDL_Quit();


    return 0;
}

template <class T>
void clamp(T& val, T minval, T maxval)
{
    if(val < minval)
        val = minval;
    if(val > maxval)
        val = maxval;
}

void draw()
{
    screen.clear(BLACK);

    static int man_frame = 0;
    static int frame_count = 0;

    Remote_buttons_t rbs;
    Remote_read(&rbs);

    static uint8_t man_anim = 1;
    static uint8_t man_subanim = 0;
    static uint8_t man_idle = 1;
    static uint8_t bmpsz = 16;

    frame_count++;

    if(frame_count % 4 == 0)
    {
        man_subanim = 0;
        man_idle = 1;
        if(rbs.buttons.left)
        {
            contx--;
            man_anim = 1;
            man_idle = 0;
        }
        if(rbs.buttons.right)
        {
            contx++;
            man_anim = 0;
            man_idle = 0;
        }
        if(rbs.buttons.up)
        {
            if(conty > 1)
                conty--;
        }
        if(rbs.buttons.down)
        {
            if(conty < 1000)
                conty++;
        }
        if(rbs.buttons.a)
        {
//            man_idle = 0;
//            man_subanim = 1;
            bmpsz++;
        }
        if(rbs.buttons.b)
        {
            bmpsz--;
        }

        clamp<int>(contx, 0, screen.getWidth()-1);
        clamp<int>(conty, 0, screen.getHeight()-1);
    }

    if(frame_count == 8)
    {

        frame_count = 0;
        man_frame++;
        man_frame = man_frame % 8;


    }

    //screen.circle(contx, conty, 20, WHITE, WHITE);

    Rect_t srcRect;

    if(man_idle)
    {
        srcRect = {0,16*(man_anim*2 + 1),16,16};
    }
    else
    {
        srcRect = {16*man_frame,16*(man_anim*2 + man_subanim),16,16};
    }

    Point_t destPoint = {contx + bmpsz/2, conty + bmpsz/2};
    Rect_t destRect = {contx, conty, bmpsz, bmpsz};

    screen.circle(&destPoint, bmpsz, WHITE, WHITE);

    screen.bitmap_scaled_nbx(&man_walk_bmp, &srcRect, &destRect, MODE_MASK);

    //screen.bitmap(0,0, hellojustin, 128, 128, MODE_BLEND);

    /*
     * TODO:
     * Your code goes here
     */

    compat_disp_interface->fillFromBitmap(TVOut_Framebuffer);
    compat_disp_interface->flip();


}
