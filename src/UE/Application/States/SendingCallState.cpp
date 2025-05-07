#include "SendingCallState.hpp"

namespace ue {
SendingCallState::SendingCallState(Context &context)
    : StartDialState(context, "SendingCallState") {
  dialing();
}

SendingCallState::SendingCallState(Context &context, const std::string &name)
    : StartDialState(context, name) {}

void SendingCallState::dialing() {
  logger.logInfo("Entered SendingCallState state.");
  // TODO: implement dialing
}

void SendingCallState::handleUiBack() {
  logger.logInfo(
      "User pressed back - droping call and returning to main menu.");
  // TODO: drop call
  context.setState<ConnectedState>();
}

void SendingCallState::handleUiAccept() {
  logger.logInfo("User pressed accept - no action taken.");
}
}