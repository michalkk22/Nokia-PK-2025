#pragma once

#include "ConnectedState.hpp"

namespace ue {

class SendingCallState : public ConnectedState {
public:
    SendingCallState(Context &context);
  void handleUiBack() override;

private:
  void dialing();
};

} // namespace ue