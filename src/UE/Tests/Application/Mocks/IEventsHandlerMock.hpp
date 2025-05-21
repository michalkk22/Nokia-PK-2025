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
        MOCK_METHOD(void, handleAttachAccept, (), (override));
        MOCK_METHOD(void, handleAttachReject, (), (override));
        MOCK_METHOD(void, handleDisconnected, (), (override));
        MOCK_METHOD(void, handleSmsReceived, (common::PhoneNumber from, std::string text), (override));
        MOCK_METHOD(void, handleSmsSentResult,
                    (common::PhoneNumber to, bool success), (override));
        MOCK_METHOD(void, handleTimeout, (), (override));
        MOCK_METHOD(void, handleUiAction, (), (override));
        MOCK_METHOD(void, handleUiBack, (), (override));
    };

} // namespace ue