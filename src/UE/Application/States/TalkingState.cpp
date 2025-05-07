#include "TalkingState.hpp"

namespace ue {
TalkingState::TalkingState(Context &context)
    : SendingCallState(context, "TalkingState") {};
}