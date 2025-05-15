#pragma once

#include "BaseState.hpp"

namespace ue
{

class ConnectedState : public BaseState
{
public:
    ConnectedState(Context& context);
    ConnectedState(Context &context, const std::string &name)
        : BaseState(context, name) {};
    void handleDisconnected() override;
    void handleSmsReceived(common::PhoneNumber fromNumber, std::string message) override;
    void handleSmsSentResult(common::PhoneNumber to, bool success) override;
    void handleCallUnknownRecipient(common::PhoneNumber to) override;
    void handleCallReceived(common::PhoneNumber fromNumber) override;
    void dropAnotherCall(common::PhoneNumber fromNumber);
    void handleUiAction() override;
    void handleUiBack() override;
    void handleUiAccept() override;

  private:
    void showMainMenu();
};

}