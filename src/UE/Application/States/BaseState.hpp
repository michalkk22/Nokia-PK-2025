#pragma once

#include "IEventsHandler.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "Context.hpp"
#include "Messages/MessageId.hpp"

namespace ue
{

class BaseState : public IEventsHandler
{
public:
    BaseState(Context& context, const std::string& name);
    ~BaseState() override;

    // ITimerEventsHandler interface
    void handleTimeout() override;

    // IBtsEventsHandler interface
    void handleSib(common::BtsId btsId) override;
    void handleAttachAccept() override;
    void handleAttachReject() override;
    void handleDisconnected() override;
    void handleSmsReceived(common::PhoneNumber fromNumber, std::string message) override;
    void handleSmsSentResult(common::PhoneNumber to, bool success) override;
    void handleSmsComposeResult(common::PhoneNumber recipient, const std::string& text) override;
    void handleUiAction(std::optional<std::size_t> selectedIndex) override;
    void handleUiBack() override;
    void handleUiAccept() override;
    void handleCallUnknownRecipient(common::PhoneNumber to) override;

  protected:
    Context& context;
    common::PrefixedLogger logger;
};

}