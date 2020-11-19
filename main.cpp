#include <SDL2/SDL.h>
#include "gamelib/compat/SDL_DisplayInterface.h"
#include "gamelib/display/screen.h"
#include "bitmaps/hellojustin.h"
#include "gamelib/input/remote.h"
#include "bitmaps/littleman_fixed.h"
#include "gamelib/compat/compat_main.h"
#include "gamelib/display/tvout.h"
#include "game/game.h"

SDL_DisplayInterface* compat_disp_interface;

Remote_buttons_t sim_hooked_buttons, sim_hooked_buttons_last;

int main(int argc, char* argv[])
{
    TVOut_Init();
    SDL_Window* window;
    compat_disp_interface = SDL_DisplayInterface::createWindow(FB_WIDTH, FB_HEIGHT, window, 2);

    SDL_SetHintWithPriority(SDL_HINT_RENDER_VSYNC, "1", SDL_HINT_OVERRIDE);

    SDL_Event gameEvent;

    screen.clear(BLACK);

    Game_init();

    while(true)
    {
        SDL_PumpEvents();

        sim_hooked_buttons.raw &= ~0xFF00;

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
                    sim_hooked_buttons.buttons.p_up = sim_hooked_buttons.buttons.up = buttonflag;
                    break;
                case SDL_SCANCODE_A:
                    sim_hooked_buttons.buttons.p_left = sim_hooked_buttons.buttons.left = buttonflag;
                    break;
                case SDL_SCANCODE_S:
                    sim_hooked_buttons.buttons.p_down = sim_hooked_buttons.buttons.down = buttonflag;
                    break;
                case SDL_SCANCODE_D:
                    sim_hooked_buttons.buttons.p_right = sim_hooked_buttons.buttons.right = buttonflag;
                    break;
                case SDL_SCANCODE_H:
                    sim_hooked_buttons.buttons.p_a = sim_hooked_buttons.buttons.a = buttonflag;
                    break;
                case SDL_SCANCODE_U:
                    sim_hooked_buttons.buttons.p_b = sim_hooked_buttons.buttons.b = buttonflag;
                    break;
                default:
                    break;
                }
            }
        }

        static int qua = 0;
        if (++qua == 5) {
            qua = 0;
            screen.clear(BLACK);
            Game_loop();
        }

        compat_disp_interface->fillFromBitmap(TVOut_Framebuffer);
        compat_disp_interface->flip();

        //compat_disp_interface->fillFromBitmap(TVOut_Framebuffer);
        //compat_disp_interface->flip();

    }

    delete compat_disp_interface;

    SDL_Quit();


    return 0;
}
