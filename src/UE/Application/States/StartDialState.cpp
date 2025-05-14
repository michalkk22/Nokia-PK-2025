#include "StartDialState.hpp"
#include "ConnectedState.hpp"
#include "SendingCallState.hpp"

namespace ue {
StartDialState::StartDialState(Context &context)
    : ConnectedState(context, "StartDialState") {
  startDial();
}

StartDialState::StartDialState(Context &context, const std::string &name)
    : ConnectedState(context, name) {}

void StartDialState::startDial() {
  logger.logInfo("Entered StartDial state.");
  context.user.startDial();
}

void StartDialState::handleUiAction(std::optional<std::size_t> selectedIndex) {
  logger.logInfo("User pressed message - no action taken.");
}

void StartDialState::handleUiAccept() {
  logger.logInfo(
      "User pressed accept in StartDial state - trying to send call request.");

  auto recipient = context.user.getDialRecipient();

  if (!recipient.isValid()) {
    logger.logInfo("Call not sent - recepient is invalid");
    context.user.displayAlert("Error", "Invalid recipient");
    return;
  }

  context.bts.sendCallRequest(recipient);
  logger.logInfo("Call request sent to: ", recipient);
  context.setState<SendingCallState>(recipient);
}
} // namespace ue