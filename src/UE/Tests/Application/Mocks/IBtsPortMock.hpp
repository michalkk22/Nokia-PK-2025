#pragma once

#include <gmock/gmock.h>
#include "Ports/IBtsPort.hpp"

namespace ue
{

    class IBtsEventsHandlerMock : public IBtsEventsHandler
    {
    public:
        IBtsEventsHandlerMock();
        ~IBtsEventsHandlerMock() override;

        MOCK_METHOD(void, handleSib, (common::BtsId), (final));
        MOCK_METHOD(void, handleAttachAccept, (), (final));
        MOCK_METHOD(void, handleAttachReject, (), (final));
        MOCK_METHOD(void, handleDisconnected, (), (final));
        MOCK_METHOD(void, handleSmsSentResult, (common::PhoneNumber to, bool success), (override)); 
        MOCK_METHOD(void, handleSmsReceived, (common::PhoneNumber fromNumber, std::string message), (override)); 
        MOCK_METHOD(void, handleCallUnknownRecipient, (common::PhoneNumber to), (override));
        MOCK_METHOD(void, handleCallReceived, (common::PhoneNumber fromNumber), (override));
        MOCK_METHOD(void, handleCallDropped, (), (override));
        MOCK_METHOD(void, handleCallAccepted, (), (override));
        MOCK_METHOD(void, handleCallTalkReceived,
                    (common::PhoneNumber fromNumber, const std::string &text),
                    (override));
    };

    class IBtsPortMock : public IBtsPort
    {
    public:
        IBtsPortMock();
        ~IBtsPortMock() override;

        MOCK_METHOD(void, sendAttachRequest, (common::BtsId), (final));
        MOCK_METHOD(void, sendSms, (common::PhoneNumber to, const std::string& text), (override));
        MOCK_METHOD(void, sendCallRequest, (common::PhoneNumber to), (override));
        MOCK_METHOD(void, sendCallAccepted, (common::PhoneNumber to), (override));
        MOCK_METHOD(void, sendCallDropped, (common::PhoneNumber to), (override));
        MOCK_METHOD(void, sendCallTalk, (common::PhoneNumber to, const std::string &msg), (override));
    };

}