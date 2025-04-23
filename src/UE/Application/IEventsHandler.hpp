#pragma once

#include "Ports/ITimerPort.hpp"
#include "Ports/IBtsPort.hpp"
#include "Ports/IUserPort.hpp"
#include <optional>

namespace ue
{
    namespace details {
        using GuiViewMode = int;
        constexpr GuiViewMode VIEW_MODE_UNKNOWN = 0;
        constexpr GuiViewMode VIEW_MODE_MAIN_MENU = 1;
        constexpr GuiViewMode VIEW_MODE_SMS_LIST = 2;
        constexpr GuiViewMode VIEW_MODE_SMS_VIEW = 3;
    }

    class IEventsHandler : public IBtsEventsHandler,
                            public IUserEventsHandler,
                            public ITimerEventsHandler
    {
    public:
        virtual ~IEventsHandler() = default;
        virtual void handleUiAction(std::optional<std::size_t> selectedIndex) = 0;
        virtual void handleUiBack() = 0;
    };

}
