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

void Application::handleShutdown() { context.state->handleShutdown(); }

void Application::handleUiAction() {
  if (context.state)
    context.state->handleUiAction();
}

void Application::handleUiBack() {
  if (context.state)
    context.state->handleUiBack();
}

void Application::handleUiAccept() {
  if (context.state)
    context.state->handleUiAccept();
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
  if (context.state)
    context.state->handleDisconnected();
}

void Application::handleSmsReceived(common::PhoneNumber fromNumber,
                                    std::string message) {
  if (context.state)
    context.state->handleSmsReceived(fromNumber, message);
}

void Application::handleSmsSentResult(common::PhoneNumber to, bool success) {
  if (context.state)
    context.state->handleSmsSentResult(to, success);
}

void Application::handleCallUnknownRecipient(common::PhoneNumber to) {
  if (context.state)
    context.state->handleCallUnknownRecipient(to);
}

void Application::handleCallAccepted() {
  if (context.state)
    context.state->handleCallAccepted();
}

void Application::handleCallDropped() {
  if (context.state)
    context.state->handleCallDropped();
}

void Application::handleCallReceived(common::PhoneNumber fromNumber) {
  if (context.state)
    context.state->handleCallReceived(fromNumber);
}

void Application::handleItemSelected(std::optional<std::size_t> index) {
  if (context.state)
    context.state->handleItemSelected(index);
}

void Application::handleCallTalkReceived(common::PhoneNumber fromNumber,
                                         const std::string &text) {
  if (context.state)
    context.state->handleCallTalkReceived(fromNumber, text);
}

} // namespace ue