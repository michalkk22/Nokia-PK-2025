#include "TalkingState.hpp"

namespace ue {
TalkingState::TalkingState(Context &context)
    : ConnectedState(context, "TalkingState") {
  logger.logInfo("Entered Talking state.");
};
} // namespace ue