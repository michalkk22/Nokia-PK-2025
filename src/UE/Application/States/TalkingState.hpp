#pragma once

#include "ConnectedState.hpp"

namespace ue {
class TalkingState : public ConnectedState {
public:
  TalkingState(Context &context, common::PhoneNumber recipient);

  void handleShutdown() override;
  void handleUiBack() override;
  void handleCallDropped() override;
  void handleTimeout() override;
  void handleCallReceived(common::PhoneNumber fromNumber) override;
  void handleCallUnknownRecipient(common::PhoneNumber fromNumber) override;
  void handleUiAccept() override;
  void handleUiAction() override;
  void handleCallTalkReceived(common::PhoneNumber fromNumber,
                              const std::string &text) override;

private:
  common::PhoneNumber recipient;
  const ITimerPort::Duration timeoutDuration = std::chrono::seconds{30};
  std::string prepareDisplayText(const std::string &prefix,
                                 const std::string &text);
};
}