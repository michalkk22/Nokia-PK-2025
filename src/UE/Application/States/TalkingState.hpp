#pragma once

#include "ConnectedState.hpp"

namespace ue {
class TalkingState : public ConnectedState {
public:
  TalkingState(Context &context, common::PhoneNumber recipient);

  void handleUiBack() override;

private:
  common::PhoneNumber recipient;
};
}