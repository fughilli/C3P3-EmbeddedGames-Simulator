#include "console.h"

void Console::ProcessControl(const Control& control) {
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
  if (control.has_estop()) {
    modal_widget_.Set(0, control.estop());
  }
  if (control.has_feedhold()) {
    modal_widget_.Set(1, control.feedhold());
  }
}

void Console::Render() {
  axis_widget_.Render();
  multiplier_widget_.Render();
  modal_widget_.Render();
  dro_widget_.Render();
}
