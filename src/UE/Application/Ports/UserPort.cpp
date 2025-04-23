#include "UserPort.hpp"
#include "UeGui/IListViewMode.hpp"
#include "UeGui/ITextMode.hpp"
#include <string>

namespace ue {

    UserPort::UserPort(common::ILogger &logger, IUeGui &gui, 
        common::PhoneNumber phoneNumber)
: logger(logger, "[USER-PORT]"), gui(gui), phoneNumber(phoneNumber) {}

void UserPort::start(IEventsHandler &handler) {
    this->handler = &handler;
    gui.setTitle("Nokia " + common::to_string(phoneNumber));
    gui.setAcceptCallback(std::bind(&UserPort::acceptCallback, this));
    gui.setRejectCallback(std::bind(&UserPort::rejectCallback, this));
}

void UserPort::stop() {
    handler = nullptr;
    gui.setAcceptCallback(nullptr);
    gui.setRejectCallback(nullptr);
}

void UserPort::showNotConnected()
{
    currentViewMode = details::VIEW_MODE_UNKNOWN;
    gui.showNotConnected();
}

void UserPort::showConnecting()
{
    currentViewMode = details::VIEW_MODE_UNKNOWN;
    gui.showConnecting();
}

void UserPort::showConnected()
{
    currentViewMode = details::VIEW_MODE_MAIN_MENU;
    logger.logInfo("Menu");
    IUeGui::IListViewMode &menu = gui.setListViewMode();
    menu.clearSelectionList();
    menu.addSelectionListItem("Compose SMS", "Send a new SMS");
    menu.addSelectionListItem("View SMS", "Read incoming SMS");
    gui.showConnected(); 
}

void UserPort::showNewSms()
{
    logger.logInfo("New SMS notification");
    gui.showNewSms(true);
}

void UserPort::displaySmsList(const std::vector<SmsMessage>& smsList)
{
    currentViewMode = details::VIEW_MODE_SMS_LIST;
    logger.logInfo("Showing SMS list, count: ", smsList.size());

    IUeGui::IListViewMode& listView = gui.setListViewMode();
    listView.clearSelectionList();

    for (const auto& msg : smsList)
    {
        std::string statusLabel = msg.isRead ? "  " : "New: ";
        std::string entry = statusLabel + "From: " + common::to_string(msg.fromNumber);
        listView.addSelectionListItem(entry, msg.message);
    }
}

void UserPort::displaySmsContent(const SmsMessage& sms)
{
    currentViewMode = details::VIEW_MODE_SMS_VIEW;
    logger.logInfo("Opening SMS from number: ", sms.fromNumber);

    IUeGui::ITextMode& textMode = gui.setViewTextMode();
    std::string content = "From: " + common::to_string(sms.fromNumber) + "\n\n" + sms.message;
    textMode.setText(content);
}

void UserPort::displayAlert(const std::string& title, const std::string& message)
{
    currentViewMode = details::VIEW_MODE_UNKNOWN; 
    logger.logInfo("Alert: ", title);
    IUeGui::ITextMode& alerter = gui.setAlertMode();
    alerter.setText(title + "\n\n" + message);
}

void UserPort::acceptCallback()
{
    if (!handler) return;

    std::optional<std::size_t> selectedIndexOpt;
   
    if (currentViewMode == details::VIEW_MODE_MAIN_MENU || currentViewMode == details::VIEW_MODE_SMS_LIST)
    {
        IUeGui::IListViewMode& currentList = gui.setListViewMode();
        auto selectionPair = currentList.getCurrentItemIndex();
        if (selectionPair.first) {
            selectedIndexOpt = selectionPair.second;
        }
    }
     else if (currentViewMode == details::VIEW_MODE_SMS_VIEW) {  
        logger.logDebug("Ignoring accept action – currently viewing SMS content");
        return;
     }
    
    logger.logDebug("User triggered accept - ViewMode: ", currentViewMode, ", Selected Index: ", (selectedIndexOpt ? std::to_string(*selectedIndexOpt) : "None"));
    handler->handleUiAction(selectedIndexOpt);
}

void UserPort::rejectCallback()
{
    if (!handler) return;
    logger.logDebug("User pressed back/cancel - Current UI state: ", currentViewMode);   
    handler->handleUiBack();
}
}