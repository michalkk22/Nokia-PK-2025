#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Application.hpp"
#include "Messages/PhoneNumber.hpp"
#include "Mocks/IBtsPortMock.hpp"
#include "Mocks/ILoggerMock.hpp"
#include "Mocks/ITimerPortMock.hpp"
#include "Mocks/IUserPortMock.hpp"
#include <memory>

namespace ue {
using namespace ::testing;

class ApplicationTestSuite : public Test {
protected:
  const common::PhoneNumber PHONE_NUMBER{112};
  NiceMock<common::ILoggerMock> loggerMock;
  StrictMock<IBtsPortMock> btsPortMock;
  StrictMock<IUserPortMock> userPortMock;
  StrictMock<ITimerPortMock> timerPortMock;

  Application objectUnderTest{PHONE_NUMBER, loggerMock, btsPortMock,
                              userPortMock, timerPortMock};
};

struct ApplicationNotConnectedTestSuite : ApplicationTestSuite {};

TEST_F(ApplicationNotConnectedTestSuite, todo) {}

} // namespace ue
