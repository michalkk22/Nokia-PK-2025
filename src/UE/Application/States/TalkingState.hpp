#pragma once

#include "ConnectedState.hpp"

namespace ue {
class TalkingState : public ConnectedState {
public:
  TalkingState(Context &context, common::PhoneNumber recipient);

  void handleUiBack() override;
  void handleCallDropped() override;
  void handleCallReceived(common::PhoneNumber fromNumber) override;

private:
  common::PhoneNumber recipient;
};
}