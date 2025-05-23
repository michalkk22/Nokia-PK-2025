#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Messages/PhoneNumber.hpp"
#include "Mocks/IEventsHandlerMock.hpp"
#include "Mocks/ILoggerMock.hpp"
#include "Mocks/IUeGuiMock.hpp"
#include "Ports/UserPort.hpp"

namespace ue {
using namespace ::testing;

class UserPortTestSuite : public Test {
protected:
  const common::PhoneNumber PHONE_NUMBER{112};
  NiceMock<common::ILoggerMock> loggerMock;
  StrictMock<IEventsHandlerMock> handlerMock;
  StrictMock<IUeGuiMock> guiMock;
  StrictMock<IListViewModeMock> listViewModeMock;

  UserPort objectUnderTest{loggerMock, guiMock, PHONE_NUMBER};

  UserPortTestSuite() {
    EXPECT_CALL(guiMock, setTitle(HasSubstr(common::to_string(PHONE_NUMBER))));
    EXPECT_CALL(guiMock, setAcceptCallback(_));
    EXPECT_CALL(guiMock, setRejectCallback(_));
    EXPECT_CALL(guiMock, setMailCallback(_));
    objectUnderTest.start(handlerMock);
  }

  ~UserPortTestSuite() override {
    EXPECT_CALL(guiMock, setAcceptCallback(IsNull()));
    EXPECT_CALL(guiMock, setRejectCallback(IsNull()));
    EXPECT_CALL(guiMock, setMailCallback(IsNull()));
    objectUnderTest.stop();
  }
};

TEST_F(UserPortTestSuite, shallStartStop) { SUCCEED(); }

TEST_F(UserPortTestSuite, shallShowNotConnected) {
  EXPECT_CALL(guiMock, showNotConnected());
  objectUnderTest.showNotConnected();
}

TEST_F(UserPortTestSuite, shallShowConnecting) {
  EXPECT_CALL(guiMock, showConnecting());
  objectUnderTest.showConnecting();
}

TEST_F(UserPortTestSuite, shallShowMenuOnConnected) {
  EXPECT_CALL(guiMock, setListViewMode()).WillOnce(ReturnRef(listViewModeMock));
  EXPECT_CALL(listViewModeMock, clearSelectionList());
  EXPECT_CALL(listViewModeMock, addSelectionListItem("Compose SMS", _));
  EXPECT_CALL(listViewModeMock, addSelectionListItem("View SMS", _));
  EXPECT_CALL(guiMock, setItemSelectedCallback(_));
  EXPECT_CALL(guiMock, showConnected());
  objectUnderTest.showConnected();
}

TEST_F(UserPortTestSuite, shallStartDialAndGetRecipient) {
  StrictMock<IDialModeMock> dialModeMock;

  EXPECT_CALL(guiMock, setDialMode())
    .Times(2)
    .WillRepeatedly(ReturnRef(dialModeMock));

  objectUnderTest.startDial();

  EXPECT_CALL(dialModeMock, getPhoneNumber())
    .WillOnce(Return(PHONE_NUMBER));

  EXPECT_EQ(objectUnderTest.getDialRecipient(), PHONE_NUMBER);
}

TEST_F(UserPortTestSuite, shallStartTalkingAndClearTexts) {
  StrictMock<ICallModeMock> callModeMock;
  EXPECT_CALL(guiMock, setCallMode()).WillRepeatedly(ReturnRef(callModeMock));

  EXPECT_CALL(callModeMock, clearIncomingText());
  EXPECT_CALL(callModeMock, clearOutgoingText());
  objectUnderTest.startTalking();
}

TEST_F(UserPortTestSuite, shallAddIncomingCallText) {
  StrictMock<ICallModeMock> callModeMock;
  EXPECT_CALL(guiMock, setCallMode()).WillOnce(ReturnRef(callModeMock));

  const std::string incoming = "Incoming speech";
  EXPECT_CALL(callModeMock, appendIncomingText(incoming));
  objectUnderTest.addCallText(incoming);
}

TEST_F(UserPortTestSuite, shallGetOutgoingCallText) {
  StrictMock<ICallModeMock> callModeMock;
  EXPECT_CALL(guiMock, setCallMode()).WillOnce(ReturnRef(callModeMock));
  EXPECT_CALL(callModeMock, getOutgoingText()).WillOnce(Return("Reply text"));

  EXPECT_EQ(objectUnderTest.getCallText(), "Reply text");
}

TEST_F(UserPortTestSuite, shallClearOutgoingCallText) {
  StrictMock<ICallModeMock> callModeMock;
  EXPECT_CALL(guiMock, setCallMode()).WillOnce(ReturnRef(callModeMock));
  EXPECT_CALL(callModeMock, clearOutgoingText());
  objectUnderTest.clearCallText();
}
} // namespace ue