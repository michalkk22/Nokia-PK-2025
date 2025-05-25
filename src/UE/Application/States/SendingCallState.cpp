#include "SendingCallState.hpp"
#include "TalkingState.hpp"

namespace ue {
SendingCallState::SendingCallState(Context &context,
                                   common::PhoneNumber recipient)
    : StartDialState(context, "SendingCallState"), recipient(recipient) {

  context.bts.sendCallRequest(recipient);
  logger.logInfo("Entered SendingCall state.");
  context.timer.startTimer(std::chrono::seconds{60});
  context.user.displayAlert("Calling", "Waiting for response...");
}

void SendingCallState::handleShutdown() {
  logger.logInfo("Application shutdown in SendingCall state - dropping call.");
  context.bts.sendCallDropped(recipient);
}

void SendingCallState::handleTimeout() {
  logger.logInfo("Call attempt timed out!");
  context.bts.sendCallDropped(recipient);
  context.timer.stopTimer();
  context.setState<ConnectedState>();
  context.user.displayAlert("", "Call attempt timed out");
}

void SendingCallState::handleUiBack() {
  logger.logInfo(
      "User pressed back - droping call and returning to main menu.");
  context.bts.sendCallDropped(recipient);
  context.timer.stopTimer();
  context.setState<ConnectedState>();
}

void SendingCallState::handleCallUnknownRecipient(common::PhoneNumber to) {
  logger.logError("Failed to send call request - recipient not found: ", to);
  context.timer.stopTimer();
  context.setState<ConnectedState>();
  context.user.displayAlert("", "Recipient not found");
}

void SendingCallState::handleUiAccept() {
  logger.logInfo("User pressed accept - no action taken.");
}

void SendingCallState::handleCallAccepted() {
  logger.logInfo("Call accepted - switching to talking state.");
  context.timer.stopTimer();
  context.setState<TalkingState>(recipient);
}

void SendingCallState::handleCallDropped() {
  logger.logInfo("Call dropped - returning to main menu.");
  context.timer.stopTimer();
  context.setState<ConnectedState>();
  context.user.displayAlert("", "Call dropped");
}

void SendingCallState::handleCallReceived(common::PhoneNumber fromNumber) {
  dropAnotherCall(fromNumber);
}
} // namespace ue