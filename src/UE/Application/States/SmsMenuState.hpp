#pragma once

#include "ConnectedState.hpp"

namespace ue
{

class SmsMenuState : public ConnectedState
{
public:
SmsMenuState(Context& context);
    void handleUiAction() override;
    void handleUiAccept() override;
    void handleItemSelected(std::optional<std::size_t> index) override;

private:
};

}