#include "Application.hpp"
#include "States/ConnectedState.hpp"
#include "States/NotConnectedState.hpp"

namespace ue {

Application::Application(common::PhoneNumber phoneNumber,
                         common::ILogger &iLogger, IBtsPort &bts,
                         IUserPort &user, ITimerPort &timer)
    : context{iLogger, bts, user, timer, SmsDb(), nullptr, phoneNumber},
      logger(iLogger, "[APP] ") {
  logger.logInfo("Started ", context.myPhoneNumber);
  context.setState<NotConnectedState>();
}

Application::~Application() { logger.logInfo("Stopped"); }

void Application::handleUiAction(std::optional<std::size_t> selectedIndex) {
  if (context.state)
    context.state->handleUiAction(selectedIndex);
}

void Application::handleUiBack() {
  if (context.state)
    context.state->handleUiBack();
}

void Application::handleTimeout() {
  if (context.state)
    context.state->handleTimeout();
}

void Application::handleSib(common::BtsId btsId) {
  if (context.state)
    context.state->handleSib(btsId);
}

void Application::handleAttachAccept() {
  if (context.state)
    context.state->handleAttachAccept();
}

void Application::handleAttachReject() {
  if (context.state)
    context.state->handleAttachReject();
}

void Application::handleDisconnected() {
  logger.logInfo("Disconnected");
  if (context.state)
    context.state->handleDisconnected();
}

void Application::handleSmsReceived(common::PhoneNumber fromNumber,
                                    std::string message) {
  logger.logInfo("SMS received from: ", fromNumber);
  if (context.state)
    context.state->handleSmsReceived(fromNumber, message);
}

void Application::handleSmsSentResult(common::PhoneNumber to, bool success) {
  logger.logInfo("SMS send result - Recipient: ", to,
                 ", Status: ", success ? "Delivered" : "Failed");
  if (context.state)
    context.state->handleSmsSentResult(to, success);
}

void Application::handleSmsComposeResult(common::PhoneNumber recipient,
                                         const std::string &text) {
  context.smsDb.addSentSms(recipient, text);
  context.bts.sendSms(recipient, text);

  context.setState<ConnectedState>();
}

} // namespace ue