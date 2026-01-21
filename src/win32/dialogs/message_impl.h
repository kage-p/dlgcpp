#pragma once

#include "dlgcpp/dialogs/message.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    namespace dialogs
    {
        class MessageDialogImpl
        {
        public:
            MessageDialogImpl(MessageDialog* messageDialog);
            ~MessageDialogImpl() = default;

            MessageDialogButton show();

        private:
            MessageDialog* _messageDialog = nullptr;
        };
    }
}
