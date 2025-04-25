#include "ViewSmsState.hpp"
#include "ViewSmsListState.hpp"
#include "NotConnectedState.hpp"
#include <stdexcept>

namespace ue
{

    ViewSmsState::ViewSmsState(Context &context, std::size_t smsIndex)
        : BaseState(context, "ViewSmsState"),
          viewingSmsIndex(smsIndex)
    {
        const auto &allSms = context.smsDb.getAllSms();
        if (viewingSmsIndex >= allSms.size())
        {
            logger.logError("Attempted to view SMS with invalid index: ", viewingSmsIndex);
            context.setState<ViewSmsListState>();
            return;
        }

        const SmsMessage &message = allSms[viewingSmsIndex];
        logger.logInfo("Opening SMS at index ", viewingSmsIndex, " (Sender: ", message.fromNumber, ")");

        if (message.direction == SmsMessage::Direction::INCOMING &&
            message.status == SmsMessage::Status::RECEIVED_UNREAD)
        {
            logger.logDebug("Updating SMS status to 'read' at index ", viewingSmsIndex);
            context.smsDb.markAsRead(viewingSmsIndex);
        }
        context.user.displaySmsContent(message);
    }

    void ViewSmsState::handleUiBack()
    {
        const auto &allSms = context.smsDb.getAllSms();
        context.user.displaySmsList(allSms);
        context.setState<ViewSmsListState>();
    }

    void ViewSmsState::handleUiAction(std::optional<std::size_t> selectedIndex)
    {
        if (!selectedIndex.has_value())
        {
            logger.logInfo("Action without index in SMS view - switching to compose");
            return;
        }

        logger.logInfo("UI action in single SMS view - returning to list");
        const auto &allSms = context.smsDb.getAllSms();
        context.user.displaySmsList(allSms);
        context.setState<ViewSmsListState>();
    }

    void ViewSmsState::handleDisconnected()
    {
        logger.logInfo("Lost connection while reading an SMS.");
        context.setState<NotConnectedState>();
    }

    void ViewSmsState::handleSmsReceived(common::PhoneNumber from, std::string text)
    {
        logger.logInfo("Received new SMS from ", from, " while reading another message.");
        std::size_t smsIndex = context.smsDb.addReceivedSms(from, text);
        logger.logDebug("New SMS saved to inbox at position ", smsIndex);
        context.user.showNewSms();
    }

} // namespace ue
