#pragma once

#include "Messages/PhoneNumber.hpp"
#include <string>
#include <vector>
#include <map>

namespace ue
{


struct SmsMessage
{
    common::PhoneNumber fromNumber;
    std::string message;
    bool isRead;
    SmsMessage(common::PhoneNumber fromNumber, std::string message, bool isRead = false)
        : fromNumber(fromNumber), message(message), isRead(isRead)
    {}
};

class SmsDb
{
public:
    SmsDb() = default;
    std::size_t addSms(common::PhoneNumber fromNumber, const std::string& message);
    const std::vector<SmsMessage>& getAllSms() const;
    std::size_t getUnreadCount() const;
    bool markAsRead(std::size_t index);

private:
    std::vector<SmsMessage> messages;
};

} // namespace ue