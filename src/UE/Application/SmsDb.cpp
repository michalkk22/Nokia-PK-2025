#include "SmsDb.hpp"

namespace ue {

std::size_t SmsDb::addReceivedSms(common::PhoneNumber fromNumber,
                                  const std::string &message) {
  messages.emplace_back(fromNumber, message);
  return messages.size() - 1;
}

std::size_t SmsDb::addSentSms(common::PhoneNumber to,
                              const std::string &message,
                              SmsMessage::Status initialStatus) {
  messages.emplace_back(to, message, initialStatus);
  lastSentSmsIndex = messages.size() - 1;
  return messages.size() - 1;
}

const std::vector<SmsMessage> &SmsDb::getAllSms() const { return messages; }

std::size_t SmsDb::getUnreadCount() const {
  std::size_t amount = 0;
  for (const auto &message : messages) {
    if (message.direction == SmsMessage::Direction::INCOMING &&
        message.status == SmsMessage::Status::RECEIVED_UNREAD) {
      amount++;
    }
  }
  return amount;
}

bool SmsDb::markAsRead(std::size_t index) {
  if (index >= messages.size()) {
    return false;
  }

  if (messages[index].direction == SmsMessage::Direction::INCOMING &&
      messages[index].status == SmsMessage::Status::RECEIVED_UNREAD) {
    messages[index].status = SmsMessage::Status::RECEIVED_READ;
    return true;
  }
  return false;
}

bool SmsDb::markLastOutgoingSmsAsFailed() {
  if (lastSentSmsIndex.has_value() &&
      lastSentSmsIndex.value() < messages.size()) {
    auto &msg = messages[lastSentSmsIndex.value()];
    if (msg.direction == SmsMessage::Direction::OUTGOING) {
      msg.status = SmsMessage::Status::FAILED;
      lastSentSmsIndex.reset();
      return true;
    }
  }
  for (int i = messages.size() - 1; i >= 0; --i) {
    if (messages[i].direction == SmsMessage::Direction::OUTGOING &&
        messages[i].status == SmsMessage::Status::SENT) {
      messages[i].status = SmsMessage::Status::FAILED;
      return true;
    }
  }
  return false;
}

} // namespace ue