#pragma once

#include "ConnectedState.hpp"

namespace ue
{

class ComposeSmsState : public ConnectedState {
public:
    ComposeSmsState(Context &context);

    void handleUiAction() override;
    void handleUiAccept() override;
    void handleUiBack() override;
    void handleSmsSentResult(common::PhoneNumber to, bool success) override;
};

} // namespace ue