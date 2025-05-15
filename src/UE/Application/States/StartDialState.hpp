#pragma once

#include "ConnectedState.hpp"

namespace ue {

class StartDialState : public ConnectedState {
public:
  StartDialState(Context &context);
  StartDialState(Context &context, const std::string &name);
  void handleUiAction() override;
  void handleUiAccept() override;

private:
  void startDial();
};

} // namespace ue