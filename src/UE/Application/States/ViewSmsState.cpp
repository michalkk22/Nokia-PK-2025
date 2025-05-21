#include "ViewSmsState.hpp"
#include "ViewSmsListState.hpp"

namespace ue {

ViewSmsState::ViewSmsState(Context &context, std::size_t smsIndex)
    : ConnectedState(context, "ViewSmsState"), viewingSmsIndex(smsIndex) {
  const auto &allSms = context.smsDb.getAllSms();
  if (viewingSmsIndex >= allSms.size()) {
    logger.logError("Attempted to view SMS with invalid index: ",
                    viewingSmsIndex);
    context.setState<ViewSmsListState>();
    return;
  }

  const SmsMessage &message = allSms[viewingSmsIndex];
  logger.logInfo("Opening SMS at index ", viewingSmsIndex,
                 " (Sender: ", message.fromNumber, ")");

  if (message.direction == SmsMessage::Direction::INCOMING &&
      message.status == SmsMessage::Status::RECEIVED_UNREAD) {
    logger.logDebug("Updating SMS status to 'read' at index ", viewingSmsIndex);
    context.smsDb.markAsRead(viewingSmsIndex);
    if (context.smsDb.getUnreadCount() == 0) {
      context.user.showNewSms(false);
    }
  }
  context.user.displaySmsContent(message);
}

void ViewSmsState::handleUiBack() {
  const auto &allSms = context.smsDb.getAllSms();
  context.user.displaySmsList(allSms);
  context.setState<ViewSmsListState>();
}

void ViewSmsState::handleUiAction() {}
void ViewSmsState::handleUiAccept() {}

} // namespace ue
