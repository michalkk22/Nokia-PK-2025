#include "NotConnectedState.hpp"
#include "ConnectingState.hpp"

namespace ue {

NotConnectedState::NotConnectedState(Context &context)
    : BaseState(context, "NotConnectedState") {
  context.user.showNotConnected();
}

void NotConnectedState::handleSib(common::BtsId btsId) {
  logger.logInfo("Received SIB from BTS with ID: ", btsId);
  context.setState<ConnectingState>(btsId);
}

} // namespace ue