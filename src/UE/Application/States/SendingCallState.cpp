#include "SendingCallState.hpp"

namespace ue {
    SendingCallState::SendingCallState(Context &context) : ConnectedState(context, "SendingCallState") {
        dialing();
    }

    void SendingCallState::dialing() {
        logger.logInfo("Entered Dialing state.");
        //TODO: implement dialing
    }
}