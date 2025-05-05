#include "ConnectedState.hpp"
#include "ComposeSmsState.hpp"
#include "NotConnectedState.hpp"
#include "StartDialState.hpp"
#include "ViewSmsListState.hpp"
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

  context.user.showNewSms();
}

void ConnectedState::handleUiAction(std::optional<std::size_t> selectedIndex) {
  if (!selectedIndex.has_value()) {
    logger.logInfo("No selection provided from UI in Main Menu");
    return;
  }

  std::size_t index = selectedIndex.value();
  logger.logInfo("User selected main menu option with index: ", index);

  switch (index) {
  case 0:
    logger.logInfo("Compose SMS selected");
    context.setState<ComposeSmsState>();
    break;

  case 1:
    logger.logInfo("View SMS selected");
    context.setState<ViewSmsListState>();
    break;

  default:
    logger.logError("Invalid menu option selected: ", index);
    break;
  }
}

void ConnectedState::handleUiBack() {
  logger.logInfo("Back button pressed in main menu – no action taken");
}

void ConnectedState::handleUiAccept() {
  logger.logInfo("Accept button pressed");
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

} // namespace ue