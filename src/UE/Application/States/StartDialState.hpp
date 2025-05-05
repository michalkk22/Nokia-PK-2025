#pragma once

#include "BaseState.hpp"

namespace ue {

class StartDialState : public BaseState {
public:
  StartDialState(Context &context);
  void handleUiBack() override;
  void handleUiAccept() override;

private:
  void startDial();
};

} // namespace ue