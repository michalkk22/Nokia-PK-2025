#include <gtest/gtest.h>
#include "Messages/PhoneNumber.hpp"
#include "Application/SmsDb.hpp"

namespace ue
{

class SmsDbTestSuite : public ::testing::Test
{
protected:
    SmsDb smsDbInstance;
    common::PhoneNumber senderPhoneNumber{100};
    std::string firstTestMessage = "Ala ma kota";
    std::string secondTestMessage = "Kot ma Ale";
};

TEST_F(SmsDbTestSuite, shouldStoreNewSms)
{
    std::size_t smsIndex = smsDbInstance.addReceivedSms(senderPhoneNumber, firstTestMessage);
    ASSERT_EQ(0u, smsIndex);
    ASSERT_EQ(1u, smsDbInstance.getAllSms().size());
    ASSERT_EQ(senderPhoneNumber, smsDbInstance.getAllSms()[0].fromNumber);
    ASSERT_EQ(firstTestMessage, smsDbInstance.getAllSms()[0].text);
    ASSERT_EQ(SmsMessage::Status::RECEIVED_UNREAD, smsDbInstance.getAllSms()[0].status);
}

TEST_F(SmsDbTestSuite, shouldCountUnreadSms)
{
    smsDbInstance.addReceivedSms(senderPhoneNumber, firstTestMessage);
    ASSERT_EQ(1u, smsDbInstance.getUnreadCount());
    
    smsDbInstance.addReceivedSms(senderPhoneNumber, secondTestMessage);
    ASSERT_EQ(2u, smsDbInstance.getUnreadCount());
    
    smsDbInstance.markAsRead(0);
    ASSERT_EQ(1u, smsDbInstance.getUnreadCount());
    
    smsDbInstance.markAsRead(1);
    ASSERT_EQ(0u, smsDbInstance.getUnreadCount());
}

TEST_F(SmsDbTestSuite, shouldMarkSmsAsRead)
{
    smsDbInstance.addReceivedSms(senderPhoneNumber, firstTestMessage);
    ASSERT_EQ(SmsMessage::Status::RECEIVED_UNREAD, smsDbInstance.getAllSms()[0].status);
    
    bool result = smsDbInstance.markAsRead(0);
    ASSERT_TRUE(result);
    ASSERT_EQ(SmsMessage::Status::RECEIVED_READ, smsDbInstance.getAllSms()[0].status);
    
    // Invalid index
    result = smsDbInstance.markAsRead(1);
    ASSERT_FALSE(result);
}

} // namespace ue
