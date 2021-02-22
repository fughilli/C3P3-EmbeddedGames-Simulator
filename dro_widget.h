#ifndef DRO_WIDGET_H_
#define DRO_WIDGET_H_

#include <stdio.h>

#include "gamelib/display/screen.h"
#include "gamelib/display/widget_util.h"

template <int kNumAxes>
class DroWidget {
 public:
  DroWidget(Screen* screen, Font_t* font, Font_anchor_pos_t anchor_pos,
            const char* axis_labels[kNumAxes])
      : screen_(screen),
        font_(font),
        anchor_pos_(anchor_pos),
        axis_labels_(axis_labels) {
    FontMaxSizeAll(font_, axis_labels_, kNumAxes, &max_label_width_,
                   &max_label_height_);

    max_label_height_ += 4;
    max_label_width_ += 4;

    total_height_ =
        max_label_height_ + (max_label_height_ - 1) * (kNumAxes - 1);

    for (int i = 0; i < kNumAxes; ++i) {
      axis_anchors_[i] = {2, 2 + (max_label_height_ - 1) * i};
    }
  }

  void Render() {
    char text_buffer[128];
    for (int i = 0; i < kNumAxes; ++i) {
      if (axis_positions_[i] == axis_target_positions_[i]) {
        snprintf(text_buffer, sizeof(text_buffer), "%s %+9.3f", axis_labels_[i],
                 axis_positions_[i]);
      } else {
        snprintf(text_buffer, sizeof(text_buffer), "%s %+9.3f (%+9.3f)",
                 axis_labels_[i], axis_positions_[i],
                 axis_target_positions_[i]);
      }
      screen_->textOption(*font_, axis_anchors_[i], text_buffer, FAP_TOP_LEFT,
                          FJ_LEFT, MODE_OVERWRITE_INVERT);
    }
  }

  void SetPosition(int axis, float position) {
    axis_positions_[axis] = position;
  }
  void SetTargetPosition(int axis, float position) {
    axis_target_positions_[axis] = position;
  }

 private:
  Screen* screen_;
  Font_t* font_;
  Font_anchor_pos_t anchor_pos_;
  const char** axis_labels_;

  float axis_positions_[kNumAxes] = {};
  float axis_target_positions_[kNumAxes] = {};

  Point_t axis_anchors_[kNumAxes];

  int max_label_width_ = 0, max_label_height_ = 0;
  int total_height_ = 0;
};

#endif  // DRO_WIDGET_H_
