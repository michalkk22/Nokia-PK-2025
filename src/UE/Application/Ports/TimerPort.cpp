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

  auto token = std::make_shared<std::atomic<bool>>(true);
  cancelToken = token;

  std::thread([this, duration, token]() {
    std::this_thread::sleep_for(duration);
    if (*token) {
      handler->handleTimeout();
    }
  }).detach();
}

void TimerPort::stopTimer() {
  if (cancelToken) {
    *cancelToken = false;
  }

  logger.logDebug("Stop timer");
}

} // namespace ue
