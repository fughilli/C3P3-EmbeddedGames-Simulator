#ifndef CONSOLE_H_
#define CONSOLE_H_

#include "control_message.pb.h"
#include "dro_widget.h"
#include "gamelib/display/screen.h"
#include "gamelib/display/selector_widget.h"

class Console {
 public:
  Console(Screen* screen, Font_t* font)
      : screen_(screen),
        font_(font),
        axis_widget_(screen_, font_, FAP_BOTTOM_LEFT, kAxisLabels),
        multiplier_widget_(screen_, font_, FAP_BOTTOM_RIGHT, kMultiplierLabels),
        dro_widget_(screen_, font_, FAP_TOP_LEFT, kDroLabels) {}

  void ProcessControl(const Control& control);
  void Render();

 private:
  int x_ = 0, y_ = 0;
  int* value_to_adjust_ = nullptr;
  Screen* screen_ = nullptr;
  Font_t* font_ = nullptr;

  const char* kAxisLabels[7] = {"~", "X", "Y", "Z", "4", "5", "6"};
  SelectorWidget<7> axis_widget_;

  const char* kMultiplierLabels[3] = {"X1", "X10", "X100"};
  SelectorWidget<3> multiplier_widget_;

  const char* kDroLabels[4] = {"X", "Y", "Z", "S"};
  DroWidget<4> dro_widget_;
};

#endif  // CONSOLE_H_
