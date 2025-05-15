#include "SmsMenuState.hpp"
#include "ComposeSmsState.hpp"
#include "ViewSmsListState.hpp"

namespace ue {
    SmsMenuState::SmsMenuState(Context &context)
    : ConnectedState(context, "SmsMenuState") {
        context.user.showSmsMenuList();
}

    void SmsMenuState::handleItemSelected(std::optional<std::size_t> index) {
        if (index.has_value()) {
            switch (index.value()) {
                case 0: // Compose SMS
                    logger.logInfo("Compose SMS selected from SMS menu");
                    context.setState<ComposeSmsState>();
                    break;
                case 1: // View SMS
                    logger.logInfo("View SMS selected from SMS menu");
                    context.setState<ViewSmsListState>();
                    break;
                default:
                    logger.logError("Invalid selection in SMS menu");
                    break;
            }
        }
    }

    void SmsMenuState::handleUiAction() {}

    void SmsMenuState::handleUiAccept() {}

} // namespace ue
