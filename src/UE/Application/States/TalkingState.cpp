#include "TalkingState.hpp"
#include <string>
#include <vector>

namespace ue {
TalkingState::TalkingState(Context &context, common::PhoneNumber recipient)
    : ConnectedState(context, "TalkingState"), recipient(recipient) {
  logger.logInfo("Entered Talking state.");
  context.user.startTalking();
  context.timer.startTimer(timeoutDuration);
};

void TalkingState::handleUiBack() {
  logger.logInfo("User dropped call - returning to main menu.");
  context.bts.sendCallDropped(recipient);
  context.timer.stopTimer();
  context.setState<ConnectedState>();
  context.user.displayAlert("Call dropped", "");
}

void TalkingState::handleCallDropped() {
  logger.logInfo("Call dropped - returning to main menu.");
  context.timer.stopTimer();
  context.setState<ConnectedState>();
  context.user.displayAlert("Call dropped", "");
}

void TalkingState::handleTimeout() {
  logger.logInfo("Timeout - dropping call.");
  context.bts.sendCallDropped(recipient);
  context.setState<ConnectedState>();
  context.user.displayAlert("Dropped call", "Inactivity timeout");
}

void TalkingState::handleCallReceived(common::PhoneNumber fromNumber) {
  dropAnotherCall(fromNumber);
}

void TalkingState::handleUiAccept() {}

void TalkingState::handleUiAction() {
  auto text = context.user.getCallText();
  if (text.empty()) {
    return;
  }
  context.timer.stopTimer();
  context.user.clearCallText();
  context.bts.sendCallTalk(recipient, text);

  context.user.addCallText(prepareDisplayText("me", text));
  context.timer.startTimer(timeoutDuration);
}

void TalkingState::handleCallTalkReceived(common::PhoneNumber fromNumber,
                                          std::string text) {
  if (fromNumber != recipient) {
    logger.logError("Received call talk from another number: ", fromNumber);
    return;
  }
  context.timer.stopTimer();

  logger.logDebug("Received call talk from: ", fromNumber);

  text.erase(0, 1); // remove the first character from incoming message

  context.user.addCallText(
      prepareDisplayText(std::to_string(fromNumber.value), text));
  context.timer.startTimer(timeoutDuration);
}

std::string TalkingState::prepareDisplayText(const std::string &prefix,
                                             const std::string &text) {
  auto displayText = text;
  displayText = prefix + ": " + displayText;
  return displayText;
}

} // namespace ue