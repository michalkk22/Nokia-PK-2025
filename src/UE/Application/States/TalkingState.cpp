#include "TalkingState.hpp"

namespace ue {
TalkingState::TalkingState(Context &context, common::PhoneNumber recipient)
    : ConnectedState(context, "TalkingState"), recipient(recipient) {
        logger.logInfo("Entered Talking state.");
    };

void TalkingState::handleUiBack() {
  context.bts.sendCallDropped(recipient);
  context.setState<ConnectedState>();
  context.user.displayAlert("Call dropped", "");
}
}