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
    void handleUiAction() override;
    void handleUiBack() override;
    void handleUiAccept() override;
    void handleCallUnknownRecipient(common::PhoneNumber to) override;
    void handleCallAccepted() override;
    void handleCallDropped() override;
    void handleCallReceived(common::PhoneNumber fromNumber) override;
    void handleItemSelected(std::optional<std::size_t> index) override;
    void handleCallTalkReceived(common::PhoneNumber fromNumber,
                                std::string text) override;

  protected:
    Context& context;
    common::PrefixedLogger logger;
};

}