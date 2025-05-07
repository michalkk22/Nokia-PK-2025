#include "SendingCallState.hpp"

namespace ue {
SendingCallState::SendingCallState(Context &context)
    : StartDialState(context, "SendingCallState") {

  logger.logInfo("Entered SendingCall state.");
  context.timer.startTimer(std::chrono::seconds{60});
  context.user.displayAlert("Calling", "Waiting for response...");
}

void SendingCallState::handleTimeout() {
  logger.logInfo("Call attempt timed out!");
  context.timer.stopTimer();
  // TODO: drop call
  context.setState<ConnectedState>();
  context.user.displayAlert("", "Call attempt timed out");
}

void SendingCallState::handleUiBack() {
  logger.logInfo(
      "User pressed back - droping call and returning to main menu.");
  // TODO: drop call
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
}