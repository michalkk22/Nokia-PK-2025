// IEventsHandlerMock.hpp 
#pragma once

#include "Application/IEventsHandler.hpp" 
#include <gmock/gmock.h>
#include <optional>

namespace ue
{

    class IEventsHandlerMock : public IEventsHandler
    {
    public:
        MOCK_METHOD(void, handleSib, (common::BtsId), (override));
        MOCK_METHOD(void, handleShutdown, (), (override));
        MOCK_METHOD(void, handleAttachAccept, (), (override));
        MOCK_METHOD(void, handleAttachReject, (), (override));
        MOCK_METHOD(void, handleDisconnected, (), (override));
        MOCK_METHOD(void, handleSmsReceived, (common::PhoneNumber from, std::string text), (override));
        MOCK_METHOD(void, handleSmsSentResult,
                    (common::PhoneNumber to, bool success), (override));
        MOCK_METHOD(void, handleTimeout, (), (override));
        MOCK_METHOD(void, handleUiAction, (), (override));
        MOCK_METHOD(void, handleUiBack, (), (override));
        MOCK_METHOD(void, handleUiAccept, (), (override));
        MOCK_METHOD(void, handleCallUnknownRecipient, (common::PhoneNumber to), (override));
        MOCK_METHOD(void, handleCallReceived, (common::PhoneNumber fromNumber), (override));
        MOCK_METHOD(void, handleCallDropped, (), (override));
        MOCK_METHOD(void, handleCallAccepted, (), (override));
        MOCK_METHOD(void, handleItemSelected, (std::optional<std::size_t> index), (override));
        MOCK_METHOD(void, handleCallTalkReceived,
                    (common::PhoneNumber fromNumber, const std::string &text),
                    (override));
    };

} // namespace ue