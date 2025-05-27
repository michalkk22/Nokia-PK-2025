#pragma once

#include "ITimerPort.hpp"
#include "Logger/PrefixedLogger.hpp"

#include <atomic>
#include <chrono>
#include <memory>
#include <thread>

namespace ue
{

class TimerPort : public ITimerPort
{
public:
    TimerPort(common::ILogger& logger);

    void start(ITimerEventsHandler& handler);
    void stop();

    // ITimerPort interface
    void startTimer(Duration duration) override;
    void stopTimer() override;

private:
    common::PrefixedLogger logger;
    ITimerEventsHandler* handler = nullptr;
    std::shared_ptr<std::atomic<bool>> cancelToken;
    std::mutex cancelTokenMutex;
};

}
