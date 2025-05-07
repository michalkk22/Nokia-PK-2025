#pragma once

#include "ConnectedState.hpp"

namespace ue {
class TalkingState : public ConnectedState {
  TalkingState(Context &context);
};
}