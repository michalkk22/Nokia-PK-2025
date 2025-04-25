#include "ConnectingState.hpp"
#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"

namespace ue {

ConnectingState::ConnectingState(Context &context, common::BtsId btsId)
    : BaseState(context, "ConnectingState"), btsId(btsId) {
  logger.logInfo("Attempting to attach to BTS with ID: ", btsId);
  context.bts.sendAttachRequest(btsId);
  context.timer.startTimer(std::chrono::milliseconds{500});
}

void ConnectingState::handleTimeout() {
  logger.logError("Attachment attempt timed out!");
  context.timer.stopTimer();
  context.setState<NotConnectedState>();
}

void ConnectingState::handleAttachAccept() {
  logger.logInfo("Successfully attached to BTS.");
  context.timer.stopTimer();
  context.setState<ConnectedState>();
}

void ConnectingState::handleAttachReject() {
  logger.logError("Attachment request rejected by BTS.");
  context.timer.stopTimer();
  context.setState<NotConnectedState>();
}

void ConnectingState::handleDisconnected() {
  logger.logError("Connection lost during attachment process.");
  context.timer.stopTimer();
  context.setState<NotConnectedState>();
}

} // namespace ue