#pragma once

#include "dlgcpp/dialogs/message.h"

#include <string>

namespace dlgcpp
{
    namespace dialogs
    {
        struct msgdlg_props
        {
            ISharedDialog parent;
            std::string title;
            std::string message;
            bool showHelp = false;
            MessageDialogIcon icon = MessageDialogIcon::Information;
            MessageDialogButtonGroup buttons = MessageDialogButtonGroup::Ok;
            MessageDialogButton defaultButton = MessageDialogButton::Undefined;
        };
    }
}
