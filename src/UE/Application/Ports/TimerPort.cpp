#include "TimerPort.hpp"

namespace ue {

TimerPort::TimerPort(common::ILogger &logger)
    : logger(logger, "[TIMER PORT]") {}

void TimerPort::start(ITimerEventsHandler &handler) {
  logger.logDebug("Started");
  this->handler = &handler;
}

void TimerPort::stop() {
  logger.logDebug("Stoped");
  stopTimer();
  handler = nullptr;
}

void TimerPort::startTimer(Duration duration) {
  if (!handler) {
    return;
  }

  logger.logDebug("Start timer: ", duration.count(), "ms");

  stopTimer();
  timerRunning = true;

  timerThread = std::make_unique<std::thread>([this, duration]() {
    std::this_thread::sleep_for(duration);
    if (timerRunning) {
      logger.logInfo("Timeout");
      handler->handleTimeout();
    }
  });

  timerThread->detach();
}

void TimerPort::stopTimer() {
  timerRunning = false;
  logger.logDebug("Stop timer");
}

} // namespace ue
