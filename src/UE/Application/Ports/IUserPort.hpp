#pragma once

#include <vector> 
#include <string>
#include "SmsDb.hpp"

namespace ue
{

class IUserEventsHandler
{
public:
    virtual ~IUserEventsHandler() = default;
};

class IUserPort
{
public:
    virtual ~IUserPort() = default;

    // BTS connection
    virtual void showNotConnected() = 0;
    virtual void showConnecting() = 0;
    virtual void showConnected() = 0;

    // SMS
    virtual void showNewSms() = 0;
    virtual void displaySmsList(const std::vector<SmsMessage>& messages) = 0;
    virtual void displaySmsContent(const SmsMessage& message) = 0;
    virtual void displayAlert(const std::string& title, const std::string& message) = 0;
    virtual void showSmsCompose() = 0;
    virtual common::PhoneNumber getSmsRecipient() const = 0;
    virtual std::string getSmsText() const = 0;

    // Calls
    virtual void startDial() = 0;
};

}
