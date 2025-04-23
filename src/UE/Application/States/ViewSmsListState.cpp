#include "ViewSmsListState.hpp"
#include "ViewSmsState.hpp"
#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"

namespace ue
{
    ViewSmsListState::ViewSmsListState(Context &context)
        : BaseState(context, "ViewSmsListState")
    {
        showList();
    }

    void ViewSmsListState::showList()
    {
        currentSmsList = context.smsDb.getAllSms();
        if (currentSmsList.empty())
        {
            logger.logInfo("No SMS messages found in inbox.");
            context.user.displayAlert("SMS Inbox", "No messages.");
            context.setState<ConnectedState>();
        }
        else
        {
            context.user.displaySmsList(currentSmsList); 
        }
    }

    void ViewSmsListState::handleUiAction(std::optional<std::size_t> selectedIndex)
    {
        if (!selectedIndex.has_value())
        {
            logger.logInfo("UI interaction with SMS list but no item was selected.");
            return;
        }

        std::size_t index = selectedIndex.value();
        if (index < currentSmsList.size())
        {
            logger.logInfo("User tapped SMS at position ", index);
            context.setState<ViewSmsState>(index);
        }
        else
        {
            logger.logError("Invalid SMS index: ", index, " – list contains only ", currentSmsList.size(), " items.");
        }
    }

    void ViewSmsListState::handleUiBack()
    {
        logger.logInfo("Returning to main menu from SMS list.");
        context.setState<ConnectedState>();
    }

    void ViewSmsListState::handleDisconnected()
    {
        logger.logInfo("Disconnected while browsing SMS inbox.");
        context.setState<NotConnectedState>();
    }

    void ViewSmsListState::handleSmsReceived(common::PhoneNumber from, std::string text)
    {
        logger.logInfo("New SMS arrived while viewing list. Sender: ", from);
        std::size_t smsIndex = context.smsDb.addSms(from, text);
        logger.logDebug("Stored new SMS at position ", smsIndex);
        context.user.showNewSms();
        showList();
    }
}
