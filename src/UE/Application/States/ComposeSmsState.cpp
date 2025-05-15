#include "ComposeSmsState.hpp"
#include "ConnectedState.hpp"
#include "Messages/PhoneNumber.hpp"
#include "SmsMenuState.hpp"

namespace ue {

ComposeSmsState::ComposeSmsState(Context &context)
    : BaseState(context, "ComposeSmsState") {
  logger.logInfo("Entered Compose SMS State – showing SMS composition screen");
  context.user.showSmsCompose();
}

void ComposeSmsState::handleUiAction() {
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

  context.user.clearSmsCompose();

  context.setState<ConnectedState>();
}

void ComposeSmsState::handleSmsSentResult(common::PhoneNumber to,
                                          bool success) {
  logger.logInfo("Send result for SMS to ", to,
                 " received during composition – ignoring result");
}

void ComposeSmsState::handleUiAccept() {}

void ComposeSmsState::handleUiBack() {
  logger.logInfo("Back button pressed");
  context.user.clearSmsCompose();
  context.setState<SmsMenuState>();
}

} // namespace ue
