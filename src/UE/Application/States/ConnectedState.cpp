#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"
#include "ViewSmsListState.hpp" 
#include <vector>

namespace ue {

    ConnectedState::ConnectedState(Context &context)
        : BaseState(context, "ConnectedState")
    {
        showMainMenu();
    }

    void ConnectedState::showMainMenu()
    {
        logger.logInfo("Entered Connected state.");
        context.user.showConnected();
    }

    void ConnectedState::handleDisconnected()
    {
        logger.logInfo("Lost connection to BTS.");
        context.setState<NotConnectedState>();
    }

    void ConnectedState::handleSmsReceived(common::PhoneNumber fromNumber, std::string message)
    {
        logger.logInfo("Received SMS from: ", fromNumber, " with content: ", message);

        std::size_t smsIndex = context.smsDb.addSms(fromNumber, message);
        logger.logDebug("Stored SMS at index: ", smsIndex);
        
        context.user.showNewSms();
    }

    void ConnectedState::handleUiAction(std::optional<std::size_t> selectedIndex)
    {
        if (!selectedIndex.has_value()) {
            logger.logInfo("No selection provided from UI in Main Menu");
            return;
        }

        std::size_t index = selectedIndex.value();
        logger.logInfo("User selected main menu option with index: ", index);

        if (index == 0) { logger.logInfo("SMS composition option triggered – functionality pending");}
        else if (index == 1)
        {
            logger.logInfo("Navigating to SMS list view");
            context.setState<ViewSmsListState>();
        }
        else { logger.logInfo("Unhandled menu selection. Index provided: ", index); }
    }

    void ConnectedState::handleUiBack() { logger.logInfo("Back button pressed in main menu – no action taken"); }

}
