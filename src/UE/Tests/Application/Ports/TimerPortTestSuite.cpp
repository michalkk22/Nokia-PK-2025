#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Messages/PhoneNumber.hpp"
#include "Mocks/IEventsHandlerMock.hpp"
#include "Mocks/ILoggerMock.hpp"
#include "Ports/TimerPort.hpp"

namespace ue {
using namespace ::testing;

class TimerPortTestSuite : public Test {
protected:
  NiceMock<common::ILoggerMock> loggerMock;
  StrictMock<IEventsHandlerMock> handlerMock;

  TimerPort objectUnderTest{loggerMock};

  TimerPortTestSuite() { objectUnderTest.start(handlerMock); }
  ~TimerPortTestSuite() { objectUnderTest.stop(); }
};

TEST_F(TimerPortTestSuite, shallStartTimerAndTriggerTimeout) {
  EXPECT_CALL(handlerMock, handleTimeout()).Times(1);

  objectUnderTest.startTimer(std::chrono::milliseconds{100});

  std::this_thread::sleep_for(std::chrono::milliseconds{150});
}

TEST_F(TimerPortTestSuite, shallStopTimerBeforeTimeout) {
  EXPECT_CALL(handlerMock, handleTimeout()).Times(0);

  objectUnderTest.startTimer(std::chrono::milliseconds{100});
  objectUnderTest.stopTimer();

  std::this_thread::sleep_for(std::chrono::milliseconds{150});
}

TEST_F(TimerPortTestSuite, shallHandleMultipleTimers) {
  EXPECT_CALL(handlerMock, handleTimeout()).Times(1);

  objectUnderTest.startTimer(std::chrono::milliseconds{100});
  std::this_thread::sleep_for(std::chrono::milliseconds{50});
  objectUnderTest.startTimer(std::chrono::milliseconds{200});

  std::this_thread::sleep_for(std::chrono::milliseconds{250});
}

TEST_F(TimerPortTestSuite, shallNotCrashWhenStoppedWithoutTimer) {
  objectUnderTest.stopTimer();
}

TEST_F(TimerPortTestSuite, shallNotTriggerTimeoutAfterStop) {
  EXPECT_CALL(handlerMock, handleTimeout()).Times(0);

  objectUnderTest.startTimer(std::chrono::milliseconds{100});
  objectUnderTest.stopTimer();

  std::this_thread::sleep_for(std::chrono::milliseconds{150});
}

} // namespace ue
