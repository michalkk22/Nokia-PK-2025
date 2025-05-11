#pragma once

#include "StartDialState.hpp"

namespace ue {

class SendingCallState : public StartDialState {
public:
  SendingCallState(Context &context);
  void handleUiBack() override;
  void handleUiAccept() override;
  void handleTimeout() override;
  void handleCallUnknownRecipient(common::PhoneNumber to) override;
  void handleCallAccepted() override;
  void handleCallDropped() override;

private:
};

} // namespace ue