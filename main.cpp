#include <SDL2/SDL.h>

#include <iostream>

#include "bitmaps/font5x7.h"
#include "control_message.pb.h"
#include "gamelib/compat/SDL_DisplayInterface.h"
#include "gamelib/compat/compat_main.h"
#include "gamelib/display/screen.h"
#include "gamelib/display/tvout.h"
#include "utils/udp_server.h"
#include "console.h"

Bitmap_t font5x7_bitmap(font5x7, 256 * 5, 7);
uint8_t mapping(uint8_t char_code) { return char_code; }
Font_t font5x7_font(&font5x7_bitmap, 5, 7, 5, 1, mapping);

SDL_DisplayInterface* compat_disp_interface;

int main(int argc, char* argv[]) {
  TVOut_Init();
  SDL_Window* window;
  compat_disp_interface =
      SDL_DisplayInterface::createWindow(FB_WIDTH, FB_HEIGHT, window, 2);
  SDL_SetHintWithPriority(SDL_HINT_RENDER_VSYNC, "1", SDL_HINT_OVERRIDE);

  utils::UdpServer server(5533);
  server.Start();
  std::vector<uint8_t> buffer;
  Console console(&screen, &font5x7_font);

  while (true) {
    SDL_PumpEvents();

    SDL_Event gameEvent;
    if (SDL_PollEvent(&gameEvent)) {
      if (gameEvent.type == SDL_QUIT ||
          (gameEvent.type == SDL_KEYDOWN &&
           gameEvent.key.keysym.scancode == SDL_SCANCODE_ESCAPE))
        break;

      if (gameEvent.type == SDL_KEYUP) {
        switch (gameEvent.key.keysym.scancode) {
          case SDL_SCANCODE_W:
            break;
          case SDL_SCANCODE_A:
            break;
          case SDL_SCANCODE_S:
            break;
          case SDL_SCANCODE_D:
            break;
          case SDL_SCANCODE_H:
            break;
          case SDL_SCANCODE_U:
            break;
          default:
            break;
        }
      }
    }
    screen.clear(BLACK);

    if (server.Read(&buffer)) {
      Control control;
      control.ParseFromString(std::string(buffer.begin(), buffer.end()));
      std::cout << control.ShortDebugString() << std::endl;
      console.ProcessControl(control);
    }

    console.Render();

    compat_disp_interface->fillFromBitmap(TVOut_Framebuffer);
    compat_disp_interface->flip();
  }

  delete compat_disp_interface;

  SDL_Quit();

  return 0;
}
