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
    const common::PhoneNumber TEST_PHONE_NUMBER{112};
    //const common::BtsId BTS_ID{1024};
    NiceMock<common::ILoggerMock> loggerMock;
    StrictMock<IBtsPortMock> btsPortMock;
    StrictMock<IUserPortMock> userPortMock;
    StrictMock<ITimerPortMock> timerPortMock;

    std::unique_ptr<Application> appInstance;

    void SetUp() override { EXPECT_CALL(userPortMock, showNotConnected()); }
    
    void createApplication() {
        appInstance = std::make_unique<Application>(TEST_PHONE_NUMBER, 
                                                   loggerMock,
                                                   btsPortMock, 
                                                   userPortMock, 
                                                   timerPortMock);
    }

    void verifyShowNotConnectedOnStart() { Mock::VerifyAndClearExpectations(&userPortMock); }
};

struct ApplicationNotConnectedTestSuite : ApplicationTestSuite {};

TEST_F(ApplicationTestSuite, UeShallAttachToBtsAfterReceivingSib)
{
    createApplication();
    verifyShowNotConnectedOnStart();

    common::BtsId testBtsId{42};
    
    EXPECT_CALL(btsPortMock, sendAttachRequest(testBtsId));
    EXPECT_CALL(timerPortMock, startTimer(std::chrono::milliseconds{500}));
    appInstance->handleSib(testBtsId);

    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(userPortMock, showConnected());
    appInstance->handleAttachAccept();
}

TEST_F(ApplicationTestSuite, UeShallHandleAttachmentRejection)
{
    createApplication();
    verifyShowNotConnectedOnStart();
    
    common::BtsId testBtsId{42};

    EXPECT_CALL(btsPortMock, sendAttachRequest(testBtsId));
    EXPECT_CALL(timerPortMock, startTimer(std::chrono::milliseconds{500}));
    appInstance->handleSib(testBtsId);

    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(userPortMock, showNotConnected());
    appInstance->handleAttachReject();
}

TEST_F(ApplicationTestSuite, UeShallHandleAttachmentTimeout)
{
    createApplication();
    verifyShowNotConnectedOnStart();

    common::BtsId testBtsId{42};

    EXPECT_CALL(btsPortMock, sendAttachRequest(testBtsId));
    EXPECT_CALL(timerPortMock, startTimer(std::chrono::milliseconds{500}));
    appInstance->handleSib(testBtsId);

    EXPECT_CALL(timerPortMock, stopTimer()); 
    EXPECT_CALL(userPortMock, showNotConnected());
    appInstance->handleTimeout();
}

TEST_F(ApplicationTestSuite, UeShallHandleDisconnection)
{
    createApplication();
    verifyShowNotConnectedOnStart();
    
    common::BtsId testBtsId{42};
    EXPECT_CALL(btsPortMock, sendAttachRequest(testBtsId));
    EXPECT_CALL(timerPortMock, startTimer(std::chrono::milliseconds{500}));
    appInstance->handleSib(testBtsId);
    
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(userPortMock, showConnected());
    appInstance->handleAttachAccept();
    
    EXPECT_CALL(userPortMock, showNotConnected());
    appInstance->handleDisconnected();
}

}
