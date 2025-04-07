#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"

namespace ue {

    ConnectedState::ConnectedState(Context &context)
        : BaseState(context, "ConnectedState")
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

}
