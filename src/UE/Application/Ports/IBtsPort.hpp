#pragma once

#include "Messages/BtsId.hpp"
#include "Messages/PhoneNumber.hpp"
#include <string>

namespace ue
{

class IBtsEventsHandler
{
public:
    virtual ~IBtsEventsHandler() = default;

    virtual void handleSib(common::BtsId) = 0;
    virtual void handleAttachAccept() = 0;
    virtual void handleAttachReject() = 0;
    virtual void handleDisconnected() = 0;
    virtual void handleSmsReceived(common::PhoneNumber fromNumber, std::string message) = 0;
    virtual void handleSmsSentResult(common::PhoneNumber to, bool success) = 0;
    virtual void handleCallUnknownRecipient(common::PhoneNumber to) = 0;
    virtual void handleCallDropped() = 0;
    virtual void handleCallAccepted() = 0;
    virtual void handleCallReceived(common::PhoneNumber fromNumber) = 0;
    virtual void handleCallTalkReceived(common::PhoneNumber fromNumber,
                                        std::string text) = 0;
};

class IBtsPort
{
public:
    virtual ~IBtsPort() = default;

    virtual void sendAttachRequest(common::BtsId) = 0;
    virtual void sendSms(common::PhoneNumber to, const std::string& text) = 0;
    virtual void sendCallRequest(common::PhoneNumber to) = 0;
    virtual void sendCallAccepted(common::PhoneNumber to) = 0;
    virtual void sendCallDropped(common::PhoneNumber to) = 0;
    virtual void sendCallTalk(common::PhoneNumber to,
                              const std::string &text) = 0;
};

}
