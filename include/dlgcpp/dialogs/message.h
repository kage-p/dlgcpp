#pragma once

#include "dlgcpp/defs.h"
#include "dlgcpp/property.h"
#include <ostream>
#include <string>

namespace dlgcpp
{
    namespace dialogs
    {
        enum class MessageDialogIcon
        {
            Information = 0,
            Question,
            Warning,
            Error
        };

        inline std::ostream& operator<<(std::ostream& os, MessageDialogIcon e)
        {
            switch (e) {
            case MessageDialogIcon::Information:   return os << "Information";
            case MessageDialogIcon::Question:  return os << "Question";
            case MessageDialogIcon::Warning: return os << "Warning";
            case MessageDialogIcon::Error: return os << "Error";
            }
            return os << static_cast<int>(e); // fallback
        }

        enum class MessageDialogButtonGroup
        {
            Ok = 0,
            OkCancel,
            AbortRetryIgnore,
            YesNoCancel,
            YesNo,
            RetryCancel
        };

        inline std::ostream& operator<<(std::ostream& os, MessageDialogButtonGroup e)
        {
            switch (e) {
            case MessageDialogButtonGroup::Ok:   return os << "Ok";
            case MessageDialogButtonGroup::OkCancel:  return os << "OkCancel";
            case MessageDialogButtonGroup::AbortRetryIgnore: return os << "AbortRetryIgnore";
            case MessageDialogButtonGroup::YesNoCancel:   return os << "YesNoCancel";
            case MessageDialogButtonGroup::YesNo:  return os << "YesNo";
            case MessageDialogButtonGroup::RetryCancel: return os << "RetryCancel";
            }
            return os << static_cast<int>(e); // fallback
        }

        enum class MessageDialogButton
        {
            Undefined,
            Ok,
            Cancel,
            Abort,
            Retry,
            Ignore,
            Yes,
            No
        };

        inline std::ostream& operator<<(std::ostream& os, MessageDialogButton e)
        {
            switch (e) {
            case MessageDialogButton::Undefined:   return os << "Undefined";
            case MessageDialogButton::Ok:  return os << "Ok";
            case MessageDialogButton::Cancel: return os << "Cancel";
            case MessageDialogButton::Abort: return os << "Abort";
            case MessageDialogButton::Retry: return os << "Retry";
            case MessageDialogButton::Ignore: return os << "Ignore";
            case MessageDialogButton::Yes: return os << "Yes";
            case MessageDialogButton::No: return os << "No";
            }
            return os << static_cast<int>(e); // fallback
        }

        class IMessageDialog;
        class MessageDialogImpl;
        typedef std::shared_ptr<IMessageDialog> ISharedMessageDialog;

        class IMessageDialog
        {
        public:
            virtual IReadOnlyProperty<ISharedDialog, ISharedMessageDialog>& parent() = 0;
            virtual IProperty<std::string, ISharedMessageDialog>& title() = 0;
            virtual IProperty<std::string, ISharedMessageDialog>& message() = 0;
            virtual IProperty<MessageDialogButtonGroup, ISharedMessageDialog>& buttons() = 0;
            virtual IProperty<MessageDialogIcon, ISharedMessageDialog>& icon() = 0;
            virtual IProperty<MessageDialogButton, ISharedMessageDialog>& defaultButton() = 0;
            virtual IProperty<bool, ISharedMessageDialog>& showHelp() = 0;

            virtual MessageDialogButton show() = 0;
        };

        class MessageDialog :
            public IMessageDialog,
            public std::enable_shared_from_this<MessageDialog>
        {
        public:
            explicit MessageDialog(ISharedDialog parent = nullptr);
            virtual ~MessageDialog() = default;

            // IMessageDialog impl.
            IReadOnlyProperty<ISharedDialog, ISharedMessageDialog>& parent() override;
            IProperty<std::string, ISharedMessageDialog>& title() override;
            IProperty<std::string, ISharedMessageDialog>& message() override;
            IProperty<MessageDialogButtonGroup, ISharedMessageDialog>& buttons() override;
            IProperty<MessageDialogIcon, ISharedMessageDialog>& icon() override;
            IProperty<MessageDialogButton, ISharedMessageDialog>& defaultButton() override;
            IProperty<bool, ISharedMessageDialog>& showHelp() override;
            MessageDialogButton show() override;

        private:
            MessageDialog(std::shared_ptr<MessageDialogImpl> impl, ISharedDialog parent);

            std::shared_ptr<MessageDialogImpl> _impl;
            Property<ISharedDialog, ISharedMessageDialog> _parent;
            Property<std::string, ISharedMessageDialog> _title;
            Property<std::string, ISharedMessageDialog> _message;
            Property<MessageDialogButtonGroup, ISharedMessageDialog> _buttons;
            Property<MessageDialogIcon, ISharedMessageDialog> _icon;
            Property<MessageDialogButton, ISharedMessageDialog> _defaultButton;
            Property<bool, ISharedMessageDialog> _showHelp;
        };
    }
}
