#include "StartDialState.hpp"
#include "ConnectedState.hpp"

namespace ue {
StartDialState::StartDialState(Context &context)
    : BaseState(context, "StartDialState") {
  startDial();
}

void StartDialState::startDial() {
  logger.logInfo("Entered StartDial state.");
  context.user.startDial();
}

void StartDialState::handleUiBack() {
  logger.logInfo(
      "User pressed back in StartDial state, returning to main menu.");
  context.setState<ConnectedState>();
}

void StartDialState::handleUiAccept() {
  logger.logInfo(
      "User pressed accept in StartDial state, trying to send call request.");
  // TODO: call request
}

} // namespace ue