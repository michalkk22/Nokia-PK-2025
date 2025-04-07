#pragma once

#include "Messages/PhoneNumber.hpp"
#include <string>
#include <vector>
#include <map>

namespace ue
{


struct SmsMessage
{
    common::PhoneNumber from;
    std::string text;
    bool isRead;
    SmsMessage(common::PhoneNumber from, std::string text, bool isRead = false)
        : from(from), text(text), isRead(isRead)
    {}
};

class SmsDb
{
public:
    SmsDb() = default;
    std::size_t addSms(common::PhoneNumber from, const std::string& text);
    const std::vector<SmsMessage>& getAllSms() const;
    std::size_t getUnreadCount() const;
    bool markAsRead(std::size_t index);

private:
    std::vector<SmsMessage> messages;
};

} // namespace ue