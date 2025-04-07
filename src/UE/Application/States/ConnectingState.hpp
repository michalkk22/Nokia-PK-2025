#pragma once

#include "BaseState.hpp"

namespace ue
{

class ConnectingState : public BaseState
{
public:
    ConnectingState(Context& context, common::BtsId btsId);

    // IBtsEventsHandler interface
    void handleAttachAccept() override;
    void handleAttachReject() override;

    // ITimerEventsHandler interface
    void handleTimeout() override;
    void handleDisconnected() override;

private:
    common::BtsId btsId;
};

}