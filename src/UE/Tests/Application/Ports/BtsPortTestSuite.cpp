#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Messages/IncomingMessage.hpp"
#include "Messages/MessageId.hpp"
#include "Messages/OutgoingMessage.hpp"
#include "Messages/PhoneNumber.hpp"
#include "Mocks/IBtsPortMock.hpp"
#include "Mocks/ILoggerMock.hpp"
#include "Mocks/ITransportMock.hpp"
#include "Ports/BtsPort.hpp"

namespace ue {
using namespace ::testing;

class BtsPortTestSuite : public Test {
protected:
  const common::PhoneNumber PHONE_NUMBER{112};
  const common::BtsId BTS_ID{13121981ll};
  NiceMock<common::ILoggerMock> loggerMock;
  StrictMock<IBtsEventsHandlerMock> handlerMock;
  NiceMock<common::ITransportMock> transportMock;
  common::ITransport::MessageCallback messageCallback;
  common::ITransport::DisconnectedCallback disconnectedCallback;
  common::BinaryMessage message;

  BtsPort objectUnderTest{loggerMock, transportMock, PHONE_NUMBER};

  void SetUp() override {
    ON_CALL(transportMock, registerMessageCallback(_))
        .WillByDefault(SaveArg<0>(&messageCallback));
    ON_CALL(transportMock, registerDisconnectedCallback(_))
        .WillByDefault(SaveArg<0>(&disconnectedCallback));

    ON_CALL(transportMock, sendMessage(_))
        .WillByDefault(DoAll(SaveArg<0>(&message), Return(true)));

    objectUnderTest.start(handlerMock);
  }

  void TearDown() override { objectUnderTest.stop(); }
  common::BinaryMessage createSibMessage() {
    common::OutgoingMessage message{
        common::MessageId::Sib, common::PhoneNumber{}, common::PhoneNumber{}};
    message.writeBtsId(BTS_ID);
    return message.getMessage();
  }

  common::BinaryMessage createAttachResponseMessage(bool accept) {
    common::OutgoingMessage message{common::MessageId::AttachResponse,
                                    common::PhoneNumber{}, PHONE_NUMBER};
    message.writeNumber<std::uint8_t>(accept ? 1u : 0u);
    return message.getMessage();
  }

  common::BinaryMessage createInvalidMessage() {
    common::OutgoingMessage message{common::MessageId::AttachRequest,
                                    common::PhoneNumber{},
                                    common::PhoneNumber{}};
    return message.getMessage();
  }

  common::BinaryMessage createCallRequestMessage(common::PhoneNumber from) {
    common::OutgoingMessage msg{
        common::MessageId::CallRequest, from, PHONE_NUMBER};
    return msg.getMessage();
  }

  common::BinaryMessage createCallDroppedMessage(common::PhoneNumber from) {
    common::OutgoingMessage msg{
        common::MessageId::CallDropped, from, PHONE_NUMBER};
    return msg.getMessage();
  }

  common::BinaryMessage createCallAcceptedMessage(common::PhoneNumber from) {
    common::OutgoingMessage msg{
        common::MessageId::CallAccepted, from, PHONE_NUMBER};
    return msg.getMessage();
  }

  common::BinaryMessage createCallTalkMessage(common::PhoneNumber from,
                                              const std::string &text) {
    common::OutgoingMessage msg{
        common::MessageId::CallTalk, from, PHONE_NUMBER};
    msg.writeText(text);
    return msg.getMessage();
  }
};

TEST_F(BtsPortTestSuite, shallRegisterHandlersBetweenStartStop) { SUCCEED(); }

TEST_F(BtsPortTestSuite, shallIgnoreWrongMessage) {
  messageCallback(createInvalidMessage());
}

TEST_F(BtsPortTestSuite, shallHandleSib) {
  EXPECT_CALL(handlerMock, handleSib(BTS_ID));
  messageCallback(createSibMessage());
}

TEST_F(BtsPortTestSuite, shallHandleAttachAccept) {
  EXPECT_CALL(handlerMock, handleAttachAccept());
  messageCallback(createAttachResponseMessage(true));
}

TEST_F(BtsPortTestSuite, shallHandleAttachReject) {
  EXPECT_CALL(handlerMock, handleAttachReject());
  messageCallback(createAttachResponseMessage(false));
}

TEST_F(BtsPortTestSuite, shallSendAttachRequest) {
  EXPECT_CALL(transportMock, sendMessage(_));
  objectUnderTest.sendAttachRequest(BTS_ID);

  common::IncomingMessage reader(message);
  ASSERT_NO_THROW(
      EXPECT_EQ(common::MessageId::AttachRequest, reader.readMessageId()));
  ASSERT_NO_THROW(EXPECT_EQ(PHONE_NUMBER, reader.readPhoneNumber()));
  ASSERT_NO_THROW(EXPECT_EQ(common::PhoneNumber{}, reader.readPhoneNumber()));
  ASSERT_NO_THROW(EXPECT_EQ(BTS_ID, reader.readBtsId()));
  ASSERT_NO_THROW(reader.checkEndOfMessage());
}

TEST_F(BtsPortTestSuite, shallHandleDisconnectedFromTransport) {
  EXPECT_CALL(handlerMock, handleDisconnected());
  disconnectedCallback();
}

TEST_F(BtsPortTestSuite, shallHandleCallRequest) {
  common::PhoneNumber from{55};
  EXPECT_CALL(handlerMock, handleCallReceived(from));
  messageCallback(createCallRequestMessage(from));
}

TEST_F(BtsPortTestSuite, shallHandleCallDropped) {
  common::PhoneNumber from{99};
  EXPECT_CALL(handlerMock, handleCallDropped());
  messageCallback(createCallDroppedMessage(from));
}

TEST_F(BtsPortTestSuite, shallHandleCallAccepted) {
  common::PhoneNumber from{123};
  EXPECT_CALL(handlerMock, handleCallAccepted());
  messageCallback(createCallAcceptedMessage(from));
}

TEST_F(BtsPortTestSuite, shallHandleCallTalk) {
  common::PhoneNumber from{7};
  const std::string text = "Hello!";
  EXPECT_CALL(handlerMock, handleCallTalkReceived(from, text));
  messageCallback(createCallTalkMessage(from, text));
}

} // namespace ue