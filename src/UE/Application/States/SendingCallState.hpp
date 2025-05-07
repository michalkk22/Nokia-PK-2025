#pragma once

#include "StartDialState.hpp"

namespace ue {

class SendingCallState : public StartDialState {
public:
    SendingCallState(Context &context);
    SendingCallState(Context &context, const std::string &name);
    void handleUiBack() override;
    void handleUiAccept() override;

  private:
    void dialing();
};

} // namespace ue