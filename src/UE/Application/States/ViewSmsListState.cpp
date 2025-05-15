#include "ViewSmsListState.hpp"
#include "SmsMenuState.hpp"
#include "ViewSmsState.hpp"

namespace ue {
ViewSmsListState::ViewSmsListState(Context &context)
    : ConnectedState(context, "ViewSmsListState") {
  showList();
}

void ViewSmsListState::showList() {
  currentSmsList = context.smsDb.getAllSms();
  if (currentSmsList.empty()) {
    logger.logInfo("No SMS messages found in inbox.");
    context.user.displayAlert("SMS Inbox", "No messages.");
  } else {
    logger.logDebug("Displaying SMS list with updated statuses:");
    for (size_t i = 0; i < currentSmsList.size(); ++i) {
      const auto &msg = currentSmsList[i];
      bool isRead = (msg.direction == SmsMessage::Direction::INCOMING)
                        ? (msg.status == SmsMessage::Status::RECEIVED_READ)
                        : true;
      logger.logDebug("SMS #", i, ": ", common::to_string(msg.fromNumber),
                      " - Read: ", isRead ? "Yes" : "No");
    }
    context.user.displaySmsList(currentSmsList);
  }
}

void ViewSmsListState::handleUiBack() {
  logger.logInfo("Back button pressed");
  context.setState<SmsMenuState>();
}

void ViewSmsListState::handleUiAction() {}

void ViewSmsListState::handleUiAccept() {}

void ViewSmsListState::handleItemSelected(std::optional<std::size_t> index) {
  if (!index.has_value()) {
    logger.logError("Invalid index selected in SMS list");
    return;
  }
  if (index.value() >= currentSmsList.size()) {
    logger.logError("Selected index out of range: ", index.value());
    return;
  }
  context.setState<ViewSmsState>(index.value());
}

} // namespace ue
