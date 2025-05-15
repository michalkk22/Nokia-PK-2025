#pragma once

#include "ConnectedState.hpp"

namespace ue
{

class ViewSmsState : public ConnectedState {
public:
  ViewSmsState(Context &context, std::size_t smsIndex);
  void handleUiBack() override;
  void handleUiAction() override;
  void handleUiAccept() override;

private:
  std::size_t viewingSmsIndex;
};
}