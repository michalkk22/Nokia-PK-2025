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
        logger.logInfo("Opening SMS at index ", viewingSmsIndex, " (Sender: ", message.from, ")");
        if (!message.isRead)
        {
            logger.logDebug("Updating SMS status to 'read' at index ", viewingSmsIndex);
            context.smsDb.markAsRead(viewingSmsIndex); 
        }
        context.user.displaySmsContent(message);
    }

    void ViewSmsState::handleUiBack()
    {
        logger.logInfo("User navigated back from SMS view.");
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
        std::size_t smsIndex = context.smsDb.addSms(from, text);
        logger.logDebug("New SMS saved to inbox at position ", smsIndex);
        context.user.showNewSms();
    }

} // namespace ue
