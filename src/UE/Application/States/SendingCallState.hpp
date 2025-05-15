#pragma once

#include "StartDialState.hpp"

namespace ue {

class SendingCallState : public StartDialState {
public:
  SendingCallState(Context &context, common::PhoneNumber recipient);
  void handleUiBack() override;
  void handleUiAccept() override;
  void handleTimeout() override;
  void handleCallUnknownRecipient(common::PhoneNumber to) override;
  void handleCallAccepted() override;
  void handleCallDropped() override;
  void handleCallReceived(common::PhoneNumber fromNumber) override;

private:
  common::PhoneNumber recipient;
};

} // namespace ue