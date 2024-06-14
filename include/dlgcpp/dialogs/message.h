#pragma once

#include "dlgcpp/dlgcpp.h"

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

        enum class MessageDialogButtonGroup
        {
            Ok = 0,
            OkCancel,
            AbortRetryIgnore,
            YesNoCancel,
            YesNo,
            RetryCancel
        };

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

        class IMessageDialog
        {
        public:
            virtual const std::string& title() const = 0;
            virtual void title(const std::string& value) = 0;
            virtual const std::string& message() const = 0;
            virtual void message(const std::string& value) = 0;
            virtual MessageDialogButtonGroup buttons() const = 0;
            virtual void buttons(MessageDialogButtonGroup value) = 0;
            virtual MessageDialogIcon icon() const = 0;
            virtual void icon(MessageDialogIcon value) = 0;
            virtual MessageDialogButton defaultButton() const = 0;
            virtual void defaultButton(MessageDialogButton value) = 0;
            virtual bool showHelp() const = 0;
            virtual void showHelp(bool value) = 0;
            virtual MessageDialogButton show() = 0;
        };

        class MessageDialog : public IMessageDialog
        {
        public:
            explicit MessageDialog(ISharedDialog parent = nullptr);
            virtual ~MessageDialog();

            // IMessageDialog impl.
            const std::string& title() const override;
            void title(const std::string& value) override;
            const std::string& message() const override;
            void message(const std::string& value) override;
            MessageDialogButtonGroup buttons() const override;
            void buttons(MessageDialogButtonGroup value) override;
            MessageDialogIcon icon() const override;
            void icon(MessageDialogIcon value) override;
            MessageDialogButton defaultButton() const override;
            void defaultButton(MessageDialogButton value) override;
            bool showHelp() const override;
            void showHelp(bool value) override;
            MessageDialogButton show() override;

        private:
            struct msgdlg_props* _props;
        };
    }
}
