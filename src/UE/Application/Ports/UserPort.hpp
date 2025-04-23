#pragma once

#include "IUserPort.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "IUeGui.hpp"
#include "Messages/PhoneNumber.hpp"
#include "IEventsHandler.hpp"
#include "SmsDb.hpp" 
#include <vector>
#include <optional> 

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
    void showNewSms() override;
    void displaySmsList(const std::vector<SmsMessage>& messages);
    void displaySmsContent(const SmsMessage& message);
    void displayAlert(const std::string& title, const std::string& message); 

private:
    void acceptCallback(); 
    void rejectCallback(); 

    common::PrefixedLogger logger;
    IUeGui& gui;
    common::PhoneNumber phoneNumber;
    IEventsHandler* handler = nullptr;
    details::GuiViewMode currentViewMode = details::VIEW_MODE_UNKNOWN; 
};

}
