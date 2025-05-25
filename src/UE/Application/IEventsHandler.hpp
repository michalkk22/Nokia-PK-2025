#pragma once

#include "Ports/ITimerPort.hpp"
#include "Ports/IBtsPort.hpp"
#include "Ports/IUserPort.hpp"
#include "Messages/PhoneNumber.hpp"
#include <optional>

namespace ue {
class IEventsHandler : public IBtsEventsHandler,
                       public IUserEventsHandler,
                       public ITimerEventsHandler {
public:
  virtual ~IEventsHandler() = default;
  virtual void handleShutdown() = 0;
  virtual void handleUiAction() = 0;
  virtual void handleUiBack() = 0;
  virtual void handleUiAccept() = 0;
  virtual void handleSmsReceived(common::PhoneNumber from,
                                 std::string text) = 0;
  virtual void handleSmsSentResult(common::PhoneNumber to, bool success) = 0;
  virtual void handleCallUnknownRecipient(common::PhoneNumber to) = 0;
  virtual void handleCallReceived(common::PhoneNumber fromNumber) = 0;
  virtual void handleCallDropped() = 0;
  virtual void handleCallAccepted() = 0;
  virtual void handleItemSelected(std::optional<std::size_t> index) = 0;
  virtual void handleCallTalkReceived(common::PhoneNumber fromNumber,
                                      const std::string &text) = 0;
};

} // namespace ue
