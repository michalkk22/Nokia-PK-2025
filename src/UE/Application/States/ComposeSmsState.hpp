#pragma once

#include "BaseState.hpp"

namespace ue
{

class ComposeSmsState : public BaseState
{
public:
    ComposeSmsState(Context &context);

    void handleUiAction() override;
    void handleUiAccept() override;
    void handleUiBack() override;
    void handleSmsSentResult(common::PhoneNumber to, bool success) override;
};

} // namespace ue