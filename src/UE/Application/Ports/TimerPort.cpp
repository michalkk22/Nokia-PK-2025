#include "TimerPort.hpp"
#include <mutex>

namespace ue {

TimerPort::TimerPort(common::ILogger &logger)
    : logger(logger, "[TIMER PORT]") {}

void TimerPort::start(ITimerEventsHandler &handler) {
  logger.logDebug("Started");
  this->handler = &handler;
}

void TimerPort::stop() {
  logger.logDebug("Stopped");
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
  {
    std::lock_guard<std::mutex> lock(cancelTokenMutex);
    cancelToken = token;
  }

  std::thread([this, duration, token]() {
    std::this_thread::sleep_for(duration);
    if (*token) {
      std::lock_guard<std::mutex> lock(cancelTokenMutex);
      if (cancelToken == token && handler) {
        handler->handleTimeout();
      }
    }
  }).detach();
}

void TimerPort::stopTimer() {
  std::lock_guard<std::mutex> lock(cancelTokenMutex);
  if (cancelToken) {
    *cancelToken = false;
    cancelToken.reset();
  }

  logger.logDebug("Stop timer");
}

} // namespace ue
