#pragma once

#include "ConnectedState.hpp"

namespace ue {

class StartDialState : public ConnectedState {
public:
  StartDialState(Context &context);
  void handleUiBack() override;
  void handleUiAction(std::optional<std::size_t> selectedIndex) override;
  void handleUiAccept() override;

private:
  void startDial();
};

} // namespace ue