#include "ConnectedState.hpp"
#include "ComposeSmsState.hpp"
#include "NotConnectedState.hpp"
#include "ReceivingCallState.hpp"
#include "SmsMenuState.hpp"
#include "StartDialState.hpp"
#include <vector>

namespace ue {
ConnectedState::ConnectedState(Context &context)
    : BaseState(context, "ConnectedState") {
  showMainMenu();
}

void ConnectedState::showMainMenu() {
  logger.logInfo("Entered Connected state.");
  context.user.showConnected();
}

void ConnectedState::handleDisconnected() {
  logger.logInfo("Lost connection to BTS.");
  context.setState<NotConnectedState>();
}

void ConnectedState::handleSmsReceived(common::PhoneNumber fromNumber,
                                       std::string message) {
  logger.logInfo("Received SMS from: ", fromNumber, " with content: ", message);

  std::size_t smsIndex = context.smsDb.addReceivedSms(fromNumber, message);
  logger.logDebug("Stored SMS at index: ", smsIndex);

  context.user.showNewSms(true);
}

void ConnectedState::handleUiAction() {
  logger.logInfo("Mail button pressed.");
  context.setState<SmsMenuState>();
}

void ConnectedState::handleUiBack() {
  logger.logInfo("Back button pressed.");
  context.setState<ConnectedState>();
}

void ConnectedState::handleUiAccept() {
  logger.logInfo("Accept button pressed.");
  context.setState<StartDialState>();
}

void ConnectedState::handleSmsSentResult(common::PhoneNumber to, bool success) {
  logger.logInfo("Received SMS send result for ", to,
                 " while in main menu. Success: ", success);
  if (!success) {
    if (!context.smsDb.markLastOutgoingSmsAsFailed()) {
      logger.logInfo("Could not mark last outgoing SMS as failed.");
    }
    context.user.displayAlert("SMS Failed",
                              "Could not send SMS to " + common::to_string(to));
  }
}

void ConnectedState::handleCallUnknownRecipient(common::PhoneNumber to) {
  context.user.displayAlert("Call Failed",
                            "Could not call " + common::to_string(to));
}

void ConnectedState::handleCallReceived(common::PhoneNumber fromNumber) {
  logger.logInfo("Call received from: ", fromNumber);
  context.setState<ReceivingCallState>(fromNumber);
  context.user.displayAlert("Incoming Call",
                            "from " + common::to_string(fromNumber));
}

void ConnectedState::dropAnotherCall(common::PhoneNumber fromNumber) {
  logger.logInfo("Dropping another call received from: ", fromNumber);
  context.bts.sendCallDropped(fromNumber);
}

} // namespace ue