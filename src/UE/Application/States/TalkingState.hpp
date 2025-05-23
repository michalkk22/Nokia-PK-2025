#pragma once

#include "ConnectedState.hpp"

namespace ue {
class TalkingState : public ConnectedState {
public:
  TalkingState(Context &context, common::PhoneNumber recipient);

  void handleUiBack() override;
  void handleCallDropped() override;
  void handleTimeout() override;
  void handleCallReceived(common::PhoneNumber fromNumber) override;
  void handleUiAccept() override;
  void handleUiAction() override;
  void handleCallTalkReceived(common::PhoneNumber fromNumber,
                              std::string text) override;

private:
  common::PhoneNumber recipient;
  const ITimerPort::Duration timeoutDuration =
      std::chrono::seconds{30}; // TODO: 30s

  std::string prepareDisplayText(const std::string &prefix,
                                 const std::string &text);
};
}