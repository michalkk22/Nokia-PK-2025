#pragma once

#include "ConnectedState.hpp"

namespace ue {

class ReceivingCallState : public ConnectedState {
public:
    ReceivingCallState(Context &context, common::PhoneNumber fromNumber);
    void handleUiAction() override;
    void handleUiAccept() override;
    void handleUiBack() override;
    void handleCallDropped() override;
    void handleCallReceived(common::PhoneNumber number) override;

  private:
    common::PhoneNumber fromNumber;
};

} // namespace ue