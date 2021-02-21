#include <SDL2/SDL.h>

#include <iostream>

#include "bitmaps/font5x7.h"
#include "control_message.pb.h"
#include "gamelib/compat/SDL_DisplayInterface.h"
#include "gamelib/compat/compat_main.h"
#include "gamelib/display/screen.h"
#include "gamelib/display/tvout.h"
#include "gamelib/input/remote.h"
#include "utils/udp_server.h"

Bitmap_t font5x7_bitmap(font5x7, 256 * 5, 7);
uint8_t mapping(uint8_t char_code) { return char_code; }
Font_t font5x7_font(&font5x7_bitmap, 5, 7, 5, 1, mapping);

SDL_DisplayInterface* compat_disp_interface;

Remote_buttons_t sim_hooked_buttons, sim_hooked_buttons_last;

template <int kNumOptions>
class SelectorWidget {
 public:
  SelectorWidget(Screen* screen, Font_t* font, Font_anchor_pos_t anchor_pos,
                 const char* option_labels[kNumOptions])
      : option_labels_(option_labels),
        screen_(screen),
        font_(font),
        anchor_pos_(anchor_pos) {
    for (int i = 0; i < kNumOptions; ++i) {
      uint16_t max_width = 0, max_height = 0;
      font->textSizePixels(option_labels_[i], &max_width, &max_height);
      if (max_width > max_label_width_) {
        max_label_width_ = max_width;
      }
      if (max_height > max_label_height_) {
        max_label_height_ = max_height;
      }
    }

    max_label_width_ += 4;
    max_label_height_ += 4;

    total_width_ =
        max_label_width_ + (max_label_width_ - 1) * (kNumOptions - 1);

    switch (anchor_pos_) {
      default:
      case FAP_BOTTOM_LEFT: {
        for (int i = 0; i < kNumOptions; ++i) {
          option_anchors_[i] = {
              max_label_width_ / 2 + (max_label_width_ - 1) * i,
              screen_->getHeight() - max_label_height_ / 2 - 1};
        }
      } break;
      case FAP_BOTTOM_RIGHT: {
        for (int i = 0; i < kNumOptions; ++i) {
          option_anchors_[i] = {
              max_label_width_ / 2 + (max_label_width_ - 1) * i +
                  screen_->getWidth() - total_width_ - 4,
              screen_->getHeight() - max_label_height_ / 2 - 1};
        }
      } break;
    }
  }

  void SetOption(int axis) {
    if (axis >= kNumOptions) {
      axis_ = kNumOptions - 1;
      return;
    }

    if (axis < 0) {
      axis_ = 0;
      return;
    }

    axis_ = axis;
  }

  void Render() {
    switch (anchor_pos_) {
      default:
      case FAP_BOTTOM_LEFT: {
        Rect_t total_box = {0, screen_->getHeight() - max_label_height_,
                            total_width_, max_label_height_};
        screen_->box(&total_box, WHITE, NONE);
      } break;
      case FAP_BOTTOM_RIGHT: {
        Rect_t total_box = {screen_->getWidth() - total_width_ - 4,
                            screen_->getHeight() - max_label_height_,
                            total_width_, max_label_height_};
        screen_->box(&total_box, WHITE, NONE);

      } break;
    }

    for (int i = 0; i < kNumOptions; ++i) {
      screen_->textOption(*font_, option_anchors_[i], option_labels_[i],
                          FAP_CENTER, FJ_LEFT, MODE_OVERWRITE_INVERT);
      if (i == axis_) {
        Rect_t highlight_box = {0, 0, max_label_width_, max_label_height_};
        highlight_box.centerOn(option_anchors_[i]);
        screen_->box(&highlight_box, INVERT, INVERT);
      }
    }
  }

 private:
  int max_label_width_ = 0;
  int max_label_height_ = 0;

  int axis_ = 0;

  const char** option_labels_;
  Point_t option_anchors_[kNumOptions] = {};

  Screen* screen_;
  Font_t* font_;
  Font_anchor_pos_t anchor_pos_;
  int total_width_ = 0;
};

class Console {
 public:
  Console(Screen* screen)
      : screen_(screen),
        axis_widget_(screen_, &font5x7_font, FAP_BOTTOM_LEFT, kAxisLabels),
        multiplier_widget_(screen_, &font5x7_font, FAP_BOTTOM_RIGHT,
                           kMultiplierLabels) {}

  void ProcessControl(const Control& control) {
    if (control.has_axis()) {
      axis_widget_.SetOption(static_cast<int>(control.axis()));
      switch (control.axis()) {
        case Control::AXIS_X:
          value_to_adjust_ = &x_;

          break;
        case Control::AXIS_Y:
          value_to_adjust_ = &y_;
          break;
        default:
          value_to_adjust_ = nullptr;
          break;
      }
    }
    if (control.has_multiplier()) {
      multiplier_widget_.SetOption(static_cast<int>(control.multiplier()));
    }
    if (control.has_value() && value_to_adjust_ != nullptr) {
      *value_to_adjust_ = control.value();
    }
  }

  void Render() {
    screen_->setPixel(x_ % screen.getWidth(), y_ % screen.getHeight(), WHITE);
    axis_widget_.Render();
    multiplier_widget_.Render();
  }

 private:
  int x_ = 0, y_ = 0;
  int* value_to_adjust_ = nullptr;
  Screen* screen_ = nullptr;

  const char* kAxisLabels[7] = {"$", "X", "Y", "Z", "4", "5", "6"};
  SelectorWidget<7> axis_widget_;

  const char* kMultiplierLabels[3] = {"X1", "X10", "X100"};
  SelectorWidget<3> multiplier_widget_;
};

int main(int argc, char* argv[]) {
  TVOut_Init();
  SDL_Window* window;
  compat_disp_interface =
      SDL_DisplayInterface::createWindow(FB_WIDTH, FB_HEIGHT, window, 2);
  SDL_SetHintWithPriority(SDL_HINT_RENDER_VSYNC, "1", SDL_HINT_OVERRIDE);

  utils::UdpServer server(5533);
  server.Start();
  std::vector<uint8_t> buffer;
  Console console(&screen);

  while (true) {
    SDL_PumpEvents();

    sim_hooked_buttons.raw &= ~0xFF00;

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
