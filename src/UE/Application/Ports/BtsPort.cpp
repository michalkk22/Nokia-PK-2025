#include "BtsPort.hpp"
#include "Messages/IncomingMessage.hpp"
#include "Messages/OutgoingMessage.hpp"

namespace ue {

BtsPort::BtsPort(common::ILogger &logger, common::ITransport &transport,
                 common::PhoneNumber phoneNumber)
    : logger(logger, "[BTS-PORT]"), transport(transport),
      phoneNumber(phoneNumber) {}

void BtsPort::start(IBtsEventsHandler &handler) {
  transport.registerMessageCallback(
      [this](BinaryMessage msg) { handleMessage(msg); });
  transport.registerDisconnectedCallback([this]() { handleDisconnected(); });
  this->handler = &handler;
}

void BtsPort::stop() {
  transport.registerMessageCallback(nullptr);
  transport.registerDisconnectedCallback(nullptr);
  handler = nullptr;
}

void BtsPort::handleMessage(BinaryMessage msg) {
  try {
    if (!handler) {
      return;
    }
    common::IncomingMessage reader{msg};
    auto msgId = reader.readMessageId();
    auto phoneNumber = reader.readPhoneNumber();
    common::MessageHeader messageHeader;

    switch (msgId) {
    case common::MessageId::Sib: {
      auto btsId = reader.readBtsId();
      handler->handleSib(btsId);
      break;
    }
    case common::MessageId::AttachResponse: {
      bool accept = reader.readNumber<std::uint8_t>() != 0u;
      if (accept)
        handler->handleAttachAccept();
      else
        handler->handleAttachReject();
      break;
    }
    case common::MessageId::Sms: {
      std::string message = reader.readRemainingText();
      handler->handleSmsReceived(phoneNumber, message);
      break;
    }
    case common::MessageId::UnknownRecipient: {
      messageHeader = reader.readMessageHeader();
      switch (messageHeader.messageId) {
      case common::MessageId::Sms:
        logger.logError("Failed to send SMS – Recipient not found.",
                        messageHeader.to);
        handler->handleSmsSentResult(messageHeader.to, false);
        break;
      case common::MessageId::CallRequest:
        logger.logError("Failed to send call request – Recipient not found.",
                        messageHeader.to);
        handler->handleCallUnknownRecipient(messageHeader.to);
        break;
      default:
        logger.logError("Failed to send message – Recipient not found.",
                        messageHeader.to);
        break;
        break;
      }
    case common::MessageId::CallRequest: {
      logger.logInfo("Call request received from: ", phoneNumber);
      handler->handleCallReceived(phoneNumber);
      break;
    }
    case common::MessageId::CallDropped:
      logger.logInfo("Call dropped");
      handler->handleCallDropped();
      break;
    case common::MessageId::CallAccepted:
      logger.logInfo("Call accepted");
      handler->handleCallAccepted();
      break;
    }
    case common::MessageId::UnknownSender: {
      logger.logError("Critical error: Unknown sender");
      break;
    }
    default:
      logger.logError("unknown message: ", msgId, ", from: ", phoneNumber);
    }
  } catch (std::exception const &ex) {
    logger.logError("handleMessage error: ", ex.what());
  }
}

void BtsPort::handleDisconnected() {
  logger.logInfo("Transport disconnected");
  if (handler)
    handler->handleDisconnected();
}

void BtsPort::sendAttachRequest(common::BtsId btsId) {
  logger.logDebug("sendAttachRequest: ", btsId);
  common::OutgoingMessage msg{common::MessageId::AttachRequest, phoneNumber,
                              common::PhoneNumber{}};
  msg.writeBtsId(btsId);
  transport.sendMessage(msg.getMessage());
}
void BtsPort::sendSms(common::PhoneNumber to, const std::string &text) {
  logger.logInfo("Sending SMS to: ", to);
  common::OutgoingMessage msg{common::MessageId::Sms, phoneNumber, to};
  msg.writeText(text);
  transport.sendMessage(msg.getMessage());
}

void BtsPort::sendCallRequest(common::PhoneNumber to) {
  logger.logInfo("Sending call request to: ", to);
  common::OutgoingMessage msg{common::MessageId::CallRequest, phoneNumber, to};
  transport.sendMessage(msg.getMessage());
}

void BtsPort::sendCallAccepted(common::PhoneNumber to) {
  logger.logInfo("Sending call accepted to: ", to);
  common::OutgoingMessage msg{common::MessageId::CallAccepted, phoneNumber, to};
  transport.sendMessage(msg.getMessage());
}

} // namespace ue
