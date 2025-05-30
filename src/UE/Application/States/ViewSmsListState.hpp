#pragma once

#include "ConnectedState.hpp"

namespace ue
{

class ViewSmsListState : public ConnectedState {
public:
    ViewSmsListState(Context& context);
    void handleUiAction() override;
    void handleUiAccept() override;
    void handleUiBack() override;
    void handleItemSelected(std::optional<std::size_t> index) override;

  private:
    void showList();
    std::vector<SmsMessage> currentSmsList;
};
}