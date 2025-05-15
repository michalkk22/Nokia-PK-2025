#pragma once

#include "IEventsHandler.hpp"
#include "IUeGui.hpp"
#include "IUserPort.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "Messages/PhoneNumber.hpp"
#include "SmsDb.hpp"
#include "UeGui/IDialMode.hpp"
#include "UeGui/ISmsComposeMode.hpp"
#include <optional>
#include <vector>

namespace ue
{

    class UserPort : public IUserPort
    {
        public:
        UserPort(common::ILogger& logger, IUeGui& gui, common::PhoneNumber phoneNumber);
        void start(IEventsHandler& handler);
        void stop();

        void showNotConnected() override;
        void showConnecting() override;
        void showConnected() override;
        void showNewSms(bool present) override;
        void displaySmsList(const std::vector<SmsMessage>& messages);
        void displaySmsContent(const SmsMessage& message);
        void displayAlert(const std::string& title, const std::string& message);
        void showSmsCompose() override;
        void showSmsMenuList() override;
        void clearSmsCompose() override;

        common::PhoneNumber getSmsRecipient() const override;
        std::string getSmsText() const override;

        void startDial() override;
        common::PhoneNumber getDialRecipient() const override;
        void startTalking() override;

      private:
        void acceptCallback();
        void rejectCallback();
        void mailCallback();
        void itemSelectedCallback();

        common::PrefixedLogger logger;
        IUeGui& gui;
        common::PhoneNumber phoneNumber;
        IEventsHandler *handler = nullptr;
    };

}
