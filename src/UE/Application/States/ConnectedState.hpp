#pragma once

#include "BaseState.hpp"

namespace ue
{

class ConnectedState : public BaseState
{
public:
    ConnectedState(Context& context);
    void handleDisconnected() override;
    void handleSmsReceived(common::PhoneNumber fromNumber, std::string message) override;
    void handleSmsSentResult(common::PhoneNumber to, bool success) override;
    void handleUiAction(std::optional<std::size_t> selectedIndex) override;
    void handleUiBack() override;
    void handleUiAccept() override;

  private:
    void showMainMenu();
};

}