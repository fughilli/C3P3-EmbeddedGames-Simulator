#include <SDL2/SDL.h>

#include <iostream>

#include "bitmaps/font5x7.h"
#include "console.h"
#include "control_message.pb.h"
#include "gamelib/compat/SDL_DisplayInterface.h"
#include "gamelib/compat/compat_main.h"
#include "gamelib/display/screen.h"
#include "gamelib/display/tvout.h"
#include "google/protobuf/util/message_differencer.h"
#include "utils/udp_client.h"

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

  utils::UdpClient client("127.0.0.1", 5533);
  if (!client.Start()) {
    std::cout << "Failed to start client" << std::endl;
  }
  std::vector<uint8_t> buffer;
  Console console(&screen, &font5x7_font);

  Control old_control, control;
  control.set_value(0);
  control.set_axis(Control::AXIS_NONE);
  control.set_multiplier(Control::MULT_X1);

  bool estopped = false;
  bool feedholded = false;

  while (true) {
    SDL_PumpEvents();

    SDL_Event gameEvent;
    if (SDL_PollEvent(&gameEvent)) {
      if (gameEvent.type == SDL_QUIT ||
          (gameEvent.type == SDL_KEYDOWN &&
           gameEvent.key.keysym.scancode == SDL_SCANCODE_ESCAPE))
        break;

      if (gameEvent.type == SDL_KEYDOWN) {
        switch (gameEvent.key.keysym.scancode) {
          case SDL_SCANCODE_Q:
            if (control.axis() > Control::Axis_MIN)
              control.set_axis(static_cast<Control::Axis>(
                  static_cast<int>(control.axis()) - 1));
            break;
          case SDL_SCANCODE_W:
            if (control.axis() < Control::Axis_MAX)
              control.set_axis(static_cast<Control::Axis>(
                  static_cast<int>(control.axis()) + 1));
            break;
          case SDL_SCANCODE_E:
            if (control.multiplier() > Control::Multiplier_MIN)
              control.set_multiplier(static_cast<Control::Multiplier>(
                  static_cast<int>(control.multiplier()) - 1));
            break;
          case SDL_SCANCODE_R:
            if (control.multiplier() < Control::Multiplier_MAX)
              control.set_multiplier(static_cast<Control::Multiplier>(
                  static_cast<int>(control.multiplier()) + 1));
            break;
          case SDL_SCANCODE_A:
            control.set_value(control.value() - 100);
            break;
          case SDL_SCANCODE_S:
            control.set_value(control.value() + 100);
            break;
          case SDL_SCANCODE_T:
            estopped = !estopped;
            break;
          case SDL_SCANCODE_Y:
            feedholded = !feedholded;
            break;
          default:
            break;
        }
      }
      if (gameEvent.type == SDL_KEYUP) {
        switch (gameEvent.key.keysym.scancode) {
          case SDL_SCANCODE_Q:
            break;
          case SDL_SCANCODE_W:
            break;
          case SDL_SCANCODE_E:
            break;
          case SDL_SCANCODE_R:
            break;
          case SDL_SCANCODE_T:
            break;
          case SDL_SCANCODE_Y:
            break;
          default:
            break;
        }
      }

      if (estopped) {
        control.set_estop(true);
      } else {
        if (control.estop()) {
          control.set_estop(false);
        } else {
          control.clear_estop();
        }
      }
      if (feedholded) {
        control.set_feedhold(true);
      } else {
        if (control.feedhold()) {
          control.set_feedhold(false);
        } else {
          control.clear_feedhold();
        }
      }

      if (!google::protobuf::util::MessageDifferencer::Equals(control,
                                                              old_control)) {
        console.ProcessControl(control);
        std::string proto_string;
        control.SerializeToString(&proto_string);
        if (!client.Write(std::vector<uint8_t>(proto_string.begin(),
                                               proto_string.end()))) {
          std::cout << "Failed to write to server" << std::endl;
        } else {
          std::cout << "Wrote: " << control.ShortDebugString() << std::endl;
        }
        old_control = control;
      }
    }
    screen.clear(BLACK);

    if (client.Read(&buffer)) {
      DroState dro_state;
      dro_state.ParseFromString(std::string(buffer.begin(), buffer.end()));
      std::cout << dro_state.ShortDebugString() << std::endl;
      console.ProcessDroState(dro_state);
    }

    console.Render();

    compat_disp_interface->fillFromBitmap(TVOut_Framebuffer);
    compat_disp_interface->flip();
  }

  delete compat_disp_interface;

  SDL_Quit();

  return 0;
}
