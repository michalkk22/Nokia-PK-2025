#include "ComposeSmsState.hpp"
#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"
#include "Messages/PhoneNumber.hpp"

namespace ue
{

ComposeSmsState::ComposeSmsState(Context &context)
    : BaseState(context, "ComposeSmsState")
{
    logger.logInfo("Entered Compose SMS State – showing SMS composition screen");
    context.user.showSmsCompose();
}

void ComposeSmsState::handleUiAction(std::optional<std::size_t> selectedIndex)
{
    logger.logInfo("User pressed 'Send' on SMS composition screen");

    auto recipient = context.user.getSmsRecipient();
    auto text = context.user.getSmsText();

    if (!recipient.isValid() || text.empty()) {
        logger.logInfo("SMS not sent – recipient is invalid or text is empty");
        context.user.displayAlert("Error", "Invalid recipient or empty text");
        return;
    }

    context.smsDb.addSentSms(recipient, text, SmsMessage::Status::SENT);
    context.bts.sendSms(recipient, text);
    logger.logInfo("Sending SMS to recipient: ", recipient);

    context.setState<ConnectedState>();
}

void ComposeSmsState::handleUiBack()
{
    logger.logInfo("User cancelled SMS composition – returning to connected state");
    context.setState<ConnectedState>();
}

void ComposeSmsState::handleDisconnected()
{
    logger.logInfo("Lost connection during SMS composition – switching to NotConnectedState");
    context.user.displayAlert("Disconnected", "Connection lost during SMS composition.");
    context.setState<NotConnectedState>();
}

void ComposeSmsState::handleSmsReceived(common::PhoneNumber from, std::string text)
{
    logger.logInfo("New SMS received from ", from, " while composing – saving to inbox");
    context.smsDb.addReceivedSms(from, text);
    context.user.showNewSms();
}

void ComposeSmsState::handleSmsSentResult(common::PhoneNumber to, bool success)
{
    logger.logInfo("Send result for SMS to ", to, " received during composition – ignoring result");
}

} // namespace ue
