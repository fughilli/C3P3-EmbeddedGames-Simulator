#ifndef GAMELIB_DISPLAY_MODAL_WIDGET_H_
#define GAMELIB_DISPLAY_MODAL_WIDGET_H_

#include "gamelib/display/screen.h"
#include "gamelib/display/widget_util.h"

template <int kNumOptions>
class ModalWidget {
 public:
  ModalWidget(Screen* screen, Font_t* font, Font_anchor_pos_t anchor_pos,
              const char* mode_labels[kNumOptions])
      : mode_labels_(mode_labels),
        screen_(screen),
        font_(font),
        anchor_pos_(anchor_pos) {
    FontMaxSizeAll(font_, mode_labels_, kNumOptions, &max_label_width_,
                   &max_label_height_);

    max_label_width_ += 4;
    max_label_height_ += 4;

    total_width_ =
        max_label_width_ + (max_label_width_ - 1) * (kNumOptions - 1);

    switch (anchor_pos_) {
      default:
      case FAP_BOTTOM_LEFT: {
        for (int i = 0; i < kNumOptions; ++i) {
          mode_anchors_[i] = {max_label_width_ / 2 + (max_label_width_ - 1) * i,
                              screen_->getHeight() - max_label_height_ / 2 - 1};
        }
      } break;
      case FAP_BOTTOM_CENTER: {
        for (int i = 0; i < kNumOptions; ++i) {
          mode_anchors_[i] = {max_label_width_ / 2 +
                                  (max_label_width_ - 1) * i +
                                  (screen_->getWidth() - total_width_) / 2,
                              screen_->getHeight() - max_label_height_ / 2 - 1};
        }
      } break;
      case FAP_BOTTOM_RIGHT: {
        for (int i = 0; i < kNumOptions; ++i) {
          mode_anchors_[i] = {max_label_width_ / 2 +
                                  (max_label_width_ - 1) * i +
                                  screen_->getWidth() - total_width_,
                              screen_->getHeight() - max_label_height_ / 2 - 1};
        }
      } break;
    }
  }

  void Set(int mode, bool value) {
    if (mode < 0 || mode >= kNumOptions) return;
    modes_[mode] = value;
  }

  void Render() {
    switch (anchor_pos_) {
      default:
      case FAP_BOTTOM_LEFT: {
        Rect_t total_box = {0, screen_->getHeight() - max_label_height_,
                            total_width_, max_label_height_};
        screen_->box(&total_box, WHITE, NONE);
      } break;
      case FAP_BOTTOM_CENTER: {
        Rect_t total_box = {(screen_->getWidth() - total_width_) / 2,
                            screen_->getHeight() - max_label_height_,
                            total_width_, max_label_height_};
        screen_->box(&total_box, WHITE, NONE);
      } break;
      case FAP_BOTTOM_RIGHT: {
        Rect_t total_box = {screen_->getWidth() - total_width_,
                            screen_->getHeight() - max_label_height_,
                            total_width_, max_label_height_};
        screen_->box(&total_box, WHITE, NONE);
      } break;
    }

    for (int i = 0; i < kNumOptions; ++i) {
      screen_->textOption(*font_, mode_anchors_[i], mode_labels_[i], FAP_CENTER,
                          FJ_LEFT, MODE_OVERWRITE_INVERT);
      if (modes_[i]) {
        Rect_t highlight_box = {0, 0, max_label_width_, max_label_height_};
        highlight_box.centerOn(mode_anchors_[i]);
        screen_->box(&highlight_box, INVERT, INVERT);
      }
    }
  }

 private:
  int max_label_width_ = 0;
  int max_label_height_ = 0;

  const char** mode_labels_;
  Point_t mode_anchors_[kNumOptions] = {};
  std::array<bool, kNumOptions> modes_ = {};

  Screen* screen_;
  Font_t* font_;
  Font_anchor_pos_t anchor_pos_;
  int total_width_ = 0;
};

#endif  // GAMELIB_DISPLAY_MODAL_WIDGET_H_
