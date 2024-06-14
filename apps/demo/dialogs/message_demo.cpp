#include "message_demo.h"

using namespace dlgcpp;
using namespace dlgcpp::dialogs;

void dialogs_message_demo(ISharedDialog parent)
{
    auto messageDlg = std::make_shared<MessageDialog>(parent);

    // if title not specified; uses parent title
    messageDlg->title("DLGCPP");
    messageDlg->message("Welcome to DLGCPP Demo Application. Press a button");
    messageDlg->buttons(MessageDialogButtonGroup::OkCancel);
    messageDlg->defaultButton(MessageDialogButton::Cancel);
    messageDlg->showHelp(false);
    auto r = messageDlg->show();

    if (r == MessageDialogButton::Ok)
        parent->message("OK was pressed");
}
