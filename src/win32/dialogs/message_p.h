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
            MessageDialogImpl(MessageDialog& messageDialog, ISharedDialog parent);
            ~MessageDialogImpl() = default;

            const std::string& title() const;
            void title(const std::string& value);
            const std::string& message() const;
            void message(const std::string& value);
            MessageDialogButtonGroup buttons() const;
            void buttons(MessageDialogButtonGroup value);
            MessageDialogIcon icon() const;
            void icon(MessageDialogIcon value);
            MessageDialogButton defaultButton() const;
            void defaultButton(MessageDialogButton value);
            bool showHelp() const;
            void showHelp(bool value);
            MessageDialogButton show();

        private:
            MessageDialog& _messageDialog;
            ISharedDialog _parent;
            std::string _title;
            std::string _message;
            bool _showHelp = false;
            MessageDialogIcon _icon = MessageDialogIcon::Information;
            MessageDialogButtonGroup _buttons = MessageDialogButtonGroup::Ok;
            MessageDialogButton _defaultButton = MessageDialogButton::Undefined;
        };
    }
}
