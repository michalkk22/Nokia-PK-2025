#include "TalkingState.hpp"

namespace ue {
TalkingState::TalkingState(Context &context, common::PhoneNumber recipient)
    : ConnectedState(context, "TalkingState"), recipient(recipient) {
        logger.logInfo("Entered Talking state.");
    };

void TalkingState::handleUiBack() {
  logger.logInfo("User dropped call - returning to main menu.");
  context.bts.sendCallDropped(recipient);
  context.setState<ConnectedState>();
  context.user.displayAlert("Call dropped", "");
}

void TalkingState::handleCallDropped() {
  logger.logInfo("Call dropped - returning to main menu.");
  context.setState<ConnectedState>();
  context.user.displayAlert("Call dropped", "");
}

void TalkingState::handleCallReceived(common::PhoneNumber fromNumber) {
  dropAnotherCall(fromNumber);
}
}