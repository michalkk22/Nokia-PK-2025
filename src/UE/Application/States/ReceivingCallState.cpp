#include "ReceivingCallState.hpp"

#include "TalkingState.hpp"

namespace ue {

ReceivingCallState::ReceivingCallState(Context &context,
                                       common::PhoneNumber fromNumber)
    : ConnectedState(context, "ReceivingCallState"), fromNumber(fromNumber) {}

void ReceivingCallState::handleUiAction() {}

void ReceivingCallState::handleUiAccept() {
  logger.logInfo(
      "User pressed accept in ReceivingCall state - trying to accept call.");
  context.bts.sendCallAccepted(fromNumber);
  context.setState<TalkingState>(fromNumber);
}

void ReceivingCallState::handleUiBack() {
  logger.logInfo("User rejected call - returning to main menu.");
  context.bts.sendCallDropped(fromNumber);
  context.setState<ConnectedState>();
}

void ReceivingCallState::handleCallDropped() {
  logger.logInfo("Call dropped by sender - returning to main menu.");
  context.setState<ConnectedState>();
  context.user.displayAlert("", "Call dropped");
}

void ReceivingCallState::handleCallReceived(common::PhoneNumber number) {
  dropAnotherCall(number);
}

} // namespace ue