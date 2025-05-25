#include "BaseState.hpp"
#include "NotConnectedState.hpp"

namespace ue {

BaseState::BaseState(Context &context, const std::string &name)
    : context(context), logger(context.logger, "[" + name + "]") {
  logger.logDebug("entry");
}

BaseState::~BaseState() { logger.logDebug("exit"); }

void BaseState::handleTimeout() { logger.logError("Uexpected: handleTimeout"); }

void BaseState::handleSib(common::BtsId btsId) {
  logger.logError("Uexpected: handleSib: ", btsId);
}

void BaseState::handleAttachAccept() {
  logger.logError("Uexpected: handleAttachAccept");
}

void BaseState::handleAttachReject() {
  logger.logError("Uexpected: handleAttachReject");
}

void BaseState::handleDisconnected() {
  logger.logError("Uexpected: handleDisconnected");
}

void BaseState::handleSmsReceived(common::PhoneNumber fromNumber,
                                  std::string message) {
  logger.logError("Unexpected: handleSmsReceived");
}

void BaseState::handleSmsSentResult(common::PhoneNumber to, bool success) {
  logger.logError("Unexpected: handleSmsSentResult for: ", to,
                  ", Success: ", success);
}

void BaseState::handleUiAction() {
  logger.logError("Unexpected: handleUiAccept");
}

void BaseState::handleUiBack() { logger.logError("Unexpected: handleUiBack"); }

void BaseState::handleUiAccept() {
  logger.logError("Unexpected: handleUiAccept");
}

void BaseState::handleCallUnknownRecipient(common::PhoneNumber to) {
  logger.logError("Unexpected: handleCallUnknownRecipient for: ", to);
}

void BaseState::handleCallAccepted() {
  logger.logError("Unexpected: handleCallAccepted");
}

void BaseState::handleCallDropped() {
  logger.logError("Unexpected: handleCallDropped");
}

void BaseState::handleCallReceived(common::PhoneNumber fromNumber) {
  logger.logError("Unexpected: handleCallReceived from: ", fromNumber);
}

void BaseState::handleItemSelected(std::optional<std::size_t> index) {
  logger.logError("Unexpected: handleItemSelected");
}

void BaseState::handleCallTalkReceived(common::PhoneNumber fromNumber,
                                       const std::string &text) {
  logger.logError("Unexpected: handleCallTalkReceived");
}

void BaseState::handleShutdown() {}

} // namespace ue
