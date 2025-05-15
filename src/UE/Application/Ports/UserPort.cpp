#include "UserPort.hpp"
#include "UeGui/IListViewMode.hpp"
#include "UeGui/ITextMode.hpp"
#include <string>

namespace ue {

UserPort::UserPort(common::ILogger &logger, IUeGui &gui,
                   common::PhoneNumber phoneNumber)
    : logger(logger, "[USER-PORT]"), gui(gui), phoneNumber(phoneNumber) {}

void UserPort::start(IEventsHandler &handler) {
  this->handler = &handler;
  gui.setTitle("Nokia " + common::to_string(phoneNumber));
  gui.setAcceptCallback(std::bind(&UserPort::acceptCallback, this));
  gui.setRejectCallback(std::bind(&UserPort::rejectCallback, this));
  gui.setMailCallback(std::bind(&UserPort::mailCallback, this));
  gui.setItemSelectedCallback(std::bind(&UserPort::itemSelectedCallback, this));
}

void UserPort::stop() {
  handler = nullptr;
  gui.setAcceptCallback(nullptr);
  gui.setRejectCallback(nullptr);
  gui.setMailCallback(nullptr);
  gui.setItemSelectedCallback(nullptr);
}

void UserPort::showNotConnected() { gui.showNotConnected(); }

void UserPort::showConnecting() { gui.showConnecting(); }

void UserPort::showConnected() {
  logger.logInfo("Main view");
  IUeGui::IListViewMode &menu = gui.setListViewMode();
  menu.clearSelectionList();
  gui.showConnected();
}

void UserPort::mailCallback() {
  if (!handler)
    return;
  handler->handleUiAction();
}

void UserPort::clearSmsCompose() {
  logger.logDebug("Clearing SMS compose screen");
  gui.getSmsComposeMode().clearSmsText();
}

void UserPort::showSmsMenuList() {
  IUeGui::IListViewMode &menu = gui.setListViewMode();
  menu.clearSelectionList();
  menu.addSelectionListItem("Compose SMS", "Send a new text message");
  menu.addSelectionListItem("View SMS", "Read received messages");
}

void UserPort::showNewSms(bool present) { gui.showNewSms(present); }

void UserPort::displaySmsList(const std::vector<SmsMessage> &smsList) {
  logger.logInfo("Showing SMS list, count: ", smsList.size());

  IUeGui::IListViewMode &listView = gui.setListViewMode();
  listView.clearSelectionList();

  for (const auto &msg : smsList) {
    bool isRead = (msg.direction == SmsMessage::Direction::INCOMING)
                      ? (msg.status == SmsMessage::Status::RECEIVED_READ)
                      : true;

    std::string prefix = isRead ? "  " : "New:  ";

    std::string directionLabel =
        (msg.direction == SmsMessage::Direction::INCOMING) ? "From: " : "To: ";

    std::string label =
        prefix + directionLabel + common::to_string(msg.fromNumber);

    if (msg.direction == SmsMessage::Direction::OUTGOING &&
        msg.status == SmsMessage::Status::FAILED) {
      label += " [FAILED]";
    }

    listView.addSelectionListItem(label, msg.text);
  }
}

void UserPort::displaySmsContent(const SmsMessage &sms) {
  std::string labelPrefix =
      (sms.direction == SmsMessage::Direction::INCOMING) ? "From: " : "To: ";
  logger.logInfo(labelPrefix, sms.fromNumber);

  IUeGui::ITextMode &viewer = gui.setViewTextMode();

  std::string displayText = labelPrefix + common::to_string(sms.fromNumber);
  std::string text = sms.text;
  displayText += "\n\n--- Message Content ---\n";
  text.erase(0,
             1); // Remove the first character which diplays as unknown '?' mark
  displayText += text;

  if (sms.direction == SmsMessage::Direction::OUTGOING) {
    std::string statusText =
        (sms.status == SmsMessage::Status::FAILED) ? " [FAILED]" : "";
    displayText += statusText;
  }
  logger.logDebug("Displaying message content: ", displayText);
  viewer.setText(displayText);
}

void UserPort::displayAlert(const std::string &title,
                            const std::string &message) {
  logger.logInfo("Alert: ", title);
  IUeGui::ITextMode &alerter = gui.setAlertMode();
  alerter.setText(title + "\n\n" + message);
}

void UserPort::showSmsCompose() {
  logger.logInfo("Showing SMS Compose screen");
  auto &composeMode = gui.setSmsComposeMode();

  composeMode.clearSmsText();
  composeMode.setPhoneNumber(common::PhoneNumber{});
}

void UserPort::itemSelectedCallback() {
  if (!handler)
    return;

  // if (currentViewMode == details::VIEW_MODE_MAIN_MENU) {
  //   logger.logDebug(
  //       "Main menu action triggered - fetching current selection index");
  //   auto &listView = gui.setListViewMode();
  //   auto indexPair = listView.getCurrentItemIndex();
  //   selectedIndexOpt = indexPair.first
  //                          ? std::optional<std::size_t>(indexPair.second)
  //                          : std::nullopt;
  // } else if (currentViewMode == details::VIEW_MODE_SMS_MENU) {
  //   logger.logDebug(
  //       "SMS menu action triggered - resolving selected menu item index");
  auto &listView = gui.setListViewMode();
  auto indexPair = listView.getCurrentItemIndex();
  auto selectedIndexOpt = indexPair.first
                              ? std::optional<std::size_t>(indexPair.second)
                              : std::nullopt;

  handler->handleItemSelected(selectedIndexOpt);

  // if (selectedIndexOpt.has_value()) {
  //   if (selectedIndexOpt.value() == 0) {
  //     logger.logInfo("Compose SMS selected from SMS menu");
  //     showSmsCompose();
  //     selectedIndexOpt = std::nullopt;
  //   }
  // }

  // } else if (currentViewMode == details::VIEW_MODE_SMS_LIST) {
  //   logger.logDebug(
  //       "Fetching selected SMS from the list after 'Accept' action");
  //   auto &listView = gui.setListViewMode();
  //   auto indexPair = listView.getCurrentItemIndex();
  //   selectedIndexOpt = indexPair.first
  //                          ? std::optional<std::size_t>(indexPair.second)
  //                          : std::nullopt;
  // } else if (currentViewMode == details::VIEW_MODE_SMS_COMPOSE) {
  //   logger.logDebug("Ignoring accept action – currently viewing SMS
  //   content"); selectedIndexOpt = std::nullopt;
  // }

  // logger.logDebug(
  //     "User triggered accept - ViewMode: ", currentViewMode,
  //     ", Selected Index: ",
  //     (selectedIndexOpt ? std::to_string(*selectedIndexOpt) : "None"));
  // handler->handleUiAction(selectedIndexOpt);
}

void UserPort::acceptCallback() {
  if (!handler)
    return;
  handler->handleUiAccept();
}

void UserPort::rejectCallback() {
  if (!handler)
    return;
  handler->handleUiBack();
}

common::PhoneNumber UserPort::getSmsRecipient() const {
  return gui.getSmsComposeMode().getPhoneNumber();
}

std::string UserPort::getSmsText() const {
  return gui.getSmsComposeMode().getSmsText();
}

void UserPort::startDial() { gui.setDialMode(); }

common::PhoneNumber UserPort::getDialRecipient() const {
  return gui.setDialMode().getPhoneNumber();
}

void UserPort::startTalking() { gui.setCallMode(); }

} // namespace ue