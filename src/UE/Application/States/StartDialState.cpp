#include "StartDialState.hpp"
#include "ConnectedState.hpp"

namespace ue {
StartDialState::StartDialState(Context &context)
    : ConnectedState(context, "StartDialState") {
  startDial();
}

void StartDialState::startDial() {
  logger.logInfo("Entered StartDial state.");
  context.user.startDial();
}

void StartDialState::handleUiAction(std::optional<std::size_t> selectedIndex) {
  logger.logInfo(
      "Message button pressed in StartDial state - no action taken.");
}

void StartDialState::handleUiBack() {
  logger.logInfo(
      "User pressed back in StartDial state, returning to main menu.");
  context.setState<ConnectedState>();
}

void StartDialState::handleUiAccept() {
  logger.logInfo(
      "User pressed accept in StartDial state, trying to send call request.");

  auto recipient = context.user.getDialRecipient();

  if (!recipient.isValid()) {
    logger.logInfo("Call not sent - recepient is invalid");
    context.user.displayAlert("Error", "Invalid recipient");
    return;
  }

  logger.logInfo("Sending call request to ", common::to_string(recipient));
}

// TODO: receiving call request cancels sending call request

} // namespace ue