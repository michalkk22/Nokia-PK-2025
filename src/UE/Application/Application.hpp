#pragma once

#include "Logger/PrefixedLogger.hpp"
#include "Messages/PhoneNumber.hpp"
#include "IEventsHandler.hpp"
#include "Context.hpp"
#include "Ports/IBtsPort.hpp"  
#include "Ports/IUserPort.hpp" 
#include "Ports/ITimerPort.hpp"
#include "SmsDb.hpp"

namespace ue
{

    using common::PhoneNumber;
    using common::ILogger;

    class Application : public IEventsHandler
    {
    public:
        Application(PhoneNumber phoneNumber,
                    ILogger& iLogger,
                    IBtsPort& bts,
                    IUserPort& user,
                    ITimerPort& timer);
        ~Application();

        void handleUiAction() override;
        void handleUiBack() override;
        void handleUiAccept() override;
        // ITimerEventsHandler interface
        void handleTimeout() override;

        // IBtsEventsHandler interface
        void handleSib(common::BtsId btsId) override;
        void handleAttachAccept() override;
        void handleAttachReject() override;
        void handleDisconnected() override;
        void handleSmsReceived(common::PhoneNumber fromNumber, std::string message) override;
        void handleSmsSentResult(common::PhoneNumber to, bool success) override;
        void handleCallUnknownRecipient(common::PhoneNumber to) override;
        void handleCallReceived(common::PhoneNumber fromNumber) override;
        void handleCallAccepted() override;
        void handleCallDropped() override;
        void handleItemSelected(std::optional<std::size_t> index) override;
        void handleCallTalkReceived(common::PhoneNumber fromNumber,
                                    const std::string &text) override;

      private:
        Context context;
        common::PrefixedLogger logger;

    };

}
